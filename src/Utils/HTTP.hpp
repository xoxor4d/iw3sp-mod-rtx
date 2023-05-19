#pragma once

namespace Utils
{
	class HTTP
	{
	public:
		using headers = std::unordered_map<std::string, std::string>;

		static std::optional<std::string> GetData(const std::string& url, const headers& headers = {}, const std::function<void(size_t, size_t, size_t)>& callback = {});
	};
}