#include "buried_core.h"



Buried::Buried(const std::string& work_dir) {
    
}

Buried::~Buried() {}

BuriedResult Buried::Start(const Config& config) {
    return BuriedResult::kBuriedOK;
}

BuriedResult Buried::Report(std::string title, std::string data, uint32_t priority) {
    return BuriedResult::kBuriedOK;
}