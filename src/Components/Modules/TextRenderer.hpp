#pragma once

namespace Components
{
	constexpr unsigned int ColorRgba(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
	{
		return (r) | (g << 8) | (b << 16) | (a << 24);
	}

	constexpr unsigned int ColorRgb(const uint8_t r, const uint8_t g, const uint8_t b)
	{
		return ColorRgba(r, g, b, 0xFF);
	}

	class TextRenderer : public Component
	{
	public:
		TextRenderer();
	private:
		static void R_TextWidth_Stub01();
		static void R_TextWidth_Stub02();
		static void DrawHudIconStub();
		static void UI_ReplaceConversionStringStub();
		static void ButtonAnimation(char* text);

		static int R_TextWidth_Hk(const char* text, int maxChars, Game::Font_s* font);
		static float DrawHudIcon(const char* text, const Game::Font_s* font, const float x, const float y, const float sinAngle, const float cosAngle, const float xScale, const float yScale, Game::GfxColor color);
	};
}