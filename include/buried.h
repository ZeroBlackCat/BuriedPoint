/*
	该文件目的是构建对外暴露可调用的API
*/
#pragma once

#include <stdint.h>
#define BURIED_EXPORT __declspec(dllexport)  //用于windows中的动态库中，是声明导出函数、类、对象等供外面调用，省略给出def文件



extern "C" {
	int Buried_Test();

	typedef struct Buried Buried;

	struct BuriedConfig {
		const char* host;				//要上报的url
		const char* port;			//对应的端口号
		const char* topic;			//对应的topic，一般是上传到url:port/topic下
		const char* user_id;		//发送报告的用户id，方便分析信息，例如哪个用户使用app发生了异常等
		const char* app_version;	//应用的版本号
		const char* app_name;		//应用的名称
		const char* custom_data;	//自定义数据，Json字符串，不同用户携带不同数据，使用该custom_data正好
	};

	//由于需要支持多实例，需设计出Create和Destory函数
	//work_dir表示该实例的工作目录，不同实例在对应的工作目录下生成文件等，不污染其他目录
	BURIED_EXPORT Buried* Buried_Create(const char* word_dir);  
	BURIED_EXPORT void Buried_Destory(Buried* buried);
	//用于开启埋点上报能力
	//第一个参数buried，传入具体的实例，表示在哪个实例下运行，第二个参数config表示该实例对应的配置
	BURIED_EXPORT int32_t Buried_Start(Buried* buried, BuriedConfig* config);
	//用于上传此处埋点报告的数据
	//第一个参数buried，传入具体的实例
	//第二个参数title，这条数据的标题，这个标题可以用于展示在网站端
	//第三个参数data，表示详细数据
	//第四个priority，表示优先级，内部模块上报时会按照优先级来上报数据，优先级高的数据会优先上报
	BURIED_EXPORT int32_t Buried_Report(Buried* buried, const char* title, const char* data, uint32_t priority);

}