/*
	���ļ�Ŀ���ǹ������Ⱪ¶�ɵ��õ�API
*/
#pragma once

#include <stdint.h>
#define BURIED_EXPORT __declspec(dllexport)  //����windows�еĶ�̬���У������������������ࡢ����ȹ�������ã�ʡ�Ը���def�ļ�



extern "C" {
	int Buried_Test();

	typedef struct Buried Buried;

	struct BuriedConfig {
		const char* host;				//Ҫ�ϱ���url
		const char* port;			//��Ӧ�Ķ˿ں�
		const char* topic;			//��Ӧ��topic��һ�����ϴ���url:port/topic��
		const char* user_id;		//���ͱ�����û�id�����������Ϣ�������ĸ��û�ʹ��app�������쳣��
		const char* app_version;	//Ӧ�õİ汾��
		const char* app_name;		//Ӧ�õ�����
		const char* custom_data;	//�Զ������ݣ�Json�ַ�������ͬ�û�Я����ͬ���ݣ�ʹ�ø�custom_data����
	};

	//������Ҫ֧�ֶ�ʵ��������Ƴ�Create��Destory����
	//work_dir��ʾ��ʵ���Ĺ���Ŀ¼����ͬʵ���ڶ�Ӧ�Ĺ���Ŀ¼�������ļ��ȣ�����Ⱦ����Ŀ¼
	BURIED_EXPORT Buried* Buried_Create(const char* word_dir);  
	BURIED_EXPORT void Buried_Destory(Buried* buried);
	//���ڿ�������ϱ�����
	//��һ������buried����������ʵ������ʾ���ĸ�ʵ�������У��ڶ�������config��ʾ��ʵ����Ӧ������
	BURIED_EXPORT int32_t Buried_Start(Buried* buried, BuriedConfig* config);
	//�����ϴ��˴���㱨�������
	//��һ������buried����������ʵ��
	//�ڶ�������title���������ݵı��⣬��������������չʾ����վ��
	//����������data����ʾ��ϸ����
	//���ĸ�priority����ʾ���ȼ����ڲ�ģ���ϱ�ʱ�ᰴ�����ȼ����ϱ����ݣ����ȼ��ߵ����ݻ������ϱ�
	BURIED_EXPORT int32_t Buried_Report(Buried* buried, const char* title, const char* data, uint32_t priority);

}