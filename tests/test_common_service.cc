#include <filesystem>
#include <iostream>
#include <unordered_set>

#include "gtest/gtest.h"

#include "src/common/common_service.h"

TEST(CommonService, RandomStringTest) {
	std::unordered_set<std::string> random_set;
	for (int i = 0; i < 1000; i++) {
		std::string rn = buried::CommonService::getRandomId();
		EXPECT_EQ(rn.size(), 32);
		EXPECT_EQ(random_set.count(rn), 0);
		random_set.insert(rn);
	}
}

TEST(CommonService, GetNowDateTest) {
	std::string time = buried::CommonService::getNowDate();
	std::cout << time << std::endl;
	EXPECT_GT(time.size(), 0);
}

TEST(CommonService, GetProcessTest) {
	std::string ProcessTime = buried::CommonService::getProcessTime();
	std::cout << ProcessTime << std::endl;
	EXPECT_GT(ProcessTime.size(), 0);
}

TEST(CommonService, BasicTest) {
	buried::CommonService cs;
	EXPECT_GT(cs.SystemVersion.size(), 0);
	EXPECT_GT(cs.DeviceId.size(), 0);
	EXPECT_GT(cs.DeviceName.size(), 0);
	EXPECT_GT(cs.BuriedVersion.size(), 0);
	EXPECT_GT(cs.LifeCycleId.size(), 0);
}
