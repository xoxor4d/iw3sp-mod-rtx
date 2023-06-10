#pragma once

namespace Components
{
	class Config : public Component
	{
	public:
		template <typename T>
		static std::optional<T> Get(const std::string& key)
		{
			const auto cfg = ReadConfig();
			if (!cfg.is_object() || !cfg.contains(key))
			{
				const auto default_value = GetDefaultValue(key);
				if (default_value.has_value())
				{
					return { default_value.value() };
				}

				return {};
			}

			const auto value = ValidateTheConfigField(key, cfg[key]);
			return { value.get<T>() };
		}

		template <typename T>
		static void Set(const std::string& key, const T& value)
		{
			auto cfg = ReadConfig();
			cfg[key] = ValidateTheConfigField(key, value);
			WriteConfig(cfg);
		}

		Config();
		~Config();
	private:
		typedef nlohmann::json::value_t field_type;
		typedef nlohmann::json field_value;

		using validate_callback_t = std::function<bool(const field_value&)>;

		struct field_definition_t
		{
			field_type type;
			field_value default_value;
			std::optional<validate_callback_t> validate_value = {};
		};

		static std::unordered_map<std::string, field_definition_t> field_definitions;

		template <typename... Args>
		static std::pair<std::string, field_definition_t> define_field(const std::string& name, Args&&... args)
		{
			return std::make_pair(name, field_definition_t{ std::forward<Args>(args)... });
		}


		static std::optional<nlohmann::json> GetDefaultValue(const std::string& key);
		static void WriteConfig(const nlohmann::json& json);
		static std::string GetConfigFile();
		static nlohmann::json ReadConfig();
		static nlohmann::json GetRaw(const std::string& key);
		static nlohmann::json ValidateTheConfigField(const std::string& key, const nlohmann::json& value);
		static void ReplaceConfig(char* __str, signed __int32 __size, const char* __format, ...);
	};
}