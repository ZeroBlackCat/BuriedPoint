#pragma once


#include <filesystem>
#include <memory>
#include <string>

#include "buried_common.h"
#include "include/buried.h"

namespace spdlog {
	class logger;
}

namespace buried {  //namespace 与struct不能同名
	class BuriedReport;
}

struct Buried {
public:
	struct Config {
		std::string host;
		std::string port;
		std::string topic;
		std::string user_id;
		std::string app_version;
		std::string app_name;
		std::string custom_data;
	};
public:
	Buried(const std::string& work_dir);

	~Buried();

	BuriedResult Start(const Config& config);

	BuriedResult Report(std::string title, std::string data, uint32_t priority);

private:
	std::shared_ptr<spdlog::logger> logger_;

};