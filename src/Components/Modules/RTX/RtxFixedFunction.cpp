#include "STDInc.hpp"

namespace Components
{
	struct unpacked_model_vert
	{
		Game::vec3_t pos;
		Game::vec3_t normal;
		Game::vec2_t texcoord;
	};

	constexpr auto MODEL_VERTEX_STRIDE = 32u;
	constexpr auto MODEL_VERTEX_FORMAT = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	// #

	struct unpacked_world_vert
	{
		Game::vec3_t pos;
		Game::vec3_t normal;
		unsigned int color;
		Game::vec2_t texcoord;
	};

	constexpr auto WORLD_VERTEX_STRIDE = 36u;
	constexpr auto WORLD_VERTEX_FORMAT = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	IDirect3DVertexBuffer9* gfx_world_vertexbuffer = nullptr;

	// #
	struct unpacked_fx_vert
	{
		Game::vec3_t pos;
		unsigned int color;
		Game::vec2_t texcoord;
		Game::vec2_t unused;
	};

	// we need the vertex color for alpha blending so we have to get rid of normal (not needed)
	// D3DFVF_TEX2 is there to get us to a stride of 32, so that we dont have to create our own indexbuffer (because og. verts are 32 in size)
	constexpr auto FX_VERTEX_STRIDE = 32u;
	constexpr auto FX_VERTEX_FORMAT = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEX2);


	// #
	struct unpacked_mark_vert
	{
		Game::vec3_t pos;
		Game::vec3_t normal;
		unsigned int color;
		Game::vec2_t texcoord;
		Game::vec2_t unused;
	};

	// D3DFVF_TEX2 is there to get us to a stride of 44, so that we dont have to create our own indexbuffer
	constexpr auto FX_MARKS_STRIDE = 44u; // GfxWorldVertex
	constexpr auto FX_MARKS_FORMAT = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEX2);

	/**
	 * @brief update 'state->prim.streams' and set d3d device stream source
	 */
	void set_stream_source(Game::GfxCmdBufState* state, IDirect3DVertexBuffer9* buffer, const std::uint32_t vertex_offset, const std::uint32_t stride)
	{
		if (state->prim.streams[0].vb != buffer || state->prim.streams[0].offset != vertex_offset || state->prim.streams[0].stride != stride)
		{
			state->prim.streams[0].vb = buffer;
			state->prim.streams[0].offset = vertex_offset;
			state->prim.streams[0].stride = stride;
			state->prim.device->SetStreamSource(0, state->prim.streams[0].vb, state->prim.streams[0].offset, state->prim.streams[0].stride);
		}
	}

	/**
	 * @brief			unpacks a packed unit vector (eg. normal in GfxWorldVertex)
	 * @param packed	PackedUnitVec
	 * @param out		vec3 out
	 */
	void unpack_normal(const Game::PackedUnitVec* packed, Game::vec3_t out)
	{
		// normal unpacking in a cod4 hlsl shader:
		// temp0	 = i.normal * float4(0.007874016, 0.007874016, 0.007874016, 0.003921569) + float4(-1, -1, -1, 0.7529412);
		// temp0.xyz = temp0.www * temp0;

		const auto scale = static_cast<float>(static_cast<std::uint8_t>(packed->array[3])) * (1.0f / 255.0f) + 0.7529412f;
		out[0] = (static_cast<float>(static_cast<std::uint8_t>(packed->array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
		out[1] = (static_cast<float>(static_cast<std::uint8_t>(packed->array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
		out[2] = (static_cast<float>(static_cast<std::uint8_t>(packed->array[2])) * (1.0f / 127.0f) + -1.0f) * scale;
	}

	// *
	// static models (rigid)

	/**
	 * @brief		creates
	 * @param surf	XSurface to create vertex buffer for
	 * @return		returns true if vertex buffer was allocated
	 */
	bool XSurfaceOptimize(Game::XSurface* surf)
	{
		const auto dev = *Game::dx9_device_ptr;
		bool allocated_any = false;

		// setup vertexbuffer
		{
			// setup vertexbuffer
			if (!surf->custom_vertexbuffer) // XSurfaceOptimizeRigid
			{
				void* vertex_buffer_data = nullptr;
				const auto vertex_bytes = surf->vertCount * MODEL_VERTEX_STRIDE;

				if (auto hr = dev->CreateVertexBuffer(vertex_bytes, D3DUSAGE_WRITEONLY, MODEL_VERTEX_FORMAT, D3DPOOL_DEFAULT, &surf->custom_vertexbuffer, nullptr); hr >= 0)
				{
					if (hr = surf->custom_vertexbuffer->Lock(0, 0, &vertex_buffer_data, 0); hr >= 0)
					{
						// we need to unpack normal and texcoords in 'GfxPackedVertex' to be able to use them for fixed-function rendering
						for (auto i = 0; i < surf->vertCount; i++)
						{
							// packed source vertex
							const auto src_vert = &surf->verts0[i];

							// position of our unpacked vert within the vertex buffer
							const auto v_pos_in_buffer = i * MODEL_VERTEX_STRIDE;
							const auto v = reinterpret_cast<unpacked_model_vert*>(((DWORD)vertex_buffer_data + v_pos_in_buffer));

							// assign pos
							v->pos[0] = src_vert->xyz[0];
							v->pos[1] = src_vert->xyz[1];
							v->pos[2] = src_vert->xyz[2];

							// unpack and assign vert normal
							unpack_normal(&src_vert->normal, v->normal);

							// uv
							Game::Vec2UnpackTexCoords(src_vert->texCoord.packed, v->texcoord);
						}

						surf->custom_vertexbuffer->Unlock();
					}
					else
					{
						surf->custom_vertexbuffer->Release();
						surf->custom_vertexbuffer = nullptr;
					}

					allocated_any = true;
				}
			}
		}

		return allocated_any;
	}

	int XModelGetSurfaces(const Game::XModel* model, Game::XSurface** surfaces, const int submodel)
	{
		*surfaces = &model->surfs[model->lodInfo[submodel].surfIndex];
		return model->lodInfo[submodel].numsurfs;
	}

	void XModelOptimize(const Game::XModel* model, bool is_loading_map = false)
	{
		Game::XSurface* surfaces;

		const auto lod_count = model->numLods;
		for (auto lod_index = 0; lod_index < lod_count; ++lod_index)
		{
			const auto surf_count = XModelGetSurfaces(model, &surfaces, lod_index);
			for (auto surf_index = 0; surf_index < surf_count; ++surf_index)
			{
				const bool allocated_any = XSurfaceOptimize(&surfaces[surf_index]);
#if DEBUG
				if (!is_loading_map && allocated_any)
				{
					Game::Com_Printf(0, Utils::String::VA("Allocated buffers for smodel '%s'\n", model->name));
				}
#endif
			}
		}
	}


	/**
	* @brief		builds a world matrix for the mesh
	* @param mtx	output matrix
	*/
	void RtxFixedFunction::build_worldmatrix_for_object(float(*mtx)[4], float(*model_axis)[3], const float* origin, const float scale)
	{
		mtx[0][0] = model_axis[0][0] * scale;
		mtx[0][1] = model_axis[0][1] * scale;
		mtx[0][2] = model_axis[0][2] * scale;
		mtx[0][3] = 0.0f;

		mtx[1][0] = model_axis[1][0] * scale;
		mtx[1][1] = model_axis[1][1] * scale;
		mtx[1][2] = model_axis[1][2] * scale;
		mtx[1][3] = 0.0f;

		mtx[2][0] = model_axis[2][0] * scale;
		mtx[2][1] = model_axis[2][1] * scale;
		mtx[2][2] = model_axis[2][2] * scale;
		mtx[2][3] = 0.0f;

		mtx[3][0] = origin[0];
		mtx[3][1] = origin[1];
		mtx[3][2] = origin[2];
		mtx[3][3] = 1.0f;
	}

	/**
	 * @brief		builds a world matrix for the mesh
	 * @param mtx	output matrix
	 */
	void RtxFixedFunction::build_worldmatrix_for_object(float(*mtx)[4], const float* quat, const float* origin, const float scale)
	{
		float model_axis[3][3] = {};
		RtxUtils::unit_quat_to_axis(quat, model_axis);
		build_worldmatrix_for_object(mtx, model_axis, origin, scale);
	}

	/**
	 * @brief completely rewritten R_DrawStaticModelDrawSurfNonOptimized to render static models using the fixed-function pipeline
	 */
	void R_DrawStaticModelDrawSurfNonOptimized(const Game::GfxStaticModelDrawStream* drawstream, Game::GfxCmdBufSourceState* src, Game::GfxCmdBufState* state)
	{
		const auto smodel_count = drawstream->smodelCount;
		const auto smodel_list = (const Game::GfxStaticModelDrawStream*) reinterpret_cast<const void*>(drawstream->smodelList);
		const auto draw_inst = Game::rgp->world->dpvs.smodelDrawInsts;
		const auto dev = Game::get_device();

		/*if (!drawstream->localSurf->custom_vertexbuffer)
		{
			__debugbreak();
			return;
		}*/

		// create buffers for all surfaces of the model (including LODs)
		// ^ already done on map load (when 'rtx_warm_smodels' is enabled) but this also covers dynamically spawned models
		for (auto index = 0u; index < smodel_count; index++)
		{
			const auto inst = &draw_inst[*((std::uint16_t*)&smodel_list->primDrawSurfPos + index)];
			XModelOptimize(inst->model);
		}

		// #
		// set streams

		// index and vertex buffers are within the fastfile that includes the model
		const auto mem = &Game::g_zones[static_cast<std::int8_t>(drawstream->localSurf->zoneHandle)].mem;

		// index buffer
		if (state->prim.indexBuffer != mem->indexBuffer)
		{
			state->prim.indexBuffer = mem->indexBuffer;
			dev->SetIndices(mem->indexBuffer);
		}

		// custom vertexbuffer
		set_stream_source(state, drawstream->localSurf->custom_vertexbuffer, 0, MODEL_VERTEX_STRIDE);

		// vertex format
		dev->SetFVF(MODEL_VERTEX_FORMAT);

		// def. needed or the game will render the mesh using shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);

		// #
		// draw prim 

		for (auto index = 0u; index < smodel_count; index++)
		{
			const auto inst = &draw_inst[*((std::uint16_t*)&smodel_list->primDrawSurfPos + index)];

			// transform model into the scene by updating the worldmatrix
			float mtx[4][4] = {};
			RtxFixedFunction::build_worldmatrix_for_object(&mtx[0], &inst->placement.axis[0], inst->placement.origin, inst->placement.scale);
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));

			// get indexbuffer offset
			const auto offset = ((char*)drawstream->localSurf->triIndices - mem->blocks[8].data) >> 1;

			// draw the prim
			dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, drawstream->localSurf->vertCount, offset, drawstream->localSurf->triCount);
		}

		dev->SetFVF(NULL);
	}


	// *
	// xmodels (rigid/skinned)

	/**
	 * @brief	draws rigid meshes using fixed function. Uses static vertex buffers
	 */
	void R_DrawXModelRigidModelSurf(Game::GfxModelRigidSurface* model, [[maybe_unused]] Game::GfxCmdBufSourceState* source, Game::GfxCmdBufState* state)
	{
		const auto dev = Game::get_device();
		const auto surf = model->surf.xsurf;

		// fixed function rendering
		bool rendering_ff = true;

		if (surf->deformed)
		{
			__debugbreak();
		}

		// build custom vertexbuffer for dynamically spawned models
		if (!surf->custom_vertexbuffer)
		{
			bool allocated_any = XSurfaceOptimize(surf);
#if DEBUG
			if (allocated_any && state->material && state->material->info.name)
			{
				Game::Com_Printf(0, Utils::String::VA("Allocated buffers for smodel using material '%s'\n", state->material->info.name), 0);
			}
#endif
			if (!surf->custom_vertexbuffer)
			{
				__debugbreak();
				return;
			}
		}

		// #
		// set streams

		// index and vertex buffers are within the fastfile that includes the model
		const auto mem = &Game::g_zones[static_cast<std::uint8_t>(surf->zoneHandle)].mem;

		state->prim.indexBuffer = mem->indexBuffer;
		dev->SetIndices(mem->indexBuffer);

		float custom_scalar = 1.0f;
		auto obj_origin = &model->placement.base.origin[0];

		// skysphere materials need to have sort = sky in assetmanager
		if (state->material && state->material->info.sortKey == 5)
		{
			obj_origin = Game::cgs->snap->ps.origin;

			custom_scalar = 1000.0f; //Rtx_gui::skysphere_scale;

			// disable fog for skysphere
			dev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}

		// def. needed or the game will render the mesh using shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);

		set_stream_source(state, surf->custom_vertexbuffer, 0, MODEL_VERTEX_STRIDE);

		// vertex format
		dev->SetFVF(MODEL_VERTEX_FORMAT);

		// transform model into the scene by updating the worldmatrix
		float mtx[4][4] = {};
		RtxFixedFunction::build_worldmatrix_for_object(&mtx[0], model->placement.base.quat, obj_origin, model->placement.scale * custom_scalar);
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));

		// #
		// draw prim

		const auto offset = ((char*)surf->triIndices - mem->blocks[8].data) >> 1;
		dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, surf->vertCount, offset, surf->triCount);
		dev->SetFVF(NULL);
	}

	__declspec(naked) void R_DrawXModelRigidModelSurf1_stub()
	{
		const static uint32_t retn_addr = 0x635685;
		__asm
		{
			// state pushed
			// source pushed
			push	eax; // GfxModelRigidSurface
			call	R_DrawXModelRigidModelSurf;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	__declspec(naked) void R_DrawXModelRigidModelSurf2_stub()
	{
		const static uint32_t retn_addr = 0x635A1E;
		__asm
		{
			// state pushed
			// source pushed
			push	ecx; // GfxModelRigidSurface
			call	R_DrawXModelRigidModelSurf;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	// ------------------------

	/**
	 * @brief	sets triangle indices within the dynamicIndexBuffer
	 * @return	base index
	 */
	int R_SetIndexData(Game::GfxCmdBufPrimState* state, const std::uint16_t* indices, std::uint32_t tri_count)
	{
		const auto dyn_ib = Game::gfx_buf->dynamicIndexBuffer;
		const auto index_count = 3 * tri_count;

		if (static_cast<int>(index_count) + dyn_ib->used > dyn_ib->total)
		{
			dyn_ib->used = 0;
		}

		if (!dyn_ib->used)
		{
			Game::gfx_buf->dynamicIndexBuffer = Game::gfx_buf->dynamicIndexBufferPool;
		}

		void* buffer_data;
		if (const auto hr = dyn_ib->buffer->Lock(2 * dyn_ib->used, 6 * tri_count, &buffer_data, dyn_ib->used != 0 ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
			hr < 0)
		{
			Game::Com_Error(1, "Fatal lock error :: R_SetIndexData");
		}

		memcpy(buffer_data, indices, 2 * index_count);
		dyn_ib->buffer->Unlock();

		if (state->indexBuffer != dyn_ib->buffer)
		{
			state->indexBuffer = dyn_ib->buffer;
			state->device->SetIndices(dyn_ib->buffer);
		}

		const auto base_index = dyn_ib->used;
		dyn_ib->used += static_cast<int>(index_count);

		return base_index;
	}

	/**
	 * @brief	draw skinned meshes using fixed function. Uses a dynamic vertex buffer.
	 */
	void R_DrawXModelSkinnedUncached(const Game::GfxModelSkinnedSurface* skinned_surf, [[maybe_unused]] Game::GfxPackedVertex* verts, Game::GfxCmdBufSourceState* source, Game::GfxCmdBufState* state)
	{
		// fixed function rendering
		//constexpr auto render_ff = true;
		const auto dev = Game::get_device();
		const auto surf = skinned_surf->xsurf;
		const auto dyn_vb = Game::gfx_buf->dynamicVertexBuffer;
		const auto start_index = R_SetIndexData(&state->prim, surf->triIndices, surf->triCount);

		if ((int)(MODEL_VERTEX_STRIDE * surf->vertCount + dyn_vb->used) > dyn_vb->total)
		{
			dyn_vb->used = 0;
		}

		// R_SetVertexData
		void* buffer_data;
		if (const auto hr = dyn_vb->buffer->Lock(dyn_vb->used, MODEL_VERTEX_STRIDE * surf->vertCount, &buffer_data, dyn_vb->used != 0 ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
			hr < 0)
		{
			Game::Com_Error(1, "Fatal lock error :: R_DrawXModelSkinnedUncached");
		}

		{
			for (auto i = 0u; i < surf->vertCount; i++)
			{
				// packed source vertex
				const auto src_vert = &skinned_surf->u.skinnedVert[i];

				// position of our unpacked vert within the vertex buffer
				const auto v_pos_in_buffer = i * MODEL_VERTEX_STRIDE;
				const auto v = reinterpret_cast<unpacked_model_vert*>(((DWORD)buffer_data + v_pos_in_buffer));

				// vert pos
				v->pos[0] = src_vert->xyz[0];
				v->pos[1] = src_vert->xyz[1];
				v->pos[2] = src_vert->xyz[2];

				// unpack and assign vert normal
				unpack_normal(&src_vert->normal, v->normal);

				// uv's
				Game::Vec2UnpackTexCoords(src_vert->texCoord.packed, v->texcoord);
			}

		}

		dyn_vb->buffer->Unlock();
		const std::uint32_t vert_offset = dyn_vb->used;
		dyn_vb->used += (MODEL_VERTEX_STRIDE * surf->vertCount);


		// #
		// #

		set_stream_source(state, dyn_vb->buffer, vert_offset, MODEL_VERTEX_STRIDE);

		// needed or game renders mesh with shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);

		// vertex format
		dev->SetFVF(MODEL_VERTEX_FORMAT);

		//transform model into the scene by updating the worldmatrix
		float mtx[4][4] = {};
		RtxFixedFunction::build_worldmatrix_for_object(&mtx[0], source->skinnedPlacement.base.quat, source->skinnedPlacement.base.origin, source->skinnedPlacement.scale);
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));

		dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, surf->vertCount, start_index, surf->triCount);
		dev->SetFVF(NULL);
	}

	__declspec(naked) void R_DrawXModelSkinnedUncached_stub()
	{
		const static uint32_t retn_addr = 0x62580D;
		__asm
		{
			// GfxPackedVertex (skinnedVert) pushed (we ignore that because we push GfxModelSkinnedSurface which holds it)
			// state pushed
			// source pushed
			push	eax; // GfxModelSkinnedSurface
			call	R_DrawXModelSkinnedUncached;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	/**
	 * @brief Draw static - skinned meshes using fixed function. Uses static and dynamic vertex buffers.
	 */
	void R_DrawStaticModelsSkinnedDrawSurf(const Game::GfxStaticModelDrawStream* draw_stream, Game::GfxCmdBufSourceState* source, Game::GfxCmdBufState* state)
	{
		const auto dev = Game::get_device();
		const auto surf = draw_stream->localSurf;
		const auto dyn_vb = Game::gfx_buf->dynamicVertexBuffer;
		const auto start_index = R_SetIndexData(&state->prim, surf->triIndices, surf->triCount);

		if (!surf->deformed && surf->custom_vertexbuffer)
		{
			set_stream_source(state, surf->custom_vertexbuffer, 0, MODEL_VERTEX_STRIDE);
		}
		else
		{
			if ((int)(MODEL_VERTEX_STRIDE * surf->vertCount + dyn_vb->used) > dyn_vb->total)
			{
				dyn_vb->used = 0;
			}

			// R_SetVertexData
			void* buffer_data;
			if (const auto hr = dyn_vb->buffer->Lock(dyn_vb->used, MODEL_VERTEX_STRIDE * surf->vertCount, &buffer_data, dyn_vb->used != 0 ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
				hr < 0)
			{
				Game::Com_Error(0, "Fatal lock error :: R_DrawXModelSkinnedUncached");
			}
			{
				for (auto i = 0u; i < surf->vertCount; i++)
				{
					// packed source vertex
					const auto src_vert = &surf->verts0[i];

					// position of our unpacked vert within the vertex buffer
					const auto v_pos_in_buffer = i * MODEL_VERTEX_STRIDE;
					const auto v = reinterpret_cast<unpacked_model_vert*>(((DWORD)buffer_data + v_pos_in_buffer));

					// vert pos
					v->pos[0] = src_vert->xyz[0];
					v->pos[1] = src_vert->xyz[1];
					v->pos[2] = src_vert->xyz[2];

					// unpack and assign vert normal
					unpack_normal(&src_vert->normal, v->normal);

					// uv's
					Game::Vec2UnpackTexCoords(src_vert->texCoord.packed, v->texcoord);
				}
			}

			dyn_vb->buffer->Unlock();
			const std::uint32_t vert_offset = dyn_vb->used;
			dyn_vb->used += (MODEL_VERTEX_STRIDE * surf->vertCount);

			// #
			// #

			set_stream_source(state, dyn_vb->buffer, vert_offset, MODEL_VERTEX_STRIDE);
		}

		// needed or game renders mesh with shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);

		// vertex format
		dev->SetFVF(MODEL_VERTEX_FORMAT);

		// #
		// build world matrix

		for (auto index = 0u; index < draw_stream->smodelCount; index++)
		{
			const auto inst = &Game::rgp->world->dpvs.smodelDrawInsts[draw_stream->smodelList[index]];

			//transform model into the scene by updating the worldmatrix
			float mtx[4][4] = {};
			RtxFixedFunction::build_worldmatrix_for_object(&mtx[0], inst->placement.axis, inst->placement.origin, inst->placement.scale);
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));


			// draw the prim
			dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, surf->vertCount, start_index, surf->triCount);
		}

		dev->SetFVF(NULL);
	}


	// *
	// world (bsp/terrain) drawing

	unsigned int R_ReadPrimDrawSurfInt(Game::GfxReadCmdBuf* cmdBuf)
	{
		return *cmdBuf->primDrawSurfPos++;
	}

	const unsigned int* R_ReadPrimDrawSurfData(Game::GfxReadCmdBuf* cmdBuf, unsigned int count)
	{
		const auto pos = cmdBuf->primDrawSurfPos;
		cmdBuf->primDrawSurfPos += count;

		return pos;
	}

	bool R_ReadBspPreTessDrawSurfs(Game::GfxReadCmdBuf* cmdBuf, Game::GfxBspPreTessDrawSurf** list, unsigned int* count, unsigned int* baseIndex)
	{
		*count = R_ReadPrimDrawSurfInt(cmdBuf);
		if (!*count)
		{
			return false;
		}

		*baseIndex = R_ReadPrimDrawSurfInt(cmdBuf);
		*list = (Game::GfxBspPreTessDrawSurf*)R_ReadPrimDrawSurfData(cmdBuf, *count);

		return true;
	}

	void R_DrawPreTessTris(Game::GfxCmdBufSourceState* src, Game::GfxCmdBufPrimState* state, Game::srfTriangles_t* tris, unsigned int baseIndex, unsigned int triCount, [[maybe_unused]] const Game::GfxSurface* surf)
	{
		const auto dev = Game::get_device();

		src->matrices.matrix[0].m[3][0] = 0.0f;
		src->matrices.matrix[0].m[3][1] = 0.0f;
		src->matrices.matrix[0].m[3][2] = 0.0f;
		src->matrices.matrix[0].m[3][3] = 1.0f;
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));

		// texture alpha + vertex alpha (decal blending)
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		dev->SetStreamSource(0, gfx_world_vertexbuffer, WORLD_VERTEX_STRIDE * tris->firstVertex, WORLD_VERTEX_STRIDE);
		dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, tris->vertexCount, baseIndex, triCount);
	}

	/**
	 * @brief completely rewritten R_DrawBspDrawSurfsPreTess to render bsp surfaces (world) using the fixed-function pipeline
	 */
	void R_DrawBspDrawSurfsPreTess(const unsigned int* primDrawSurfPos, Game::GfxCmdBufSourceState* src, Game::GfxCmdBufState* state)
	{
		const auto dev = Game::get_device();

		// #
		// setup fixed-function

		// vertex format
		dev->SetFVF(WORLD_VERTEX_FORMAT);

		// def. needed or the game will render the mesh using shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);


		// #
		// draw prims

		unsigned int base_index, count;
		Game::GfxBspPreTessDrawSurf* list;
		Game::GfxReadCmdBuf cmd_buf = { primDrawSurfPos };

		while (R_ReadBspPreTessDrawSurfs(&cmd_buf, &list, &count, &base_index))
		{
			Game::GfxSurface* bsp_surf = nullptr;
			Game::srfTriangles_t* prev_tris = nullptr;
			auto tri_count = 0u;
			auto base_vertex = -1;

			for (auto index = 0u; index < count; ++index)
			{
				const auto surf_index = static_cast<unsigned>(list[index].baseSurfIndex);
				if (surf_index >= static_cast<unsigned>(Game::rgp->world->surfaceCount))
				{
					__debugbreak();
					Game::Com_Error(1, "R_DrawBspDrawSurfsPreTess :: surf_index >= static_cast<unsigned>(game::rgp->world->surfaceCount)");
				}

				bsp_surf = &Game::rgp->world->dpvs.surfaces[surf_index];
				const auto tris = &bsp_surf->tris;

				if (base_vertex != bsp_surf->tris.firstVertex)
				{
					// never triggers?
					if (tri_count)
					{
						R_DrawPreTessTris(src, &state->prim, prev_tris, base_index, tri_count, bsp_surf);
						base_index += 3 * tri_count;
						tri_count = 0;
					}

					prev_tris = tris;
					base_vertex = tris->firstVertex;
				}

				tri_count += list[index].totalTriCount;
			}

			R_DrawPreTessTris(src, &state->prim, prev_tris, base_index, tri_count, bsp_surf);
		}

		dev->SetFVF(NULL);
	}


	/**
	 * @brief	completely rewritten R_TessBModel to render brushmodels using the fixed-function pipeline
	 */
	std::uint32_t R_TessBModel(const Game::GfxDrawSurfListArgs* list_args, [[maybe_unused]] void* x, [[maybe_unused]] void* y)
	{
		const auto source = list_args->context.source;
		const auto state = list_args->context.state;
		const auto prim = &list_args->context.state->prim;
		const auto dev = Game::get_device();

		// #
		// setup fixed-function

		// vertex format
		dev->SetFVF(WORLD_VERTEX_FORMAT);

		// def. needed or the game will render the mesh using shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);

		// texture alpha + vertex alpha (decal blending)
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);


		// #
		// draw prims

		const auto draw_surf_list = &list_args->info->drawSurfs[list_args->firstDrawSurfIndex];
		const auto draw_surf_count = list_args->info->drawSurfCount - list_args->firstDrawSurfIndex;

		const auto draw_surf_sub_mask = 0xFFFFFFFFE0000000;

		Game::GfxDrawSurf draw_surf = {};
		draw_surf.fields = draw_surf_list->fields;

		Game::GfxDrawSurf draw_surf_key = {};
		draw_surf_key.packed = draw_surf.packed & draw_surf_sub_mask;

		const std::uint64_t first_surf = draw_surf_key.packed;
		auto draw_surf_index = 0u;

		while (true)
		{
			const auto bsurf = reinterpret_cast<Game::BModelSurface*>(&source->input.data->surfsBuffer[4u * draw_surf.fields.objectId]);

			//transform model into the scene by updating the worldmatrix
			float mtx[4][4] = {};
			RtxFixedFunction::build_worldmatrix_for_object(&mtx[0], bsurf->placement->base.quat, bsurf->placement->base.origin, bsurf->placement->scale);
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));

			// #
			// ------

			const auto gfxsurf = bsurf->surf;
			const auto base_vertex = WORLD_VERTEX_STRIDE * gfxsurf->tris.firstVertex;

			set_stream_source(state, gfx_world_vertexbuffer, base_vertex, WORLD_VERTEX_STRIDE);

			const auto base_index = R_SetIndexData(prim, &Game::rgp->world->indices[gfxsurf->tris.baseIndex], gfxsurf->tris.triCount);
			dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, gfxsurf->tris.vertexCount, base_index, gfxsurf->tris.triCount);

			++draw_surf_index;
			if (draw_surf_index == draw_surf_count)
			{
				break;
			}

			draw_surf.fields = draw_surf_list[draw_surf_index].fields;
			draw_surf_key.packed = draw_surf_list[draw_surf_index].packed & draw_surf_sub_mask;

			if (draw_surf_key.packed != first_surf)
			{
				break;
			}
		}

		dev->SetFVF(NULL);
		return draw_surf_index;
	}

	// *
	// effects

	void R_TessCodeMeshList_begin(const Game::GfxDrawSurfListArgs* list_args)
	{
		const auto state = list_args->context.state;
		const auto dev = Game::get_device();

		// #
		// setup fixed-function

		// vertex format
		dev->SetFVF(FX_VERTEX_FORMAT);

		// def. needed or the game will render the mesh using shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);

		// vertices are already in 'world space' so origin is at 0 0 0
		Game::GfxMatrix world_mtx = {};
		world_mtx.m[0][0] = 1.0f;
		world_mtx.m[1][1] = 1.0f;
		world_mtx.m[2][2] = 1.0f;
		world_mtx.m[3][3] = 1.0f;
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&world_mtx.m[0]));

		// texture alpha + vertex alpha (decal blending)
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // D3DTA_TEXTURE
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // modulate is not enough

		// set stream source
		set_stream_source(state, RtxFixedFunction::dynamic_codemesh_vb, 0, FX_VERTEX_STRIDE);

		// R_TessCodeMeshList :: game code will now render all codemesh drawsurfs - functions nop'd:
		// > R_UpdateVertexDecl - sets vertexshader
		// > R_SetStreamSource  - sets vertexbuffer (codeMesh.vb)
	}

	// reset ff at the end of R_TessCodeMeshList
	void R_TessCodeMeshList_end()
	{
		const auto dev = Game::get_device();
		dev->SetFVF(NULL);
	}

	__declspec(naked) void R_TessCodeMeshList_begin_stub()
	{
		const static uint32_t retn_addr = 0x6244E8;
		__asm
		{
			pushad;
			push	eax; // listArgs
			call	R_TessCodeMeshList_begin;
			add		esp, 4;
			popad;

			// og code
			mov     ebx, [ecx + 4];
			mov     ecx, [ecx];
			jmp		retn_addr;
		}
	}

	__declspec(naked) void R_TessCodeMeshList_end_stub()
	{
		const static uint32_t retn_addr = 0x62474F;
		__asm
		{
			pushad;
			call	R_TessCodeMeshList_end;
			popad;

			// og code
			pop     edi;
			pop     esi;
			pop     ebx;
			mov     esp, ebp;
			jmp		retn_addr;
		}
	}

	// func is called twice, once for world vertices and once for packed vertices
	void R_TessMarkMeshList_begin([[maybe_unused]] const Game::GfxCmdBufState* state)
	{
		const auto dev = Game::get_device();
		const auto decl = state->prim.vertDeclType; // world or packed

		// #
		// setup fixed-function

		// vertex format
		dev->SetFVF(FX_MARKS_FORMAT);

		// def. needed or the game will render the mesh using shaders
		dev->SetVertexShader(nullptr);
		dev->SetPixelShader(nullptr);

		// vertices are already in 'world space' so origin is at 0 0 0
		Game::GfxMatrix world_mtx = {};
		world_mtx.m[0][0] = 1.0f;
		world_mtx.m[1][1] = 1.0f;
		world_mtx.m[2][2] = 1.0f;
		world_mtx.m[3][3] = 1.0f;
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&world_mtx.m[0]));

		// texture alpha + vertex alpha (decal blending)
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // D3DTA_TEXTURE
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // modulate is not enough

		// set stream source
		dev->SetStreamSource(0,
			decl == Game::VERTDECL_WORLD ? RtxFixedFunction::dynamic_markmesh_world_vb : RtxFixedFunction::dynamic_markmesh_model_vb,
			0, FX_MARKS_STRIDE);
	}

	__declspec(naked) void R_TessMarkMeshList_begin_stub()
	{
		const static uint32_t retn_addr = 0x6248A8;
		__asm
		{
			pushad;
			push	edi;
			call	R_TessMarkMeshList_begin;
			add		esp, 4;
			popad;

			jmp		retn_addr;
		}
	}

	// *
	// build / copy / unpack vertex buffers

	/**
	 * @brief  copies / unpacks effects vertex buffer (codeMesh) before the frame gets rendered by the "backend"
	 */
	void RtxFixedFunction::copy_fx_buffer()
	{
		const auto dev = Game::get_device();
		const auto frontend_data = Game::get_frontenddata();

		// alloc buffer on first use
		// released on map shutdown - free_fixed_function_buffers
		if (!dynamic_codemesh_vb)
		{
			//const int additional_size_to_add = (FX_VERTEX_STRIDE - MODEL_VERTEX_STRIDE) * 0x4000;
			if (const auto hr = dev->CreateVertexBuffer(frontend_data->codeMesh.vb.total, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, FX_VERTEX_FORMAT, D3DPOOL_DEFAULT, &dynamic_codemesh_vb, nullptr);
				hr > 0)
			{
				__debugbreak();
				Game::Com_Error(0, "rtx_fixed_function::copy_fx_buffer :: Failed to create dynamic vertexbufffer (dynamic_codemesh_vb)");
			}
		}

		// #
		// lock codemesh vb -> unpack vertex data and place new data into the dynamic_codemesh_vb

		void* og_buffer_data;
		if (const auto hr = frontend_data->codeMesh.vb.buffer->Lock(0, frontend_data->codeMesh.vb.used, &og_buffer_data, D3DLOCK_READONLY);
			hr < 0)
		{
			__debugbreak();
			Game::Com_Error(0, "rtx_fixed_function::copy_fx_buffer :: failed locking the codeMesh buffer");
		}

		// #

		void* buffer_data = nullptr;
		if (const auto hr = dynamic_codemesh_vb->Lock(0, frontend_data->codeMesh.vb.used, &buffer_data, D3DUSAGE_WRITEONLY);
			hr >= 0)
		{
			for (auto i = 0u; i * frontend_data->codeMesh.vertSize < (unsigned)frontend_data->codeMesh.vb.used && i < 0x4000; i++)
			{
				// position of vert within the vertex buffer
				const auto v_pos_in_buffer = i * frontend_data->codeMesh.vertSize; // size of GfxPackedVertex

				// interpret GfxPackedVertex as unpacked_fx_vert
				const auto v = reinterpret_cast<unpacked_fx_vert*>(((DWORD)buffer_data + v_pos_in_buffer));

				// interpret GfxPackedVertex as GfxPackedVertex 
				const auto src_vert = reinterpret_cast<Game::GfxPackedVertex*>(((DWORD)og_buffer_data + v_pos_in_buffer));

				// vert pos
				v->pos[0] = src_vert->xyz[0];
				v->pos[1] = src_vert->xyz[1];
				v->pos[2] = src_vert->xyz[2];

				// packed vertex color : used for color and alpha manip.
				//v->color = src_vert->color.packed;

				auto alpha_scalar = 1.0f;
				/*if (dvars::rtx_fx_alpha_scalar)
				{
					alpha_scalar = dvars::rtx_fx_alpha_scalar->current.value;
				}*/

				Game::GfxColor packed_color = {};
				const auto alpha_val = static_cast<float>((double)src_vert->color.array[3] * 0.003921568859368563) * alpha_scalar;
				packed_color.array[3] = Game::Byte1PackClamp(alpha_val);
				packed_color.array[0] = src_vert->color.array[0];
				packed_color.array[1] = src_vert->color.array[1];
				packed_color.array[2] = src_vert->color.array[2];
				v->color = packed_color.packed;

				// uv's
				Game::Vec2UnpackTexCoords(src_vert->texCoord.packed, v->texcoord);

				// populate second set of texcoords, just in case
				v->unused[0] = v->texcoord[0];
				v->unused[1] = v->texcoord[1];
			}

			dynamic_codemesh_vb->Unlock();
		}
		else
		{
			dynamic_codemesh_vb->Release();
			dynamic_codemesh_vb = nullptr;
		}

		// unlock og codemesh vb
		frontend_data->codeMesh.vb.buffer->Unlock();
	}

	void RtxFixedFunction::copy_marks_buffer()
	{
		const auto dev = Game::get_device();
		const auto frontend_data = Game::get_frontenddata();

		// alloc buffer on first use
		// released on map shutdown - free_fixed_function_buffers
		if (!dynamic_markmesh_world_vb)
		{
			if (const auto hr = dev->CreateVertexBuffer(frontend_data->markMesh.vb.total, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, FX_MARKS_FORMAT, D3DPOOL_DEFAULT, &dynamic_markmesh_world_vb, nullptr);
				hr > 0)
			{
				__debugbreak();
				Game::Com_Error(0, "rtx_fixed_function::copy_marks_buffer :: Failed to create dynamic vertexbufffer (dynamic_markmesh_vb)");
			}
		}

		// 
		if (!dynamic_markmesh_model_vb)
		{
			if (const auto hr = dev->CreateVertexBuffer(frontend_data->markMesh.vb.total, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, FX_MARKS_FORMAT, D3DPOOL_DEFAULT, &dynamic_markmesh_model_vb, nullptr);
				hr > 0)
			{
				__debugbreak();
				Game::Com_Error(0, "rtx_fixed_function::copy_marks_buffer :: Failed to create dynamic vertexbufffer (dynamic_markmesh_model_vb)");
			}
		}


		// #
		// lock markmesh vb -> unpack vertex data and place new data into the dynamic_markmesh_world_vb & dynamic_markmesh_model_vb

		void* og_buffer_data;
		if (const auto hr = frontend_data->markMesh.vb.buffer->Lock(0, frontend_data->markMesh.vb.used, &og_buffer_data, D3DLOCK_READONLY);
			hr < 0)
		{
			__debugbreak();
			Game::Com_Error(0, "rtx_fixed_function::copy_marks_buffer :: failed locking the markMesh buffer");
		}

		// #

		void* buffer_data = nullptr; void* buffer_data_model = nullptr;
		if (auto hr = dynamic_markmesh_world_vb->Lock(0, frontend_data->markMesh.vb.used, &buffer_data, D3DUSAGE_WRITEONLY);
			hr >= 0)
		{
			if (hr = dynamic_markmesh_model_vb->Lock(0, frontend_data->markMesh.vb.used, &buffer_data_model, D3DUSAGE_WRITEONLY);
				hr >= 0)
			{
				memset(buffer_data, 0, frontend_data->markMesh.vb.used);
				memset(buffer_data_model, 0, frontend_data->markMesh.vb.used);
				for (auto i = 0u; i * frontend_data->markMesh.vertSize < (unsigned)frontend_data->markMesh.vb.used && i < 0x1800; i++)
				{
					// position of vert within the vertex buffer
					const auto v_pos_in_buffer = i * frontend_data->markMesh.vertSize; // size of GfxWorldVertex

					// vert in our custom buffers
					const auto v = reinterpret_cast<unpacked_mark_vert*>(((DWORD)buffer_data + v_pos_in_buffer));
					const auto v_model = reinterpret_cast<unpacked_mark_vert*>(((DWORD)buffer_data_model + v_pos_in_buffer));

					// source vertex (can be GfxPackedVertex or GfxWorldVertex)
					const auto src_vert = reinterpret_cast<Game::GfxWorldVertex*>(((DWORD)og_buffer_data + v_pos_in_buffer));
					const auto src_vert_model = reinterpret_cast<Game::GfxPackedVertex*>(((DWORD)og_buffer_data + v_pos_in_buffer));

					// packed vertex color : used for color and alpha manip.
					v->color = src_vert->color.packed;
					v_model->color = v->color;

					// there are two vertex types in the og buffer
					// 1) world (bsp brush) verts with unpacked texcoords (stride = 44)
					// 2) model verts with packed texcoords (stride = 36)
					// -> determine the type by checking if the last 2 elements are 0
					// ^ not reliable - using two buffers now
					//if (src_vert->normal.packed == 0u && src_vert->tangent.packed == 0u)
					{
						// unpack and assign vert normal
						unpack_normal(&src_vert_model->normal, v_model->normal); // offset of packed normal differs
						Game::Vec2UnpackTexCoords(src_vert_model->texCoord.packed, v_model->texcoord);


					}
					//else
					{
						unpack_normal(&src_vert->normal, v->normal); // offset of packed normal differs
						v->texcoord[0] = src_vert->texCoord[0];
						v->texcoord[1] = src_vert->texCoord[1];
					}

					// populate second set of texcoords, just in case
					v->unused[0] = 0.0f;
					v->unused[1] = 0.0f;
					v_model->unused[0] = 0.0f;
					v_model->unused[1] = 0.0f;

					// vert pos + slightly offset mark along its surface normal to reduce z conflicts
					v->pos[0] = src_vert->xyz[0]; //+ (v->normal[0] * 0.001f * static_cast<float>((i % 100)));
					v->pos[1] = src_vert->xyz[1]; //+ (v->normal[1] * 0.001f * static_cast<float>((i % 100)));
					v->pos[2] = src_vert->xyz[2]; //+ (v->normal[2] * 0.001f * static_cast<float>((i % 100)));

					v_model->pos[0] = src_vert->xyz[0];
					v_model->pos[1] = src_vert->xyz[1];
					v_model->pos[2] = src_vert->xyz[2];
				}

				dynamic_markmesh_model_vb->Unlock();
			}
			else
			{
				dynamic_markmesh_model_vb->Release();
				dynamic_markmesh_model_vb = nullptr;
			}

			dynamic_markmesh_world_vb->Unlock();
		}
		else
		{
			dynamic_markmesh_world_vb->Release();
			dynamic_markmesh_world_vb = nullptr;
		}

		// unlock og codemesh vb
		frontend_data->markMesh.vb.buffer->Unlock();
	}

	void r_endframe_hk()
	{
		// og call - R_EndFrame
		Utils::Hook::Call<void(__cdecl)()>(0x5DC330)();

		RtxFixedFunction::copy_fx_buffer();
		RtxFixedFunction::copy_marks_buffer();
	}

	/**
	 * @brief	builds static vertex buffer for world geometry (BSP)
	 */
	void build_gfxworld_buffers()
	{
		const auto dev = Game::get_device();
		void* vertex_buffer_data = nullptr;

		if (Game::rgp->world)
		{
			if (gfx_world_vertexbuffer)
			{
				gfx_world_vertexbuffer->Release();
				gfx_world_vertexbuffer = nullptr;

				__debugbreak();
				Game::Com_Error(1, "build_gfxworld_buffers :: gfx_world_vertexbuffer != nullptr");
			}

			if (auto hr = dev->CreateVertexBuffer(WORLD_VERTEX_STRIDE * Game::rgp->world->vertexCount, D3DUSAGE_WRITEONLY, WORLD_VERTEX_FORMAT, D3DPOOL_DEFAULT, &gfx_world_vertexbuffer, nullptr);
				hr >= 0)
			{
				if (hr = gfx_world_vertexbuffer->Lock(0, 0, &vertex_buffer_data, 0);
					hr >= 0)
				{
					for (auto i = 0u; i < Game::rgp->world->vertexCount; i++)
					{
						// packed source vertex
						const auto src_vert = &Game::rgp->world->vd.vertices[i];

						// position of our unpacked vert within the vertex buffer
						const auto v_pos_in_buffer = i * WORLD_VERTEX_STRIDE; // pos-xyz ; normal-xyz ; texcoords uv = 32 byte 
						const auto v = reinterpret_cast<unpacked_world_vert*>(((DWORD)vertex_buffer_data + v_pos_in_buffer));

						// vert pos
						v->pos[0] = src_vert->xyz[0];
						v->pos[1] = src_vert->xyz[1];
						v->pos[2] = src_vert->xyz[2];

						// unpack and assign vert normal
						unpack_normal(&src_vert->normal, v->normal);

						// packed vertex color : used for alpha blending of decals
						v->color = src_vert->color.packed;

						// uv's
						v->texcoord[0] = src_vert->texCoord[0];
						v->texcoord[1] = src_vert->texCoord[1];
					}

					gfx_world_vertexbuffer->Unlock();
				}
				else
				{
					gfx_world_vertexbuffer->Release();
					gfx_world_vertexbuffer = nullptr;
				}
			}
		}
	}

	void build_static_model_buffers()
	{
		// builds buffers at runtime if 'rtx_warm_smodels' is disabled
		//if (dvars::rtx_warm_smodels && dvars::rtx_warm_smodels->current.enabled)
		{
			Game::DB_EnumXAssets(Game::XAssetType::ASSET_TYPE_XMODEL, [](Game::XAssetHeader header, [[maybe_unused]] void* data)
			{
				// ignore deformed (skinned) models because they use more then 2 partBits (partBit 3-4 are used for custom buffers)
				// vertex buffers for skinned models are created dynamically
				if (!header.model->surfs->deformed)
				{
					XModelOptimize(header.model, true);
				}
			}, nullptr, false);
		}
	}

	// called on map load (cg_init)
	__declspec(naked) void init_fixed_function_buffers_stub()
	{
		const static uint32_t stock_func = 0x421410; // hud init func
		const static uint32_t retn_addr = 0x42118C;
		__asm
		{
			pushad;
			call	build_static_model_buffers;
			call	build_gfxworld_buffers;
			call	Rtx::on_map_load;
			popad;

			call	stock_func;
			jmp		retn_addr;
		}
	}

	// *
	// cleanup buffers

	void free_fixed_function_buffers()
	{
		// #
		// cleanup world buffer

		if (gfx_world_vertexbuffer)
		{
			gfx_world_vertexbuffer->Release();
			gfx_world_vertexbuffer = nullptr;
		}

		// cleanup effects buffer
		if (RtxFixedFunction::dynamic_codemesh_vb)
		{
			RtxFixedFunction::dynamic_codemesh_vb->Release();
			RtxFixedFunction::dynamic_codemesh_vb = nullptr;
		}

		// cleanup marks buffer
		if (RtxFixedFunction::dynamic_markmesh_world_vb)
		{
			RtxFixedFunction::dynamic_markmesh_world_vb->Release();
			RtxFixedFunction::dynamic_markmesh_world_vb = nullptr;
		}
		if (RtxFixedFunction::dynamic_markmesh_model_vb)
		{
			RtxFixedFunction::dynamic_markmesh_model_vb->Release();
			RtxFixedFunction::dynamic_markmesh_model_vb = nullptr;
		}

		// #
		// cleanup model buffer

		Game::DB_EnumXAssets(Game::XAssetType::ASSET_TYPE_XMODEL, [](Game::XAssetHeader header, [[maybe_unused]] void* data)
		{
			const auto dev = Game::get_device();

			// ignore deformed (skinned) models because they might use more then 2 partBits (partBit 4 is used for custom vertex buffer)
			if (dev && !header.model->surfs->deformed && header.model->surfs->custom_vertexbuffer)
			{
				Game::XSurface* surfaces;

				const auto lod_count = header.model->numLods;
				for (auto lod_index = 0; lod_index < lod_count; ++lod_index)
				{
					const auto surf_count = XModelGetSurfaces(header.model, &surfaces, lod_index);
					for (auto surf_index = 0; surf_index < surf_count; ++surf_index)
					{
						if (!surfaces[surf_index].deformed && surfaces[surf_index].custom_vertexbuffer)
						{
							surfaces[surf_index].custom_vertexbuffer->Release();
							surfaces[surf_index].custom_vertexbuffer = nullptr;
						}
					}
				}
			}
		}, nullptr, false);
	}

	// called on renderer shutdown (R_Shutdown)
	__declspec(naked) void free_fixed_function_buffers_stub()
	{
		const static uint32_t stock_func = 0x616460; // R_ResetModelLighting
		const static uint32_t retn_addr = 0x5D9D17;
		__asm
		{
			pushad;
			call	free_fixed_function_buffers;
			call	Rtx::on_map_shutdown;
			popad;

			call	stock_func;
			jmp		retn_addr;
		}
	}

	// ------------------------------------

	RtxFixedFunction::RtxFixedFunction()
	{
		// fixed function rendering of static models (R_TessStaticModelRigidDrawSurfList)
		//if (Flags::HasFlag("fixed_function"))
		{
			// fixed-function rendering of static models (R_TessStaticModelRigidDrawSurfList)
			Utils::Hook(0x633E60, R_DrawStaticModelDrawSurfNonOptimized, HOOK_CALL).install()->quick();

			// fixed-function rendering of rigid xmodels - call 1 (RB_TessXModelRigidDrawSurfList-> R_DrawXModelSurfCamera-> R_DrawXModelRigidModelSurf1
			Utils::Hook(0x635680, R_DrawXModelRigidModelSurf1_stub, HOOK_JUMP).install()->quick();

			// ^ call 2
			Utils::Hook(0x635A19, R_DrawXModelRigidModelSurf2_stub, HOOK_JUMP).install()->quick();

			// fixed-function rendering of skinned (animated) models (R_TessXModelSkinnedDrawSurfList)
			Utils::Hook::Nop(0x625802, 3); // nop eax overwrite before calling our stub (we need the ptr in eax)
			Utils::Hook(0x625808, R_DrawXModelSkinnedUncached_stub, HOOK_JUMP).install()->quick();

			// fixed-function rendering of static skinned models
			Utils::Hook(0x6345DE, R_DrawStaticModelsSkinnedDrawSurf, HOOK_CALL).install()->quick();

			// fixed-function rendering of world surfaces (R_TessTrianglesPreTessList)
			Utils::Hook(0x627014, R_DrawBspDrawSurfsPreTess, HOOK_CALL).install()->quick();

			// fixed-function rendering of brushmodels
			Utils::Hook(0x627030, R_TessBModel, HOOK_JUMP).install()->quick();

			// fixed-function rendering of effects - R_TessCodeMeshList (particle clounds are still shader based)
			if (!Flags::HasFlag("stock_effects"))
			{
				Utils::Hook::Nop(0x6245A1, 5); // R_UpdateVertexDecl
				Utils::Hook::Nop(0x6245EB, 5); // R_SetStreamSource
				Utils::Hook(0x6244E3, R_TessCodeMeshList_begin_stub, HOOK_JUMP).install()->quick();
				Utils::Hook(0x62474A, R_TessCodeMeshList_end_stub, HOOK_JUMP).install()->quick();
				Utils::Hook(0x44789D, r_endframe_hk, HOOK_CALL).install()->quick(); // copy & unpack data of effect vertex buffer 
			}

			// R_TessMarkMeshList
			Utils::Hook::Nop(0x624A23, 5); // R_SetLightmap
			Utils::Hook::Nop(0x624AA4, 5); // R_SetSampler - reflectionProbe
			Utils::Hook::Nop(0x6248E9, 5); // R_SetStreamSource
			Utils::Hook(0x6248A3, R_TessMarkMeshList_begin_stub, HOOK_JUMP).install()->quick();

			// ----

			// on map load :: build custom buffers for fixed-function rendering
			Utils::Hook(0x421187, init_fixed_function_buffers_stub, HOOK_JUMP).install()->quick(); // CG_Init :: some hud init func

			// on renderer shutdown :: release custom buffers used by fixed-function rendering
			Utils::Hook(0x5D9D12, free_fixed_function_buffers_stub, HOOK_JUMP).install()->quick(); // R_Shutdown :: R_ResetModelLighting call
		}
	}
}