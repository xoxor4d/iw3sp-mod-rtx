#pragma once

namespace Components
{
	class Branding : public Component
	{
	public:
		Branding();
		~Branding();
	private:
		static void DrawClientVersion();
		static void VersionOfTheGameStub();
	};
}