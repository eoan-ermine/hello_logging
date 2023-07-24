#pragma once

#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <mutex>

using namespace std::literals;

#define LOG(...) Logger::GetInstance().Log(__VA_ARGS__)

class Logger {
    auto GetTime() const {
        if (manual_ts_) {
            return *manual_ts_;
        }

        return std::chrono::system_clock::now();
    }

    auto GetTimeStamp() const {
        const auto now = GetTime();
        const auto t_c = std::chrono::system_clock::to_time_t(now);
        return std::put_time(std::localtime(&t_c), "%F %T");
    }

    static std::string GetFileName() {
        std::ostringstream time_stream;
        const auto now = std::chrono::system_clock::now();
        const auto t_c = std::chrono::system_clock::to_time_t(now);

        time_stream << std::put_time(
            std::localtime(&t_c),
            "/var/log/sample_log_%Y_%m_%d"
        );
        return time_stream.str();
    }

    Logger() = default;
    Logger(const Logger&) = delete;

public:
    static Logger& GetInstance() {
        static Logger obj;
        return obj;
    }

    // Выведите в поток все аргументы.
    template<class... Ts>
    void Log(const Ts&... args) {
        std::scoped_lock lock{m_};

        std::ofstream log_file_{GetFileName(), std::ios::app};

        log_file_ << GetTimeStamp() << ": ";
        ((log_file_ << args), ...);
        log_file_ << '\n';
    }

    // Установите manual_ts_. Учтите, что эта операция может выполняться
    // параллельно с выводом в поток, вам нужно предусмотреть 
    // синхронизацию.
    void SetTimestamp(std::chrono::system_clock::time_point ts) {
        std::scoped_lock ts_lock{m_};
        manual_ts_ = ts;
    }

private:
    std::optional<std::chrono::system_clock::time_point> manual_ts_;
    std::mutex m_;
};
