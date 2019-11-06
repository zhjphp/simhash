#define LOGDIR "log"
#define MKDIR "mkdir -p " LOGDIR

#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <stdlib.h>

class GlogHelper
{
  public:
    GlogHelper(std::string);
    ~GlogHelper();
};

GlogHelper::GlogHelper(std::string program)
{
    system(MKDIR);
    // 初始化应用进程名
    google::InitGoogleLogging(program.c_str());
    // 设置错误级别大于等于多少时输出到文件
    // 参数2为日志存放目录和日志文件前缀
    google::SetLogDestination(google::INFO, LOGDIR "/INFO_");       //设置 google::INFO 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::WARNING, LOGDIR "/WARNING_"); //设置 google::WARNING 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::ERROR, LOGDIR "/ERROR_");     //设置 google::ERROR 级别的日志存储路径和文件名前缀
    // 设置文件名扩展，如平台或其它需要区分的信息
    // google::SetLogFilenameExtension(obj.c_str());
    // 是否将日志输出到标准错误是不是日志文件
    FLAGS_logtostderr = true;
    // 是否同时将日志发送到标准错误和日志文件中
    FLAGS_alsologtostderr = false;
    // 当日志级别大于此级别时，自动将此日志输出到标准错误中
    FLAGS_stderrthreshold = google::FATAL;
    // 当日志级别大于此级别时会马上输出，而不缓存
    FLAGS_logbuflevel = google::WARNING;
    // 缓存最久长时间为多久
    FLAGS_logbufsecs = 0;
    // 当日志文件达到多少时，进行转存，以M为单位
    FLAGS_max_log_size = 10;
    // 当磁盘已满时,停止输出日志文件
    FLAGS_stop_logging_if_full_disk = true;
}

GlogHelper::~GlogHelper()
{
    google::ShutdownGoogleLogging();
}