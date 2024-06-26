#include "STDInc.hpp"

namespace Components
{
	void Branding::DrawClientVersion()
	{
		if (Dvars::Functions::Dvar_FindVar("cg_drawBrandingInfo")->current.enabled)
		{
			const char* text = "IW3SP_MOD " VERSION;
			const float color[4] = { 0.0f, 0.80f, 0.0f, 0.69f };
			const float shadowColor[4] = { 0.0f, 0.0f, 0.0f, 0.69f };

			Game::Font_s* fontHandle = Game::R_RegisterFont("fonts/objectiveFont", sizeof("fonts/objectiveFont"));

			float fontScaleWidth = (0.2f * 48.0f / static_cast<float>(fontHandle->pixelHeight)) * Game::scrPlace->scaleVirtualToReal[0];
			float fontScaleHeight = (0.2f * 48.0f / static_cast<float>(fontHandle->pixelHeight)) * Game::scrPlace->scaleVirtualToReal[1];

			float xPos = 2.5f * Game::scrPlace->scaleVirtualToReal[0];
			float yPos = 13.5f * Game::scrPlace->scaleVirtualToReal[1];

			Game::R_AddCmdDrawTextASM(text, 0x7FFFFFFF, fontHandle, floor((xPos + 1.0) + 0.5), floor((yPos + 1.0) + 0.5), fontScaleWidth, fontScaleHeight, 0, shadowColor, 0);
			Game::R_AddCmdDrawTextASM(text, 0x7FFFFFFF, fontHandle, floor(xPos + 0.5), floor(yPos + 0.5), fontScaleWidth, fontScaleHeight, 0, color, 0);
		}
	}

	void Branding::VersionOfTheGameStub()
	{
		Branding::DrawClientVersion();
		Utils::Hook::Call<void()>(0x438B20)();
	}

	Branding::Branding()
	{
		//Branding stuffs
		// Game window name
		Utils::Hook::Set<const char*>(0x5D9680, "IW3SP_MOD");
		// Game console name (external console)
		Utils::Hook::Set<const char*>(0x5963F7, "IW3SP Modification");

		// Game console name (~)
		Utils::Hook::Set<const char*>(0x43B9E1, "IW3SP_MOD: > ");
		// Short version
		Utils::Hook::Set<const char*>(0x534FE3, "1.5");

		Utils::Hook::Set<const char*>(0x53535E, "// generated by IW3SP_MOD\n");
		Utils::Hook::Set<const char*>(0x5353F0, "// generated by IW3SP_MOD\n");

		// SCR_UpdateFrame hooking
		Utils::Hook(0x447888, VersionOfTheGameStub, HOOK_CALL).install()->quick();
	}

	Branding::~Branding()
	{
	}
}