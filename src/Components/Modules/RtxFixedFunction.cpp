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

	// *
	// static models (rigid)

	bool XSurfaceOptimize(Game::XSurface* surf)
	{
		const auto dev = *Game::dx9_device_ptr;
		bool allocated_any = false;

		// setup vertexbuffer
		{
			//XSurfaceOptimizeRigid(model, surf);
			if (!surf->custom_vertexbuffer)
			{
				void* vertex_buffer_data = nullptr;
				const auto vertex_bytes = surf->vertCount * MODEL_VERTEX_STRIDE;

				if (auto hr = dev->CreateVertexBuffer(vertex_bytes, 8, 0, D3DPOOL_DEFAULT, &surf->custom_vertexbuffer, nullptr);
					hr >= 0)
				{
					if (hr = surf->custom_vertexbuffer->Lock(0, 0, &vertex_buffer_data, 0);
						hr >= 0)
					{
						// we need to unpack normal and texcoords in 'GfxPackedVertex' to be able to use them for fixed-function rendering
						for (auto i = 0; i < surf->vertCount; i++)
						{
							// packed source vertex
							const auto src_vert = surf->verts0[i];

							// position of our unpacked vert within the vertex buffer
							const auto v_pos_in_buffer = i * MODEL_VERTEX_STRIDE;
							const auto v = reinterpret_cast<unpacked_model_vert*>(((DWORD)vertex_buffer_data + v_pos_in_buffer));

							// assign pos
							v->pos[0] = src_vert.xyz[0];
							v->pos[1] = src_vert.xyz[1];
							v->pos[2] = src_vert.xyz[2];

							// unpack normal and texcoords
							const auto scale = static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[3])) * (1.0f / 255.0f) + 0.7529412f;
							v->normal[0] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
							v->normal[1] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
							v->normal[2] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[2])) * (1.0f / 127.0f) + -1.0f) * scale;

							Game::Vec2UnpackTexCoords(src_vert.texCoord.packed, v->texcoord);
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
	 * @brief builds a world matrix for the mesh
	 */
	void R_DrawStaticModelDrawSurfPlacement(Game::GfxCmdBufSourceState* src, const Game::GfxStaticModelDrawInst* inst, bool fixed_function = true)
	{
		const auto scale = inst->placement.scale;

		// inlined ikMatrixSet44
		src->matrices.matrix[0].m[0][0] = inst->placement.axis[0][0] * scale;
		src->matrices.matrix[0].m[0][1] = inst->placement.axis[0][1] * scale;
		src->matrices.matrix[0].m[0][2] = inst->placement.axis[0][2] * scale;
		src->matrices.matrix[0].m[0][3] = 0.0f;

		src->matrices.matrix[0].m[1][0] = inst->placement.axis[1][0] * scale;
		src->matrices.matrix[0].m[1][1] = inst->placement.axis[1][1] * scale;
		src->matrices.matrix[0].m[1][2] = inst->placement.axis[1][2] * scale;
		src->matrices.matrix[0].m[1][3] = 0.0f;

		src->matrices.matrix[0].m[2][0] = inst->placement.axis[2][0] * scale;
		src->matrices.matrix[0].m[2][1] = inst->placement.axis[2][1] * scale;
		src->matrices.matrix[0].m[2][2] = inst->placement.axis[2][2] * scale;
		src->matrices.matrix[0].m[2][3] = 0.0f;

		src->matrices.matrix[0].m[3][0] = inst->placement.origin[0];
		src->matrices.matrix[0].m[3][1] = inst->placement.origin[1];
		src->matrices.matrix[0].m[3][2] = inst->placement.origin[2];
		src->matrices.matrix[0].m[3][3] = 1.0f;

		if (!fixed_function)
		{
			src->matrices.matrix[0].m[3][0] -= src->eyeOffset[0];
			src->matrices.matrix[0].m[3][1] -= src->eyeOffset[1];
			src->matrices.matrix[0].m[3][2] -= src->eyeOffset[2];
		}
	}

	/**
	 * @brief completely rewritten R_DrawStaticModelDrawSurfNonOptimized to render static models using the fixed-function pipeline
	 */
	void R_DrawStaticModelDrawSurfNonOptimized(const Game::GfxStaticModelDrawStream* drawstream, Game::GfxCmdBufSourceState* src, Game::GfxCmdBufState* cmd)
	{
		const auto smodel_count = drawstream->smodelCount;
		const auto smodel_list = (const Game::GfxStaticModelDrawStream*) reinterpret_cast<const void*>(drawstream->smodelList);
		const auto draw_inst = Game::rgp->world->dpvs.smodelDrawInsts;
		const auto dev = *Game::dx9_device_ptr;

		// fixed function rendering
		bool render_ff = true;

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
		if (cmd->prim.indexBuffer != mem->indexBuffer)
		{
			cmd->prim.indexBuffer = mem->indexBuffer;
			dev->SetIndices(mem->indexBuffer);
		}

		// custom vertexbuffer
		if (drawstream->localSurf->custom_vertexbuffer)
		{
			cmd->prim.streams[0].vb = drawstream->localSurf->custom_vertexbuffer;
			cmd->prim.streams[0].offset = 0;
			cmd->prim.streams[0].stride = MODEL_VERTEX_STRIDE;
			dev->SetStreamSource(0, drawstream->localSurf->custom_vertexbuffer, 0, MODEL_VERTEX_STRIDE);
		}
		else // fallback to shader rendering if there is no custom vertexbuffer
		{
			render_ff = false;
			const auto offset = (char*)drawstream->localSurf->verts0 - mem->blocks[7].data;

			cmd->prim.streams[0].vb = mem->vertexBuffer;
			cmd->prim.streams[0].offset = offset;
			cmd->prim.streams[0].stride = MODEL_VERTEX_STRIDE;
			dev->SetStreamSource(0, mem->vertexBuffer, offset, MODEL_VERTEX_STRIDE);
		}

		IDirect3DVertexShader9* og_vertex_shader = nullptr;
		IDirect3DPixelShader9* og_pixel_shader = nullptr;

		if (render_ff)
		{
			// vertex format
			dev->SetFVF(MODEL_VERTEX_FORMAT);

			// save shaders
			dev->GetVertexShader(&og_vertex_shader);
			dev->GetPixelShader(&og_pixel_shader);

			// def. needed or the game will render the mesh using shaders
			dev->SetVertexShader(nullptr);
			dev->SetPixelShader(nullptr);
		}

		// #
		// draw prim

		for (auto index = 0u; index < smodel_count; index++)
		{
			const auto inst = &draw_inst[*((std::uint16_t*)&smodel_list->primDrawSurfPos + index)];

			// transform model into the scene by updating the worldmatrix
			R_DrawStaticModelDrawSurfPlacement(src, inst, render_ff);
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));

			// get indexbuffer offset
			const auto offset = ((char*)drawstream->localSurf->triIndices - mem->blocks[8].data) >> 1;

			// draw the prim
			cmd->prim.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, drawstream->localSurf->vertCount, offset, drawstream->localSurf->triCount);
		}

		if (render_ff)
		{
			// #
			// restore everything for following meshes rendered via shaders

			if (og_vertex_shader) dev->SetVertexShader(og_vertex_shader);
			if (og_pixel_shader) dev->SetPixelShader(og_pixel_shader);

			// restore world matrix
			Rtx::r_set_3d();
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&Game::gfxCmdBufSourceState->matrices.matrix[0].m));

			dev->SetFVF(NULL);
		}
	}


	// *
	// xmodels (rigid/skinned)

	void R_DrawXModelRigidModelSurf(Game::GfxModelRigidSurface* model, [[maybe_unused]] Game::GfxCmdBufSourceState* source, Game::GfxCmdBufState* state)
	{
		const auto dev = *Game::dx9_device_ptr;
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
		}

		// #
		// set streams

		// index and vertex buffers are within the fastfile that includes the model
		const auto mem = &Game::g_zones[surf->zoneHandle].mem;

		state->prim.indexBuffer = mem->indexBuffer;
		state->prim.device->SetIndices(mem->indexBuffer);

		float custom_scalar = 1.0f;

		// skysphere materials need to have sort = sky in assetmanager
		if (state->material && state->material->info.sortKey == 5)
		{
			custom_scalar = 1000.0f; //Rtx_gui::skysphere_scale;
		}

		// #
		// build world matrix

		float model_axis[3][3] = {};
		RtxUtils::unit_quat_to_axis(model->placement.base.quat, model_axis);

		//const auto mtx = source->matrices.matrix[0].m;
		float mtx[4][4] = {};
		const auto scale = model->placement.scale;

		// inlined ikMatrixSet44
		mtx[0][0] = model_axis[0][0] * scale * custom_scalar;
		mtx[0][1] = model_axis[0][1] * scale * custom_scalar;
		mtx[0][2] = model_axis[0][2] * scale * custom_scalar;
		mtx[0][3] = 0.0f;

		mtx[1][0] = model_axis[1][0] * scale * custom_scalar;
		mtx[1][1] = model_axis[1][1] * scale * custom_scalar;
		mtx[1][2] = model_axis[1][2] * scale * custom_scalar;
		mtx[1][3] = 0.0f;

		mtx[2][0] = model_axis[2][0] * scale * custom_scalar;
		mtx[2][1] = model_axis[2][1] * scale * custom_scalar;
		mtx[2][2] = model_axis[2][2] * scale * custom_scalar;
		mtx[2][3] = 0.0f;

		mtx[3][0] = model->placement.base.origin[0];
		mtx[3][1] = model->placement.base.origin[1];
		mtx[3][2] = model->placement.base.origin[2];
		mtx[3][3] = 1.0f;


		IDirect3DVertexShader9* og_vertex_shader = nullptr;
		IDirect3DPixelShader9* og_pixel_shader = nullptr;

		if (surf->custom_vertexbuffer)
		{
			// save shaders
			dev->GetVertexShader(&og_vertex_shader);
			dev->GetPixelShader(&og_pixel_shader);

			// def. needed or the game will render the mesh using shaders
			dev->SetVertexShader(nullptr);
			dev->SetPixelShader(nullptr);

			state->prim.streams[0].vb = surf->custom_vertexbuffer;
			state->prim.streams[0].offset = 0;
			state->prim.streams[0].stride = MODEL_VERTEX_STRIDE;
			state->prim.device->SetStreamSource(0, surf->custom_vertexbuffer, 0, MODEL_VERTEX_STRIDE);

			// vertex format
			dev->SetFVF(MODEL_VERTEX_FORMAT);

			// set world matrix
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));
		}
		else // fallback to shader rendering in case there is an xmodel without a custom vertexbuffer (shouldn't happen?)
		{
			rendering_ff = false;
			const auto offset = (char*)surf->verts0 - mem->blocks[7].data;

			state->prim.streams[0].vb = mem->vertexBuffer;
			state->prim.streams[0].offset = offset;
			state->prim.streams[0].stride = MODEL_VERTEX_STRIDE;
			dev->SetStreamSource(0, mem->vertexBuffer, offset, MODEL_VERTEX_STRIDE);

			// setup vertex shader constants
			const auto primArgCount = static_cast<unsigned int>(state->pass->perPrimArgCount);
			const auto materialArgs = state->pass->args;

			// untested stub
			if (primArgCount)
			{
				// R_SetupPassPerPrimArgs
				const static uint32_t func_addr = 0x627D00;
				__asm
				{
					pushad;
					push	state;
					push	source;
					push	primArgCount;
					mov		eax, materialArgs;
					call	func_addr;
					add		esp, 12;
					popad;
				}
			}
		}

		// #
		// draw prim

		const auto offset = ((char*)surf->triIndices - mem->blocks[8].data) >> 1;
		state->prim.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, surf->vertCount, offset, surf->triCount);


		// #
		// restore everything for following meshes rendered via shaders

		if (rendering_ff)
		{
			if (og_vertex_shader) dev->SetVertexShader(og_vertex_shader);
			if (og_pixel_shader) dev->SetPixelShader(og_pixel_shader);
			dev->SetFVF(NULL);
		}
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

	int R_SetIndexData(Game::GfxCmdBufPrimState* state, const unsigned __int16* indices, int tri_count)
	{
		const auto index_count = 3 * tri_count;

		if (index_count + Game::gfx_buf->dynamicIndexBuffer->used > Game::gfx_buf->dynamicIndexBuffer->total)
		{
			Game::gfx_buf->dynamicIndexBuffer->used = 0;
		}

		if (!Game::gfx_buf->dynamicIndexBuffer->used)
		{
			Game::gfx_buf->dynamicIndexBuffer = Game::gfx_buf->dynamicIndexBufferPool;
		}

		void* buffer_data;
		if (const auto hr = Game::gfx_buf->dynamicIndexBuffer->buffer->Lock(2 * Game::gfx_buf->dynamicIndexBuffer->used, 6 * tri_count, &buffer_data, Game::gfx_buf->dynamicIndexBuffer->used != 0 ? 0x1000 : 0x2000);
			hr < 0)
		{
			//R_FatalLockError(hr);
			Game::Com_Error(1, "Fatal lock error :: R_SetIndexData");
		}

		memcpy(buffer_data, indices, 2 * index_count);
		Game::gfx_buf->dynamicIndexBuffer->buffer->Unlock();

		if (state->indexBuffer != Game::gfx_buf->dynamicIndexBuffer->buffer)
		{
			state->indexBuffer = Game::gfx_buf->dynamicIndexBuffer->buffer;
			state->device->SetIndices(Game::gfx_buf->dynamicIndexBuffer->buffer);
		}

		const auto base_index = Game::gfx_buf->dynamicIndexBuffer->used;
		Game::gfx_buf->dynamicIndexBuffer->used += index_count;

		return base_index;
	}

	// GfxModelSkinnedSurface, verts , src , state
	void R_DrawXModelSkinnedUncached(Game::GfxModelSkinnedSurface* skinned_surf, [[maybe_unused]] Game::GfxPackedVertex* verts, Game::GfxCmdBufSourceState* src, Game::GfxCmdBufState* state)
	{
		// fixed function rendering
		constexpr auto render_ff = true;

		const auto surf = skinned_surf->xsurf;
		const auto start_index = R_SetIndexData(&state->prim, surf->triIndices, surf->triCount);

		if ((int)(MODEL_VERTEX_STRIDE * surf->vertCount + Game::gfx_buf->dynamicVertexBuffer->used) > Game::gfx_buf->dynamicVertexBuffer->total)
		{
			Game::gfx_buf->dynamicVertexBuffer->used = 0;
		}

		// R_SetVertexData
		void* buffer_data;
		if (const auto hr = Game::gfx_buf->dynamicVertexBuffer->buffer->Lock(Game::gfx_buf->dynamicVertexBuffer->used, MODEL_VERTEX_STRIDE * surf->vertCount, &buffer_data, Game::gfx_buf->dynamicVertexBuffer->used != 0 ? 0x1000 : 0x2000);
			hr < 0)
		{
			//R_FatalLockError(hr);
			Game::Com_Error(1, "Fatal lock error :: R_DrawXModelSkinnedUncached");
		}

		if (!render_ff)
		{
			memcpy(buffer_data, skinned_surf->u.skinnedVert, MODEL_VERTEX_STRIDE * surf->vertCount);
		}
		else
		{
			for (auto i = 0u; i < surf->vertCount; i++)
			{
				// packed source vertex
				const auto src_vert = skinned_surf->u.skinnedVert[i];

				// position of our unpacked vert within the vertex buffer
				const auto v_pos_in_buffer = i * MODEL_VERTEX_STRIDE;
				const auto v = reinterpret_cast<unpacked_model_vert*>(((DWORD)buffer_data + v_pos_in_buffer));

				// vert pos
				v->pos[0] = src_vert.xyz[0];
				v->pos[1] = src_vert.xyz[1];
				v->pos[2] = src_vert.xyz[2];

				// unpack and assign vert normal

				// normal unpacking in a cod4 hlsl shader:
				// temp0	 = i.normal * float4(0.007874016, 0.007874016, 0.007874016, 0.003921569) + float4(-1, -1, -1, 0.7529412);
				// temp0.xyz = temp0.www * temp0;

				const auto scale = static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[3])) * (1.0f / 255.0f) + 0.7529412f;
				v->normal[0] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
				v->normal[1] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
				v->normal[2] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[2])) * (1.0f / 127.0f) + -1.0f) * scale;

				// uv's
				Game::Vec2UnpackTexCoords(src_vert.texCoord.packed, v->texcoord);
			}

		}

		Game::gfx_buf->dynamicVertexBuffer->buffer->Unlock();
		const std::uint32_t vert_offset = Game::gfx_buf->dynamicVertexBuffer->used;
		Game::gfx_buf->dynamicVertexBuffer->used += (MODEL_VERTEX_STRIDE * surf->vertCount);


		// #
		// #

		if (state->prim.streams[0].vb != Game::gfx_buf->dynamicVertexBuffer->buffer || state->prim.streams[0].offset != vert_offset || state->prim.streams[0].stride != MODEL_VERTEX_STRIDE)
		{
			state->prim.streams[0].vb = Game::gfx_buf->dynamicVertexBuffer->buffer;
			state->prim.streams[0].offset = vert_offset;
			state->prim.streams[0].stride = MODEL_VERTEX_STRIDE;
			state->prim.device->SetStreamSource(0, Game::gfx_buf->dynamicVertexBuffer->buffer, vert_offset, MODEL_VERTEX_STRIDE);
		}

		IDirect3DVertexShader9* og_vertex_shader;
		IDirect3DPixelShader9* og_pixel_shader;

		if (render_ff)
		{
			// save shaders
			state->prim.device->GetVertexShader(&og_vertex_shader);
			state->prim.device->GetPixelShader(&og_pixel_shader);

			// needed or game renders mesh with shaders
			state->prim.device->SetVertexShader(nullptr);
			state->prim.device->SetPixelShader(nullptr);

			// vertex format
			state->prim.device->SetFVF(MODEL_VERTEX_FORMAT);


			// #
			// build world matrix

			float model_axis[3][3] = {};
			RtxUtils::unit_quat_to_axis(src->skinnedPlacement.base.quat, model_axis);

			//const auto mtx = source->matrices.matrix[0].m;
			float mtx[4][4] = {};
			const auto scale = src->skinnedPlacement.scale;

			// inlined ikMatrixSet44
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

			mtx[3][0] = src->skinnedPlacement.base.origin[0];
			mtx[3][1] = src->skinnedPlacement.base.origin[1];
			mtx[3][2] = src->skinnedPlacement.base.origin[2];
			mtx[3][3] = 1.0f;

			// set world matrix
			state->prim.device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mtx));
		}

		state->prim.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, surf->vertCount, start_index, surf->triCount);

		if (render_ff)
		{
			state->prim.device->SetFVF(NULL);
			state->prim.device->SetVertexShader(og_vertex_shader);
			state->prim.device->SetPixelShader(og_pixel_shader);
		}
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

	void R_DrawPreTessTris(Game::GfxCmdBufSourceState* src, Game::GfxCmdBufPrimState* state, Game::srfTriangles_t* tris, unsigned int baseIndex, unsigned int triCount)
	{
		const auto dev = *Game::dx9_device_ptr;

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
		state->device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, tris->vertexCount, baseIndex, triCount);
	}

	/**
	 * @brief completely rewritten R_DrawBspDrawSurfsPreTess to render bsp surfaces (world) using the fixed-function pipeline
	 */
	void R_DrawBspDrawSurfsPreTess(const unsigned int* primDrawSurfPos, Game::GfxCmdBufSourceState* src, Game::GfxCmdBufState* state)
	{
		const auto dev = *Game::dx9_device_ptr;

		// #
		// setup fixed-function

		// vertex format
		dev->SetFVF(WORLD_VERTEX_FORMAT);

		// save shaders
		IDirect3DVertexShader9* og_vertex_shader;
		dev->GetVertexShader(&og_vertex_shader);

		IDirect3DPixelShader9* og_pixel_shader;
		dev->GetPixelShader(&og_pixel_shader);

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

				const auto bsp_surf = &Game::rgp->world->dpvs.surfaces[surf_index];
				const auto tris = &bsp_surf->tris;

				if (base_vertex != bsp_surf->tris.firstVertex)
				{
					// never triggers?
					if (tri_count)
					{
						R_DrawPreTessTris(src, &state->prim, prev_tris, base_index, tri_count);
						base_index += 3 * tri_count;
						tri_count = 0;
					}

					prev_tris = tris;
					base_vertex = tris->firstVertex;
				}

				tri_count += list[index].totalTriCount;
			}

			R_DrawPreTessTris(src, &state->prim, prev_tris, base_index, tri_count);
		}

		// #
		// restore everything for following meshes rendered via shaders

		dev->SetVertexShader(og_vertex_shader);
		dev->SetPixelShader(og_pixel_shader);

		// restore world matrix
		Rtx::r_set_3d();
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&Game::gfxCmdBufSourceState->matrices.matrix[0].m));

		dev->SetFVF(NULL);
	}

	// *
	// build buffers

	void build_gfxworld_buffers()
	{
		const auto dev = *Game::dx9_device_ptr;
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
					/*	struct GfxWorldVertex = 44 bytes
					{
						float xyz[3];
						float binormalSign;
						GfxColor color;
						float texCoord[2];
						float lmapCoord[2];
						PackedUnitVec normal;
						PackedUnitVec tangent;
					};*/

					// unpack normal so we can use fixed-function rendering with normals
					for (auto i = 0u; i < Game::rgp->world->vertexCount; i++)
					{
						// packed source vertex
						const auto src_vert = Game::rgp->world->vd.vertices[i];

						// position of our unpacked vert within the vertex buffer
						const auto v_pos_in_buffer = i * WORLD_VERTEX_STRIDE; // pos-xyz ; normal-xyz ; texcoords uv = 32 byte 
						const auto v = reinterpret_cast<unpacked_world_vert*>(((DWORD)vertex_buffer_data + v_pos_in_buffer));

						// vert pos
						v->pos[0] = src_vert.xyz[0];
						v->pos[1] = src_vert.xyz[1];
						v->pos[2] = src_vert.xyz[2];

						// unpack and assign vert normal

						// normal unpacking in a cod4 hlsl shader:
						// temp0	 = i.normal * float4(0.007874016, 0.007874016, 0.007874016, 0.003921569) + float4(-1, -1, -1, 0.7529412);
						// temp0.xyz = temp0.www * temp0;

						const auto scale = static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[3])) * (1.0f / 255.0f) + 0.7529412f;
						v->normal[0] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[0])) * (1.0f / 127.0f) + -1.0f) * scale;
						v->normal[1] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[1])) * (1.0f / 127.0f) + -1.0f) * scale;
						v->normal[2] = (static_cast<float>(static_cast<std::uint8_t>(src_vert.normal.array[2])) * (1.0f / 127.0f) + -1.0f) * scale;

						// packed vertex color : used for alpha blending of decals
						v->color = src_vert.color.packed;

						// uv's
						v->texcoord[0] = src_vert.texCoord[0];
						v->texcoord[1] = src_vert.texCoord[1];
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

		// #
		// cleanup model buffer

		Game::DB_EnumXAssets(Game::XAssetType::ASSET_TYPE_XMODEL, [](Game::XAssetHeader header, [[maybe_unused]] void* data)
			{
				const auto dev = *Game::dx9_device_ptr;

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
			popad;

			call	stock_func;
			jmp		retn_addr;
		}
	}

	// ------------------------------------

	RtxFixedFunction::RtxFixedFunction()
	{
		// fixed function rendering of static models (R_TessStaticModelRigidDrawSurfList)
		if (Flags::HasFlag("fixed_function"))
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

			// fixed-function rendering of world surfaces (R_TessTrianglesPreTessList)
			Utils::Hook(0x627014, R_DrawBspDrawSurfsPreTess, HOOK_CALL).install()->quick();

			// ----

			// on map load :: build custom buffers for fixed-function rendering
			Utils::Hook(0x421187, init_fixed_function_buffers_stub, HOOK_JUMP).install()->quick(); // CG_Init :: some hud init func

			// on renderer shutdown :: release custom buffers used by fixed-function rendering
			Utils::Hook(0x5D9D12, free_fixed_function_buffers_stub, HOOK_JUMP).install()->quick(); // R_Shutdown :: R_ResetModelLighting call
		}
	}
}