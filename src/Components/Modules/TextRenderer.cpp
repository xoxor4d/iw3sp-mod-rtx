#include "STDInc.hpp"

namespace Components
{
	float TextRenderer::DrawHudIcon(const char* text, const Game::Font_s* font, const float x, const float y, const float sinAngle, const float cosAngle, const float xScale, const float yScale, Game::GfxColor color)
	{
		float s0, s1, t0, t1;

		if (*text == '\x01')
		{
			s0 = 0.0;
			t0 = 0.0;
			s1 = 1.0;
			t1 = 1.0;
		}
		else
		{
			s0 = 1.0;
			t0 = 0.0;
			s1 = 0.0;
			t1 = 1.0;
		}

		++text; //push to width

		if (*text == 0)
		{
			return 0.0f;
		}

		const auto v12 = font->pixelHeight * (*text - 16) + 16;
		const auto w = static_cast<float>((((v12 >> 24) & 0x1F) + v12) >> 5) * xScale;
		++text; //push to height

		if (*text == 0)
		{
			return 0.0f;
		}

		const auto h = static_cast<float>((font->pixelHeight * (*text - 16) + 16) >> 5) * yScale;
		++text; //push to nameLen

		if (*text == 0)
		{
			return 0.0f;
		}

		const auto materialNameLen = static_cast<uint8_t>(*text);
		++text; //push to materialName

		for (auto i = 0u; i < materialNameLen; i++)
		{
			if (text[i] == 0)
			{
				return 0.0f;
			}
		}

		const std::string materialName(text, materialNameLen);
		text += materialNameLen;

		auto* material = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, materialName.data()).material;
		if (material == nullptr || material->techniqueSet == nullptr || material->techniqueSet->name == nullptr || std::strcmp(material->techniqueSet->name, "2d") != 0)
		{
			material = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, "default").material;
		}

		const auto yy = y - (h + yScale * static_cast<float>(font->pixelHeight)) * 0.5f;

		Game::RB_DrawStretchPicRotate(material, x, yy, w, h, s0, t0, s1, t1, sinAngle, cosAngle, ColorRgba(255, 255, 255, color.array[3]));

		return w;
	}

	int TextRenderer::R_TextWidth_Hk(const char* text, int maxChars, Game::Font_s* font)
	{
		auto lineWidth = 0;
		auto maxWidth = 0;

		if (maxChars <= 0)
		{
			maxChars = std::numeric_limits<int>::max();
		}

		if (text == nullptr)
		{
			return 0;
		}

		auto count = 0;
		while (text && *text && count < maxChars)
		{
			const auto letter = Game::SEH_ReadCharFromString(&text);
			if (letter == '\r' || letter == '\n')
			{
				lineWidth = 0;
			}
			else
			{
				if (letter == '^' && text)
				{
					if (*text >= '0' && *text <= '9')
					{
						++text;
						continue;
					}

					if (*text >= '\x01' && *text <= '\x02' && text[1] != '\0' && text[2] != '\0' && text[3] != '\0')
					{
						const auto width = text[1];
						const auto materialNameLength = text[3];

						// This is how the game calculates width and height. Probably some 1 byte floating point number.
						// Details to be investigated if necessary.
						const auto v9 = font->pixelHeight * (width - 16) + 16;
						const auto w = ((((v9 >> 24) & 0x1F) + v9) >> 5);

						lineWidth += w;
						if (lineWidth > maxWidth)
						{
							maxWidth = lineWidth;
						}

						text += 4;
						for (auto currentLength = 0; currentLength < materialNameLength && *text; currentLength++)
						{
							++text;
						}
						continue;
					}
				}

				lineWidth += Game::R_GetCharacterGlyph(font, letter)->dx;
				if (lineWidth > maxWidth)
				{
					maxWidth = lineWidth;
				}

				++count;
			}
		}

		return maxWidth;
	}

	void TextRenderer::ButtonAnimation(char* text)
	{
		Game::UI_FilterStringForButtonAnimation(text, 1024);
	}

	__declspec(naked) void TextRenderer::R_TextWidth_Stub01()
	{
		const static uint32_t jump_offset = 0x56E618;
		__asm
		{
			push	eax;
			call	TextRenderer::R_TextWidth_Hk;
			add		esp, 4;
			jmp		jump_offset;
		}
	}

	__declspec(naked) void TextRenderer::R_TextWidth_Stub02()
	{
		const static uint32_t jump_offset = 0x564BB6;
		__asm
		{
			push	eax;
			call	TextRenderer::R_TextWidth_Hk;
			add		esp, 4;
			jmp		jump_offset;
		}
	}

	__declspec(naked) void TextRenderer::DrawHudIconStub()
	{
		const static uint32_t jump_offset = 0x5FE407;
		__asm
		{
			push	eax; //font
			push	ecx; //text
			call	TextRenderer::DrawHudIcon;
			add		esp, 0x8;
			jmp		jump_offset;
		}
	}

	__declspec(naked) void TextRenderer::UI_ReplaceConversionStringStub()
	{
		const static uint32_t UI_ReplaceConversions_func = 0x568240;
		const static uint32_t jump_offset = 0x568229;
		__asm
		{
			call	UI_ReplaceConversions_func;

			lea     ecx, [esp + 34h];
			push	ecx;
			call	ButtonAnimation;
			add		esp, 4;

			jmp		jump_offset;
		}
	}

	TextRenderer::TextRenderer()
	{
		Utils::Hook(0x56E613, R_TextWidth_Stub01, HOOK_JUMP).install()->quick();
		Utils::Hook(0x564BB1, R_TextWidth_Stub02, HOOK_JUMP).install()->quick();

		Utils::Hook::Set<BYTE>(0x5FE438, 0x12);
		Utils::Hook(0x5FE402, DrawHudIconStub, HOOK_JUMP).install()->quick();

		Utils::Hook(0x568224, UI_ReplaceConversionStringStub, HOOK_JUMP).install()->quick();
	}
}