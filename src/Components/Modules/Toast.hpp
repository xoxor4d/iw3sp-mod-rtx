#pragma once

namespace Components
{
	class Toast : public Component
	{
	public:
		Toast();
		~Toast();
		static void Show(const std::string& image, const std::string& title, const std::string& description, int length, bool special, const Utils::Slot<void()>& callback = Utils::Slot<void()>());
		static void Show(Game::Material* material, const std::string& title, const std::string& description, int length, bool special, const Utils::Slot<void()>& callback = Utils::Slot<void()>());

	private:
		class UIToast
		{
		public:
			Game::Material* image;
			std::string title;
			std::string desc;
			int length;
			int start;
			bool special;
			Utils::Slot<void()> callback;
			bool handledOnce;
		};

		static void Handler();
		static void Draw(UIToast* toast);

		static std::queue<UIToast> Queue;
		static std::mutex Mutex;
	};
}