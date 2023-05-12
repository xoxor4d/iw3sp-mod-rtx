#pragma once

namespace Components
{
	class Command : public Component
	{
	public:
		class Params
		{
		public:
			Params() = default;
			virtual ~Params() = default;

			virtual int size() = 0;
			virtual const char* get(int index) = 0;
			virtual std::string join(int index);

			virtual const char* operator[](const int index)
			{
				return this->get(index);
			}
		};

		class ClientParams final : public Params
		{
		public:
			ClientParams();

			int size() override;
			const char* get(int index) override;

		private:
			int nesting_;
		};

		Command() = default;

		static Game::cmd_function_s* Allocate();

		static void Add(const char* name, const std::function<void()>& callback);
		static void Add(const char* name, const std::function<void(Command::Params*)>& callback);
		static void Add(const char* name, const char* args, const char* description, const std::function<void()>& callback);
		static void Add(const char* name, const char* args, const char* description, const std::function<void(Command::Params*)>& callback);
		static void AddRaw(const char* name, void(*callback)(), bool key = false);
		static void AddRaw(const char* name, const char* args, const char* description, void(*callback)(), bool key = false);
		static void Execute(std::string command, bool sync = true);

		static Game::cmd_function_s* Find(const std::string& command);

	private:
		static std::unordered_map<std::string, std::function<void(Command::Params*)>> FunctionMap;
		static void MainCallback();
	};
}