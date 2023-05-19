#pragma once

namespace Components
{
	class FileSystem : public Component
	{
	public:
		FileSystem();
		~FileSystem();

		static std::filesystem::path GetAppdataPath();
	private:
	};
}