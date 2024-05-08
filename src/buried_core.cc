#include "buried_core.h"



#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"



Buried::Buried(const std::string& work_dir) {
    //Loggerһ������Formatter��sink����,����sinkΪ������־д���Ŀ�ĵأ�����console_sinke��file_sink
    //�˴�Buried�ı�������console_sink����ʵʱ��ʾ��file_sink�����º���
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

    logger_ = std::shared_ptr<spdlog::logger>(new spdlog::logger("buried_sink", { console_sink, file_sink }));//ע�����ﲻ��ʹ��make_shared�������������

    logger_->set_pattern("[%c] [%s:%#] [%l] %v");
    logger_->set_level(spdlog::level::trace);

    //spdlog::info("...."); //�÷�ʽ��ʹ��Ĭ��Logger�����޷��������룬����ڶ�ʵ�����ֻ���¼��ͬһ���ط�
    //��Զ�ʵ�������Ӧ����������ʽ����¼����Բ�ͬ����logger��ʾ��Ӧ����Ϣ��
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