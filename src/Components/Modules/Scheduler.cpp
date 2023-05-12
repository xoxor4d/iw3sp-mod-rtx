#include "STDInc.hpp"

constexpr bool COND_CONTINUE = false;
constexpr bool COND_END = true;

namespace Components
{
	std::mutex Scheduler::mutex_;
	std::queue<std::pair<std::string, int>> Scheduler::errors_;

	std::thread Scheduler::Thread;
	volatile bool Scheduler::Kill = false;
	Scheduler::TaskPipeline Scheduler::Pipelines[static_cast<std::underlying_type_t<Pipeline>>(Pipeline::COUNT)];

	void Scheduler::TaskPipeline::add(Task&& task)
	{
		newCallbacks_.access([&task](taskList& tasks)
		{
			tasks.emplace_back(std::move(task));
		});
	}

	void Scheduler::TaskPipeline::execute()
	{
		callbacks_.access([&](taskList& tasks)
		{
			this->mergeCallbacks();

			for (auto i = tasks.begin(); i != tasks.end();)
			{
				const auto now = std::chrono::high_resolution_clock::now();
				const auto diff = now - i->lastCall;

				if (diff < i->interval)
				{
					++i;
					continue;
				}

				i->lastCall = now;

				const auto res = i->handler();
				if (res == COND_END)
				{
					i = tasks.erase(i);
				}
				else
				{
					++i;
				}
			}
		});
	}
	
	void Scheduler::TaskPipeline::mergeCallbacks()
	{
		callbacks_.access([&](taskList& tasks)
		{
			newCallbacks_.access([&](taskList& new_tasks)
			{
				tasks.insert(tasks.end(), std::move_iterator<taskList::iterator>(new_tasks.begin()), std::move_iterator<taskList::iterator>(new_tasks.end()));
				new_tasks = {};
			});
		});
	}

	void Scheduler::Execute(Pipeline type)
	{
		assert(type < Pipeline::COUNT);
		const auto index = static_cast<std::underlying_type_t<Pipeline>>(type);
		Pipelines[index].execute();
	}

		void Scheduler::Schedule(const std::function<bool()>& callback, const Pipeline type,
		const std::chrono::milliseconds delay)
	{
		assert(type < Pipeline::COUNT);

		Task task;
		task.handler = callback;
		task.interval = delay;
		task.lastCall = std::chrono::high_resolution_clock::now();

		const auto index = static_cast<std::underlying_type_t<Pipeline>>(type);
		Pipelines[index].add(std::move(task));
	}

	void Scheduler::Loop(const std::function<void()>& callback, const Pipeline type, const std::chrono::milliseconds delay)
	{
		Schedule([callback]
		{
			callback();
			return COND_CONTINUE;
		}, type, delay);
	}

	void Scheduler::Once(const std::function<void()>& callback, const Pipeline type, const std::chrono::milliseconds delay)
	{
		Schedule([callback]
		{
			callback();
			return COND_END;
		}, type, delay);
	}

	void Scheduler::error(const std::string& message, int level)
	{
		std::lock_guard _(mutex_);
		errors_.emplace(message, level);
	}

	__declspec(naked) void Scheduler::main_frame_stub()
	{
		static const int ExecutionPipeline = Pipeline::MAIN;
		const static uint32_t retn_addr = 0x5356F0;
		__asm
		{
			pushad;
			push	ExecutionPipeline;
			call	Execute;
			pop		eax;
			popad;

			jmp		retn_addr;
		}
	}

	Scheduler::Scheduler()
	{
		Utils::Hook(0x535A78, main_frame_stub, HOOK_CALL).install()->quick();
	}
}