

#include <string>
#include <stdint.h>

#include "nlohmann/json.hpp"

namespace buried {
	struct CommonService {
	public:
		std::string host;
		std::string port;
		std::string topic;
		std::string user_id;
		std::string app_version;
		std::string app_name;
	public:
		nlohmann::json custom_data;
	public:
		std::string SystemVersion;
		std::string DeviceName;
		std::string DeviceId;
		std::string BuriedVersion;
		std::string LifeCycleId;

	public:
		CommonService();

		static std::string getProcessTime();
		static std::string getNowDate();
		static std::string getRandomId();
		
	private:
		void Init();

	};
}