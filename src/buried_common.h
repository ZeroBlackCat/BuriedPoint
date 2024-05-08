#pragma once

/*该文件用于声明埋点数据报告上传是否成功*/
enum BuriedResult {
	kBuriedOK = 1,
	kBuriedInvalidParam = 0,
	kBuriedUnkown = -1,
};