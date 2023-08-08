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
	};
}