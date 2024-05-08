#include "include/buried.h"

#include <iostream>

#include "buried_core.h"

extern "C" {
	//对外暴露的接口(buried.h)→实现(映射层，buried.cc会映射成C++的实现)←buried_core.h,buried_core.cc
	//其中的buried指针会内部自动转化为C++指针
	Buried* Buried_Create(const char* work_dir) {
		if (!work_dir) {
			return nullptr;
		}

		return new Buried(work_dir);
	}

	void Buried_Destory(Buried* buried) {
		if (buried) {
			delete buried;
		}
	}

	int32_t Buried_Start(Buried* buried, BuriedConfig* config) {
		if (!buried || !config)
			return BuriedResult::kBuriedUnkown;
		Buried::Config buried_config;
		if (config->host) {
			buried_config.host = config->host;
		}
		if (config->port) {
			buried_config.port = config->port;
		}
		if (config->user_id) {
			buried_config.user_id = config->user_id;
		}
		if (config->app_name) {
			buried_config.app_name = config->app_name;
		}
		if (config->app_version) {
			buried_config.app_version = config->app_version;
		}
		if (config->custom_data) {
			buried_config.custom_data = config->custom_data;
		}

		//the code will fill in the future..
		return buried->Start(buried_config);
	}

	int32_t Buried_Report(Buried* buried, const char* title, const char* data, uint32_t priority) {
		if (!buried || !title || !data)
			return BuriedResult::kBuriedInvalidParam;

		return buried->Report(title, data, priority);
	}

	int Buried_Test()
	{
		std::cout << "Test!";

		return 1;
	}
}

