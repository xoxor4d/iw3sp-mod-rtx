#pragma once

namespace Components
{
	class Gamepad : public Component
	{
		struct GamePadCheat
		{
			Game::GamePadButton codeButtons[10];
			Game::GamePadButton holdButtons[2];
			std::function<void()> unlockFunc;
			const char* unlockCmd;
			const char* menu;
			const char* sound;
			int buttonTimer;
			int codeIndex;
		};

		struct ControllerMenuKeyMapping
		{
			Game::keyNum_t controllerKey;
			Game::keyNum_t pcKey;
		};

		struct GamePad
		{
			bool enabled;
			bool inUse;
			int portIndex;
			unsigned short digitals;
			unsigned short lastDigitals;
			float analogs[2];
			float lastAnalogs[2];
			float sticks[4];
			float lastSticks[4];
			bool stickDown[4][Game::GPAD_STICK_DIR_COUNT];
			bool stickDownLast[4][Game::GPAD_STICK_DIR_COUNT];
			float lowRumble;
			float highRumble;
			//XINPUT_BATTERY_INFORMATION battery_info;
			XINPUT_VIBRATION rumble;
			XINPUT_CAPABILITIES caps;
			GamePadCheat IWCode;
		};

		struct GamePadGlobals
		{
			Game::GpadAxesGlob axes;
			unsigned nextScrollTime;

			GamePadGlobals();
		};

	public:
		Gamepad();
		static void OnMouseMove(int dx, int dy);
		~Gamepad();
	private:
		static Game::ButtonToCodeMap_t buttonList[];
		static Game::StickToCodeMap_t analogStickList[4];
		static Game::GamePadStick stickForAxis[];
		static Game::GamepadPhysicalAxis axisSameStick[];
		static const char* physicalAxisNames[];
		static const char* virtualAxisNames[];
		static const char* gamePadMappingTypeNames[];
		static Game::keyNum_t menuScrollButtonList[];
		static Game::keyname_t extendedKeyNames[];
		static Game::keyname_t extendedLocalizedKeyNamesXenon[];
		static Game::keyname_t extendedLocalizedKeyNamesPs3[];
		static Game::keyname_t combinedKeyNames[];
		static Game::keyname_t combinedLocalizedKeyNamesXenon[];
		static Game::keyname_t combinedLocalizedKeyNamesPs3[];
		static ControllerMenuKeyMapping controllerMenuKeyMappings[];

		static GamePad gamePads[Game::MAX_GPAD_COUNT];
		static GamePadGlobals gamePadGlobals[Game::MAX_GPAD_COUNT];

		static int gamePadBindingsModifiedFlags;

		static float LinearTrack(float target, float current, float rate, float deltaTime);
		static bool AimAssist_DoBoundsIntersectCenterBox(const float* clipMins, const float* clipMaxs, float clipHalfWidth, float clipHalfHeight);
		static bool AimAssist_IsPlayerUsingOffhand(const Game::AimInput* input);
		static const Game::AimScreenTarget* AimAssist_GetBestTarget(const Game::AimAssistGlobals* aaGlob, float range, float regionWidth, float regionHeight);
		static const Game::AimScreenTarget* AimAssist_GetTargetFromEntity(const Game::AimAssistGlobals* aaGlob, int entIndex);
		static const Game::AimScreenTarget* AimAssist_GetPrevOrBestTarget(const Game::AimAssistGlobals* aaGlob, float range, float regionWidth, float regionHeight, int prevTargetEnt);
		static bool AimAssist_IsLockonActive(int localClientNum, const Game::AimInput* input);
		static bool AimAssist_IsAutoAimActive(int localClientNum, const Game::AimInput* input);
		static void AimAssist_ApplyLockOn(const Game::AimInput* input, Game::AimOutput* output);
		static void AimAssist_CalcAdjustedAxis(const Game::AimInput* input, float* pitchAxis, float* yawAxis);
		static bool AimAssist_IsSlowdownActive(const Game::AimInput* input);
		static void AimAssist_CalcSlowdown(const Game::AimInput* input, float* pitchScale, float* yawScale);
		static float AimAssist_Lerp(float from, float to, float fraction);
		static void AimAssist_ApplyTurnRates(const Game::AimInput* input, Game::AimOutput* output);
		static void AimAssist_UpdateGamePadInput(const Game::AimInput* input, Game::AimOutput* output);
		static void AimAssist_ApplyAutoAim(const Game::AimInput* input, Game::AimOutput* output);
		static void AimAssist_SetAutoAimTarget(Game::AimAssistGlobals* aaGlob, const Game::AimScreenTarget* screenTarget);
		static bool AimAssist_UpdateAutoAimTarget(Game::AimAssistGlobals* aaGlob);
		static void AimAssist_ClearAutoAimTarget(Game::AimAssistGlobals* aaGlob);

		static bool CG_ShouldUpdateViewAngles();
		static float CL_GamepadAxisValue(Game::GamepadVirtualAxis virtualAxis);
		static char ClampChar(int value);
		static void CL_GamepadMove(Game::usercmd_s* cmd);
		static void CL_MouseMove(Game::usercmd_s* cmd);
		static void CL_MouseMove_Stub();

		static bool Key_IsValidGamePadChar(int key);
		static bool CL_CheckForIgnoreDueToRepeat(int localClientNum, int key, int repeatCount, unsigned int time);
		static void CL_GamepadResetMenuScrollTime(int localClientNum, int key, bool down, unsigned int time);
		static void UI_GamepadKeyEvent(int localClientNum, int key, bool down);
		static void CL_GamepadEvent(int localClientNum, Game::GamepadPhysicalAxis physicalAxis, float value, unsigned time);
		static void CL_GamepadGenerateAPad(int localClientNum, Game::GamepadPhysicalAxis physicalAxis, unsigned time);
		static void CL_GamepadButtonEvent(int localClientNum, int key, Game::GamePadButtonEvent buttonEvent, unsigned time);
		static void CL_GamepadButtonEventForPort(int localClientNum, int key, Game::GamePadButtonEvent buttonEvent, unsigned int time);

		static void GPad_ConvertStickToFloat(short x, short y, float& outX, float& outY);
		static float GPad_GetStick(int localClientNum, Game::GamePadStick stick);
		static float GPad_GetButton(int localClientNum, Game::GamePadButton button);
		static bool GPad_IsButtonPressed(int localClientNum, Game::GamePadButton button);
		static bool GPad_ButtonRequiresUpdates(int localClientNum, Game::GamePadButton button);
		static bool GPad_IsButtonReleased(int localClientNum, Game::GamePadButton button);

		static void GPad_UpdateSticksDown(int localClientNum);
		static void GPad_UpdateSticks(int localClientNum, const XINPUT_GAMEPAD& state);
		static void GPad_UpdateDigitals(int localClientNum, const XINPUT_GAMEPAD& state);
		static void GPad_UpdateAnalogs(int localClientNum, const XINPUT_GAMEPAD& state);


		static bool GPad_Check(int localClientNum, int portIndex);
		static void GPad_RefreshAll();
		static void GPad_UpdateAll();
		static void IN_GamePadsMove();
		static void IN_Frame_Hk();

		static void Gamepad_WriteBindings(int localClientNum, int handle);
		void Key_WriteBindingsGamepad(const int localClientNum, char* handle);
		static void Key_WriteBindings_Hk(const int localClientNum, char* buffer);
		static void Gamepad_WriteBindings_Hk(int handle);
		static void Key_WriteBindings_Stub();
		static void Com_WriteConfiguration_Modified_Stub();

		static void Gamepad_BindAxis(int localClientNum, Game::GamepadPhysicalAxis realIndex, Game::GamepadVirtualAxis axisIndex, Game::GamepadMapping mapType);
		static Game::GamepadPhysicalAxis StringToPhysicalAxis(const char* str);
		static Game::GamepadVirtualAxis StringToVirtualAxis(const char* str);
		static Game::GamepadMapping StringToGamePadMapping(const char* str);
		static void Axis_Bind_f(Command::Params* params);
		static void Axis_Unbindall_f();
		static void Bind_GP_SticksConfigs_f();
		static void Bind_GP_ButtonsConfigs_f();
		static void SetAimAssist_f();
		static void PauseMenu_Toggle_f();

		static const char* GetGamePadCommand(const char* command);
		static int Key_GetCommandAssignmentInternal(int localClientNum, const char* cmd, int(*keys)[2]);
		static void Key_GetCommandAssignmentInternal_Stub();
		static void Key_SetBinding_Hk(int localClientNum, int keyNum, const char* binding);
		static void Key_SetBinding_stub01();
		static void Key_SetBinding_stub02();
		static void Key_SetBinding_stub03();
		static bool IsGamePadInUse();
		static void CL_KeyEvent_Hk(int localClientNum, int key, int down, unsigned int time);
		static bool UI_RefreshViewport_Hk();
		static void UI_RefreshStub();
		static void UI_MouseEventStub();

		static bool Gamepad_ShouldUse(const Game::gentity_s* playerEnt, unsigned useTime);
		static void Player_UseEntity_Stub();

		static Game::keyname_t* GetLocalizedKeyNameMap();
		static void GetLocalizedKeyName_Stub();
		static void GetLocalizedKeyName_Stub02();
		static void CreateKeyNameMap();

		static void HideSystemCursor();

		static void CL_GamepadCheatInit();
		static void CL_GamepadCheatReset();
		static void CL_GamepadUnlockAll(const char* cmd, const char* sound);
		static void CL_GamepadCheat();
		static void Vibrate(int leftVal, int rightVal);
	};
}