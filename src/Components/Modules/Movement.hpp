#pragma once

namespace Components
{
	class Movement : public Component
	{
	public:
		Movement();
		~Movement();
	private:
		static void PM_GetSprintLeft_stub();
		static void PM_GetSprintLeftLastTime_stub();
		static void PM_UpdateSprint_stub();
		static void PM_UFOMove(Game::pmove_t* pm, Game::pml_t* pml);
		static void PM_NoclipMove(Game::pmove_t* pm, Game::pml_t* pml);
		static float PM_MoveScale(Game::playerState_s* ps, float fmove, float rmove, float umove);
		static void PM_Accelerate(Game::vec3_t wishdir, Game::playerState_s* ps, Game::pml_t* pml, float wishspeed, float accel);
	};
}