#include "buried_core.h"



#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"



Buried::Buried(const std::string& work_dir) {
    //Logger一般是由Formatter和sink构成,其中sink为控制日志写入的目的地，例如console_sinke，file_sink
    //此处Buried的报告是由console_sink用于实时显示和file_sink用于事后检查
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    std::filesystem::path _work_dir(work_dir);
    if (!std::filesystem::exists(_work_dir)) {
        std::filesystem::create_directories(_work_dir);
    }
    _work_dir /= "buried";

    if (!std::filesystem::exists(_work_dir)) {
        std::filesystem::create_directories(_work_dir);
    }

    std::filesystem::path _log_dir(_work_dir);
    _log_dir /= "buried.log";

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(_log_dir.string(), true);

    logger_ = std::shared_ptr<spdlog::logger>(new spdlog::logger("buried_sink", { console_sink, file_sink }));//注意这里不能使用make_shared，否则会编译错误

    logger_->set_pattern("[%c] [%s:%#] [%l] %v");
    logger_->set_level(spdlog::level::trace);

    //spdlog::info("...."); //该方式是使用默认Logger对象，无法做到隔离，针对于多实例情况只会记录到同一个地方
    //针对多实例情况，应该用下述方式来记录，针对不同级别logger显示对应的信息。
    SPDLOG_LOGGER_TRACE(logger_, "Some trace message with param: {}", 42);
    SPDLOG_LOGGER_DEBUG(logger_, "Some debug message");
}

Buried::~Buried() {}

BuriedResult Buried::Start(const Config& config) {
    return BuriedResult::kBuriedOK;
}

BuriedResult Buried::Report(std::string title, std::string data, uint32_t priority) {
    return BuriedResult::kBuriedOK;
}