#pragma once
#include "Utils/ConcurrentList.hpp"

namespace Components
{
	class Scheduler : public Component
	{
	public:
		enum Pipeline : int
		{
			MAIN,
			RENDERED,
			COUNT,
		};

		enum evaluation : bool
		{
			reschedule = false,
			remove = true,
		};

		Scheduler();

		static void Schedule(const std::function<bool()>& callback, Pipeline type,
			std::chrono::milliseconds delay = 0ms);
		static void Loop(const std::function<void()>& callback, Pipeline type,
			std::chrono::milliseconds delay = 0ms);
		static void Once(const std::function<void()>& callback, Pipeline type,
			std::chrono::milliseconds delay = 0ms);

		//static void on_frame(const std::function<void()>& callback, thread thread = main);
		//static void delay(const std::function<void()>& callback, std::chrono::milliseconds delay, thread thread = main);
		//static void once(const std::function<void()>& callback, thread thread = main);
		//static void until(const std::function<evaluation()>& callback, thread thread = main);
		//static void loop(const std::function<void()>& callback, const std::chrono::milliseconds delay, const thread thread);

		static void error(const std::string& message, int level);

	private:

		struct Task
		{
			std::function<bool()> handler{};
			std::chrono::milliseconds interval{};
			std::chrono::high_resolution_clock::time_point lastCall{};
		};

		using taskList = std::vector<Task>;

		class TaskPipeline
		{
		public:
			void add(Task&& task);
			void execute();

		private:
			Utils::ConcurrentList<taskList> newCallbacks_;
			Utils::ConcurrentList<taskList, std::recursive_mutex> callbacks_;

			void mergeCallbacks();
		};

		static volatile bool Kill;
		static std::thread Thread;
		static TaskPipeline Pipelines[];

		static void Execute(Pipeline type);

		static std::mutex mutex_;
		static std::queue<std::pair<std::string, int>> errors_;
		//static Utils::ConcurrentList<std::pair<std::function<void()>, thread>> callbacks_;
		//static Utils::ConcurrentList<std::pair<std::function<void()>, thread>> single_callbacks_;
		//static Utils::ConcurrentList<std::pair<std::function<Scheduler::evaluation()>, thread>> condition_callbacks_;

		static void main_frame_stub();

		static void execute(Pipeline thread);
		static void execute_safe(Pipeline thread);
		static void execute_error(Pipeline thread);
		static bool get_next_error(const char** error_message, int* error_level);
	};
}