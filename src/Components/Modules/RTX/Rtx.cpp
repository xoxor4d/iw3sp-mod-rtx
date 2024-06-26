#include "STDInc.hpp"

namespace Components
{
	void Rtx::setup_rtx()
	{
		const auto dev = Game::get_device();
		const auto data = Game::get_backenddata();

		// populate viewParms3D because R_Set3D needs it
		Game::gfxCmdBufSourceState->viewParms3D = &data->viewInfo->viewParms;

		// update world matrix
		Rtx::r_set_3d();

		// directly set matrices on the device so that rtx-remix finds the camera
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&Game::gfxCmdBufSourceState->matrices.matrix[0].m));
		dev->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&Game::gfxCmdBufSourceState->viewParms.viewMatrix.m));
		dev->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&Game::gfxCmdBufSourceState->viewParms.projectionMatrix.m));

		Rtx::force_dvars_on_frame();

		// needed for skysphere (unlit rendering)
		dev->SetRenderState(D3DRS_LIGHTING, FALSE);

		// make sky follow the player - G_SetOrigin
		skysphere_model->s.lerp.pos.trBase[0] = Game::cgs->snap->ps.origin[0];
		skysphere_model->s.lerp.pos.trBase[1] = Game::cgs->snap->ps.origin[1];
		skysphere_model->s.lerp.pos.trBase[2] = Game::cgs->snap->ps.origin[2];
		skysphere_model->s.lerp.pos.trType = Game::TR_STATIONARY;
		skysphere_model->s.lerp.pos.trTime = 0;
		skysphere_model->s.lerp.pos.trDuration = 0;
		skysphere_model->s.lerp.pos.trDelta[0] = 0.0f;
		skysphere_model->s.lerp.pos.trDelta[1] = 0.0f;
		skysphere_model->s.lerp.pos.trDelta[2] = 0.0f;
		skysphere_model->r.currentOrigin[0] = Game::cgs->snap->ps.origin[0];
		skysphere_model->r.currentOrigin[1] = Game::cgs->snap->ps.origin[1];
		skysphere_model->r.currentOrigin[2] = Game::cgs->snap->ps.origin[2];

		if (!Flags::HasFlag("no_fog"))
		{
			const auto s = RtxMapSettings::settings();
			const float fog_start = 1.0f;
			dev->SetRenderState(D3DRS_FOGENABLE, TRUE);
			dev->SetRenderState(D3DRS_FOGCOLOR, s->fog_color.packed);
			dev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
			dev->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fog_start));
			dev->SetRenderState(D3DRS_FOGEND, *(DWORD*)&s->fog_distance);
		}

		// spawn sun
		if (!Flags::HasFlag("no_sun"))
		{
			const auto s = RtxMapSettings::settings();

			D3DLIGHT9 light = {};
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Diffuse.r = s->sun_color[0] * s->sun_intensity;
			light.Diffuse.g = s->sun_color[1] * s->sun_intensity;
			light.Diffuse.b = s->sun_color[2] * s->sun_intensity;

			D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, (const D3DXVECTOR3*)&s->sun_direction);

			dev->SetLight(0, &light);
			dev->LightEnable(0, TRUE);
		}
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

	void post_scene_rendering_stub()
	{
		const auto dev = *Game::dx9_device_ptr;

		// disable fog before rendering UI (who wants foggy UI elements right?)
		// ^ can happen if no skysphere is rendered, which is rendered last and thus disables fog for everything afterwards
		dev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}

	// stub at the beginning of 'RB_CallExecuteRenderCommands' (before UI)
	__declspec(naked) void rb_call_execute_render_commands_stub()
	{
		const static uint32_t retn_addr = 0x5FF656;
		__asm
		{
			pushad;
			call	post_scene_rendering_stub;
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

	// model:	[1] techset - [2] material
	// bsp:		[3] techset - [4] material
	// bmodel:	[5] techset - [6] material
	void Rtx::rb_show_tess(Game::GfxCmdBufSourceState* source, Game::GfxCmdBufState* state, const float* center, const char* name, const float* color, Game::DebugGlobals* manual_debug_glob)
	{
		const auto debug_glob = manual_debug_glob ? manual_debug_glob : &source->input.data->debugGlobals;

		float offset_center[3];
		offset_center[0] = center[0];
		offset_center[1] = center[1];
		offset_center[2] = center[2];

		const auto dist_to_str = RtxUtils::distance3(source->eyeOffset, offset_center);
		if (dist_to_str > 1000.0f)
		{
			return;
		}

		bool viewmodel_string = false;
		auto font_scale = 0.25f;
		if (dist_to_str < 25.0f)
		{
			viewmodel_string = true;
			font_scale = 0.025f;
		}

		const Game::MaterialTechnique* tech = nullptr;
		if (state->material && state->material->techniqueSet->techniques[static_cast<std::uint8_t>(state->techType)])
		{
			tech = state->material->techniqueSet->techniques[static_cast<std::uint8_t>(state->techType)];
		}

		if (const auto r_showTess = Dvars::Functions::Dvar_FindVar("r_showTess"); r_showTess && tech)
		{
			switch (r_showTess->current.integer)
			{
			case 1: // techset model
			case 3: // techset bsp
			case 5: // techset bmodel
			{
				// offset_center[2] = (((float)state->techType - 16.0f) * 0.3f) + offset_center[2];
				// header
				Game::R_AddDebugString(debug_glob, offset_center, color, font_scale, Utils::String::VA("%s: %s", name, tech->name));
				font_scale *= 0.5f;

				offset_center[2] -= viewmodel_string ? 0.25f : 2.5f;
				Game::R_AddDebugString(debug_glob, offset_center, color, font_scale, Utils::String::VA("> [TQ]: %s", state->material->techniqueSet->name));

				offset_center[2] -= viewmodel_string ? 0.25f : 2.5f;
				Game::R_AddDebugString(debug_glob, offset_center, color, font_scale, Utils::String::VA("> [VS] %s", tech->passArray[0].vertexShader ? tech->passArray[0].vertexShader->name : "<NONE>"));

				offset_center[2] -= viewmodel_string ? 0.25f : 2.5f;
				Game::R_AddDebugString(debug_glob, offset_center, color, font_scale, Utils::String::VA("> [PS] %s", tech->passArray[0].pixelShader ? tech->passArray[0].pixelShader->name : "<NONE>"));
				break;
			}

			case 2: // material model
			case 4: // material bsp
			case 6: // material bmodel
			{
				// header
				Game::R_AddDebugString(debug_glob, offset_center, color, font_scale, Utils::String::VA("%s: %s", name, state->material->info.name));
				font_scale *= 0.5f;

				for (auto i = 0; i < state->material->textureCount; i++)
				{
					if (&state->material->textureTable[i] && state->material->textureTable[i].u.image && state->material->textureTable[i].u.image->name)
					{
						const auto img = state->material->textureTable[i].u.image;
						offset_center[2] -= viewmodel_string ? 0.25f : 2.5f;

						const char* semantic_str;
						switch (static_cast<std::uint8_t>(img->semantic))
						{
						case 0: semantic_str = "2D"; break;
						case 1: semantic_str = "F"; break;
						case 2: semantic_str = "C"; break;
						case 5: semantic_str = "N"; break;
						case 8: semantic_str = "S"; break;
						case 11: semantic_str = "W"; break;
						default: semantic_str = "C+"; break;
						}

						Game::R_AddDebugString(debug_glob, offset_center, color, font_scale, Utils::String::VA("> [%s] %s", semantic_str, img->name)); // static_cast<std::uint8_t>(img->semantic)
					}
				}
				break;
			}

			default:
				break;
			}
		}
	}

	void Rtx::force_dvars_on_frame()
	{
		if (Dvars::rtx_culling_tweak_mins) loc_culling_tweak_mins = Dvars::rtx_culling_tweak_mins->current.enabled;
		if (Dvars::rtx_culling_tweak_maxs) loc_culling_tweak_maxs = Dvars::rtx_culling_tweak_maxs->current.enabled;
		if (Dvars::rtx_culling_tweak_frustum) loc_culling_tweak_frustum = Dvars::rtx_culling_tweak_frustum->current.enabled;
		if (Dvars::rtx_culling_tweak_smodel) loc_culling_tweak_smodel = Dvars::rtx_culling_tweak_smodel->current.enabled;

		Dvars::Override::DvarBoolOverride("r_smp_backend", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_skinCache", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_smc_enable", false, Game::dvar_flags::saved);
	}

	// one renderer init
	void Rtx::force_dvars_on_init()
	{
		Rtx::force_dvars_on_frame();

		Dvars::Override::DvarBoolOverride("r_multiGpu", true, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_floatz", true, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_autopriority", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_vsync", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_preloadShaders", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_altModelLightingUpdate", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_zFeather", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_aaAlpha", false, Game::dvar_flags::saved);
		Dvars::Override::DvarIntOverride("r_aaSamples", 1, Game::dvar_flags::saved);
		Dvars::Override::DvarIntOverride("r_dlightLimit", 0, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("sm_enable", false, Game::dvar_flags::saved);

		Dvars::Override::DvarBoolOverride("r_specular", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_drawSun", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_drawWater", false, Game::dvar_flags::saved);
	}

	// on each mapload
	void Rtx::set_dvars_defaults_on_mapload()
	{
		Dvars::Override::DvarFloatOverride("r_lodScaleRigid", 2.0f, Game::dvar_flags::saved, 0.0f, 100.0f);

		if (!Flags::HasFlag("no_forced_lod"))
		{
			Dvars::Override::DvarIntOverride("r_forceLod", 0, Game::dvar_flags::saved);
		}
		
		Dvars::Override::DvarBoolOverride("fx_drawClouds", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_pretess", true, Game::dvar_flags::saved);
		Dvars::Override::DvarFloatOverride("r_znear", 40.0f, Game::dvar_flags::saved); // decreases floating point imp. on larger maps
		Dvars::Override::DvarFloatOverride("r_znear_depthhack", 4.0f, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_glow", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_glow_allowed", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_depthPrepass", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_dof_enable", false, Game::dvar_flags::saved);
		Dvars::Override::DvarBoolOverride("r_distortion", false, Game::dvar_flags::saved);
	}

	// ----------------------------------------------

	const char* Rtx::skysphere_get_name_for_variant(std::uint32_t variant)
	{
		if (variant >= SKY::COUNT)
		{
			return SKY_STRINGS[SKY::SUNSET];
		}

		return SKY_STRINGS[variant];
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
		if (RtxUtils::starts_with(swm->current_material->info.name, "wc/sky_"))
		{
			swm->technique_type = Game::TECHNIQUE_UNLIT;
			switch_material(swm, "rtx_sky");
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

			//if (state->material && std::string_view(state->material->info.name).contains("cinematic_3d"))
			//{
			//	mat.technique_type = Game::TECHNIQUE_UNLIT;
			//	switch_material(&mat, "white");
			//}
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
		int _last_active_valid_cell = 0; //-1;
		void R_AddWorldSurfacesPortalWalk_hk(int camera_cell_index, Game::DpvsView* dpvs)
		{
			//const auto dpvsGlob = reinterpret_cast<game::DpvsGlob*>(0x3957100);

			const auto dpvsGlob = Game::dpvsGlob;

			// never show the complete map, only the last valid cell
			if (camera_cell_index < 0)
			{
				const auto cell = &Game::rgp->world->cells[_last_active_valid_cell];
				const auto cell_index = cell - Game::rgp->world->cells;
				Game::R_AddCellSurfacesAndCullGroupsInFrustumDelayed(cell, dpvs->frustumPlanes, dpvs->frustumPlaneCount, dpvs->frustumPlaneCount);
				dpvsGlob->cellVisibleBits[(cell_index >> 5) + 3] |= (1 << (cell_index & 0x1F)); // is the +3 correct here?
			}
			else
			{
				_last_active_valid_cell = camera_cell_index;

				const float plane_dist = Dvars::rtx_culling_plane_dist ? Dvars::rtx_culling_plane_dist->current.value : 1200.0f;

				// hack - disable most frustum culling
				dpvsGlob->views[0][0].frustumPlanes[0].coeffs[3] += plane_dist; //rtx::m_frustum_plane_offsets[0]; //5000.0f;
				dpvsGlob->views[0][0].frustumPlanes[1].coeffs[3] += plane_dist; //rtx::m_frustum_plane_offsets[1];
				dpvsGlob->views[0][0].frustumPlanes[2].coeffs[3] += plane_dist; //rtx::m_frustum_plane_offsets[2];
				dpvsGlob->views[0][0].frustumPlanes[3].coeffs[3] += plane_dist; //rtx::m_frustum_plane_offsets[3];
				dpvsGlob->views[0][0].frustumPlanes[4].coeffs[3] += plane_dist; //rtx::m_frustum_plane_offsets[4];
				dpvsGlob->views[0][0].frustumPlanes[5].coeffs[3] += plane_dist; //rtx::m_frustum_plane_offsets[5];
				dpvsGlob->viewPlane.coeffs[3] += plane_dist; //rtx::m_frustum_plane_offsets[6]; //5000.0f;
				// ^ needs to be viewPlane here?

				const auto& var = Dvars::Functions::Dvar_FindVar("r_singleCell");
				const bool single_cell = (var && var->current.enabled) ? true : false;

				// #
				// always add full cell the player is in (same as r_singlecell)
				const auto cell = &Game::rgp->world->cells[camera_cell_index];
				const auto cell_index = cell - Game::rgp->world->cells;

				if (single_cell)
				{
					dpvsGlob->farPlane = nullptr;
				}

				Game::R_AddCellSurfacesAndCullGroupsInFrustumDelayed(cell, dpvs->frustumPlanes, dpvs->frustumPlaneCount, dpvs->frustumPlaneCount); // dpvs->frustumPlaneCount
				dpvsGlob->cellVisibleBits[(cell_index >> 5) + 3] |= (1 << (cell_index & 0x1F));


				// #
				// draw cell index at the center of the current cell
				if (Dvars::r_showCellIndex && Dvars::r_showCellIndex->current.enabled)
				{
					const Game::vec4_t col = { 1.0f, 1.0f, 1.0f, 1.0f };
					void* fontHandle = Game::R_RegisterFont("fonts/boldFont", 4);
					Game::R_AddCmdDrawTextASM(Utils::String::VA("Current Cell: %d", camera_cell_index), 0x7FFFFFFF, fontHandle, 50.0f, 500.0f, 1.5f, 1.5f, 0.0f, col, 0);
				}

				// #
				// force cells defined in map_settings.ini

				if (!single_cell && RtxMapSettings::settings()->cell_overrides_exist && camera_cell_index < 1024)
				{
					const auto& c_ow = RtxMapSettings::settings()->cell_settings[camera_cell_index];
					if (c_ow.active)
					{
						for (const auto& i : c_ow.forced_cell_indices)
						{
							const auto forced_cell = &Game::rgp->world->cells[i];
							const auto c_index = forced_cell - Game::rgp->world->cells;
							Game::R_AddCellSurfacesAndCullGroupsInFrustumDelayed(forced_cell, dpvs->frustumPlanes, dpvs->frustumPlaneCount, dpvs->frustumPlaneCount);
							dpvsGlob->cellVisibleBits[(c_index >> 5) + 3] |= (1 << (c_index & 0x1F));
						}
					}
				}

				if (!single_cell)
				{
					// R_VisitPortals
					Game::R_VisitPortals(dpvs->frustumPlaneCount, cell, &dpvsGlob->viewPlane, dpvs->frustumPlanes); // viewplane here .. or is that the nearplane?
				}
			}
		}

		__declspec(naked) void cell_stub()
		{
			const static uint32_t retn_addr = 0x5ECBDF;
			__asm
			{
				// ebx = world
				// esi = cameraCellIndex
				// eax = DpvsView

				push	eax;
				push	esi;
				call	R_AddWorldSurfacesPortalWalk_hk;
				add		esp, 8;
				jmp		retn_addr;
			}
		}

		// #
		// # tweakable culling dvars

		__declspec(naked) void disable_mins_culling_stub()
		{
			const static uint32_t stock_addr = 0x6226C8;
			const static uint32_t disable_culling_addr = 0x6226CE;
			__asm
			{
				// jump if culling is less or disabled
				cmp		Rtx::loc_culling_tweak_mins, 1;
				je		SKIP;

				fnstsw  ax;
				test    ah, 0x41;
				jmp		stock_addr;

			SKIP:
				jmp		disable_culling_addr;
			}
		}

		__declspec(naked) void disable_maxs_culling_stub()
		{
			const static uint32_t stock_addr = 0x6226F9;
			const static uint32_t disable_culling_addr = 0x622708;
			__asm
			{
				// jump if culling is less or disabled
				cmp		Rtx::loc_culling_tweak_maxs, 1;
				je		SKIP;

				fnstsw  ax;
				test    ah, 1;
				jmp		stock_addr;

			SKIP:
				jmp		disable_culling_addr;
	}
		}

		__declspec(naked) void disable_frustum_culling_stub()
		{
			const static uint32_t stock_addr = 0x622904;
			const static uint32_t disable_culling_addr = 0x622940;
			__asm
			{
				// og
				xor		ecx, ecx;

				// jump if culling is less or disabled
				cmp		Rtx::loc_culling_tweak_frustum, 1;
				je		SKIP;

				cmp		[esp + 0x54], ecx;
				jmp		stock_addr;

			SKIP:
				jmp		disable_culling_addr;
			}
		}

		__declspec(naked) void disable_smodel_culling_stub()
		{
			const static uint32_t stock_addr = 0x622839;
			const static uint32_t disable_culling_addr = 0x622875;
			__asm
			{
				// og
				xor		ecx, ecx;

				// jump if culling is less or disabled
				cmp		Rtx::loc_culling_tweak_smodel, 1;
				je		SKIP;

				cmp		[esp + 0x54], ecx;
				jmp		stock_addr;

			SKIP:
				jmp		disable_culling_addr;
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

	// 
	int xmodel_get_lod_for_dist_global_3 = 0;
	__declspec(naked) void xmodel_get_lod_for_dist_inlined2()
	{
		const static uint32_t break_addr = 0x59B692;
		const static uint32_t og_logic_addr = 0x59B669;
		__asm
		{
			pushad;
			push	edx;					// save edx
			call	forcelod_is_enabled;
			cmp		eax, 1;
			pop		edx;					// restore edx
			jne		OG_LOGIC;				// if r_forceLod != 1

			push	edx;					// holds model->numLods
			call	forcelod_get_lod;
			add		esp, 4;
			mov		xmodel_get_lod_for_dist_global_3, eax;
			popad;

			mov		ecx, xmodel_get_lod_for_dist_global_3; // move returned lodindex into the register the game expects it to be
			jmp		break_addr;


		OG_LOGIC:
			popad;
			fld     dword ptr[edi];
			fcomp   st(1);
			fnstsw  ax;
			jmp		og_logic_addr;
		}
	}

	// *
	// fix resolution issues by removing duplicates returned by EnumAdapterModes
	// ^ this was fixed on the dxvk branch - TODO: remove when latest dxvk changes were merged into dxvk-remix

	namespace resolution
	{
		auto hash = [](const _D3DDISPLAYMODE& d) { return d.Width + 10 * d.Height + d.RefreshRate; };
		auto equal = [](const _D3DDISPLAYMODE& d1, const _D3DDISPLAYMODE& d2) { return d1.Width == d2.Width && d1.Height == d2.Height && d1.RefreshRate == d2.RefreshRate; };
		std::unordered_set<_D3DDISPLAYMODE, decltype(hash), decltype(equal)> modes(256, hash, equal);

		int enum_adapter_modes_intercept(std::uint32_t adapter_index, std::uint32_t mode_index)
		{
			_D3DDISPLAYMODE current = {};
			const auto hr = Game::dx->d3d9->EnumAdapterModes(adapter_index, D3DFMT_X8R8G8B8, mode_index, &current) < 0;
			modes.emplace(current);
			return hr;
		}

		__declspec(naked) void R_EnumDisplayModes_stub()
		{
			const static uint32_t retn_addr = 0x5D8EA2;
			__asm
			{
				push	esi; // mode index
				push	ebx; // adapter index
				call	enum_adapter_modes_intercept;
				add		esp, 8;
				jmp		retn_addr;
			}
		}

		void enum_adapter_modes_write_array()
		{
			std::uint32_t idx = 0;
			for (auto& m : modes)
			{
				if (idx >= 256)
				{
					Game::Com_Printf(0, "EnumAdapterModes : Failed to grab all possible resolutions. Array to small!\n");
					break;
				}

				memcpy(&Game::dx->displayModes[idx], &m, sizeof(_D3DDISPLAYMODE));
				idx++;
			}
		}

		__declspec(naked) void R_EnumDisplayModes_stub2()
		{
			const static uint32_t R_CompareDisplayModes_addr = 0x5D8E20;
			const static uint32_t retn_addr = 0x5D8EDE;
			__asm
			{
				pushad;
				call	enum_adapter_modes_write_array;
				popad;

				push	R_CompareDisplayModes_addr;
				jmp		retn_addr;
			}
		}
	}

	void fix_aspect_ratio(int* window_parms)
	{
		*reinterpret_cast<float*>(0x1621DC4) = static_cast<float>(window_parms[7]) / static_cast<float>(window_parms[8]);
	}

	void __declspec(naked) fix_aspect_ratio_stub()
	{
		const static uint32_t retn_addr = 0x5D8395;
		__asm
		{
			pop		eax;
			pushad;
			push	eax;
			call	fix_aspect_ratio;
			add		esp, 4;
			popad;
			push	eax;

			jmp		retn_addr;
		}
	}

	int skip_image_load(Game::GfxImage* img)
	{
		// 0x2 = color, 0x5 = normal, 0x8 = spec
		if (img->semantic == 0x5 || img->semantic == 0x8)
		{
			return 1;
		}

		return 0;
	}

	__declspec(naked) void load_image_stub()
	{
		const static uint32_t skip_img_addr = 0x6206A3;
		const static uint32_t og_logic_addr = 0x62069B;
		__asm
		{
			pushad;
			push	ebx;					// img
			call	skip_image_load;
			pop		ebx;
			cmp		eax, 1;
			jne		OG_LOGIC;

			popad;
			//xor		eax, eax;
			//mov     eax, 1;
			jmp		skip_img_addr;

			// og code
		OG_LOGIC:
			popad;
			push    ebx;
			mov     edx, edi;
			lea     eax, [esp + 0x10];
			jmp		og_logic_addr;
		}
	}

	// pre RB_DrawDebug :: additional debug functions here
	void additional_debug(Game::GfxViewParms* view_parms)
	{
		if (Dvars::r_showModelNames && Dvars::r_showModelNames->current.enabled)
		{
			const auto data = Game::get_backenddata();

			for (auto s = 0u; s < Game::gfx_world->dpvs.smodelCount; s++)
			{
				const auto smodel = &Game::gfx_world->dpvs.smodelDrawInsts[s];
				if (RtxUtils::distance3(view_parms->origin, smodel->placement.origin) <= 1000.0f)
				{
					const Game::vec3_t pos = { smodel->placement.origin[0], smodel->placement.origin[1], smodel->placement.origin[2] + 4.0f };

					Game::R_AddDebugString(
						&data->debugGlobals,
						pos,
						Game::COLOR_WHITE,
						0.25f,
						smodel->model->name);
				}
			}

			for (auto m = 0u; m < *Game::gfx_scene_model_count; m++)
			{
				const auto scene_model = &Game::gfx_scene_model[m];
				if (RtxUtils::distance3(view_parms->origin, scene_model->placement.base.origin) <= 1000.0f)
				{
					const Game::vec3_t pos = { scene_model->placement.base.origin[0], scene_model->placement.base.origin[1], scene_model->placement.base.origin[2] + 4.0f };

					Game::R_AddDebugString(
						&data->debugGlobals,
						pos,
						Game::COLOR_RED,
						0.25f,
						scene_model->model->name);
				}
			}

			for (auto o = 0u; o < *Game::gfx_scene_dobj_count; o++)
			{
				const auto dobj = &Game::gfx_scene_dobjs[o];
				if (RtxUtils::distance3(view_parms->origin, dobj->placement.base.origin) <= 1000.0f)
				{
					Game::vec3_t pos = { dobj->placement.base.origin[0], dobj->placement.base.origin[1], dobj->placement.base.origin[2] + 4.0f };

					for (auto sub = 0u; sub < static_cast<std::uint8_t>(dobj->obj->numModels); sub++)
					{
						Game::R_AddDebugString(
							&data->debugGlobals,
							pos,
							Game::COLOR_GREEN,
							0.25f,
							dobj->obj->models[sub]->name);

						pos[2] -= 5.0f;
					}
				}
			}
		}
	}

	// additional_debug :: hook RB_DrawDebug call to implement additional debug functions
	__declspec(naked) void RB_EndSceneRendering_stub()
	{
		const static uint32_t RB_DrawDebug_func = 0x636D30;
		__asm
		{
			pushad;
			push	[0x1D44450];
			call	additional_debug;
			add		esp, 4h;
			popad;

			call	RB_DrawDebug_func;
			retn;
		}
	}

	// R_AllocDynamicIndexBuffer
	int alloc_dynamic_index_buffer(IDirect3DIndexBuffer9** ib, int size_in_bytes, const char* buffer_name)
	{
		if (HRESULT hr = Game::get_device()->CreateIndexBuffer(size_in_bytes, (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY), D3DFMT_INDEX16, D3DPOOL_DEFAULT, ib, nullptr);
			hr < 0)
		{
			const char* msg = Utils::function<const char* __stdcall(HRESULT)>(0x65350E)(hr); // R_ErrorDescription
			msg = Utils::String::VA("DirectX didn't create a 0x%.8x dynamic index buffer: %s\n", size_in_bytes, msg);

			Utils::function<void(const char*)>(0x5951C0)(msg); // Sys_Error
		}

		Game::Com_PrintMessage(0, Utils::String::VA("D3D9: Created Indexbuffer (%s) of size: 0x%.8x\n", buffer_name, size_in_bytes), 0);
		return 0;
	}

	// R_InitDynamicIndexBufferState
	void init_dynamic_index_buffer_state(Game::GfxIndexBufferState* ib, std::uint32_t index_count, const char* buffer_name)
	{
		ib->used = 0;
		ib->total = index_count;

		alloc_dynamic_index_buffer(&ib->buffer, 2 * index_count, buffer_name);
	}

	// R_AllocDynamicVertexBuffer
	char* alloc_dynamic_vertex_buffer(IDirect3DVertexBuffer9** vb, std::uint32_t size_in_bytes, const char* buffer_name)
	{
		if (HRESULT hr = Game::get_device()->CreateVertexBuffer(size_in_bytes, (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY), 0, D3DPOOL_DEFAULT, vb, nullptr);
			hr < 0)
		{
			const char* msg = Utils::function<const char* __stdcall(HRESULT)>(0x65350E)(hr); // R_ErrorDescription
			msg = Utils::String::VA("DirectX didn't create a 0x%.8x dynamic vertex buffer: %s\n", size_in_bytes, msg);

			Utils::function<void(const char*)>(0x5951C0)(msg); // Sys_Error
		}

		Game::Com_PrintMessage(0, Utils::String::VA("D3D9: Created Vertexbuffer (%s) of size: 0x%.8x\n", buffer_name, size_in_bytes), 0);
		return nullptr;
	}

	// R_InitDynamicVertexBufferState
	void init_dynamic_vertex_buffer_state(Game::GfxVertexBufferState* vb, std::uint32_t bytes, const char* buffer_name)
	{
		vb->used = 0;
		vb->total = static_cast<int>(bytes);
		vb->verts = nullptr;

		alloc_dynamic_vertex_buffer(&vb->buffer, bytes, buffer_name);
	}

	// R_InitTempSkinBuf :: Temp skin buffer within backenddata
	void init_temp_skin_buf(std::uint32_t bytes)
	{
		auto* back_end_data = reinterpret_cast<Game::GfxBackEndData*>(0x1629100);
		for (auto i = 0; i < 2; ++i)
		{
			back_end_data[i].tempSkinBuf = (char*)VirtualAlloc(nullptr, bytes, MEM_RESERVE, PAGE_READWRITE);
			Game::Com_PrintMessage(0, Utils::String::VA("Allocated tempSkinBuffer of size: 0x%.8x\n", bytes), 0);
		}
	}

	constexpr std::uint32_t dynamic_vb_size = 2 * 1048576;
	constexpr std::uint32_t skinned_cache_size = 8 * 1048576;
	constexpr std::uint32_t temp_skin_size = 12 * 1048576;
	constexpr std::uint32_t dynamic_ib_size = 4 * 1048576;
	constexpr std::uint32_t pretess_ib_size = 4 * 1048576;

	// R_CreateDynamicBuffers
	void create_dynamic_buffers()
	{
		init_dynamic_vertex_buffer_state(&Game::gfx_buf->dynamicVertexBufferPool[0], dynamic_vb_size, "dynamicVertexBufferPool");
		Game::gfx_buf->dynamicVertexBuffer = Game::gfx_buf->dynamicVertexBufferPool;

		init_dynamic_vertex_buffer_state(&Game::gfx_buf->skinnedCacheVbPool[0], skinned_cache_size, "skinnedCacheVbPool");
		init_dynamic_vertex_buffer_state(&Game::gfx_buf->skinnedCacheVbPool[1], skinned_cache_size, "skinnedCacheVbPool");
		init_temp_skin_buf(temp_skin_size);

		init_dynamic_index_buffer_state(&Game::gfx_buf->dynamicIndexBufferPool[0], dynamic_ib_size, "dynamicIndexBufferPool");
		Game::gfx_buf->dynamicIndexBuffer = Game::gfx_buf->dynamicIndexBufferPool;

		init_dynamic_index_buffer_state(&Game::gfx_buf->preTessIndexBufferPool[0], pretess_ib_size, "preTessIndexBufferPool");
		init_dynamic_index_buffer_state(&Game::gfx_buf->preTessIndexBufferPool[1], pretess_ib_size, "preTessIndexBufferPool");
		Game::gfx_buf->preTessBufferFrame = 0;
		Game::gfx_buf->preTessIndexBuffer = Game::gfx_buf->preTessIndexBufferPool;
	}

	/* ---------------------------------------------------------- */
	/* ------ change warning limits to fit new buffer size ------ */

	__declspec(naked) void r_warn_temp_skin_size_limit_stub()
	{
		const static uint32_t retn_addr = 0x61E388;
		__asm
		{
			// replace 'cmp edx, 480000h'

			push	eax;
			mov		eax, temp_skin_size;
			cmp		edx, eax;					// warning limit compare
			pop		eax;

			jmp		retn_addr;
		}
	}

	__declspec(naked) void r_warn_max_skinned_cache_vertices_limit_stub()
	{
		const static uint32_t retn_addr = 0x61E262;
		__asm
		{
			// replace 'cmp edx, 480000h'

			push	eax;
			mov		eax, skinned_cache_size;
			cmp		edx, eax;					// warning limit compare
			pop		eax;

			jmp		retn_addr;
		}
	}

	// *
	// Event stubs

	// > RtxFixedFunction::init_fixed_function_buffers_stub
	void Rtx::on_map_load()
	{
		RtxMapSettings::get()->set_settings_for_loaded_map();
		Rtx::set_dvars_defaults_on_mapload();

		// increase culling distances for certain objects
		if (Game::gfx_world)
		{
			for (auto m = 0u; m < Game::gfx_world->dpvs.smodelCount; m++)
			{
				if (std::string_view(Game::gfx_world->dpvs.smodelDrawInsts[m].model->name)._Equal("cs_cargoship_wall_light_on"))
				{
					Game::gfx_world->dpvs.smodelDrawInsts[m].cullDist = 20000.0f;
				}
			}
		}
	}

	// > RtxFixedFunction::free_fixed_function_buffers_stub
	void Rtx::on_map_shutdown()
	{
	}

	Rtx::Rtx()
	{
		/*
		* Increase the amount of skinned vertices (bone controlled meshes) per frame.
		*      (R_MAX_SKINNED_CACHE_VERTICES | TEMP_SKIN_BUF_SIZE) Warnings
		*           'r_fastSkin' or 'r_skinCache' needs to be disabled or
		*			  the client will crash if you hit an unkown limit
		*/

		// Create dynamic rendering buffers
		Utils::Hook(0x5D8BD2, create_dynamic_buffers, HOOK_CALL).install()->quick();

		// Change 'R_WARN_TEMP_SKIN_BUF_SIZE' warning limit to new buffer size
		Utils::Hook::Nop(0x61E382, 6); // clear
		Utils::Hook(0x61E382, r_warn_temp_skin_size_limit_stub, HOOK_JUMP).install()->quick();

		// Change 'R_WARN_MAX_SKINNED_CACHE_VERTICES' warning limit to new buffer size
		Utils::Hook::Nop(0x61E25C, 6); // clear
		Utils::Hook(0x61E25C, r_warn_max_skinned_cache_vertices_limit_stub, HOOK_JUMP).install()->quick();

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

		// hook beginning of 'RB_CallExecuteRenderCommands' (before UI)
		Utils::Hook(0x5FF651, rb_call_execute_render_commands_stub, HOOK_JUMP).install()->quick();

		// Precaching beyond level load (skysphere spawning)
		Utils::Hook::Set<BYTE>(0x4ECEE6, 0xEB);


		// allow 4k images (skysphere)
		// set img alloc size from 0x600000 to 0x4000000
		Utils::Hook::Set<BYTE>(0x61FE77 + 3, 0x00);
		Utils::Hook::Set<BYTE>(0x61FE77 + 4, 0x04);

		// ^ cmp -> error if image larger
		Utils::Hook::Set<BYTE>(0x61FE39 + 3, 0x00);
		Utils::Hook::Set<BYTE>(0x61FE39 + 4, 0x04);


		// hook R_SetMaterial
		Utils::Hook(0x6278E6, R_SetMaterial_stub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x627A17, R_SetMaterial_Emissive_stub, HOOK_JUMP).install()->quick();

		// msg "too many static models ..." (disabled culling: the engine cant handle modellighting for so many static models, thus not drawing them)
		Utils::Hook::Nop(0x611161, 5); Utils::Hook(0x611161, alloc_smodel_lighting_stub, HOOK_JUMP).install()->quick();

		// modellight alloc
		Utils::Hook::Set(0x615BB0, (PBYTE)"\xB8\x01\x00\x00\x00\xC3", 6); // always "alloc" and return 1 as lightingHandle

		// disable the need for developer 1 to display collisions / debug lines / strings w/e (nop check if dvar is set in RB_EndSceneRendering)
		Utils::Hook::Nop(0x628948, 2);

		// disable loading of specular and normalmaps (de-clutter remix ui)
		if (!Flags::HasFlag("load_normal_spec"))
		{
			Utils::Hook::Nop(0x600F75, 5);
			Utils::Hook::Nop(0x601052, 5);
			Utils::Hook::Nop(0x620694, 7); Utils::Hook(0x620694, load_image_stub, HOOK_JUMP).install()->quick();
		}

		// dxvk's 'EnumAdapterModes' returns a lot of duplicates and the games array only has a capacity of 256 which is not enough depending on max res. and refreshrate
		// fix resolution issues by removing duplicates returned by EnumAdapterModes - then write the array ourselfs
		Utils::Hook(0x5D8E92, resolution::R_EnumDisplayModes_stub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x5D8ED9, resolution::R_EnumDisplayModes_stub2, HOOK_JUMP).install()->quick();
		Utils::Hook::Set<BYTE>(0x5D8E80 + 2, 0x04); // set max array size check to 1024 (check within loop)

		// :*
		Utils::Hook(0x5D8303, fix_aspect_ratio_stub, HOOK_JUMP).install()->quick();

		// *
		// culling

		// rewrite some logic in 'R_AddWorldSurfacesPortalWalk'
		Utils::Hook::Nop(0x5ECB8F, 9); Utils::Hook(0x5ECB8F, cull::cell_stub, HOOK_JUMP).install()->quick();

		// ^ - never show all cells at once when the camera cell index is < 0, we handle that in the func above
		Utils::Hook::Nop(0x5ECB7E, 2);


		// #
		// tweakable culling via dvars

		// disable mins culling
		Utils::Hook(0x6226C3, cull::disable_mins_culling_stub, HOOK_JUMP).install()->quick();

		// disable maxs culling
		Utils::Hook(0x6226F4, cull::disable_maxs_culling_stub, HOOK_JUMP).install()->quick();

		// disable (most) frustum culling
		Utils::Hook::Nop(0x6228FE, 6); Utils::Hook(0x6228FE, cull::disable_frustum_culling_stub, HOOK_JUMP).install()->quick();

		// ^ for smodels
		Utils::Hook::Nop(0x622833, 6); Utils::Hook(0x622833, cull::disable_smodel_culling_stub, HOOK_JUMP).install()->quick();

		// never cull brushmodels via dpvs
		Utils::Hook::Nop(0x62F12B, 2);
		Utils::Hook::Set<BYTE>(0x62F138, 0xEB); // ^

		// ^ scene ents (spawned map markers (script models))
		Utils::Hook::Nop(0x62EF4A, 2);
		Utils::Hook::Set<BYTE>(0x62EF79, 0xEB); // ^

		/*Command::Add("rtx_sky_clear", []() { Rtx::skysphere_spawn(0); });
		Command::Add("rtx_sky_desert", []() { Rtx::skysphere_spawn(1); });
		Command::Add("rtx_sky_city", []() { Rtx::skysphere_spawn(2); });
		Command::Add("rtx_sky_night", []() { Rtx::skysphere_spawn(3); });
		Command::Add("rtx_sky_overcast", []() { Rtx::skysphere_spawn(4); });
		Command::Add("rtx_sky_sunset", []() { Rtx::skysphere_spawn(5); });*/

		// #
		// LOD

		Utils::Hook::Nop(0x5D4ECA, 4); // do not scale lodScaleRigid with fov
		Utils::Hook::Nop(0x5D4ED7, 2); // do not scale lodBiasRigid with fov

		// check if r_forceLod is enabled and force LOD's accordingly
		Utils::Hook(0x5AC840, xmodel_get_lod_for_dist_detour, HOOK_JUMP).install()->quick();

		// ^ but inlined ..... for all other static models (R_AddAllStaticModelSurfacesCamera)
		Utils::Hook::Nop(0x611125, 6); Utils::Hook(0x611125, xmodel_get_lod_for_dist_inlined, HOOK_JUMP).install()->quick();

		// ^ inlined .. dobj's (DObjGetSurfaceData)
		Utils::Hook::Nop(0x59B663, 6); Utils::Hook(0x59B663, xmodel_get_lod_for_dist_inlined2, HOOK_JUMP).install()->quick();

		Command::Add("borderless", [this]()
		{
			const auto hwnd = Game::dx->windows->hwnd ? Game::dx->windows->hwnd : FindWindow(nullptr, L"IW3SP_MOD");

			// calculate titlebar height
			RECT w, c; GetWindowRect(hwnd, &w); GetClientRect(hwnd, &c);
			Rtx::noborder_titlebar_height = (w.bottom - w.top) - (c.bottom - c.top);

			SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			SetWindowPos(hwnd, nullptr, 0, 0, Game::dx->windows->width, Game::dx->windows->height, SWP_SHOWWINDOW | SWP_NOACTIVATE);
		});

		Command::Add("windowed", [this]()
		{
			if (Rtx::noborder_titlebar_height)
			{
				const auto hwnd = Game::dx->windows->hwnd ? Game::dx->windows->hwnd : FindWindow(nullptr, L"IW3SP_MOD");
				SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
				SetWindowPos(hwnd, nullptr, 0, 0, Game::dx->windows->width, Game::dx->windows->height + Rtx::noborder_titlebar_height, SWP_SHOWWINDOW | SWP_NOACTIVATE);
			}
		});

		// #
		// dvars

		Events::OnDvarInit([]
		{
			Dvars::rtx_extend_smodel_drawing = Dvars::Register::Dvar_RegisterBool("rtx_extend_smodel_drawing", "Disable static model drawing limit", false, Game::saved);

			Dvars::rtx_culling_tweak_mins = Dvars::Register::Dvar_RegisterBool(
				/* name		*/ "rtx_culling_tweak_mins",
				/* desc		*/ "Disable dpvs mins check (reduces culling)",
				/* default	*/ false,
				/* flags	*/ Game::dvar_flags::saved);

			Dvars::rtx_culling_tweak_maxs = Dvars::Register::Dvar_RegisterBool(
				/* name		*/ "rtx_culling_tweak_maxs",
				/* desc		*/ "Disable dpvs maxs check (reduces culling)",
				/* default	*/ false,
				/* flags	*/ Game::dvar_flags::saved);

			Dvars::rtx_culling_tweak_frustum = Dvars::Register::Dvar_RegisterBool(
				/* name		*/ "rtx_culling_tweak_frustum",
				/* desc		*/ "Disable (most) frustum culling (reduces culling)",
				/* default	*/ true,
				/* flags	*/ Game::dvar_flags::saved);

			Dvars::rtx_culling_tweak_smodel = Dvars::Register::Dvar_RegisterBool(
				/* name		*/ "rtx_culling_tweak_smodel",
				/* desc		*/ "Disable static model frustum culling (reduces culling)",
				/* default	*/ true,
				/* flags	*/ Game::dvar_flags::saved);

			Dvars::rtx_culling_plane_dist = Dvars::Register::Dvar_RegisterFloat(
				/* name		*/ "rtx_culling_plane_dist",
				/* desc		*/ "Distance of dpvs culling-planes",
				/* default	*/ 1250.0f,
				/* minVal	*/ -50000.0f,
				/* maxVal	*/ 50000.0f,
				/* flags	*/ Game::dvar_flags::saved);

			Dvars::r_showCellIndex = Dvars::Register::Dvar_RegisterBool(
				/* name		*/ "r_showCellIndex",
				/* desc		*/ "draw cell index at the center of current cell (useful for map_settings)",
				/* default	*/ false,
				/* flags	*/ Game::dvar_flags::none);

			static const char* r_showTess_enum[] = { "off", "model_techset", "model_material", "bsp_techset", "bsp_material", "bmodel_techset", "bmodel_material" };
			Dvars::r_showTess = Dvars::Register::Dvar_RegisterEnum(
				/* name		*/ "r_showTess",
				/* desc		*/ "surface data info",
				/* default	*/ 0,
				/* enumSize */ 7,
				/* enumData */ r_showTess_enum,
				/* flags	*/ Game::dvar_flags::none);

			// hook RB_DrawDebug call in RB_EndSceneRendering to implement additional debug functions
			Utils::Hook(0x628959, RB_EndSceneRendering_stub, HOOK_CALL).install()->quick();

			Dvars::r_showModelNames = Dvars::Register::Dvar_RegisterBool(
				/* name		*/ "r_showModelNames",
				/* desc		*/ "Show names of static models as 3D Text",
				/* default	*/ false,
				/* flags	*/ Game::dvar_flags::none);
		});
	}
}