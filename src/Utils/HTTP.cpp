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

	int CalculateProgressBarWidth(int totalWidth, curl_off_t downloadedSize, curl_off_t totalSize) {
		double progress = static_cast<double>(downloadedSize) / totalSize;
		int progressBarWidth = static_cast<int>(progress * totalWidth + 0.5);
		return progressBarWidth;
	}

	int ProgressCallbackDownloading(void* clientp, const curl_off_t dltotal, const curl_off_t dlnow, const curl_off_t /*ultotal*/, const curl_off_t /*ulnow*/)
	{
		Game::menuDef_t* menu = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "updater_download_menu").menu;
		menu->items[9]->window.rectClient.w = 0.000000;
		menu->items[5]->text = "";
		menu->items[7]->text = "";
		menu->items[10]->text = "";

		auto* helper = static_cast<progress_helper*>(clientp);

		try
		{
			if (Components::Updater::UpdateCancelled())
			{
				menu->items[9]->window.rectClient.w = 0.000000;
				menu->items[5]->text = "";
				menu->items[7]->text = "";
				menu->items[10]->text = "";
			}

			const auto now = std::chrono::high_resolution_clock::now();
			const auto count = std::max(1, static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - helper->start).count()));
			const auto speed = dlnow / count;

			if (*helper->callback)
			{
				(*helper->callback)(dlnow, dltotal, speed);
			}

			const std::string formattedTotalSize = Utils::String::FormatSize(dltotal);
			const std::string formattedProgress = Utils::String::FormatSize(dlnow);
			const std::string formattedSpeed = Utils::String::FormatBandwidth(speed, 1000);


			double progressPercentage = (dltotal != 0) ? (static_cast<double>(dlnow) / dltotal * 100) : 0.0;
			double progressBarWidth = (progressPercentage / 100) * 313;

			menu->items[5]->text = Utils::String::VA("(%s/%s)", formattedProgress.c_str(), formattedTotalSize.c_str());
			menu->items[7]->text = Utils::String::VA("%s", formattedSpeed.c_str());
			menu->items[10]->text = Utils::String::VA("%.2f%%", progressPercentage);
			menu->items[9]->window.rectClient.w = progressBarWidth;

			// In the current moment, this code it's broken.
			//if (Components::Updater::UpdateCancelled())
			//{
			//	return -1;
			//}
		}
		catch (...)
		{
			helper->exception = std::current_exception();
			return -1;
		}

		return 0;
	}

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

	std::optional<std::string> HTTP::GetData(const std::string& url, const headers& headers, const std::function<void(size_t, size_t, size_t)>& callback, bool downloading)
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

		if(downloading)
			curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, ProgressCallbackDownloading);
		else
			curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);

		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &helper);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

		if (curl_easy_perform(curl) == CURLE_OK)
		{
			return { std::move(buffer) };
		}

		if (helper.exception)
		{
			std::rethrow_exception(helper.exception);
		}

		return {};
	}
}