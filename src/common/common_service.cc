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
			HKEY_CURRENT_USER,		//��ǰ���г�����û�ע����
			"Software\\\\Buried",	//Ҫ������򿪵�������
			0,						//����ֵ��ͨ��Ϊ0
			NULL,					//��ȫ�����������ΪNULL������̳и����İ�ȫ������
			REG_OPTION_VOLATILE,	//��֤�����������Դ�����ע�����
			KEY_ALL_ACCESS,			//�����дȨ��
			NULL,					//������Ѵ��ڣ������滻���İ�ȫ����������ʹ�ô˲���
			&h_key,					//���ش򿪵ľ��
			NULL);					//����ֵ��ͨ��ΪNULL
		if (ret != ERROR_SUCCESS)return;
		ret = ::RegSetValueExA(h_key,				//Ҫ����ֵ�ļ��ľ��
			key.c_str(),							//Ҫд���ֵ	
			0,										//����ֵ
			REG_SZ,									//ָ��ֵ�����ͣ�����ָ�ַ���
			reinterpret_cast<const BYTE*>(value.c_str()),	//ֵ���ַ�������
			value.size()							//д��ֵ�Ĵ�С
		);
		if (ret != ERROR_SUCCESS) return;
		::RegCloseKey(h_key);
	}
	static std::string ReadRegister(const std::string& key) {
		HKEY h_key;
		//�򿪾��
		LONG ret = ::RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\\\Buried", 0, KEY_ALL_ACCESS, &h_key);
		if (ret == ERROR_SUCCESS) return "";
		char buf[1024] = { 0 };
		DWORD buf_size = sizeof(buf);
		//Ҫ��ѯ�ļ�ֵ
		ret = ::RegQueryValueExA(h_key, key.c_str(), NULL, NULL, reinterpret_cast<BYTE*>(buf), &buf_size);
		if (ret != ERROR_SUCCESS) return "";
		::RegCloseKey(h_key);
		return buf;
	}

	std::string CommonService::getRandomId() {
		static constexpr size_t len = 32;  //�������ɵ�ֵ����Ϊ32
		static constexpr auto chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; //������ַ����������ȡ������32λ���ȵ�ֵ
		//�����Ļ����ű�ʾ�б��ʼ����ʽ()������Ļ������Ǳ�ʾstd::random_device���б��ʼ����()�ǵ���random_device�ع�������()�ķ���
		//ͨ��random_device()��֤���ӵķ�ȷ���ԣ���mt199937���������
		static std::mt19937_64 rng{ std::random_device{}() };
		static std::uniform_int_distribution<size_t> dist{ 0,60 };
		std::string res;
		res.reserve(len);
		//��Ҫ�������������ȡ�ض��ַ�����ض��ַ�������ʹ�����ַ���(���mt199937_64,random_device, uniform_int_distribution��generate_n)
		std::generate_n(std::back_inserter(res), len,
			[&]() {	return chars[dist(rng)]; });

		return res;
	}

	static std::string GetDeviceId() {
		//�����豸ID�������豸�Դ��ģ������Ҫͨ��������ɲ���¼���̶�λ������֤�����豸�ͳ��������ܹ���ȡ��ͬһ��ID���������ע����¼�ķ�ʽ�����д洢��
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
		ZeroMemory(&osv_info, sizeof(OSVERSIONINFOEXA));  //���Ҫ��ƽ̨��ʹ��memset(var, 0, var_length);
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
		//������Ҫ����ʱ�䴦�����ٵ���FileTimeToLocalTime(FILETIME*,FILETIME*)
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