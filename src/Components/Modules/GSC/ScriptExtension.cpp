#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<const char*, const char*> ScriptExtension::ReplacedFunctions;
	const char* ScriptExtension::ReplacedPos = nullptr;

	const char* ScriptExtension::GetCodePosForParam(int index)
	{
		if (static_cast<unsigned int>(index) >= Game::scrVmPub->outparamcount)
		{
			Game::Scr_ParamError("GetCodePosForParam: Index is out of range!", static_cast<unsigned int>(index));
			return "";
		}

		const auto* value = &Game::scrVmPub->top[-index];

		if (value->type != Game::VAR_FUNCTION)
		{
			Game::Scr_ParamError("GetCodePosForParam: Expects a function as parameter!", static_cast<unsigned int>(index));
			return "";
		}

		return value->u.codePosValue;
	}

	void ScriptExtension::SetReplacedPos(const char* what, const char* with)
	{
		if (!*what || !*with)
		{
			Game::Com_Printf(0, "Invalid parameters passed to ReplacedFunctions\n");
			return;
		}

		if (ReplacedFunctions.contains(what))
		{
			Game::Com_Printf(0, "ReplacedFunctions already contains codePosValue for a function\n");
		}

		ReplacedFunctions[what] = with;
	}

	void ScriptExtension::GetReplacedPos(const char* pos)
	{
		if (!pos)
		{
			// This seems to happen often and there should not be pointers to NULL in our map
			return;
		}

		if (const auto itr = ReplacedFunctions.find(pos); itr != ReplacedFunctions.end())
		{
			ReplacedPos = itr->second;
		}
	}

	__declspec(naked) void ScriptExtension::VMExecuteInternalStub()
	{
		__asm
		{
			pushad

			push edx
			call GetReplacedPos

			pop edx
			popad

			cmp ReplacedPos, 0
			jne SetPos

			movzx eax, byte ptr	[edx]
			mov     edi, 1
			add     edx, edi

		Loc1:
			cmp eax, 0x86

			push ecx

			mov ecx, 0xFFD478
			mov	[ecx], eax

			mov ecx, 0x12869EC
			mov	[ecx], edx

			pop ecx

			push 0x55229C
			ret
		SetPos:
			mov		edx, ReplacedPos;
			mov		ReplacedPos, 0;

			movzx	eax, byte ptr [edx]
			mov     edi, 1
			add     edx, edi

			jmp Loc1
		}
	}

	ScriptExtension::ScriptExtension()
	{
		GSC::AddFunction("ReplaceFunc", [] // gsc: ReplaceFunc(<function>, <function>)
		{
			if (Game::Scr_GetNumParam() != 2)
			{
				Game::Scr_Error("ReplaceFunc: Needs two parameters!");
				return;
			}

			const auto what = GetCodePosForParam(0);
			const auto with = GetCodePosForParam(1);

			SetReplacedPos(what, with);
		});

		Utils::Hook(0x552282, VMExecuteInternalStub, HOOK_JUMP).install()->quick();
		Utils::Hook::Nop(0x552287, 5);

		Events::OnVMShutdown([]
		{
			ReplacedFunctions.clear();
		});
	}

	ScriptExtension::~ScriptExtension()
	{
	}
}