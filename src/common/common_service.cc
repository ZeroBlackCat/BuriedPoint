#include "common/common_service.h"

#include <windows.h>

#include <algorithm>
#include <chrono>
#include <ctime>
#include <random>
#include "buried_configure.h"

namespace buried {
	CommonService::CommonService() { Init(); }



	static std::string getDeviceName() {
		char buf[1024];
		DWORD buf_size = sizeof(buf);
		::GetComputerName(buf, &buf_size);

		return buf;
	}

	static void WriteRegister(const std::string& key, const std::string& value) {
		HKEY h_key;
		LONG ret = ::RegCreateKeyExA(
			HKEY_CURRENT_USER,		//当前运行程序的用户注册表键
			"Software\\\\Buried",	//要创建或打开的子链名
			0,						//保留值，通常为0
			NULL,					//安全描述符，如果为NULL，则键继承父键的安全描述符
			REG_OPTION_VOLATILE,	//保证重启后数据仍存在在注册表中
			KEY_ALL_ACCESS,			//允许读写权限
			NULL,					//如果键已存在，且需替换它的安全描述符，则使用此参数
			&h_key,					//返回打开的句柄
			NULL);					//保留值，通常为NULL
		if (ret != ERROR_SUCCESS)return;
		ret = ::RegSetValueExA(h_key,				//要设置值的键的句柄
			key.c_str(),							//要写入的值	
			0,										//保留值
			REG_SZ,									//指明值的类型，这里指字符串
			reinterpret_cast<const BYTE*>(value.c_str()),	//值的字符串类型
			value.size()							//写入值的大小
		);
		if (ret != ERROR_SUCCESS) return;
		::RegCloseKey(h_key);
	}
	static std::string ReadRegister(const std::string& key) {
		HKEY h_key;
		//打开句柄
		LONG ret = ::RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\\\Buried", 0, KEY_ALL_ACCESS, &h_key);
		if (ret == ERROR_SUCCESS) return "";
		char buf[1024] = { 0 };
		DWORD buf_size = sizeof(buf);
		//要查询的键值
		ret = ::RegQueryValueExA(h_key, key.c_str(), NULL, NULL, reinterpret_cast<BYTE*>(buf), &buf_size);
		if (ret != ERROR_SUCCESS) return "";
		::RegCloseKey(h_key);
		return buf;
	}

	std::string CommonService::getRandomId() {
		static constexpr size_t len = 32;  //定义生成的值长度为32
		static constexpr auto chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; //从这段字符串中随机抽取并生成32位长度的值
		//最外层的花括号表示列表初始化形式()，里面的花括号是表示std::random_device的列表初始化，()是调用random_device重构操作符()的方法
		//通过random_device()保证种子的非确定性，用mt199937生成随机数
		static std::mt19937_64 rng{ std::random_device{}() };
		static std::uniform_int_distribution<size_t> dist{ 0,60 };
		std::string res;
		res.reserve(len);
		//若要从数组中随机抽取特定字符组成特定字符串可以使用这种方法(结合mt199937_64,random_device, uniform_int_distribution和generate_n)
		std::generate_n(std::back_inserter(res), len,
			[&]() {	return chars[dist(rng)]; });

		return res;
	}

	static std::string GetDeviceId() {
		//由于设备ID并不是设备自带的，因此需要通过随机生成并记录到固定位置来保证重启设备和程序依旧能够读取到同一个ID，这里采用注册表记录的方式来进行存储。
		static constexpr auto kDeviceIdKey = "device_id";
		static std::string device_id = ReadRegister(kDeviceIdKey);
		if (device_id.empty()) {
			device_id = CommonService::getRandomId();
			WriteRegister(kDeviceIdKey, device_id);
		}
		return device_id;
	}

	static std::string GetLifeCycleId() {
		static std::string lifecycle_id = CommonService::getRandomId();
		return lifecycle_id;
	}

	static std::string GetSystemVersion() {
		OSVERSIONINFOEXA osv_info;
		ZeroMemory(&osv_info, sizeof(OSVERSIONINFOEXA));  //如果要跨平台就使用memset(var, 0, var_length);
		osv_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
		::GetVersionExA(reinterpret_cast<OSVERSIONINFOA*>(&osv_info));
		std::string version_str = std::to_string(osv_info.dwMajorVersion) + "." + 
			std::to_string(osv_info.dwMinorVersion) + "." + 
			std::to_string(osv_info.dwBuildNumber);


		return version_str;
	}

	std::string CommonService::getProcessTime() {
		DWORD pid = ::GetCurrentProcessId();
		HANDLE h_process = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		if (h_process == NULL) return "";
		FILETIME create_time;
		FILETIME exit_time;
		FILETIME kernel_time;
		FILETIME user_time;
		BOOL ret = ::GetProcessTimes(h_process, &create_time, &exit_time, &kernel_time, &user_time);
		::CloseHandle(h_process);
		if (ret == 0) return "";
		//若还需要当地时间处理，则再调用FileTimeToLocalTime(FILETIME*,FILETIME*)
		FILETIME create_local_time;
		::FileTimeToLocalFileTime(&create_time, &create_local_time);
		SYSTEMTIME create_sys_time;
		::FileTimeToSystemTime(&create_local_time, &create_sys_time);

		char buf[128] = { 0 };
		sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d.%03d", create_sys_time.wYear, create_sys_time.wMonth, create_sys_time.wDay,
			create_sys_time.wHour, create_sys_time.wMinute, create_sys_time.wSecond, create_sys_time.wMilliseconds);

		return buf;
	};

	std::string CommonService::getNowDate()
	{
		auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		
		return std::ctime(&time);
	}

	void CommonService::Init() {
		SystemVersion = GetSystemVersion();
		DeviceName = getDeviceName();
		DeviceId = GetDeviceId();
		BuriedVersion = PROJECT_VERSION;
		LifeCycleId = GetLifeCycleId();
	}
}