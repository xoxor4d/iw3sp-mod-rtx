#include "STDInc.hpp"
#include "Gamepad.hpp"

namespace Components
{
	Game::ButtonToCodeMap_t Gamepad::buttonList[]
	{
		{Game::GPAD_X, Game::K_BUTTON_X},
		{Game::GPAD_A, Game::K_BUTTON_A},
		{Game::GPAD_B, Game::K_BUTTON_B},
		{Game::GPAD_Y, Game::K_BUTTON_Y},
		{Game::GPAD_L_TRIG, Game::K_BUTTON_LTRIG},
		{Game::GPAD_R_TRIG, Game::K_BUTTON_RTRIG},
		{Game::GPAD_L_SHLDR, Game::K_BUTTON_LSHLDR},
		{Game::GPAD_R_SHLDR, Game::K_BUTTON_RSHLDR},
		{Game::GPAD_START, Game::K_BUTTON_START},
		{Game::GPAD_BACK, Game::K_BUTTON_BACK},
		{Game::GPAD_L3, Game::K_BUTTON_LSTICK},
		{Game::GPAD_R3, Game::K_BUTTON_RSTICK},
		{Game::GPAD_UP, Game::K_DPAD_UP},
		{Game::GPAD_DOWN, Game::K_DPAD_DOWN},
		{Game::GPAD_LEFT, Game::K_DPAD_LEFT},
		{Game::GPAD_RIGHT, Game::K_DPAD_RIGHT},
	};

	Game::StickToCodeMap_t Gamepad::analogStickList[4]
	{
		{Game::GPAD_LX, Game::K_APAD_RIGHT, Game::K_APAD_LEFT},
		{Game::GPAD_LY, Game::K_APAD_UP, Game::K_APAD_DOWN},
		{Game::GPAD_RX, Game::K_APAD_RIGHT, Game::K_APAD_LEFT},
		{Game::GPAD_RY, Game::K_APAD_UP, Game::K_APAD_DOWN},
	};

	Game::GamePadStick Gamepad::stickForAxis[Game::GPAD_PHYSAXIS_COUNT]
	{
		Game::GPAD_RX,
		Game::GPAD_RY,
		Game::GPAD_LX,
		Game::GPAD_LY,
		Game::GPAD_INVALID,
		Game::GPAD_INVALID
	};

	Game::GamepadPhysicalAxis Gamepad::axisSameStick[Game::GPAD_PHYSAXIS_COUNT]
	{
		Game::GPAD_PHYSAXIS_RSTICK_Y,
		Game::GPAD_PHYSAXIS_RSTICK_X,
		Game::GPAD_PHYSAXIS_LSTICK_Y,
		Game::GPAD_PHYSAXIS_LSTICK_X,
		Game::GPAD_PHYSAXIS_NONE,
		Game::GPAD_PHYSAXIS_NONE
	};

	const char* Gamepad::physicalAxisNames[Game::GPAD_PHYSAXIS_COUNT]
	{
		"A_RSTICK_X",
		"A_RSTICK_Y",
		"A_LSTICK_X",
		"A_LSTICK_Y",
		"A_RTRIGGER",
		"A_LTRIGGER"
	};

	const char* Gamepad::virtualAxisNames[Game::GPAD_VIRTAXIS_COUNT]
	{
		"VA_SIDE",
		"VA_FORWARD",
		"VA_UP",
		"VA_YAW",
		"VA_PITCH",
		"VA_ATTACK"
	};

	const char* Gamepad::gamePadMappingTypeNames[Game::GPAD_MAP_COUNT]
	{
		"MAP_LINEAR",
		"MAP_SQUARED"
	};

	Game::keyNum_t Gamepad::menuScrollButtonList[]
	{
		Game::K_APAD_UP,
		Game::K_APAD_DOWN,
		Game::K_APAD_LEFT,
		Game::K_APAD_RIGHT,
		Game::K_DPAD_UP,
		Game::K_DPAD_DOWN,
		Game::K_DPAD_LEFT,
		Game::K_DPAD_RIGHT
	};

	Game::keyname_t Gamepad::extendedKeyNames[]
	{
		{"BUTTON_A", Game::K_BUTTON_A},
		{"BUTTON_B", Game::K_BUTTON_B},
		{"BUTTON_X", Game::K_BUTTON_X},
		{"BUTTON_Y", Game::K_BUTTON_Y},
		{"BUTTON_LSHLDR", Game::K_BUTTON_LSHLDR},
		{"BUTTON_RSHLDR", Game::K_BUTTON_RSHLDR},
		{"BUTTON_START", Game::K_BUTTON_START},
		{"BUTTON_BACK", Game::K_BUTTON_BACK},
		{"BUTTON_LSTICK", Game::K_BUTTON_LSTICK},
		{"BUTTON_RSTICK", Game::K_BUTTON_RSTICK},
		{"BUTTON_LTRIG", Game::K_BUTTON_LTRIG},
		{"BUTTON_RTRIG", Game::K_BUTTON_RTRIG},
		{"DPAD_UP", Game::K_DPAD_UP},
		{"DPAD_DOWN", Game::K_DPAD_DOWN},
		{"DPAD_LEFT", Game::K_DPAD_LEFT},
		{"DPAD_RIGHT", Game::K_DPAD_RIGHT},
	};

	Game::keyname_t Gamepad::extendedLocalizedKeyNamesXenon[]
	{
		// Material text icons pattern: 0x01 width height material_name_len
		{"^\x01\x32\x32\x0E""button_xbox_01", Game::K_BUTTON_A},
		{"^\x01\x32\x32\x0E""button_xbox_02", Game::K_BUTTON_B},
		{"^\x01\x32\x32\x0E""button_xbox_03", Game::K_BUTTON_X},
		{"^\x01\x32\x32\x0E""button_xbox_04", Game::K_BUTTON_Y},
		{"^\x01\x32\x32\x0E""button_xbox_05", Game::K_BUTTON_LSHLDR},
		{"^\x01\x32\x32\x0E""button_xbox_06", Game::K_BUTTON_RSHLDR},
		{"^\x01\x32\x32\x0E""button_xbox_07", Game::K_BUTTON_START},
		{"^\x01\x32\x32\x0E""button_xbox_08", Game::K_BUTTON_BACK},
		{"^\x01\x48\x32\x0E""button_xbox_09", Game::K_BUTTON_LSTICK},
		{"^\x01\x48\x32\x0E""button_xbox_10", Game::K_BUTTON_RSTICK},
		{"^\x01\x32\x32\x0E""button_xbox_11", Game::K_BUTTON_LTRIG},
		{"^\x01\x32\x32\x0E""button_xbox_12", Game::K_BUTTON_RTRIG},
		{"^\x01\x32\x32\x0E""button_xbox_13", Game::K_DPAD_UP},
		{"^\x01\x32\x32\x0E""button_xbox_14", Game::K_DPAD_DOWN},
		{"^\x01\x32\x32\x0E""button_xbox_15", Game::K_DPAD_LEFT},
		{"^\x01\x32\x32\x0E""button_xbox_16", Game::K_DPAD_RIGHT},
	};

	Game::keyname_t Gamepad::extendedLocalizedKeyNamesPs3[]
	{
		// Material text icons pattern: 0x01 width height material_name_len
		{"^\x01\x32\x32\x0E""button_sony_01", Game::K_BUTTON_A},
		{"^\x01\x32\x32\x0E""button_sony_02", Game::K_BUTTON_B},
		{"^\x01\x32\x32\x0E""button_sony_03", Game::K_BUTTON_X},
		{"^\x01\x32\x32\x0E""button_sony_04", Game::K_BUTTON_Y},
		{"^\x01\x32\x32\x0E""button_sony_05", Game::K_BUTTON_LSHLDR},
		{"^\x01\x32\x32\x0E""button_sony_06", Game::K_BUTTON_RSHLDR},
		{"^\x01\x32\x24\x0E""button_sony_07", Game::K_BUTTON_START},
		{"^\x01\x32\x24\x0E""button_sony_08", Game::K_BUTTON_BACK},
		{"^\x01\x48\x32\x0E""button_sony_09", Game::K_BUTTON_LSTICK},
		{"^\x01\x48\x32\x0E""button_sony_10", Game::K_BUTTON_RSTICK},
		{"^\x01\x32\x32\x0E""button_sony_11", Game::K_BUTTON_LTRIG},
		{"^\x01\x32\x32\x0E""button_sony_12", Game::K_BUTTON_RTRIG},
		{"^\x01\x32\x32\x0E""button_sony_13", Game::K_DPAD_UP},
		{"^\x01\x32\x32\x0E""button_sony_14", Game::K_DPAD_DOWN},
		{"^\x01\x32\x32\x0E""button_sony_15", Game::K_DPAD_LEFT},
		{"^\x01\x32\x32\x0E""button_sony_16", Game::K_DPAD_RIGHT},
	};

	Game::keyname_t Gamepad::combinedKeyNames[Game::KEY_NAME_COUNT + std::extent_v<decltype(extendedKeyNames)> +1];
	Game::keyname_t Gamepad::combinedLocalizedKeyNamesXenon[Game::KEY_NAME_COUNT + std::extent_v<decltype(extendedLocalizedKeyNamesXenon)> +1];
	Game::keyname_t Gamepad::combinedLocalizedKeyNamesPs3[Game::KEY_NAME_COUNT + std::extent_v<decltype(extendedLocalizedKeyNamesPs3)> +1];

	Gamepad::ControllerMenuKeyMapping Gamepad::controllerMenuKeyMappings[]
	{
		{Game::K_BUTTON_A, Game::K_ENTER},
		{Game::K_BUTTON_START, Game::K_ENTER},
		{Game::K_BUTTON_B, Game::K_ESCAPE},
		{Game::K_BUTTON_BACK, Game::K_ESCAPE},
		{Game::K_DPAD_UP, Game::K_UPARROW},
		{Game::K_APAD_UP, Game::K_UPARROW},
		{Game::K_DPAD_DOWN, Game::K_DOWNARROW},
		{Game::K_APAD_DOWN, Game::K_DOWNARROW},
		{Game::K_DPAD_LEFT, Game::K_LEFTARROW},
		{Game::K_APAD_LEFT, Game::K_LEFTARROW},
		{Game::K_DPAD_RIGHT, Game::K_RIGHTARROW},
		{Game::K_APAD_RIGHT, Game::K_RIGHTARROW},
	};

	Gamepad::GamePad Gamepad::gamePads[Game::MAX_GPAD_COUNT]{};
	Gamepad::GamePadGlobals Gamepad::gamePadGlobals[Game::MAX_GPAD_COUNT]{ {} };
	int Gamepad::gamePadBindingsModifiedFlags = 0;

	Gamepad::GamePadGlobals::GamePadGlobals()
		: axes{},
		nextScrollTime(0)
	{
		for (auto& virtualAxis : axes.virtualAxes)
		{
			virtualAxis.physicalAxis = Game::GPAD_PHYSAXIS_NONE;
			virtualAxis.mapType = Game::GPAD_MAP_NONE;
		}
	}

	float Gamepad::AimAssist_Lerp(const float from, const float to, const float fraction)
	{
		return (to - from) * fraction + from;
	}

	float Gamepad::LinearTrack(const float target, const float current, const float rate, const float deltaTime)
	{
		const auto err = target - current;
		float step;
		if (err <= 0.0f)
		{
			step = -rate * deltaTime;
		}
		else
		{
			step = rate * deltaTime;
		}

		if (std::fabs(err) <= 0.001f)
		{
			return target;
		}

		if (std::fabs(step) <= std::fabs(err))
		{
			return current + step;
		}

		return target;
	}

	void Gamepad::AimAssist_CalcAdjustedAxis(const Game::AimInput* input, float* pitchAxis, float* yawAxis)
	{
		assert(input);
		assert(pitchAxis);
		assert(yawAxis);

		const auto graphIndex = Dvars::Functions::Dvar_FindVar("aim_input_graph_index")->current.integer;
		if (Dvars::Functions::Dvar_FindVar("aim_input_graph_enabled")->current.enabled && graphIndex >= 0 && static_cast<unsigned>(graphIndex) < 4)
		{
			const auto deflection = std::sqrt(input->pitchAxis * input->pitchAxis + input->yawAxis * input->yawAxis);

			float fraction;
			if (deflection - 1.0f < 0.0f)
				fraction = deflection;
			else
				fraction = 1.0f;

			if (0.0f - deflection >= 0.0f)
				fraction = 0.0f;

			const auto graphScale = Game::GraphFloat_GetValue(&Game::aaInputGraph[graphIndex], fraction);
			*pitchAxis = input->pitchAxis * graphScale;
			*yawAxis = input->yawAxis * graphScale;
		}
		else
		{
			*pitchAxis = input->pitchAxis;
			*yawAxis = input->yawAxis;
		}

		if (Dvars::Functions::Dvar_FindVar("aim_scale_view_axis")->current.enabled)
		{
			const auto absPitchAxis = std::fabs(*pitchAxis);
			const auto absYawAxis = std::fabs(*yawAxis);

			if (absPitchAxis <= absYawAxis)
			{
				*pitchAxis = (1.0f - (absYawAxis - absPitchAxis)) * *pitchAxis;
			}
			else
			{
				*yawAxis = (1.0f - (absPitchAxis - absYawAxis)) * *yawAxis;
			}
		}
	}

	bool Gamepad::AimAssist_IsSlowdownActive(const Game::AimInput* input)
	{
		if (!Dvars::Functions::Dvar_FindVar("aim_slowdown_enabled")->current.enabled || !Dvars::Functions::Dvar_FindVar("gpad_slowdown_enabled")->current.enabled)
		{
			return false;
		}

		if (!input->ps->weapon)
		{
			return false;
		}

		const auto* weaponDef = Game::BG_WeaponNames[input->ps->weapon];
		if (weaponDef->requireLockonToFire)
		{
			return false;
		}

		if (input->ps->eFlags & 0x300)
		{
			return false;
		}

		if (!Game::BG_WeaponAmmo(input->ps->weapon, input->ps))
		{
			return false;
		}

		return true;
	}

	void Gamepad::AimAssist_CalcSlowdown(const Game::AimInput* input, float* pitchScale, float* yawScale)
	{
		assert(input);
		assert(pitchScale);
		assert(yawScale);
		AssertIn(input->localClientNum, 1);

		auto& aaGlob = Game::aaGlobArray[input->localClientNum];

		*pitchScale = 1.0f;
		*yawScale = 1.0f;

		if (!AimAssist_IsSlowdownActive(input))
		{
			return;
		}

		const auto* weaponDef = Game::BG_WeaponNames[input->ps->weapon];
		const auto aimAssistRange = AimAssist_Lerp(weaponDef->aimAssistRange, weaponDef->aimAssistRangeAds, aaGlob.adsLerp) * Dvars::Functions::Dvar_FindVar("aim_aimAssistRangeScale")->current.value;
		const auto screenTarget = AimAssist_GetBestTarget(&aaGlob, aimAssistRange, aaGlob.tweakables.slowdownRegionWidth, aaGlob.tweakables.slowdownRegionHeight);

		if (screenTarget)
		{
			*pitchScale = AimAssist_Lerp(Dvars::Functions::Dvar_FindVar("aim_slowdown_pitch_scale")->current.value, Dvars::Functions::Dvar_FindVar("aim_slowdown_pitch_scale_ads")->current.value, aaGlob.adsLerp);
			*yawScale = AimAssist_Lerp(Dvars::Functions::Dvar_FindVar("aim_slowdown_yaw_scale")->current.value, Dvars::Functions::Dvar_FindVar("aim_slowdown_yaw_scale_ads")->current.value, aaGlob.adsLerp);
		}

		if (AimAssist_IsPlayerUsingOffhand(input))
		{
			*pitchScale = 1.0f;
		}
	}


	void Gamepad::AimAssist_ApplyTurnRates(const Game::AimInput* input, Game::AimOutput* output)
	{
		assert(input);
		assert(output);
		AssertIn(input->localClientNum, 1);

		auto& aaGlob = Game::aaGlobArray[input->localClientNum];

		auto slowdownPitchScale = 0.0f;
		auto slowdownYawScale = 0.0f;
		float adjustedPitchAxis = 0.0f;
		float adjustedYawAxis = 0.0f;

		if (aaGlob.autoMeleeActive)
		{
			adjustedPitchAxis = 0.0f;
			adjustedYawAxis = 0.0f;
			slowdownPitchScale = 1.0f;
			slowdownYawScale = 1.0f;
		}
		else
		{
			AimAssist_CalcAdjustedAxis(input, &adjustedPitchAxis, &adjustedYawAxis);
			AimAssist_CalcSlowdown(input, &slowdownPitchScale, &slowdownYawScale);
		}

		const auto sensitivity = Dvars::Functions::Dvar_FindVar("input_viewSensitivity")->current.value;
		auto pitchTurnRate = AimAssist_Lerp(Dvars::Functions::Dvar_FindVar("aim_turnrate_pitch")->current.value, Dvars::Functions::Dvar_FindVar("aim_turnrate_pitch_ads")->current.value, aaGlob.adsLerp);
		pitchTurnRate = slowdownPitchScale * aaGlob.fovTurnRateScale * sensitivity * pitchTurnRate;
		auto yawTurnRate = AimAssist_Lerp(Dvars::Functions::Dvar_FindVar("aim_turnrate_yaw")->current.value, Dvars::Functions::Dvar_FindVar("aim_turnrate_yaw_ads")->current.value, aaGlob.adsLerp);
		yawTurnRate = slowdownYawScale * aaGlob.fovTurnRateScale * sensitivity * yawTurnRate;

		if (input->pitchMax > 0 && input->pitchMax < pitchTurnRate)
			pitchTurnRate = input->pitchMax;
		if (input->yawMax > 0 && input->yawMax < yawTurnRate)
			yawTurnRate = input->yawMax;

		const auto pitchSign = adjustedPitchAxis >= 0.0f ? 1.0f : -1.0f;
		const auto yawSign = adjustedYawAxis >= 0.0f ? 1.0f : -1.0f;

		const auto pitchDelta = std::fabs(adjustedPitchAxis) * pitchTurnRate;
		const auto yawDelta = std::fabs(adjustedYawAxis) * yawTurnRate;

		if (!Dvars::Functions::Dvar_FindVar("aim_accel_turnrate_enabled")->current.enabled)
		{
			aaGlob.pitchDelta = pitchDelta;
			aaGlob.yawDelta = yawDelta;
		}
		else
		{
			const auto accel = Dvars::Functions::Dvar_FindVar("aim_accel_turnrate_lerp")->current.value * sensitivity;
			if (pitchDelta <= aaGlob.pitchDelta)
				aaGlob.pitchDelta = pitchDelta;
			else
				aaGlob.pitchDelta = LinearTrack(pitchDelta, aaGlob.pitchDelta, accel, input->deltaTime);

			if (yawDelta <= aaGlob.yawDelta)
				aaGlob.yawDelta = yawDelta;
			else
				aaGlob.yawDelta = LinearTrack(yawDelta, aaGlob.yawDelta, accel, input->deltaTime);
		}

		output->pitch += aaGlob.pitchDelta * input->deltaTime * pitchSign;
		output->yaw += aaGlob.yawDelta * input->deltaTime * yawSign;
	}

	bool Gamepad::AimAssist_DoBoundsIntersectCenterBox(const float* clipMins, const float* clipMaxs, const float clipHalfWidth, const float clipHalfHeight)
	{
		return clipHalfWidth >= clipMins[0] && clipMaxs[0] >= -clipHalfWidth && clipHalfHeight >= clipMins[1] && clipMaxs[1] >= -clipHalfHeight;
	}

	bool Gamepad::AimAssist_IsPlayerUsingOffhand(const Game::AimInput* input)
	{
		// Check offhand flag
		if ((input->ps->weapFlags & 2) == 0)
		{
			return false;
		}

		// If offhand weapon has no id we are not using one
		if (!input->ps->weapon)
		{
			return false;
		}

		const auto* weaponDef = Game::BG_WeaponNames[input->ps->weapon];

		return weaponDef->offhandClass != Game::OFFHAND_CLASS_NONE;
	}

	bool Gamepad::AimAssist_IsLockonActive(const int localClientNum, const Game::AimInput* input)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		if (!Dvars::Functions::Dvar_FindVar("aim_lockon_enabled")->current.enabled || !Dvars::Functions::Dvar_FindVar("gpad_lockon_enabled")->current.enabled)
		{
			return false;
		}

		auto& aaGlob = Game::aaGlobArray[localClientNum];
		if (AimAssist_IsPlayerUsingOffhand(input))
		{
			return false;
		}

		if (aaGlob.autoAimActive || aaGlob.autoMeleeActive)
		{
			return false;
		}

		return true;
	}

	const Game::AimScreenTarget* Gamepad::AimAssist_GetBestTarget(const Game::AimAssistGlobals* aaGlob, const float range, const float regionWidth, const float regionHeight)
	{
		const auto rangeSqr = range * range;
		for (auto targetIndex = 0; targetIndex < aaGlob->screenTargetCount; targetIndex++)
		{
			const auto* currentTarget = &aaGlob->screenTargets[targetIndex];
			if (currentTarget->distSqr <= rangeSqr && AimAssist_DoBoundsIntersectCenterBox(currentTarget->clipMins, currentTarget->clipMaxs, regionWidth, regionHeight))
			{
				return currentTarget;
			}
		}

		return nullptr;
	}

	const Game::AimScreenTarget* Gamepad::AimAssist_GetTargetFromEntity(const Game::AimAssistGlobals* aaGlob, const int entIndex)
	{
		if (entIndex == Game::AIM_TARGET_INVALID)
		{
			return nullptr;
		}

		for (auto targetIndex = 0; targetIndex < aaGlob->screenTargetCount; targetIndex++)
		{
			const auto* currentTarget = &aaGlob->screenTargets[targetIndex];
			if (currentTarget->entIndex == entIndex)
			{
				return currentTarget;
			}
		}

		return nullptr;
	}

	const Game::AimScreenTarget* Gamepad::AimAssist_GetPrevOrBestTarget(const Game::AimAssistGlobals* aaGlob, const float range, const float regionWidth, const float regionHeight,
		const int prevTargetEnt)
	{
		const auto screenTarget = AimAssist_GetTargetFromEntity(aaGlob, prevTargetEnt);

		if (screenTarget && (range * range) > screenTarget->distSqr && AimAssist_DoBoundsIntersectCenterBox(screenTarget->clipMins, screenTarget->clipMaxs, regionWidth, regionHeight))
		{
			return screenTarget;
		}

		return AimAssist_GetBestTarget(aaGlob, range, regionWidth, regionHeight);
	}


	void Gamepad::AimAssist_ApplyLockOn(const Game::AimInput* input, Game::AimOutput* output)
	{
		assert(input);
		assert(output);
		AssertIn(input->localClientNum, 1);

		auto& aaGlob = Game::aaGlobArray[input->localClientNum];

		const auto prevTargetEnt = aaGlob.lockOnTargetEnt;
		aaGlob.lockOnTargetEnt = Game::AIM_TARGET_INVALID;

		if (!AimAssist_IsLockonActive(input->localClientNum, input))
		{
			return;
		}

		const auto* weaponDef = Game::BG_WeaponNames[input->ps->weapon];
		if (weaponDef->requireLockonToFire)
		{
			return;
		}
		
		const auto deflection = Dvars::Functions::Dvar_FindVar("aim_lockon_deflection")->current.value;
		if (deflection > std::fabs(input->pitchAxis) && deflection > std::fabs(input->yawAxis) && deflection > std::fabs(input->rightAxis))
		{
			return;
		}

		if (!input->ps->weapon)
		{
			return;
		}

		const auto aimAssistRange = AimAssist_Lerp(weaponDef->aimAssistRange, weaponDef->aimAssistRangeAds, aaGlob.adsLerp) * Dvars::Functions::Dvar_FindVar("aim_aimAssistRangeScale")->current.value;
		const auto screenTarget = AimAssist_GetPrevOrBestTarget(&aaGlob, aimAssistRange, aaGlob.tweakables.lockOnRegionWidth, aaGlob.tweakables.lockOnRegionHeight, prevTargetEnt);

		if (screenTarget && screenTarget->distSqr > 0.0f)
		{
			aaGlob.lockOnTargetEnt = screenTarget->entIndex;
			const auto arcLength = std::sqrt(screenTarget->distSqr) * static_cast<float>(M_PI);

			const auto pitchTurnRate =
				(screenTarget->velocity[0] * aaGlob.viewAxis[2][0] + screenTarget->velocity[1] * aaGlob.viewAxis[2][1] + screenTarget->velocity[2] * aaGlob.viewAxis[2][2]
					- (input->ps->velocity[0] * aaGlob.viewAxis[2][0] + input->ps->velocity[1] * aaGlob.viewAxis[2][1] + input->ps->velocity[2] * aaGlob.viewAxis[2][2]))
				/ arcLength * 180.0f * Dvars::Functions::Dvar_FindVar("aim_lockon_pitch_strength")->current.value;

			const auto yawTurnRate =
				(screenTarget->velocity[0] * aaGlob.viewAxis[1][0] + screenTarget->velocity[1] * aaGlob.viewAxis[1][1] + screenTarget->velocity[2] * aaGlob.viewAxis[1][2]
					- (input->ps->velocity[0] * aaGlob.viewAxis[1][0] + input->ps->velocity[1] * aaGlob.viewAxis[1][1] + input->ps->velocity[2] * aaGlob.viewAxis[1][2]))
				/ arcLength * 180.0f * Dvars::Functions::Dvar_FindVar("aim_lockon_strength")->current.value;

			output->pitch -= pitchTurnRate * input->deltaTime;
			output->yaw += yawTurnRate * input->deltaTime;
		}
	}

	bool Gamepad::AimAssist_IsAutoAimActive(const int localClientNum, const Game::AimInput* input)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		if (!Dvars::Functions::Dvar_FindVar("aim_autoaim_enabled")->current.enabled || !Dvars::Functions::Dvar_FindVar("gpad_autoaim_enabled")->current.enabled)
		{
			return false;
		}

		auto& aaGlob = Game::aaGlobArray[localClientNum];
		if (!(input->buttons & Game::CMD_BUTTON_ADS))
		{
			return false;
		}

		if (aaGlob.adsLerp == 0.0f)
		{
			return false;
		}

		return true;
	}

	bool Gamepad::AimAssist_UpdateAutoAimTarget(Game::AimAssistGlobals* aaGlob)
	{
		float targetDir[3];
		float targetAngles[3];

		const auto screenTarget = AimAssist_GetTargetFromEntity(aaGlob, aaGlob->autoAimTargetEnt);
		if (!screenTarget)
			return false;

		targetDir[0] = screenTarget->aimPos[0] - aaGlob->viewOrigin[0];
		targetDir[1] = screenTarget->aimPos[1] - aaGlob->viewOrigin[1];
		targetDir[2] = screenTarget->aimPos[2] - aaGlob->viewOrigin[2];
		Game::vectoangles(targetDir, targetAngles);
		aaGlob->autoAimPitchTarget = targetAngles[0];
		aaGlob->autoAimYawTarget = targetAngles[1];
	}

	void Gamepad::AimAssist_ClearAutoAimTarget(Game::AimAssistGlobals* aaGlob)
	{
		aaGlob->autoAimTargetEnt = Game::AIM_TARGET_INVALID;
		aaGlob->autoAimActive = 0;
		aaGlob->autoAimPitch = 0;
		aaGlob->autoAimPitchTarget = 0;
		aaGlob->autoAimYaw = 0;
		aaGlob->autoAimYawTarget = 0;
	}

	void Gamepad::AimAssist_SetAutoAimTarget(Game::AimAssistGlobals* aaGlob, const Game::AimScreenTarget* screenTarget)
	{
		AimAssist_ClearAutoAimTarget(aaGlob);
		aaGlob->autoAimTargetEnt = screenTarget->entIndex;
		aaGlob->autoAimActive = 1;
		aaGlob->autoAimPitch = aaGlob->viewAngles[0];
		aaGlob->autoAimYaw = aaGlob->viewAngles[1];
		AimAssist_UpdateAutoAimTarget(aaGlob);
	}

	void Gamepad::AimAssist_ApplyAutoAim(const Game::AimInput* input, Game::AimOutput* output)
	{
		assert(input);
		assert(output);
		AssertIn(input->localClientNum, 1);

		auto& aaGlob = Game::aaGlobArray[input->localClientNum];
		const auto* weaponDef = Game::BG_WeaponNames[input->ps->weapon];

		if (!input->ps->weapon)
			return;

		if ((input->ps->weaponstate >= Game::WEAPON_RELOADING && input->ps->weaponstate <= Game::WEAPON_RELOAD_END))
			return;

		if (!weaponDef->aimDownSight)
			return;

		if (AimAssist_IsAutoAimActive(input->localClientNum, input))
		{
			if (!aaGlob.autoAimPressed)
			{
				const auto screenTarget = AimAssist_GetBestTarget(&aaGlob, weaponDef->autoAimRange * Dvars::Functions::Dvar_FindVar("aim_autoAimRangeScale")->current.value, aaGlob.tweakables.autoAimRegionWidth, aaGlob.tweakables.autoAimRegionHeight);
				if (screenTarget)
					AimAssist_SetAutoAimTarget(&aaGlob, screenTarget);
				aaGlob.autoAimPressed = 1;
			}
			if (aaGlob.autoAimActive)
			{
				if (AimAssist_UpdateAutoAimTarget(&aaGlob) && aaGlob.adsLerp > 0.0f)
				{
					//Game::Com_Printf(0, "^2auto aim enabled!\n");
					const auto newPitch = Game::DiffTrackAngle(aaGlob.autoAimPitchTarget, aaGlob.autoAimPitch, Dvars::Functions::Dvar_FindVar("aim_autoaim_lerp")->current.value, input->deltaTime);
					const auto newYaw = Game::DiffTrackAngle(aaGlob.autoAimYawTarget, aaGlob.autoAimYaw, Dvars::Functions::Dvar_FindVar("aim_autoaim_lerp")->current.value, input->deltaTime);
					const auto pitchDelta = Game::AngleSubtract(newPitch, aaGlob.autoAimPitch);
					const auto yawDelta = Game::AngleSubtract(newYaw, aaGlob.autoAimYaw);
					aaGlob.autoAimPitch = newPitch;
					aaGlob.autoAimYaw = newYaw;
					output->pitch = output->pitch + pitchDelta;
					output->yaw = output->yaw + yawDelta;
				}
				else
				{
					//Game::Com_Printf(0, "1 jump!\n");
					AimAssist_ClearAutoAimTarget(&aaGlob);
				}
			}
		}
		else
		{
			//Game::Com_Printf(0, "2 jump!\n");
			AimAssist_ClearAutoAimTarget(&aaGlob);
			aaGlob.autoAimPressed = 0;
		}
	}

	void Gamepad::AimAssist_UpdateGamePadInput(const Game::AimInput* input, Game::AimOutput* output)
	{
		assert(input);
		assert(output);
		AssertIn(input->localClientNum, 1);

		auto& aaGlob = Game::aaGlobArray[input->localClientNum];

		output->pitch = input->pitch;
		output->yaw = input->yaw;

		if (aaGlob.initialized)
		{
			Game::AimAssist_UpdateTweakables(input);
			Game::AimAssist_UpdateAdsLerp(input);
			AimAssist_ApplyTurnRates(input, output);
			AimAssist_ApplyAutoAim(input, output);
			Game::AimAssist_ApplyAutoMelee(input, output);
			Game::AimAssist_ApplyMeleeCharge(input, output);
			AimAssist_ApplyLockOn(input, output);
		}
	}

	bool Gamepad::CG_ShouldUpdateViewAngles()
	{
		return !Game::Key_IsCatcherActive(0, Game::KEYCATCH_MASK_ANY);
	}

	float Gamepad::CL_GamepadAxisValue(const Game::GamepadVirtualAxis virtualAxis)
	{
		assert(virtualAxis > Game::GPAD_VIRTAXIS_NONE && virtualAxis < Game::GPAD_VIRTAXIS_COUNT);

		const auto& gamePadGlobal = gamePadGlobals[0];
		const auto& [physicalAxis, mapType] = gamePadGlobal.axes.virtualAxes[virtualAxis];

		if (physicalAxis <= Game::GPAD_PHYSAXIS_NONE || physicalAxis >= Game::GPAD_PHYSAXIS_COUNT)
		{
			return 0.0f;
		}

		auto axisDeflection = gamePadGlobal.axes.axesValues[physicalAxis];

		if (mapType == Game::GPAD_MAP_SQUARED)
		{
			const auto otherAxisSameStick = axisSameStick[physicalAxis];

			float otherAxisDeflection;
			if (otherAxisSameStick <= Game::GPAD_PHYSAXIS_NONE || otherAxisSameStick >= Game::GPAD_PHYSAXIS_COUNT)
			{
				otherAxisDeflection = 0.0f;
			}
			else
			{
				otherAxisDeflection = gamePadGlobal.axes.axesValues[otherAxisSameStick];
			}

			axisDeflection = std::sqrt(axisDeflection * axisDeflection + otherAxisDeflection * otherAxisDeflection) * axisDeflection;
		}

		return axisDeflection;
	}

	char Gamepad::ClampChar(const int value)
	{
		return static_cast<char>(std::clamp<int>(value, std::numeric_limits<char>::min(), std::numeric_limits<char>::max()));
	}

	void Gamepad::CL_GamepadMove(Game::usercmd_s* cmd)
	{
		auto& gamePad = gamePads[0];
		auto& clientActive = Game::clients[0];

		auto pitch = CL_GamepadAxisValue(Game::GPAD_VIRTAXIS_PITCH);
		if (!Dvars::Functions::Dvar_FindVar("input_invertPitch")->current.enabled)
			pitch *= -1;

		auto yaw = -CL_GamepadAxisValue(Game::GPAD_VIRTAXIS_YAW);
		auto forward = CL_GamepadAxisValue(Game::GPAD_VIRTAXIS_FORWARD);
		auto side = CL_GamepadAxisValue(Game::GPAD_VIRTAXIS_SIDE);

		// The game implements an attack axis at this location. This axis is unused however so for this patch it was not implemented.
		//auto attack = CL_GamepadAxisValue(localClientNum, Game::GPAD_VIRTAXIS_ATTACK);

		auto moveScale = static_cast<float>(std::numeric_limits<char>::max());

		if (std::fabs(side) > 0.0f || std::fabs(forward) > 0.0f)
		{
			const auto length = std::fabs(side) <= std::fabs(forward)
									? side / forward
									: forward / side;
			moveScale = std::sqrt((length * length) + 1.0f) * moveScale;
		}

		const auto forwardMove = static_cast<int>(std::floor(forward * moveScale));
		const auto rightMove = static_cast<int>(std::floor(side * moveScale));
		const auto pitchMove = static_cast<int>(std::floor(pitch * moveScale));
		const auto yawMove = static_cast<int>(std::floor(yaw * moveScale));

		cmd->rightmove = ClampChar(cmd->rightmove + rightMove);
		cmd->forwardmove = ClampChar(cmd->forwardmove + forwardMove);
		cmd->pitchmove = ClampChar(cmd->pitchmove + pitchMove);
		cmd->yawmove = ClampChar(cmd->yawmove + yawMove);

		// Check for frozen controls. Flag name should start with PMF_
		if (CG_ShouldUpdateViewAngles() && (clientActive.snap.ps.pm_flags & Game::PMF_FROZEN) == 0)
		{
			Game::AimInput aimInput{};
			Game::AimOutput aimOutput{};

			int cls_framerate = *(int*)(0x85BC10); //temp, because need reverse the sp structure
			aimInput.deltaTime = static_cast<float>(cls_framerate) * 0.001f;
			aimInput.buttons = cmd->buttons;
			aimInput.localClientNum = 0;
			aimInput.ps = &Game::g_clients->ps;
			aimInput.pitch = clientActive.viewangles[0];
			aimInput.pitchAxis = pitch;
			aimInput.pitchMax = clientActive.cgameMaxPitchSpeed;
			aimInput.yaw = clientActive.viewangles[1];
			aimInput.yawAxis = yaw;
			aimInput.yawMax = clientActive.cgameMaxYawSpeed;
			aimInput.forwardAxis = forward;
			aimInput.rightAxis = side;
			AimAssist_UpdateGamePadInput(&aimInput, &aimOutput);
			clientActive.viewangles[0] = aimOutput.pitch;
			clientActive.viewangles[1] = aimOutput.yaw;
			cmd->meleeChargeDist = aimOutput.meleeChargeDist;
			cmd->meleeChargeYaw = aimOutput.meleeChargeYaw;
		}
	}


	void Gamepad::CL_MouseMove(Game::usercmd_s* cmd)
	{
		auto& gamePad = gamePads[0];
		if (!gamePad.inUse)
		{
			Utils::Hook::Call<void(Game::usercmd_s*)>(0x43FA90)(cmd);
		}
		else if (Dvars::Functions::Dvar_FindVar("gpad_enabled")->current.enabled && gamePad.enabled)
		{
			CL_GamepadMove(cmd);
		}
	}

	void Gamepad::Vibrate(int leftVal, int rightVal)
	{
		// Create a Vibraton State
		XINPUT_VIBRATION Vibration;

		// Zeroise the Vibration
		ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

		// Set the Vibration ValuesXInputSetState
		Vibration.wLeftMotorSpeed = leftVal;
		Vibration.wRightMotorSpeed = rightVal;

		// Vibrate the controller
		XInputSetState(0, &Vibration);
	}

	const char* Gamepad::GetGamePadCommand(const char* command)
	{
		if (std::strcmp(command, "+activate") == 0 || std::strcmp(command, "+reload") == 0)
		{
			return "+usereload";
		}

		if (std::strcmp(command, "+melee_breath") == 0)
		{
			return "+holdbreath";
		}

		return command;
	}

	int Gamepad::Key_GetCommandAssignmentInternal([[maybe_unused]] int localClientNum, const char* cmd, int(*keys)[2])
	{
		auto keyCount = 0;

		(*keys)[0] = -1;
		(*keys)[1] = -1;

		if (gamePads[0].inUse)
		{
			const auto gamePadCmd = GetGamePadCommand(cmd);
			for (auto keyNum = 0; keyNum < Game::K_LAST_KEY; keyNum++)
			{
				if (!Key_IsValidGamePadChar(keyNum))
				{
					continue;
				}

				if (Game::playerKeys[0].keys[keyNum].binding && std::strcmp(Game::playerKeys[0].keys[keyNum].binding, gamePadCmd) == 0)
				{
					(*keys)[keyCount++] = keyNum;

					if (keyCount >= 2)
					{
						return keyCount;
					}
				}
			}
		}
		else
		{
			for (auto keyNum = 0; keyNum < Game::K_LAST_KEY; keyNum++)
			{
				if (Key_IsValidGamePadChar(keyNum))
				{
					continue;
				}

				if (Game::playerKeys[0].keys[keyNum].binding && std::strcmp(Game::playerKeys[0].keys[keyNum].binding, cmd) == 0)
				{
					(*keys)[keyCount++] = keyNum;

					if (keyCount >= 2)
					{
						return keyCount;
					}
				}
			}
		}

		return keyCount;
	}


	void __declspec(naked) Gamepad::Key_GetCommandAssignmentInternal_Stub()
	{
		__asm
		{
			push eax
			pushad

			push[esp + 0x20 + 0x4 + 0x8] // keyNums
			push[esp + 0x20 + 0x4 + 0x8] // command
			push eax // localClientNum
			call Key_GetCommandAssignmentInternal
			add esp, 0xC

			mov[esp + 0x20], eax

			popad
			pop eax
			ret
		}
	}

	void Gamepad::Key_SetBinding_Hk(const int localClientNum, const int keyNum, const char* binding)
	{
		if (Key_IsValidGamePadChar(keyNum))
		{
			Dvars::Functions::Dvar_FindVar("gpad_buttonConfig")->current.string = "custom";
			Dvars::Functions::Dvar_FindVar("gpad_buttonConfig")->latched.string = "custom";
		}

		Game::Key_SetBinding(localClientNum, keyNum, binding);
	}

	void Gamepad::CL_KeyEvent_Hk(const int localClientNum, const int key, const int down, const unsigned time)
	{
		// A keyboard key has been pressed. Mark controller as unused.
		gamePads[0].inUse = false;
		Dvars::Functions::Dvar_FindVar("gpad_in_use")->current.enabled = false;
		Dvars::Functions::Dvar_FindVar("gpad_in_use")->latched.enabled = false;

		// Call original function
		Utils::Hook::Call<void(int, int, int, unsigned)>(0x443D60)(localClientNum, key, down, time); //CL_KeyEvent
	}

	void Gamepad::HideSystemCursor()
	{
		ShowCursor(false);
	}

	bool Gamepad::IsGamePadInUse()
	{
		return gamePads[0].inUse;
	}

	void Gamepad::OnMouseMove(const int dx, const int dy)
	{
		if (dx != 0 || dy != 0)
		{
			gamePads[0].inUse = false;
			Dvars::Functions::Dvar_FindVar("gpad_in_use")->current.enabled = false;
			Dvars::Functions::Dvar_FindVar("gpad_in_use")->latched.enabled = false;
		}
	}

	bool Gamepad::UI_RefreshViewport_Hk()
	{
		return IsGamePadInUse();
	}

	Game::keyname_t* Gamepad::GetLocalizedKeyNameMap()
	{
		if(Dvars::Functions::Dvar_FindVar("gpad_style")->current.enabled)
			return combinedLocalizedKeyNamesPs3;
		else
			return combinedLocalizedKeyNamesXenon;
	}

	void __declspec(naked) Gamepad::GetLocalizedKeyName_Stub()
	{
		__asm
		{
			push eax
			pushad

			call GetLocalizedKeyNameMap
			mov	[esp + 0x20], eax

			popad
			pop eax

			// Re-execute last instruction from game to set flags again for upcoming jump
			test edi, edi
			ret
		}
	}

	void __declspec(naked) Gamepad::GetLocalizedKeyName_Stub02()
	{
		__asm
		{
			push eax
			pushad

			call GetLocalizedKeyNameMap
			mov	[esp + 0x20], eax

			popad
			pop eax

			// Re-execute last instruction from game to set flags again for upcoming jump
			cmp ds:0x6DFE30, 0
			ret
		}
	}

	void __declspec(naked) Gamepad::UI_RefreshStub()
	{
		const static uint32_t jump_offset = 0x5653D0;
		const static uint32_t jump_offset_return = 0x565456;
		__asm
		{
			call	Gamepad::UI_RefreshViewport_Hk;
			test	al, al;
			jz		showCursor;

			jmp		jump_offset_return;

		showCursor:
			mov		eax, Game::scrPlace;
			fld		dword ptr[eax];
			jmp		jump_offset;
		}
	}

	void __declspec(naked) Gamepad::UI_MouseEventStub()
	{
		const static uint32_t UI_MouseEventDisabledJump = 0x43F93E;
		const static uint32_t jump_offset = 0x43F92B;
		const static uint32_t dword_offset = 0x85BD74;
		__asm
		{
			mov     eax, dword_offset;
			mov     al, byte ptr[eax];
			test    al, 0x10;
			push	esi;
			jz		UI_MouseEventDisabled;

			//If gamepad is touched then remove the mouse events and hide the real cursor from the game.
			//This tweak fixes the problems with UI system.
			call	Gamepad::IsGamePadInUse;
			test    al, al;
			jne     UI_MouseEventDisabledWithCursor;

			//if mouse detecter
			jmp		jump_offset;

		UI_MouseEventDisabled:
			jmp		UI_MouseEventDisabledJump;

		UI_MouseEventDisabledWithCursor:
			call	Gamepad::HideSystemCursor;
			jmp		UI_MouseEventDisabledJump;
		}
	}

	bool Gamepad::GPad_Check(const int localClientNum, const int portIndex)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		auto& gamePad = gamePads[localClientNum];

		if (XInputGetCapabilities(portIndex, XINPUT_FLAG_GAMEPAD, &gamePad.caps) == ERROR_SUCCESS)
		{
			gamePad.enabled = true;
			gamePad.portIndex = portIndex;
			return true;
		}

		gamePad.enabled = false;
		return false;
	}

	void Gamepad::GPad_RefreshAll()
	{
		auto currentGamePadNum = 0;
		for (auto currentPort = 0; currentPort < XUSER_MAX_COUNT && currentGamePadNum < Game::MAX_GPAD_COUNT; currentPort++)
		{
			if (GPad_Check(currentGamePadNum, currentPort))
			{
				++currentGamePadNum;
			}
		}
	}

	bool Gamepad::CL_CheckForIgnoreDueToRepeat(const int localClientNum, const int key, const int repeatCount, const unsigned time)
	{
		AssertIn(localClientNum, 1);

		auto& gamePadGlobal = gamePadGlobals[localClientNum];

		if (Game::Key_IsCatcherActive(localClientNum, Game::KEYCATCH_UI))
		{
			const int scrollDelayFirst = Dvars::Functions::Dvar_FindVar("gpad_menu_scroll_delay_first")->current.integer;
			const int scrollDelayRest = Dvars::Functions::Dvar_FindVar("gpad_menu_scroll_delay_rest")->current.integer;

			for (const auto menuScrollButton : menuScrollButtonList)
			{
				if (key == menuScrollButton)
				{
					if (repeatCount == 1)
					{
						gamePadGlobal.nextScrollTime = time + scrollDelayFirst;
						return false;
					}

					if (time > gamePadGlobal.nextScrollTime)
					{
						gamePadGlobal.nextScrollTime = time + scrollDelayRest;
						return false;
					}
					break;
				}
			}
		}

		return repeatCount > 1;
	}

	void Gamepad::CL_GamepadButtonEvent(const int localClientNum, const int key, const Game::GamePadButtonEvent buttonEvent, const unsigned time)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		const auto pressed = buttonEvent == Game::GPAD_BUTTON_PRESSED;
		const auto pressedOrUpdated = pressed || buttonEvent == Game::GPAD_BUTTON_UPDATE;

		auto& keyState = Game::playerKeys[localClientNum];
		keyState.keys[key].down = pressedOrUpdated;

		if (pressedOrUpdated)
		{
			if (++keyState.keys[key].repeats == 1)
			{
				keyState.anyKeyDown++;
			}
		}
		else if (buttonEvent == Game::GPAD_BUTTON_RELEASED && keyState.keys[key].repeats > 0)
		{
			keyState.keys[key].repeats = 0;
			if (--keyState.anyKeyDown < 0)
			{
				keyState.anyKeyDown = 0;
			}
		}

		if (pressedOrUpdated && CL_CheckForIgnoreDueToRepeat(localClientNum, key, keyState.keys[key].repeats, time))
			return;

		if (Game::Key_IsCatcherActive(localClientNum, Game::KEYCATCH_LOCATION_SELECTION) && pressedOrUpdated)
		{
			if (key == Game::K_BUTTON_B || keyState.keys[key].binding && std::strcmp(keyState.keys[key].binding, "+actionslot 4") == 0)
			{
				keyState.locSelInputState = Game::LOC_SEL_INPUT_CANCEL;
			}
			else if (key == Game::K_BUTTON_A || keyState.keys[key].binding && std::strcmp(keyState.keys[key].binding, "+attack") == 0)
			{
				keyState.locSelInputState = Game::LOC_SEL_INPUT_CONFIRM;
			}
			return;
		}

		keyState.locSelInputState = Game::LOC_SEL_INPUT_NONE;

		const auto* keyBinding = keyState.keys[key].binding;

		char cmd[1024];
		if (pressedOrUpdated)
		{
			if (Game::Key_IsCatcherActive(localClientNum, Game::KEYCATCH_UI))
			{
				UI_GamepadKeyEvent(localClientNum, key, pressedOrUpdated);
				return;
			}

			if (keyBinding)
			{
				if (keyBinding[0] == '+')
				{
					sprintf_s(cmd, "%s %i %i\n", keyBinding, key, time);
					Game::Cbuf_AddText(localClientNum, cmd);
				}
				else
				{
					Game::Cbuf_InsertText(localClientNum, keyBinding);
				}
			}
		}
		else
		{
			if (keyBinding && keyBinding[0] == '+')
			{
				sprintf_s(cmd, "-%s %i %i\n", &keyBinding[1], key, time);
				Game::Cbuf_AddText(localClientNum, cmd);
			}

			if (Game::Key_IsCatcherActive(localClientNum, Game::KEYCATCH_UI))
			{
				UI_GamepadKeyEvent(localClientNum, key, pressedOrUpdated);
			}
		}
	}

	void Gamepad::CL_GamepadButtonEventForPort(const int localClientNum, const int key, const Game::GamePadButtonEvent buttonEvent, const unsigned time)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		auto& gamePad = gamePads[localClientNum];
		gamePad.inUse = true;
		Dvars::Functions::Dvar_FindVar("gpad_in_use")->current.enabled = true;
		Dvars::Functions::Dvar_FindVar("gpad_in_use")->latched.enabled = true;

		if (Game::Key_IsCatcherActive(localClientNum, Game::KEYCATCH_UI))
		{
			CL_GamepadResetMenuScrollTime(localClientNum, key, buttonEvent == Game::GPAD_BUTTON_PRESSED, time);
		}


		CL_GamepadButtonEvent(localClientNum, key, buttonEvent, time);
	}

	void Gamepad::GPad_ConvertStickToFloat(const short x, const short y, float& outX, float& outY)
	{
		if (x == 0 && y == 0)
		{
			outX = 0.0f;
			outY = 0.0f;
			return;
		}

		Game::vec2_t stickVec;
		stickVec[0] = static_cast<float>(x) / static_cast<float>(std::numeric_limits<short>::max());
		stickVec[1] = static_cast<float>(y) / static_cast<float>(std::numeric_limits<short>::max());

		const auto deadZoneTotal = Dvars::Functions::Dvar_FindVar("gpad_stick_deadzone_min")->current.value + Dvars::Functions::Dvar_FindVar("gpad_stick_deadzone_max")->current.value;
		auto len = Game::Vec2Normalize(stickVec);

		if (Dvars::Functions::Dvar_FindVar("gpad_stick_deadzone_min")->current.value <= len)
		{
			if (1.0f - Dvars::Functions::Dvar_FindVar("gpad_stick_deadzone_max")->current.value >= len)
			{
				len = (len - Dvars::Functions::Dvar_FindVar("gpad_stick_deadzone_min")->current.value) / (1.0f - deadZoneTotal);
			}
			else
			{
				len = 1.0f;
			}
		}
		else
		{
			len = 0.0f;
		}

		outX = stickVec[0] * len;
		outY = stickVec[1] * len;
	}

	bool Gamepad::Gamepad_ShouldUse(const Game::gentity_s* playerEnt, const unsigned useTime)
	{
		Game::Com_Printf(0, "use hold time: %d\n", useTime);
		// Only apply hold time to +usereload keybind

		//Dvars::Functions::Dvar_FindVar("gpad_use_hold_time")->current.integer

		return !(playerEnt->client->buttons & Game::CMD_BUTTON_USE_RELOAD) || (useTime >= 250);
	}

	__declspec(naked) void Gamepad::Player_UseEntity_Stub()
	{
		__asm
		{
			// Execute overwritten instructions
			cmp     esi, [ecx + 0xC]
			jl		skipUse

			// Call our custom check
			push	esi
			pushad
			push	esi //useTime
			push	edx //playerEnt
			call	Gamepad_ShouldUse
			add		esp, 8h
			mov		[esp + 0x20], esi
			popad
			pop		esi

			// Skip use if custom check returns false
			test	al, al
			jz		skipUse

			// perform use
			push	 0x4F92DC
			ret

		skipUse:
			pop		eax;
			ret
		}
	}

	bool Gamepad::Key_IsValidGamePadChar(const int key)
	{
		return key >= Game::K_FIRSTGAMEPADBUTTON_RANGE_1 && key <= Game::K_LASTGAMEPADBUTTON_RANGE_1
			|| key >= Game::K_FIRSTGAMEPADBUTTON_RANGE_2 && key <= Game::K_LASTGAMEPADBUTTON_RANGE_2
			|| key >= Game::K_FIRSTGAMEPADBUTTON_RANGE_3 && key <= Game::K_LASTGAMEPADBUTTON_RANGE_3;
	}

	void Gamepad::CL_GamepadResetMenuScrollTime(const int localClientNum, const int key, const bool down, const unsigned time)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		auto& gamePadGlobal = gamePadGlobals[localClientNum];

		if (!down)
		{
			return;
		}

		const auto scrollDelayFirst = Dvars::Functions::Dvar_FindVar("gpad_menu_scroll_delay_first")->current.integer;
		for (const auto scrollButton : menuScrollButtonList)
		{
			if (key == scrollButton)
			{
				gamePadGlobal.nextScrollTime = scrollDelayFirst + time;
				return;
			}
		}
	}

	void Gamepad::CL_GamepadGenerateAPad(const int localClientNum, const Game::GamepadPhysicalAxis physicalAxis, unsigned time)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);
		assert(physicalAxis >= 0 && physicalAxis < Game::GPAD_PHYSAXIS_COUNT);

		auto& gamePad = gamePads[localClientNum];

		const auto stick = stickForAxis[physicalAxis];
		const auto stickIndex = stick & Game::GPAD_VALUE_MASK;
		if (stick != Game::GPAD_INVALID)
		{
			assert(stickIndex < 4);
			const auto& mapping = analogStickList[stickIndex];

			if (gamePad.stickDown[stickIndex][Game::GPAD_STICK_POS])
			{
				const Game::GamePadButtonEvent event = gamePad.stickDownLast[stickIndex][Game::GPAD_STICK_POS] ? Game::GPAD_BUTTON_UPDATE : Game::GPAD_BUTTON_PRESSED;
				CL_GamepadButtonEvent(localClientNum, mapping.posCode, event, time);
			}
			else if (gamePad.stickDown[stickIndex][Game::GPAD_STICK_NEG])
			{
				const Game::GamePadButtonEvent event = gamePad.stickDownLast[stickIndex][Game::GPAD_STICK_NEG] ? Game::GPAD_BUTTON_UPDATE : Game::GPAD_BUTTON_PRESSED;
				CL_GamepadButtonEvent(localClientNum, mapping.negCode, event, time);
			}
			else if (gamePad.stickDownLast[stickIndex][Game::GPAD_STICK_POS])
			{
				CL_GamepadButtonEvent(localClientNum, mapping.posCode, Game::GPAD_BUTTON_RELEASED, time);
			}
			else if (gamePad.stickDownLast[stickIndex][Game::GPAD_STICK_NEG])
			{
				CL_GamepadButtonEvent(localClientNum, mapping.negCode, Game::GPAD_BUTTON_RELEASED, time);
			}
		}
	}

	void Gamepad::CL_GamepadEvent(const int localClientNum, const Game::GamepadPhysicalAxis physicalAxis, const float value, const unsigned time)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);
		assert(physicalAxis >= 0 && physicalAxis < Game::GPAD_PHYSAXIS_COUNT);

		auto& gamePad = gamePads[localClientNum];
		auto& gamePadGlobal = gamePadGlobals[localClientNum];

		gamePadGlobal.axes.axesValues[physicalAxis] = value;
		CL_GamepadGenerateAPad(localClientNum, physicalAxis, time);

		if (std::fabs(value) > 0.0f)
		{
			gamePad.inUse = true;

			Dvars::Functions::Dvar_FindVar("gpad_in_use")->current.enabled = true;
			Dvars::Functions::Dvar_FindVar("gpad_in_use")->latched.enabled = true;
		}
	}

	void Gamepad::UI_GamepadKeyEvent(const int localClientNum, const int key, const bool down)
	{
		// If we are currently capturing a key for menu bind inputs then do not map keys and pass to game
		if (*Game::g_waitingForKey)
		{
			Game::UI_KeyEvent(down, localClientNum, key);
			return;
		}

		for (const auto& mapping : controllerMenuKeyMappings)
		{
			if (mapping.controllerKey == key)
			{
				Game::UI_KeyEvent(down, localClientNum, mapping.pcKey);
				return;
			}
		}

		// No point in sending unmapped controller keystrokes to the key event handler since it doesn't know how to use it anyway
		// Game::UI_KeyEvent(localClientNum, key, down);
	}

	float Gamepad::GPad_GetStick(const int localClientNum, const Game::GamePadStick stick)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);
		assert(stick & Game::GPAD_STICK_MASK);

		auto& gamePad = gamePads[localClientNum];
		return gamePad.sticks[stick];
	}

	float Gamepad::GPad_GetButton(const int localClientNum, Game::GamePadButton button)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);
		auto& gamePad = gamePads[localClientNum];

		float value = 0.0f;

		if (button & Game::GPAD_DIGITAL_MASK)
		{
			const auto buttonValue = button & Game::GPAD_VALUE_MASK;
			value = buttonValue & gamePad.digitals ? 1.0f : 0.0f;
		}
		else if (button & Game::GPAD_ANALOG_MASK)
		{
			const auto analogIndex = button & Game::GPAD_VALUE_MASK;
			if (analogIndex < std::extent_v<decltype(gamePad.analogs)>)
			{
				value = gamePad.analogs[analogIndex];
			}
		}

		return value;
	}

	bool Gamepad::GPad_IsButtonPressed(const int localClientNum, Game::GamePadButton button)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);
		assert(button & (Game::GPAD_DIGITAL_MASK | Game::GPAD_ANALOG_MASK));

		auto& gamePad = gamePads[localClientNum];

		bool down = false;
		bool lastDown = false;

		if (button & Game::GPAD_DIGITAL_MASK)
		{
			const auto buttonValue = button & Game::GPAD_VALUE_MASK;
			down = (buttonValue & gamePad.digitals) != 0;
			lastDown = (buttonValue & gamePad.lastDigitals) != 0;
		}
		else if (button & Game::GPAD_ANALOG_MASK)
		{
			const auto analogIndex = button & Game::GPAD_VALUE_MASK;
			assert(analogIndex < std::extent_v<decltype(gamePad.analogs)>);

			if (analogIndex < std::extent_v<decltype(gamePad.analogs)>)
			{
				down = gamePad.analogs[analogIndex] > 0.0f;
				lastDown = gamePad.lastAnalogs[analogIndex] > 0.0f;
			}
		}

		return down && !lastDown;
	}

	bool Gamepad::GPad_ButtonRequiresUpdates(const int localClientNum, Game::GamePadButton button)
	{
		return (button & Game::GPAD_ANALOG_MASK || button & Game::GPAD_DPAD_MASK) && GPad_GetButton(localClientNum, button) > 0.0f;
	}

	bool Gamepad::GPad_IsButtonReleased(int localClientNum, Game::GamePadButton button)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		auto& gamePad = gamePads[localClientNum];

		bool down = false;
		bool lastDown = false;

		if (button & Game::GPAD_DIGITAL_MASK)
		{
			const auto buttonValue = button & Game::GPAD_VALUE_MASK;

			down = (gamePad.digitals & buttonValue) != 0;
			lastDown = (gamePad.lastDigitals & buttonValue) != 0;
		}
		else if (button & Game::GPAD_ANALOG_MASK)
		{
			const auto analogIndex = button & Game::GPAD_VALUE_MASK;
			assert(analogIndex < std::extent_v<decltype(gamePad.analogs)>);

			if (analogIndex < std::extent_v<decltype(gamePad.analogs)>)
			{
				down = gamePad.analogs[analogIndex] > 0.0f;
				lastDown = gamePad.lastAnalogs[analogIndex] > 0.0f;
			}
		}

		return !down && lastDown;
	}

	void Gamepad::GPad_UpdateSticksDown(const int localClientNum)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		for (auto stickIndex = 0u; stickIndex < std::extent_v<decltype(GamePad::sticks)>; stickIndex++)
		{
			for (auto dir = 0; dir < Game::GPAD_STICK_DIR_COUNT; dir++)
			{
				auto& gamePad = gamePads[localClientNum];
				gamePad.stickDownLast[stickIndex][dir] = gamePad.stickDown[stickIndex][dir];

				auto threshold = Dvars::Functions::Dvar_FindVar("gpad_stick_pressed")->current.value;

				if (gamePad.stickDownLast[stickIndex][dir])
				{
					threshold -= Dvars::Functions::Dvar_FindVar("gpad_stick_pressed_hysteresis")->current.value;
				}
				else
				{
					threshold += Dvars::Functions::Dvar_FindVar("gpad_stick_pressed_hysteresis")->current.value;
				}

				if (dir == Game::GPAD_STICK_POS)
				{
					gamePad.stickDown[stickIndex][dir] = gamePad.sticks[stickIndex] > threshold;
				}
				else
				{
					assert(dir == Game::GPAD_STICK_NEG);
					gamePad.stickDown[stickIndex][dir] = gamePad.sticks[stickIndex] < -threshold;
				}
			}
		}
	}

	void Gamepad::GPad_UpdateSticks(const int localClientNum, const XINPUT_GAMEPAD& state)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		auto& gamePad = gamePads[localClientNum];

		Game::vec2_t lVec, rVec;
		GPad_ConvertStickToFloat(state.sThumbLX, state.sThumbLY, lVec[0], lVec[1]);
		GPad_ConvertStickToFloat(state.sThumbRX, state.sThumbRY, rVec[0], rVec[1]);

		gamePad.lastSticks[0] = gamePad.sticks[0];
		gamePad.sticks[0] = lVec[0];
		gamePad.lastSticks[1] = gamePad.sticks[1];
		gamePad.sticks[1] = lVec[1];
		gamePad.lastSticks[2] = gamePad.sticks[2];
		gamePad.sticks[2] = rVec[0];
		gamePad.lastSticks[3] = gamePad.sticks[3];
		gamePad.sticks[3] = rVec[1];

		GPad_UpdateSticksDown(localClientNum);

		if (Dvars::Functions::Dvar_FindVar("gpad_debug")->current.enabled)
		{
			Game::Com_Printf(0, "Left: X: {%f} Y: {%f}\n", lVec[0], lVec[1]);
			Game::Com_Printf(0, "Right: X: {%f} Y: {%f}\n", rVec[0], rVec[1]);
			Game::Com_Printf(0, "Down: {%d}:{%d} {%d}:{%d} {%d}:{%d} {%d}:{%d}\n",
				gamePad.stickDown[0][Game::GPAD_STICK_POS], gamePad.stickDown[0][Game::GPAD_STICK_NEG],
				gamePad.stickDown[1][Game::GPAD_STICK_POS], gamePad.stickDown[1][Game::GPAD_STICK_NEG],
				gamePad.stickDown[2][Game::GPAD_STICK_POS], gamePad.stickDown[2][Game::GPAD_STICK_NEG],
				gamePad.stickDown[3][Game::GPAD_STICK_POS], gamePad.stickDown[3][Game::GPAD_STICK_NEG]);
		}
	}

	void Gamepad::GPad_UpdateDigitals(const int localClientNum, const XINPUT_GAMEPAD& state)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		auto& gamePad = gamePads[localClientNum];

		gamePad.lastDigitals = gamePad.digitals;
		gamePad.digitals = state.wButtons;

		const auto leftDeflect = Dvars::Functions::Dvar_FindVar("gpad_button_lstick_deflect_max")->current.value;
		if (std::fabs(gamePad.sticks[0]) > leftDeflect || std::fabs(gamePad.sticks[1]) > leftDeflect)
		{
			gamePad.digitals &= ~static_cast<short>(XINPUT_GAMEPAD_LEFT_THUMB);
		}

		const auto rightDeflect = Dvars::Functions::Dvar_FindVar("gpad_button_rstick_deflect_max")->current.value;
		if (std::fabs(gamePad.sticks[2]) > leftDeflect || std::fabs(gamePad.sticks[3]) > rightDeflect)
		{
			gamePad.digitals &= ~static_cast<short>(XINPUT_GAMEPAD_RIGHT_THUMB);
		}

		if (Dvars::Functions::Dvar_FindVar("gpad_debug")->current.enabled)
		{
			Game::Com_Printf(0, "Buttons: {%hu}\n", gamePad.digitals);
		}
	}

	void Gamepad::GPad_UpdateAnalogs(const int localClientNum, const XINPUT_GAMEPAD& state)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		auto& gamePad = gamePads[localClientNum];

		const auto buttonDeadZone = Dvars::Functions::Dvar_FindVar("gpad_button_deadzone")->current.value;

		gamePad.lastAnalogs[0] = gamePad.analogs[0];
		gamePad.analogs[0] = static_cast<float>(state.bLeftTrigger) / static_cast<float>(std::numeric_limits<unsigned char>::max());
		if (gamePad.analogs[0] < buttonDeadZone)
		{
			gamePad.analogs[0] = 0.0f;
		}

		gamePad.lastAnalogs[1] = gamePad.analogs[1];
		gamePad.analogs[1] = static_cast<float>(state.bRightTrigger) / static_cast<float>(std::numeric_limits<unsigned char>::max());
		if (gamePad.analogs[1] < buttonDeadZone)
		{
			gamePad.analogs[1] = 0.0f;
		}

		if (Dvars::Functions::Dvar_FindVar("gpad_debug")->current.enabled)
		{
			Game::Com_Printf(0, "Triggers: {%f} {%f}\n", gamePad.analogs[0], gamePad.analogs[1]);
		}
	}

	void Gamepad::GPad_UpdateAll()
	{
		GPad_RefreshAll();

		for (auto localClientNum = 0; localClientNum < Game::MAX_GPAD_COUNT; ++localClientNum)
		{
			const auto& gamePad = gamePads[localClientNum];
			if (!gamePad.enabled)
			{
				continue;
			}

			XINPUT_STATE inputState;
			if (XInputGetState(gamePad.portIndex, &inputState) != ERROR_SUCCESS)
			{
				continue;
			}

			GPad_UpdateSticks(localClientNum, inputState.Gamepad);
			GPad_UpdateDigitals(localClientNum, inputState.Gamepad);
			GPad_UpdateAnalogs(localClientNum, inputState.Gamepad);
		}
	}

	void Gamepad::CL_GamepadUnlockAll(const char* cmd, const char* sound)
	{
		Command::Execute(cmd, false);
		Command::Execute(Utils::String::VA("snd_playlocal %s", sound));
	}

	void Gamepad::CL_GamepadCheatReset()
	{
		gamePads->IWCode.codeIndex = 0;
	}

	void Gamepad::CL_GamepadCheat()
	{
		for (auto localClientNum = 0; localClientNum < Game::MAX_GPAD_COUNT; ++localClientNum)
		{
			const auto& gamePad = gamePads[localClientNum];


			for (auto m = 0; m < Game::uiInfo->uiDC.openMenuCount; m++)
			{
				if (Game::uiInfo->uiDC.menuStack[m] && !_stricmp(Game::uiInfo->uiDC.menuStack[m]->window.name, gamePad.IWCode.menu))
				{
					if (GPad_ButtonRequiresUpdates(localClientNum, gamePad.IWCode.holdButtons[0]) && GPad_ButtonRequiresUpdates(localClientNum, gamePad.IWCode.holdButtons[1])) //hold triggers
					{
						for (const auto& buttonMapping : buttonList)
						{
							if (GPad_IsButtonPressed(localClientNum, buttonMapping.padButton))
							{
								bool isCodeButton = std::find(std::begin(gamePad.IWCode.codeButtons), std::end(gamePad.IWCode.codeButtons), buttonMapping.padButton) != std::end(gamePad.IWCode.codeButtons);

								if (isCodeButton)
								{
									if (gamePads->IWCode.codeButtons[gamePads->IWCode.codeIndex] == buttonMapping.padButton)
									{
										++gamePads->IWCode.codeIndex;
										if (gamePads->IWCode.codeIndex == 6)
										{
											gamePads->IWCode.unlockFunc();
											goto RESET;
										}
									}
									else goto RESET;
								}
								else goto RESET;
							}
						}
					}
					else
					{
					RESET:
						CL_GamepadCheatReset();
					}
				}
			}
		}
	}

	void Gamepad::CL_GamepadCheatInit()
	{
		gamePads->IWCode.codeIndex = 0;
		gamePads->IWCode.menu = "levels";
		gamePads->IWCode.sound = "mission_unlock_cheat_og";
		gamePads->IWCode.unlockCmd = "mis_cheat 1\n";

		gamePads->IWCode.holdButtons[0] = Game::GPAD_L_TRIG;
		gamePads->IWCode.holdButtons[1] = Game::GPAD_R_TRIG;
		gamePads->IWCode.codeButtons[0] = Game::GPAD_UP;
		gamePads->IWCode.codeButtons[1] = Game::GPAD_UP;
		gamePads->IWCode.codeButtons[2] = Game::GPAD_DOWN;
		gamePads->IWCode.codeButtons[3] = Game::GPAD_DOWN;
		gamePads->IWCode.codeButtons[4] = Game::GPAD_X;
		gamePads->IWCode.codeButtons[5] = Game::GPAD_Y;

		gamePads->IWCode.unlockFunc = [] {
			CL_GamepadUnlockAll(gamePads->IWCode.unlockCmd, gamePads->IWCode.sound);
		};
	}

	void Gamepad::IN_GamePadsMove()
	{
		if (!Dvars::Functions::Dvar_FindVar("gpad_enabled")->current.enabled)
			return;

		GPad_UpdateAll();
		const auto time = Game::Sys_MilliSeconds();

		bool gpadPresent = false;
		for (auto localClientNum = 0; localClientNum < Game::MAX_GPAD_COUNT; ++localClientNum)
		{
			auto& gamePad = gamePads[localClientNum];
			if (!gamePad.enabled)
			{
				continue;
			}

			gpadPresent = true;
			const auto lx = GPad_GetStick(localClientNum, Game::GPAD_LX);
			const auto ly = GPad_GetStick(localClientNum, Game::GPAD_LY);
			const auto rx = GPad_GetStick(localClientNum, Game::GPAD_RX);
			const auto ry = GPad_GetStick(localClientNum, Game::GPAD_RY);
			const auto leftTrig = GPad_GetButton(localClientNum, Game::GPAD_L_TRIG);
			const auto rightTrig = GPad_GetButton(localClientNum, Game::GPAD_R_TRIG);

			// only without mods
			if (strlen(Dvars::Functions::Dvar_FindVar("fs_game")->current.string) == 0)
			{
				CL_GamepadCheat();
			}

			//if (XInputGetBatteryInformation(gamePad.portIndex, BATTERY_DEVTYPE_GAMEPAD, &gamePad.battery_info) == ERROR_SUCCESS)
			//{
			//	Game::Com_Printf(0, "Battery Level: %d\n", gamePad.battery_info.BatteryLevel);
			//}

			CL_GamepadEvent(localClientNum, Game::GPAD_PHYSAXIS_LSTICK_X, lx, time);
			CL_GamepadEvent(localClientNum, Game::GPAD_PHYSAXIS_LSTICK_Y, ly, time);
			CL_GamepadEvent(localClientNum, Game::GPAD_PHYSAXIS_RSTICK_X, rx, time);
			CL_GamepadEvent(localClientNum, Game::GPAD_PHYSAXIS_RSTICK_Y, ry, time);
			CL_GamepadEvent(localClientNum, Game::GPAD_PHYSAXIS_LTRIGGER, leftTrig, time);
			CL_GamepadEvent(localClientNum, Game::GPAD_PHYSAXIS_RTRIGGER, rightTrig, time);

			for (const auto& buttonMapping : buttonList)
			{
				if (GPad_IsButtonPressed(localClientNum, buttonMapping.padButton))
				{
					CL_GamepadButtonEventForPort(localClientNum, buttonMapping.code, Game::GPAD_BUTTON_PRESSED, time);
				}
				else if (GPad_ButtonRequiresUpdates(localClientNum, buttonMapping.padButton))
				{
					CL_GamepadButtonEventForPort(localClientNum, buttonMapping.code, Game::GPAD_BUTTON_UPDATE, time);
				}
				else if (GPad_IsButtonReleased(localClientNum, buttonMapping.padButton))
				{
					CL_GamepadButtonEventForPort(localClientNum, buttonMapping.code, Game::GPAD_BUTTON_RELEASED, time);
				}
			}
		}

		Dvars::Functions::Dvar_FindVar("gpad_present")->current.enabled = gpadPresent;
		Dvars::Functions::Dvar_FindVar("gpad_present")->latched.enabled = gpadPresent;
	}

	void Gamepad::IN_Frame_Hk()
	{
		Utils::Hook::Call<void()>(0x594730)();
		IN_GamePadsMove();
	}

	void Gamepad::Gamepad_WriteBindings(const int localClientNum, const int handle)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);

		auto& gamePadGlobal = gamePadGlobals[localClientNum];

		Game::FS_Printf(handle, "unbindallaxis\n");

		for (auto virtualAxisIndex = 0u; virtualAxisIndex < Game::GPAD_VIRTAXIS_COUNT; ++virtualAxisIndex)
		{
			const auto& axisMapping = gamePadGlobal.axes.virtualAxes[virtualAxisIndex];
			if (axisMapping.physicalAxis <= Game::GPAD_PHYSAXIS_NONE || axisMapping.physicalAxis >= Game::GPAD_PHYSAXIS_COUNT ||
				axisMapping.mapType <= Game::GPAD_MAP_NONE || axisMapping.mapType >= Game::GPAD_MAP_COUNT)
			{
				continue;
			}

			const auto* physicalAxisName = physicalAxisNames[axisMapping.physicalAxis];
			const auto* virtualAxisName = virtualAxisNames[virtualAxisIndex];
			const auto* mappingName = gamePadMappingTypeNames[axisMapping.mapType];

			Game::FS_Printf(handle, "bindaxis %s %s %s\n", physicalAxisName, virtualAxisName, mappingName);
		}
	}

	void Gamepad::Key_WriteBindings_Hk(const int localClientNum, char* buffer)
	{
		Game::Key_WriteBindings(localClientNum, buffer);
	}

	void Gamepad::Gamepad_WriteBindings_Hk(int handle)
	{
		Gamepad_WriteBindings(0, handle);
	}

	void __declspec(naked) Gamepad::Key_WriteBindings_Stub()
	{
		const static uint32_t jump_offset = 0x535384;
		__asm
		{
			push	eax;
			call	Gamepad::Key_WriteBindings_Hk;
			add		esp, 4;

			pushad;
			push	ebx;
			call	Gamepad::Gamepad_WriteBindings_Hk;
			add		esp, 4;
			popad;

			jmp		jump_offset;
		}
	}

	void __declspec(naked) Gamepad::Com_WriteConfiguration_Modified_Stub()
	{
		const static uint32_t dvar_modifiedFlags = 0x1330C94;
		__asm
		{			
			//ce
			//test byte ptr[01330C94], 01


			test	byte ptr[dvar_modifiedFlags], 1;
			or		eax, gamePadBindingsModifiedFlags;	// Also check for gamePadBindingsModifiedFlags
			test	al, 1;
			jz		endMethod;
			mov		gamePadBindingsModifiedFlags, 0;	// Reset gamePadBindingsModifiedFlags
			test	byte ptr[dvar_modifiedFlags], 1;	// Restore eax to dvar_modified_flags

			push 0x535449
			retn

		endMethod:
			push 0x535468
			retn
		}
	}

	void __declspec(naked) Gamepad::Key_SetBinding_stub01()
	{
		const static uint32_t jump_offset = 0x56F72D;
		__asm
		{
			push	ecx;
			push	eax;
			call	Gamepad::Key_SetBinding_Hk;
			add		esp, 0x8;
			jmp		jump_offset;
		}
	}

	void __declspec(naked) Gamepad::Key_SetBinding_stub02()
	{
		const static uint32_t jump_offset = 0x56F740;
		__asm
		{
			push	ecx;
			push	eax;
			call	Gamepad::Key_SetBinding_Hk;
			add		esp, 0x8;
			jmp		jump_offset;
		}
	}

	void __declspec(naked) Gamepad::Key_SetBinding_stub03()
	{
		const static uint32_t jump_offset = 0x56F758;
		__asm
		{
			push	edx;
			push	ecx;
			push	eax;
			call	Gamepad::Key_SetBinding_Hk;
			add		esp, 0xC;
			jmp		jump_offset;
		}
	}

	void __declspec(naked) Gamepad::CL_MouseMove_Stub()
	{
		const static uint32_t jump_offset = 0x4402FC;
		__asm
		{
			call	CL_MouseMove;
			jmp		jump_offset;
		}
	}

	void Gamepad::CreateKeyNameMap()
	{
		std::memcpy(combinedKeyNames, Game::keyNames, sizeof(Game::keyname_t) * Game::KEY_NAME_COUNT);
		std::memcpy(&combinedKeyNames[Game::KEY_NAME_COUNT], extendedKeyNames, sizeof(Game::keyname_t) * std::extent_v<decltype(extendedKeyNames)>);
		combinedKeyNames[std::extent_v<decltype(combinedKeyNames)> -1] = { nullptr, 0 };

		std::memcpy(combinedLocalizedKeyNamesXenon, Game::localizedKeyNames, sizeof(Game::keyname_t) * Game::LOCALIZED_KEY_NAME_COUNT);
		std::memcpy(&combinedLocalizedKeyNamesXenon[Game::LOCALIZED_KEY_NAME_COUNT], extendedLocalizedKeyNamesXenon, sizeof(Game::keyname_t) * std::extent_v<decltype(extendedLocalizedKeyNamesXenon)>);

		combinedLocalizedKeyNamesXenon[std::extent_v<decltype(combinedLocalizedKeyNamesXenon)> -1] = { nullptr, 0 };

		std::memcpy(combinedLocalizedKeyNamesPs3, Game::localizedKeyNames, sizeof(Game::keyname_t) * Game::LOCALIZED_KEY_NAME_COUNT);
		std::memcpy(&combinedLocalizedKeyNamesPs3[Game::LOCALIZED_KEY_NAME_COUNT], extendedLocalizedKeyNamesPs3, sizeof(Game::keyname_t) * std::extent_v<decltype(extendedLocalizedKeyNamesPs3)>);
		
		combinedLocalizedKeyNamesPs3[std::extent_v<decltype(combinedLocalizedKeyNamesPs3)> -1] = { nullptr, 0 };

		Utils::Hook::Set<Game::keyname_t*>(0x44367D, combinedKeyNames);
		Utils::Hook::Set<Game::keyname_t*>(0x443685, combinedKeyNames);
		Utils::Hook::Set<Game::keyname_t*>(0x443737, combinedKeyNames);
		Utils::Hook(0x44372F, GetLocalizedKeyName_Stub, HOOK_CALL).install()->quick();
		Utils::Hook(0x447E01, GetLocalizedKeyName_Stub02, HOOK_CALL).install()->quick();
	}

	void Gamepad::Gamepad_BindAxis(const int localClientNum, const Game::GamepadPhysicalAxis realIndex, const Game::GamepadVirtualAxis axisIndex, const Game::GamepadMapping mapType)
	{
		AssertIn(localClientNum, Game::MAX_GPAD_COUNT);
		assert(realIndex > Game::GPAD_PHYSAXIS_NONE && realIndex < Game::GPAD_PHYSAXIS_COUNT);
		assert(axisIndex > Game::GPAD_VIRTAXIS_NONE && axisIndex < Game::GPAD_VIRTAXIS_COUNT);
		assert(mapType > Game::GPAD_MAP_NONE && mapType < Game::GPAD_MAP_COUNT);

		auto& gamePadGlobal = gamePadGlobals[localClientNum];
		gamePadGlobal.axes.virtualAxes[axisIndex].physicalAxis = realIndex;
		gamePadGlobal.axes.virtualAxes[axisIndex].mapType = mapType;

		gamePadBindingsModifiedFlags |= 1;
	}

	Game::GamepadPhysicalAxis Gamepad::StringToPhysicalAxis(const char* str)
	{
		for (std::size_t i = 0; i < std::extent_v<decltype(physicalAxisNames)>; i++)
		{
			if (std::strcmp(str, physicalAxisNames[i]) == 0)
			{
				return static_cast<Game::GamepadPhysicalAxis>(i);
			}
		}

		return Game::GPAD_PHYSAXIS_NONE;
	}

	Game::GamepadVirtualAxis Gamepad::StringToVirtualAxis(const char* str)
	{
		for (std::size_t i = 0; i < std::extent_v<decltype(virtualAxisNames)>; ++i)
		{
			if (std::strcmp(str, virtualAxisNames[i]) == 0)
			{
				return static_cast<Game::GamepadVirtualAxis>(i);
			}
		}

		return Game::GPAD_VIRTAXIS_NONE;
	}

	Game::GamepadMapping Gamepad::StringToGamePadMapping(const char* str)
	{
		for (std::size_t i = 0; i < std::extent_v<decltype(gamePadMappingTypeNames)>; ++i)
		{
			if (std::strcmp(str, gamePadMappingTypeNames[i]) == 0)
			{
				return static_cast<Game::GamepadMapping>(i);
			}
		}

		return Game::GPAD_MAP_NONE;
	}

	void Gamepad::Axis_Bind_f(Command::Params* params)
	{
		if (params->size() < 4)
		{
			Game::Com_Printf(0, "bindaxis <real axis> <virtual axis> <input type>\n");
			return;
		}

		const auto* physicalAxisText = params->get(1);
		const auto* virtualAxisText = params->get(2);
		const auto* mappingText = params->get(3);

		const Game::GamepadPhysicalAxis physicalAxis = StringToPhysicalAxis(physicalAxisText);
		if (physicalAxis == Game::GPAD_PHYSAXIS_NONE)
		{
			Game::Com_Printf(0, "\"{}\" isn't a valid physical axis\n", physicalAxisText);
			return;
		}

		const Game::GamepadVirtualAxis virtualAxis = StringToVirtualAxis(virtualAxisText);
		if (virtualAxis == Game::GPAD_VIRTAXIS_NONE)
		{
			Game::Com_Printf(0, "\"{}\" isn't a valid virtual axis\n", virtualAxisText);
			return;
		}

		const Game::GamepadMapping mapping = StringToGamePadMapping(mappingText);
		if (mapping == Game::GPAD_MAP_NONE)
		{
			Game::Com_Printf(0, "\"{}\" isn't a valid input type\n", mappingText);
			return;
		}

		Gamepad_BindAxis(0, physicalAxis, virtualAxis, mapping);
	}

	void Gamepad::Axis_Unbindall_f()
	{
		auto& gamePadGlobal = gamePadGlobals[0];

		for (auto& virtualAxis : gamePadGlobal.axes.virtualAxes)
		{
			virtualAxis.physicalAxis = Game::GPAD_PHYSAXIS_NONE;
			virtualAxis.mapType = Game::GPAD_MAP_NONE;
		}
	}

	void Gamepad::Bind_GP_SticksConfigs_f()
	{
		const auto* stickConfigName = Dvars::Functions::Dvar_FindVar("gpad_sticksConfig")->current.string;
		Game::Cbuf_AddText(0, Utils::String::VA("exec %s\n", stickConfigName));
	}

	void Gamepad::Bind_GP_ButtonsConfigs_f()
	{
		const auto* buttonConfigName = Dvars::Functions::Dvar_FindVar("gpad_buttonConfig")->current.string;
		Game::Cbuf_AddText(0, Utils::String::VA("exec %s\n", buttonConfigName));
	}

	void Gamepad::SetAimAssist_f()
	{
		if (Dvars::Functions::Dvar_FindVar("gpad_aimassist")->current.enabled)
		{
			Command::Execute("set gpad_autoaim_enabled 1", false);
			Command::Execute("set gpad_lockon_enabled 1", false);
			Command::Execute("set gpad_slowdown_enabled 1", false);
		}
		else
		{
			Command::Execute("set gpad_autoaim_enabled 0", false);
			Command::Execute("set gpad_lockon_enabled 0", false);
			Command::Execute("set gpad_slowdown_enabled 0", false);
		}
	}

	void Gamepad::PauseMenu_Toggle_f()
	{
		Command::Execute("pause", false);
	}

	Gamepad::Gamepad()
	{
		CL_GamepadCheatInit();

		Events::OnDvarInit([]
		{
			Game::dvar_s* gpad_enabled = Dvars::Register::Dvar_RegisterBool("gpad_enabled", "Game pad enabled", false, Game::saved);
			Game::dvar_s* gpad_debug = Dvars::Register::Dvar_RegisterBool("gpad_debug", "Game pad debugging", false, Game::none);
			Game::dvar_s* gpad_present = Dvars::Register::Dvar_RegisterBool("gpad_present", "Game pad present", false, Game::none);
			Game::dvar_s* gpad_in_use = Dvars::Register::Dvar_RegisterBool("gpad_in_use", "A game pad is in use", false, Game::none);
			Game::dvar_s* gpad_style = Dvars::Register::Dvar_RegisterBool("gpad_style", "Switch between Xbox and PS HUD", false, Game::saved);
			Game::dvar_s* gpad_sticksConfig = Dvars::Register::Dvar_RegisterString("gpad_sticksConfig", "Game pad stick configuration", "", Game::saved);
			Game::dvar_s* gpad_buttonConfig = Dvars::Register::Dvar_RegisterString("gpad_buttonConfig", "Game pad button configuration", "", Game::saved);
			Game::dvar_s* gpad_menu_scroll_delay_first = Dvars::Register::Dvar_RegisterInt("gpad_menu_scroll_delay_first", "Menu scroll key-repeat delay, for the first repeat, in milliseconds", 420, 0, 1000, Game::none);
			Game::dvar_s* gpad_menu_scroll_delay_rest = Dvars::Register::Dvar_RegisterInt("gpad_menu_scroll_delay_rest", "Menu scroll key-repeat delay, for repeats after the first, in milliseconds", 210, 0, 1000, Game::none);
			Game::dvar_s* gpad_stick_pressed_hysteresis = Dvars::Register::Dvar_RegisterFloat("gpad_stick_pressed_hysteresis", "Game pad stick pressed no-change-zone around gpad_stick_pressed to prevent bouncing", 0.1f, 0.0f, 1.0f, Game::none);
			Game::dvar_s* gpad_stick_pressed = Dvars::Register::Dvar_RegisterFloat("gpad_stick_pressed", "Game pad stick pressed threshhold", 0.4f, 0.0f, 1.0f, Game::none);
			Game::dvar_s* gpad_stick_deadzone_min = Dvars::Register::Dvar_RegisterFloat("gpad_stick_deadzone_min", "Game pad minimum stick deadzone", 0.2f, 0.0f, 1.0f, Game::none);
			Game::dvar_s* gpad_stick_deadzone_max = Dvars::Register::Dvar_RegisterFloat("gpad_stick_deadzone_max", "Game pad maximum stick deadzone", 0.01f, 0.0f, 1.0f, Game::none);
			Game::dvar_s* gpad_button_deadzone = Dvars::Register::Dvar_RegisterFloat("gpad_button_deadzone", "Game pad button deadzone threshhold", 0.13f, 0.0f, 1.0f, Game::none);
			Game::dvar_s* gpad_button_lstick_deflect_max = Dvars::Register::Dvar_RegisterFloat("gpad_button_lstick_deflect_max", "Game pad maximum pad stick pressed value", 1.0f, 0.0f, 1.0f, Game::none);
			Game::dvar_s* gpad_button_rstick_deflect_max = Dvars::Register::Dvar_RegisterFloat("gpad_button_rstick_deflect_max", "Game pad maximum pad stick pressed value", 1.0f, 0.0f, 1.0f, Game::none);
			Game::dvar_s* gpad_use_hold_time = Dvars::Register::Dvar_RegisterInt("gpad_use_hold_time", "Time to hold the 'use' button on gamepads taim_lockon_strengtho activate use", 250, 0, std::numeric_limits<int>::max(), Game::none);
			Game::dvar_s* gpad_lockon_enabled = Dvars::Register::Dvar_RegisterBool("gpad_lockon_enabled", "Game pad lockon aim assist enabled", true, Game::saved);
			Game::dvar_s* gpad_slowdown_enabled = Dvars::Register::Dvar_RegisterBool("gpad_slowdown_enabled", "Game pad slowdown aim assist enabled", true, Game::saved);
			Game::dvar_s* gpad_autoaim_enabled = Dvars::Register::Dvar_RegisterBool("gpad_autoaim_enabled", "Gamepad auto aim", true, Game::saved);
			Game::dvar_s* gpad_aimassist = Dvars::Register::Dvar_RegisterBool("gpad_aimassist", "Gamepad aim assist", true, Game::saved);

			Game::dvar_s* input_viewSensitivity = Dvars::Register::Dvar_RegisterFloat("input_viewSensitivity", "View Sensitivity", 1.0f, 0.0001f, 5.0f, Game::saved);
			Game::dvar_s* input_invertPitch = Dvars::Register::Dvar_RegisterBool("input_invertPitch", "Invert gamepad pitch", false, Game::saved);
			Game::dvar_s* aim_aimAssistRangeScale = Dvars::Register::Dvar_RegisterFloat("aim_aimAssistRangeScale", "Scales the weapon'aim_autoAimRangeScales aim assist range", 1.0f, 0.0f, 2.0f, Game::none);
			Game::dvar_s* aim_autoAimRangeScale = Dvars::Register::Dvar_RegisterFloat("aim_autoAimRangeScale", "Scales the weapon's auto aim range", 1.0f, 0.0f, 2.0f, Game::none);
			Game::dvar_s* aim_lockon_pitch_strength = Dvars::Register::Dvar_RegisterFloat("aim_lockon_pitch_strength", "The amount of aim assistance given by the target lock on (pitch)", 0.6f, 0.0f, 1.0f, Game::none);
		});

		// Add hold time to gamepad usereload on hold prompts
		// work in progress
		//Utils::Hook(0x4F92D7, Player_UseEntity_Stub, HOOK_JUMP).install()->quick();

		CreateKeyNameMap();

		// Also rewrite configuration when gamepad config is dirty
		//Utils::Hook::Nop(0x53542C, 7);
		//Utils::Hook(0x53542C, Com_WriteConfiguration_Modified_Stub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x53537F, Key_WriteBindings_Stub, HOOK_JUMP).install()->quick();

		Command::Add("bindaxis", Axis_Bind_f);
		Command::Add("unbindallaxis", Axis_Unbindall_f);
		Command::Add("bindgpsticksconfigs", Bind_GP_SticksConfigs_f);
		Command::Add("bindgpbuttonsconfigs", Bind_GP_ButtonsConfigs_f);
		Command::Add("setaimassist", SetAimAssist_f);
		Command::Add("togglemenu", PauseMenu_Toggle_f);

		// Gamepad on frame hook
		Utils::Hook(0x594913, IN_Frame_Hk, HOOK_CALL).install()->quick();

		// Mark controller as unused when keyboard key is pressed
		Utils::Hook(0x533B8F, CL_KeyEvent_Hk, HOOK_CALL).install()->quick();

		// Mark controller as unused when mouse is moved (need to fix!)
		// P.S: Broken in IW3
		//Utils::Hook::Nop(0x5947AC, 5);
		//Utils::Hook(0x5947A7, CL_MouseEvent_Stub, HOOK_JUMP).install()->quick();
		//Utils::Hook(0x5947AC, CL_MouseEvent_Stub, HOOK_JUMP).install()->quick();
		
		Utils::Hook::Nop(0x43F921, 10);
		Utils::Hook(0x43F921, UI_MouseEventStub, HOOK_JUMP).install()->quick();

		// Hide cursor when controller is active
		Utils::Hook(0x5653CA, UI_RefreshStub, HOOK_JUMP).install()->quick();

		// Only return gamepad keys when gamepad enabled and only non gamepad keys when not
		Utils::Hook(0x4437E0, Key_GetCommandAssignmentInternal_Stub, HOOK_JUMP).install()->quick();

		// Whenever a key binding for a gamepad key is replaced update the button config
		Utils::Hook(0x56F728, Key_SetBinding_stub01, HOOK_JUMP).install()->quick();
		Utils::Hook(0x56F73B, Key_SetBinding_stub02, HOOK_JUMP).install()->quick();
		Utils::Hook(0x56F753, Key_SetBinding_stub03, HOOK_JUMP).install()->quick();

		// Add gamepad inputs to user commands if it is enabled
		Utils::Hook(0x4402F7, CL_MouseMove_Stub, HOOK_JUMP).install()->quick();

		UIScript::Add("gamepadFirstExecIfEnabled", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{			
			if (Dvars::Functions::Dvar_FindVar("gpad_enabled")->current.enabled)
			{
				std::string gpadSticksConfig = Dvars::Functions::Dvar_FindVar("gpad_sticksConfig")->current.string;
				std::string gpadButtonConfig = Dvars::Functions::Dvar_FindVar("gpad_buttonConfig")->current.string;
				if (gpadSticksConfig == "")
				{
					Dvars::Functions::Dvar_FindVar("gpad_sticksConfig")->current.string = "thumbstick_default";
					Dvars::Functions::Dvar_FindVar("gpad_sticksConfig")->latched.string = "thumbstick_default";
					Command::Execute("bindgpsticksconfigs", false);
				}
				if (gpadSticksConfig == "")
				{
					Dvars::Functions::Dvar_FindVar("gpad_buttonConfig")->current.string = "buttons_default";
					Dvars::Functions::Dvar_FindVar("gpad_buttonConfig")->latched.string = "buttons_default";
					Command::Execute("bindgpbuttonsconfigs", false);
				}

			}
		});

		// Check the cursor movement
		Scheduler::Loop([]
		{
			tagPOINT Point;
			GetCursorPos(&Point);

			auto dx = Point.x - Game::s_wmv->oldPos.x;
			auto dy = Point.y - Game::s_wmv->oldPos.y;

			OnMouseMove(dx, dy);
		}, Scheduler::Pipeline::MAIN);
	}

	Gamepad::~Gamepad()
	{
	}
}