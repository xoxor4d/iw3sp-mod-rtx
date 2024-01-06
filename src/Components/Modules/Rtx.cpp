#include "STDInc.hpp"

namespace Components
{
	void Rtx::setup_rtx()
	{
		const auto dev = *Game::dx9_device_ptr;
		const auto data = Game::get_backenddata();

		// populate viewParms3D because R_Set3D needs it
		Game::gfxCmdBufSourceState->viewParms3D = &data->viewInfo->viewParms;

		// update world matrix
		Rtx::r_set_3d();

		// directly set matrices on the device so that rtx-remix finds the camera
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&Game::gfxCmdBufSourceState->matrices.matrix[0].m));
		dev->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&Game::gfxCmdBufSourceState->viewParms.viewMatrix.m));
		dev->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&Game::gfxCmdBufSourceState->viewParms.projectionMatrix.m));

		//rtx_lights::spawn_light();
		Rtx::force_dvars_on_frame();

		// needed for skysphere (unlit rendering)
		dev->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	// stub at the beginning of 'RB_Draw3DInternal' (frame begin)
	__declspec(naked) void rb_standard_drawcommands_stub()
	{
		const static uint32_t retn_addr = 0x62A9E6;
		__asm
		{
			pushad;
			call	Rtx::setup_rtx;
			popad;

			// og instructions
			mov     ebp, esp;
			and		esp, 0xFFFFFFF8;
			jmp		retn_addr;
		}
	}

	// rewritten R_Set3D function
	void Rtx::r_set_3d()
	{
		const auto state = Game::gfxCmdBufSourceState;

		if (state->viewMode != Game::VIEW_MODE_3D)
		{
			state->viewMode = Game::VIEW_MODE_3D;
			memcpy(&state->viewParms, state->viewParms3D, sizeof(state->viewParms));

			if (state->viewParms.origin[3] == 0.0f)
			{
				state->eyeOffset[0] = 0.0f;
				state->eyeOffset[1] = 0.0f;
				state->eyeOffset[2] = 0.0f;
			}
			else
			{
				state->eyeOffset[0] = state->viewParms.origin[0];
				state->eyeOffset[1] = state->viewParms.origin[1];
				state->eyeOffset[2] = state->viewParms.origin[2];
			}

			state->eyeOffset[3] = 1.0f;

			//
			// inlined R_CmdBufSet3D;

			++state->matrixVersions[0];
			++state->matrixVersions[1];
			++state->matrixVersions[2];
			++state->matrixVersions[4];
			++state->matrixVersions[3];
			++state->matrixVersions[5];
			++state->matrixVersions[7];
			state->constVersions[58] = state->matrixVersions[0];

			//memcpy(state, &identity_matrix, 0x40u);
			const auto mtx = state->matrices.matrix[0].m;
			mtx[0][0] = 1.0f;  mtx[0][1] = 0.0f; mtx[0][2] = 0.0f; mtx[0][3] = 0.0f;
			mtx[1][0] = 0.0f;  mtx[1][1] = 1.0f; mtx[1][2] = 0.0f; mtx[1][3] = 0.0f;
			mtx[2][0] = 0.0f;  mtx[2][1] = 0.0f; mtx[2][2] = 1.0f; mtx[2][3] = 0.0f;
			mtx[3][0] = 0.0f;  mtx[3][1] = 0.0f; mtx[3][2] = 0.0f; mtx[3][3] = 1.0f;

			mtx[3][0] = mtx[3][0] - state->eyeOffset[0];
			mtx[3][1] = mtx[3][1] - state->eyeOffset[1];
			mtx[3][2] = mtx[3][2] - state->eyeOffset[2];
		}
	}

	void Rtx::force_dvars_on_frame()
	{
		// update world culling
		if (Dvars::rtx_disable_world_culling)
		{
			loc_disable_world_culling = Dvars::rtx_disable_world_culling->current.integer;
			loc_disable_world_culling = loc_disable_world_culling < 0 ? 0 :
				loc_disable_world_culling > 3 ? 3 : loc_disable_world_culling;
		}

		// update entity culling
		if (Dvars::rtx_disable_entity_culling)
		{
			loc_disable_entity_culling = Dvars::rtx_disable_entity_culling->current.enabled ? 1 : 0;
		}

		Dvars::Override::DvarIntOverride("r_aaSamples", 1, Game::dvar_flags::saved);
		Dvars::Override::DvarIntOverride("r_dlightLimit", 0, Game::dvar_flags::saved);

		Dvars::Override::DvarBoolOverride("r_depthPrepass", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_distortion", false, Game::dvar_flags::saved);
		//Dvars::Override::DvarBoolOverride("r_drawDecals", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_drawSun", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_drawWater", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_dof_enable", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_glow_allowed", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_specular", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_zFeather", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("sm_enable", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_altModelLightingUpdate", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_vsync", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_multiGpu", true, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_smp_worker", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_smp_backend", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_smc_enable", false, Game::dvar_flags::saved);

		//Dvars::Override::DvarBoolOverride("fx_enable", false, Game::dvar_flags::saved);

		Dvars::Override::DvarFloatOverride("cg_tracerlength", 0.0f, Game::dvar_flags::saved);
		Dvars::Override::DvarFloatOverride("r_znear", 4.00195f, Game::dvar_flags::saved);
		Dvars::Override::DvarFloatOverride("r_znear_depthhack", 4.0f, Game::dvar_flags::saved);
	}

	void Rtx::force_dvars_on_init()
	{
		Rtx::force_dvars_on_frame();
		Dvars::Override::DvarFloatOverride("r_lodScaleRigid", 2.5f, Game::dvar_flags::saved, 0.0f, 100.0f);
	}

	// ----------------------------------------------

	const char* Rtx::skysphere_get_name_for_variant(int variant)
	{
		switch (variant)
		{
		default:
		case 0: return "rtx_skysphere_oceanrock";
		case 1: return "rtx_skysphere_desert";
		case 2: return "rtx_skysphere_overcast_city";
		case 3: return "rtx_skysphere_night";
		case 4: return "rtx_skysphere_overcast";
		case 5: return "rtx_skysphere_sunset_clouds";
		}
	}

	bool Rtx::skysphere_is_model_valid()
	{
		// if not spawned an entity yet
		if (!skysphere_spawned)
		{
			return false;
		}

		// check if the entity is valid (player changed level etc.)
		if (skysphere_model == nullptr || skysphere_model->classname == 0
			|| skysphere_model->model != Game::G_ModelIndex(skysphere_get_name_for_variant(skysphere_variant)))
		{
			return false;
		}

		return true;
	}

	void Rtx::skysphere_change_model(int variant)
	{
		const std::int16_t model_index = Game::G_ModelIndex(skysphere_get_name_for_variant(variant));

		skysphere_model->model = model_index;
		skysphere_model->s.index = model_index;
		skysphere_model->classname = Game::scr_const->script_model;

		Game::G_DObjUpdate(&skysphere_model->s);

		skysphere_variant = variant;
	}

	void Rtx::skysphere_spawn(int variant)
	{
		if (skysphere_is_model_valid())
		{
			skysphere_change_model(variant);
			return;
		}

		// needs :: 
		// s->index = modelIndex
		// linked = 0x1;
		// svFlags = 0x04; // even = visible, uneven = hidden

		const std::int16_t model_index = Game::G_ModelIndex(skysphere_get_name_for_variant(variant));

		skysphere_model = Game::G_Spawn();
		skysphere_model->classname = Game::scr_const->script_model;
		skysphere_model->model = model_index;
		skysphere_model->s.index = model_index;
		skysphere_model->r.svFlags = 0x04;
		skysphere_model->r.linked = 0x1;

		//Game::G_SetOrigin(skysphere_model, skysphere_model_origin);
		skysphere_model->s.lerp.pos.trBase[0] = 0.0f;
		skysphere_model->s.lerp.pos.trBase[1] = 0.0f;
		skysphere_model->s.lerp.pos.trBase[2] = 0.0f;
		skysphere_model->s.lerp.pos.trType = Game::TR_STATIONARY;
		skysphere_model->s.lerp.pos.trTime = 0;
		skysphere_model->s.lerp.pos.trDuration = 0;
		skysphere_model->s.lerp.pos.trDelta[0] = 0.0;
		skysphere_model->s.lerp.pos.trDelta[1] = 0.0;
		skysphere_model->s.lerp.pos.trDelta[2] = 0.0;
		skysphere_model->r.currentOrigin[0] = 0.0f;
		skysphere_model->r.currentOrigin[1] = 0.0f;
		skysphere_model->r.currentOrigin[2] = 0.0f;

		//Game::G_SetAngles(skysphere_model, skysphere_model_rotation);
		skysphere_model->s.lerp.apos.trBase[0] = 0.0f;
		skysphere_model->s.lerp.apos.trBase[1] = 0.0f;
		skysphere_model->s.lerp.apos.trBase[2] = 0.0f;
		skysphere_model->s.lerp.apos.trType = Game::TR_STATIONARY;
		skysphere_model->s.lerp.apos.trTime = 0;
		skysphere_model->s.lerp.apos.trDuration = 0;
		skysphere_model->s.lerp.apos.trDelta[0] = 0.0;
		skysphere_model->s.lerp.apos.trDelta[1] = 0.0;
		skysphere_model->s.lerp.apos.trDelta[2] = 0.0;
		skysphere_model->r.currentAngles[0] = 0.0f;
		skysphere_model->r.currentAngles[1] = 0.0f;
		skysphere_model->r.currentAngles[2] = 0.0f;

		Game::G_CallSpawnEntity(skysphere_model);

		skysphere_spawned = true;
		skysphere_variant = variant;
	}

	// ----------------------------------------------

	// *
	// mat->techniqueSet->remappedTechniqueSet->techniques[type]
	bool is_valid_technique_for_type(const Game::Material* mat, const Game::MaterialTechniqueType type)
	{
		if (mat
			&& mat->techniqueSet
			&& mat->techniqueSet->remappedTechniqueSet
			&& mat->techniqueSet->remappedTechniqueSet->techniques
			&& mat->techniqueSet->remappedTechniqueSet->techniques[type])
		{
			return true;
		}

		return false;
	}

	// *
	// return remappedTechnique for technique_type if valid, stock technique otherwise
	void switch_technique(Game::switch_material_t* swm, Game::Material* material)
	{
		if (material)
		{
			swm->technique = nullptr;

			if (is_valid_technique_for_type(material, swm->technique_type))
			{
				swm->technique = material->techniqueSet->remappedTechniqueSet->techniques[swm->technique_type];
			}

			swm->switch_technique = true;
			return;
		}

		// return stock technique if the above failed
		swm->technique = swm->current_technique;
	}


	// *
	// return remappedTechnique for technique_type if valid, stock technique otherwise
	void switch_technique(Game::switch_material_t* swm, const char* material_name)
	{


		if (const auto	material = Game::Material_RegisterHandle(material_name, 3);
			material)
		{
			swm->technique = nullptr;

			if (is_valid_technique_for_type(material, swm->technique_type))
			{
				swm->technique = material->techniqueSet->remappedTechniqueSet->techniques[swm->technique_type];
			}

			swm->switch_technique = true;
			return;
		}

		// return stock technique if the above failed
		swm->technique = swm->current_technique;
	}


	//*
	// return new material if valid, stock material otherwise
	void switch_material(Game::switch_material_t* swm, const char* material_name)
	{


		if (const auto	material = Game::Material_RegisterHandle(material_name, 3);
			material)
		{
			swm->material = material;
			swm->technique = nullptr;

			if (is_valid_technique_for_type(material, swm->technique_type))
			{
				swm->technique = material->techniqueSet->remappedTechniqueSet->techniques[swm->technique_type];
			}

			swm->switch_material = true;
			return;
		}

		// return stock material if the above failed
		swm->material = swm->current_material;
	}

	// replace skies with temp material that can be tagged as 'sky' in remix
	bool fix_sky_for_rtx(Game::switch_material_t* swm, const Game::GfxCmdBufState* state)
	{
		if (Utils::String::StartsWith(swm->current_material->info.name, "wc/sky_"))
		{
			swm->technique_type = Game::TECHNIQUE_UNLIT;
			switch_material(swm, "rtx_sky");
			return false;
		}

		// fix remix normals for viewmodels - needs the material string check because the weapon is in both depth-ranges for some reason
		if (state->depthRangeType == Game::GFX_DEPTH_RANGE_VIEWMODEL || Utils::String::StartsWith(swm->current_material->info.name, "mc/mtl_weapon_"))
		{
			swm->technique_type = Game::TECHNIQUE_LIT;
			swm->switch_technique_type = true;

			return false;
		}

		return true;
	}

	/**
	 * @brief triggered each time the game sets up the material pass for the next object to be rendered
	 *		  - can be used to switch the material or the shaders used for rendering the next obj
	 */
	int R_SetMaterial(Game::MaterialTechniqueType techType, Game::GfxCmdBufSourceState* src, Game::GfxCmdBufState* state, Game::GfxDrawSurf drawSurf)
	{
		auto& rg = *reinterpret_cast<Game::r_globals_t*>(0x1626C90);
		auto rgp = reinterpret_cast<Game::r_global_permanent_t*>(0x1621E00);

		Game::switch_material_t mat = {};

		mat.current_material = rgp->sortedMaterials[(drawSurf.packed >> 29) & 0x7FF];
		mat.current_technique = mat.current_material->techniqueSet->remappedTechniqueSet->techniques[techType];

		mat.material = mat.current_material;
		mat.technique = mat.current_technique;
		mat.technique_type = techType;

		//disable_prepass = false; // always reset

		if (mat.current_material)
		{
			fix_sky_for_rtx(&mat, state);

			if (!mat.switch_material && !mat.switch_technique && !mat.switch_technique_type)
			{
				if (state->origMaterial)
				{
					state->material = state->origMaterial;
				}
				if (state->origTechType)
				{
					state->techType = state->origTechType;
				}
			}
		}

		// save the original material
		state->origMaterial = state->material;

		// only switch to a different technique_type
		if (mat.switch_technique_type)
		{
			if (is_valid_technique_for_type(mat.current_material, mat.technique_type))
			{
				switch_technique(&mat, mat.current_material);
			}
		}

		// set stock or new material & technique
		state->material = mat.material;
		state->technique = mat.technique;


		if (!state->technique || (state->technique->flags & 1) != 0 && !rg.distortion)
		{
			return 0;
		}

		if (!mat.switch_material && !mat.switch_technique && !mat.switch_technique_type)
		{
			if ((mat.technique_type == Game::TECHNIQUE_EMISSIVE || mat.technique_type == Game::TECHNIQUE_UNLIT) && (state->technique->flags & 0x10) != 0 && !src->constVersions[4])
			{
				return 0;
			}
		}

		state->origTechType = state->techType;
		state->techType = mat.technique_type;

		return 1;
	}

	__declspec(naked) void R_SetMaterial_stub()
	{
		const static uint32_t rtn_to_set_shadowable_light = 0x6278F2;
		const static uint32_t retn_to_retn = 0x6278A8;
		__asm
		{
			push	esi;		// techType
			call	R_SetMaterial;
			pop		esi;
			add     esp, 0x10;

			test    eax, eax;	// do not return BOOL if you test 4 byte sized registers :>
			jz      memes;
			jmp		rtn_to_set_shadowable_light;

		memes:
			jmp		retn_to_retn;
		}
	}

	__declspec(naked) void R_SetMaterial_Emissive_stub()
	{
		const static uint32_t retn_addr = 0x627A1F;
		__asm
		{
			push	esi;		// techType
			call	R_SetMaterial;
			pop		esi;
			add     esp, 0x10;
			jmp		retn_addr;
		}
	}


	// *
	// general stubs

	/*
	00611161	call    R_AllocStaticModelLighting
	00611166	add     esp, 8
	00611169	test    al, al
	0061116B	jnz     short loc_611184
	*/

	__declspec(naked) void alloc_smodel_lighting_stub()
	{
		const static uint32_t retn_addr = 0x61116B;
		const static uint32_t draw_model_addr = 0x611184;
		__asm
		{
			pushad;
			push	eax;
			mov		eax, Dvars::rtx_extend_smodel_drawing;
			cmp		byte ptr [eax + 12], 1;
			pop		eax;

			jne		OG_LOGIC;
			popad;

			add     esp, 8 // og instruction overwritten by hook
			jmp		draw_model_addr;

		OG_LOGIC:
			popad;

			// og instructions
			add     esp, 8;
			test    al, al;
			jmp		retn_addr;
		}
	}

	// ----------------------------------------------

	// *
	// Culling

	namespace cull
	{
		// R_AddWorldSurfacesPortalWalk
		__declspec(naked) void world_stub_01()
		{
			const static uint32_t retn_skip = 0x5ECBE5;
			const static uint32_t retn_stock = 0x5ECB7E;
			__asm
			{
				// jump if culling is less or disabled
				cmp		Rtx::loc_disable_world_culling, 1;
				jge		SKIP;

				// og code
				cmp     esi, ebp;
				mov		[esp + 0x10], eax;
				jmp		retn_stock;

			SKIP:
				mov		[esp + 0x10], eax;
				jmp		retn_skip;
			}
		}

		int _skipped_cull = 0; // helper var
		__declspec(naked) void world_stub_02_reset_helper()
		{
			const static uint32_t retn_stock = 0x622B2A;
			__asm
			{
				mov		_skipped_cull, 0;
				mov		[ecx + 0x10], edi;
				mov		[ecx + 8], edx;
				jmp		retn_stock;
			}
		}

		__declspec(naked) void world_stub_02_skip_static_model()
		{
			const static uint32_t retn_stock = 0x622730;
			const static uint32_t retn_stock_jumped = 0x62274F;
			__asm
			{
				// do we want to cull static models the way geo would be culled?
				cmp		Rtx::loc_disable_world_culling, 3;
				jl		STOCK;

				// did we skip the culling check in 'r_cull_world_stub_02'?
				cmp		_skipped_cull, 1;
				je		SKIP;

			STOCK:		// og code
				jz		loc_62274F;
				mov     esi, [edx + 0x24];
				jmp		retn_stock;

			loc_62274F: // og code
				jmp		retn_stock_jumped;


			SKIP:		// skip static model rendering
				mov     esi, [edx + 0x24];
				jmp		retn_stock_jumped;
			}
		}

		// R_AddAabbTreeSurfacesInFrustum_r
		__declspec(naked) void world_stub_02()
		{
			const static uint32_t retn_skip = 0x6226CE;
			const static uint32_t retn_stock = 0x6226C8;
			__asm
			{
				// jump if culling is disabled
				cmp		Rtx::loc_disable_world_culling, 2;
				jge		SKIP;


				// stock op's
				fnstsw  ax;
				test    ah, 0x41;

				// og code
				fnstsw  ax;
				test    ah, 0x41;
				jmp		retn_stock;

			SKIP: // jumped here because culling is disabled 
				fnstsw  ax;
				test    ah, 0x41;
				jnp		HACKED_CULLING;
				jmp		retn_skip;

			HACKED_CULLING: // jumped here because the game would have culled this object
				mov		_skipped_cull, 1;
				jmp		retn_skip;
			}
		}

		// R_AddAabbTreeSurfacesInFrustum_r
		__declspec(naked) void world_stub_03()
		{
			const static uint32_t retn_skip = 0x622708;
			const static uint32_t retn_stock = 0x6226F9;
			__asm
			{
				// jump if culling is less or disabled
				cmp		Rtx::loc_disable_world_culling, 1;
				jge		SKIP;

				// og code
				fnstsw  ax;
				test    ah, 1;
				jmp		retn_stock;

			SKIP:
				fnstsw  ax;
				jmp		retn_skip;
			}
		}

		// R_AddCellSceneEntSurfacesInFrustumCmd
		__declspec(naked) void entities_stub()
		{
			const static uint32_t retn_skip = 0x62EF4C;
			const static uint32_t retn_stock = 0x62EF4A;
			__asm
			{
				cmp		Rtx::loc_disable_entity_culling, 1;
				je		SKIP;

				// stock op's
				and		[esp + 0x18], edx;
				cmp     byte ptr [esi + eax], 0;
				jmp		retn_stock;

			SKIP:
				//popad;
				and		[esp + 0x18], edx;
				jmp		retn_skip;
			}
		}
	}

	// -------------------

	// *
	// LOD: altered forceLod logic to actually force a SPECIFIC LOD at ALL times

	int xmodel_get_lod_for_dist(const Game::XModel* model, const float* base_dist)
	{
		const auto lod_count = model->numLods;
		const auto& r_forceLod = Dvars::Functions::Dvar_FindVar("r_forceLod");

		// 4 = none - disabled
		if (r_forceLod->current.integer == r_forceLod->reset.integer)
		{
			for (auto lod_index = 0; lod_index < lod_count; ++lod_index)
			{
				const auto lod_dist = model->lodInfo[lod_index].dist;

				if (lod_dist == 0.0f || lod_dist > *base_dist)
				{
					return lod_index;
				}
			}
		}
		// r_forcelod enabled
		else
		{
			if (r_forceLod->current.integer > lod_count // force lowest possible LOD
				|| (/*dvars::r_forceLod_second_lowest->current.enabled &&*/ r_forceLod->current.integer >= lod_count)) // force second lowest possible LOD
			{
				return lod_count - 1 >= 0 ? lod_count - 1 : 0;
			}

			return r_forceLod->current.integer;
		}

		return -1;
	}

	int xmodel_get_lod_for_dist_global = 0;
	__declspec(naked) void xmodel_get_lod_for_dist_detour()
	{
		const static uint32_t retn_addr = 0x5AC870;
		__asm
		{
			lea		ecx, [esp + 4];
			pushad;
			push	ecx; // base_dist
			push	eax; // model
			call	xmodel_get_lod_for_dist;
			add		esp, 8;
			mov     xmodel_get_lod_for_dist_global, eax;
			popad;

			mov     eax, xmodel_get_lod_for_dist_global;
			jmp		retn_addr;
		}
	}

	// -----

	int forcelod_get_lod(const int lod_count)
	{
		const auto& r_forceLod = Dvars::Functions::Dvar_FindVar("r_forceLod");

		if (r_forceLod->current.integer > lod_count // force lowest possible LOD
			|| (/*dvars::r_forceLod_second_lowest->current.enabled &&*/ r_forceLod->current.integer >= lod_count)) // force second lowest possible LOD
		{
			return lod_count - 1 >= 0 ? lod_count - 1 : 0;
		}

		return r_forceLod->current.integer;
	}

	int forcelod_is_enabled()
	{
		const auto& r_forceLod = Dvars::Functions::Dvar_FindVar("r_forceLod");

		// 4 = none - disabled
		if (r_forceLod->current.integer == r_forceLod->reset.integer)
		{
			return 0;
		}

		return 1;
	}

	int xmodel_get_lod_for_dist_global_2 = 0;
	__declspec(naked) void xmodel_get_lod_for_dist_inlined()
	{
		const static uint32_t break_addr = 0x611149;
		const static uint32_t og_logic_addr = 0x61112B;
		__asm
		{
			pushad;
			push	ecx;					// save ecx
			call	forcelod_is_enabled;
			cmp		eax, 1;
			pop		ecx;					// restore ecx
			jne		OG_LOGIC;				// if r_forceLod != 1

			push	ecx;					// holds model->numLods
			call	forcelod_get_lod;
			add		esp, 4;
			mov		xmodel_get_lod_for_dist_global_2, eax;
			popad;

			mov		esi, xmodel_get_lod_for_dist_global_2; // move returned lodindex into the register the game expects it to be
			jmp		break_addr;


		OG_LOGIC:
			popad;
			fld     dword ptr [edx];
			fcomp   st(1);
			fnstsw  ax;
			jmp		og_logic_addr;
		}
	}

	Rtx::Rtx()
	{
		// *
		// general

		// force fullbright (switching to fullbright in-game will result in wrong surface normals)
		Utils::Hook::Nop(0x614649, 2);

		// Ignore hardware changes // xor al,al - pop ecx - ret
		Utils::Hook::Set<DWORD>(0x594E71, 0xC359C032);
		// Remove improper quit popup
		Utils::Hook::Set<BYTE>(0x595B26, 0xEB);

		// hook beginning of 'RB_Draw3DInternal' to setup general stuff required for rtx-remix
		Utils::Hook(0x62A9E1, rb_standard_drawcommands_stub, HOOK_JUMP).install()->quick();

		// render third person model in first person
		//Utils::Hook::Nop(0x42E187, 6); // CG_DrawFriendlyNames: do not disable name drawing
		//Utils::Hook::Nop(0x42E2BA, 6); // CG_DrawCrosshairNames: ^
		//Utils::Hook::Nop(0x4311AE, 6); // CG_DrawCrosshair: do not disable crosshair
		//Utils::Hook::Nop(0x443007, 2); // CG_UpdateCursorHints: do not disable cursor hints
		//Utils::Hook::Nop(0x45083C, 2); // CG_CalcTurretViewValues: calculate first person viewangles
		//Utils::Hook::Nop(0x4508C1, 6); // CG_ApplyViewAnimation: first person anims
		//Utils::Hook::Nop(0x451C70, 2); // CG_CalcViewValues: always call CG_OffsetFirstPersonView
		//Utils::Hook::Set<BYTE>(0x451C9E, 0xEB); // CG_CalcViewValues: do not call CG_OffsetThirdPersonView
		//Utils::Hook::Set<BYTE>(0x456D36, 0xEB); // CG_AddPlayerWeapon: do not disable bolt effects
		//Utils::Hook::Nop(0x457054, 6); // CG_AddViewWeapon: do not disable viewmodel
		//Utils::Hook::Nop(0x451D8E, 2); // CG_UpdateThirdPerson: always enable "thirdperson"


		// *
		// allow 4k images (skysphere)

		// set img alloc size from 0x600000 to 0x1000000
		Utils::Hook::Set<BYTE>(0x61FE77 + 3, 0x00);
		Utils::Hook::Set<BYTE>(0x61FE77 + 4, 0x01);

		//// ^ cmp -> error if image larger
		Utils::Hook::Set<BYTE>(0x61FE39 + 3, 0x00);
		Utils::Hook::Set<BYTE>(0x61FE39 + 4, 0x01);


		// *
		// hook R_SetMaterial
		Utils::Hook(0x6278E6, R_SetMaterial_stub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x627A17, R_SetMaterial_Emissive_stub, HOOK_JUMP).install()->quick();


		// *
		// Precaching beyond level load (skysphere spawning)
		Utils::Hook::Set<BYTE>(0x4ECEE6, 0xEB);


		// *
		// culling

		//if (Flags::HasFlag("disable_culling"))
		{
			// R_AddWorldSurfacesPortalWalk :: less culling
			// 0x60B02E -> jl to jmp // 0x7C -> 0xEB //utils::hook::set<BYTE>(0x60B02E, 0xEB);
			Utils::Hook::Nop(0x5ECB78, 6); Utils::Hook(0x5ECB78, cull::world_stub_01, HOOK_JUMP).install()->quick(); // crashes on release

			{
				// note:
				// using 'rtx_disable_world_culling' = 'less' results in unstable world geometry hashes (but stable static model hashes)
				// -> using 'all-but-models' leaves culling logic for static models at 'less' but does not cull world geometry

				// R_AddCellStaticSurfacesInFrustumCmd :: stub used to reset the skip model check from the stub below
				Utils::Hook::Nop(0x622B24, 6); Utils::Hook(0x622B24, cull::world_stub_02_reset_helper, HOOK_JUMP).install()->quick();
				// R_AddAabbTreeSurfacesInFrustum_r :: check if culling mode 'all-but-models' is active - check note above
				Utils::Hook(0x62272B, cull::world_stub_02_skip_static_model, HOOK_JUMP).install()->quick();

				// R_AddAabbTreeSurfacesInFrustum_r :: disable all surface culling (bad fps)
				// 0x643B08 -> nop //utils::hook::nop(0x643B08, 6);
				Utils::Hook(0x6226C3, cull::world_stub_02, HOOK_JUMP).install()->quick();
			}

			// 0x643B39 -> jmp ^ // 0x74 -> 0xEB //utils::hook::set<BYTE>(0x643B39, 0xEB);
			Utils::Hook(0x6226F4, cull::world_stub_03, HOOK_JUMP).install()->quick();

			// R_AddCellSceneEntSurfacesInFrustumCmd :: active ents like destructible cars / players (disable all culling)
			// 0x64D17A -> nop // 2 bytes //utils::hook::nop(0x64D17A, 2);
			Utils::Hook::Nop(0x62EF42, 8); Utils::Hook(0x62EF42, cull::entities_stub, HOOK_JUMP).install()->quick();

			// R_AddCellSceneEntSurfacesInFrustumCmd :: disable brushmodel culling
			Utils::Hook::Nop(0x62F12B, 2);

			// R_AddCellSceneEntSurfacesInFrustumCmd / R_AddEntitySurfacesInFrustumCmd :: do not cull characters
			Utils::Hook::Nop(0x62F06D, 2); Utils::Hook::Set<BYTE>(0x62ED65, 0xEB); // the two below are not needed I think
			Utils::Hook::Nop(0x62EDC7, 2); Utils::Hook::Set<BYTE>(0x62EDDD, 0xEB);
		}

		Command::Add("rtx_sky_clear", []() { Rtx::skysphere_spawn(0); });
		Command::Add("rtx_sky_desert", []() { Rtx::skysphere_spawn(1); });
		Command::Add("rtx_sky_city", []() { Rtx::skysphere_spawn(2); });
		Command::Add("rtx_sky_night", []() { Rtx::skysphere_spawn(3); });
		Command::Add("rtx_sky_overcast", []() { Rtx::skysphere_spawn(4); });
		Command::Add("rtx_sky_sunset", []() { Rtx::skysphere_spawn(5); });

		// msg "too many static models ..." (disabled culling: the engine cant handle modellighting for so many static models, thus not drawing them)
		Utils::Hook::Nop(0x611161, 5); Utils::Hook(0x611161, alloc_smodel_lighting_stub, HOOK_JUMP).install()->quick();


		// #
		// LOD

		Utils::Hook::Nop(0x5D4ECA, 4); // do not scale lodScaleRigid with fov
		Utils::Hook::Nop(0x5D4ED7, 2); // do not scale lodBiasRigid with fov

		// check if r_forceLod is enabled and force LOD's accordingly
		Utils::Hook(0x5AC840, xmodel_get_lod_for_dist_detour, HOOK_JUMP).install()->quick();

		// ^ but inlined ..... for all other static models (R_AddAllStaticModelSurfacesCamera)
		Utils::Hook::Nop(0x611125, 6);  Utils::Hook(0x611125, xmodel_get_lod_for_dist_inlined, HOOK_JUMP).install()->quick();


		// #
		// dvars

		Events::OnDvarInit([]
		{
			Dvars::rtx_extend_smodel_drawing = Dvars::Register::Dvar_RegisterBool("rtx_extend_smodel_drawing", "Disable static model drawing limit", false, Game::saved);

			Dvars::rtx_disable_world_culling = Dvars::Register::Dvar_RegisterEnum(
				/* name		*/ "rtx_disable_world_culling",
				/* desc		*/ "Disable world culling. 'all' is needed for stable geometry hashes!\n"
				"- less: reduces culling to portals only (unstable world geo hashes!)\n"
				"- all: disable culling of all surfaces including models\n"
				"- all-but-models: disable culling of all surfaces excluding models",
				/* default	*/ 1,
				/* enumSize	*/ Rtx::rtx_disable_world_culling_enum.size(),
				/* enumData */ Rtx::rtx_disable_world_culling_enum.data(),
				/* flags	*/ Game::saved);

			Dvars::rtx_disable_entity_culling = Dvars::Register::Dvar_RegisterBool(
				/* name		*/ "rtx_disable_entity_culling",
				/* desc		*/ "Disable culling of game entities (script objects/destructible cars ...)",
				/* default	*/ true,
				/* flags	*/ Game::saved);
		});
	}
}