#include "STDInc.hpp"
#include <Shlwapi.h>
#include "HTTP.hpp"

#ifdef max
#undef max
#endif

namespace Utils
{
	struct progress_helper
	{
		const std::function<void(size_t, size_t, size_t)>* callback{};
		std::exception_ptr exception{};
		std::chrono::high_resolution_clock::time_point start{};
	};

	int ProgressCallback(void* clientp, const curl_off_t dltotal, const curl_off_t dlnow, const curl_off_t /*ultotal*/, const curl_off_t /*ulnow*/)
	{
		auto* helper = static_cast<progress_helper*>(clientp);

		try
		{
			const auto now = std::chrono::high_resolution_clock::now();
			const auto count = std::max(1, static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - helper->start).count()));
			const auto speed = dlnow / count;

			if (*helper->callback)
			{
				(*helper->callback)(dlnow, dltotal, speed);
			}

			//Game::Com_Printf(0, "^1Total file size: %d, ^2Current Download Progress: %d, ^3Current Progress: %d\n", dltotal, dlnow, speed);
		}
		catch (...)
		{
			helper->exception = std::current_exception();
			return -1;
		}

		return 0;
	}

	size_t WriteCallback(void* contents, const size_t size, const size_t nmemb, void* userp)
	{
		auto* buffer = static_cast<std::string*>(userp);

		const auto total_size = size * nmemb;
		buffer->append(static_cast<char*>(contents), total_size);
		return total_size;
	}

	std::optional<std::string> HTTP::GetData(const std::string& url, const headers& headers, const std::function<void(size_t, size_t, size_t)>& callback)
	{
		curl_slist* header_list = nullptr;
		auto* curl = curl_easy_init();
		if (!curl)
		{
			return {};
		}

		auto _ = gsl::finally([&]()
		{
			curl_slist_free_all(header_list);
			curl_easy_cleanup(curl);
		});

		for (const auto& header : headers)
		{
			auto data = header.first + ": " + header.second;
			header_list = curl_slist_append(header_list, data.data());
		}

		std::string buffer{};
		progress_helper helper{};
		helper.callback = &callback;
		helper.start = std::chrono::high_resolution_clock::now();

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &helper);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

		if (curl_easy_perform(curl) == CURLE_OK)
		{
			//Game::Com_Printf(0, "HTTP: buffer return - good\n");
			return { std::move(buffer) };
		}

		if (helper.exception)
		{
			//Game::Com_Printf(0, "HTTP: Error\n");
			std::rethrow_exception(helper.exception);
		}

		//Game::Com_Printf(0, "HTTP: All good\n");
		return {};
	}
}