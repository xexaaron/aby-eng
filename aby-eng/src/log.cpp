#include "log.hpp"

#include <chrono>
#include <string>
#include <string_view>

#ifdef _WIN32
#	include <Windows.h>
#	include <processthreadsapi.h>
#endif

namespace aby::eng {

	Logger::MapTy<ELogger, std::shared_ptr<Logger>> Logger::s_Loggers = {};
	std::thread Logger::s_Thread;

	auto Logger::get(ELogger logger) -> std::shared_ptr<Logger> {
		auto it = s_Loggers.find(logger);
		if (it == s_Loggers.end()) {
			auto logger_ref = std::make_shared<Logger>();
			logger_ref->add_sink(LogSink([](const LogRecord& record) {
				*record.stream << record.msg << '\n';
			}));
			s_Loggers[logger] = logger_ref;
			return logger_ref;
		}
		return it->second;
	}

	auto Logger::run() -> void {
		if (s_Running) return;
		s_Running = true;

		s_Thread = std::thread([]() {
			auto internal = get(ELogger::internal);
			auto client   = get(ELogger::client);
			while (true) {
				size_t popped = 0;
				LogRecord record;

				while (internal->m_Queue.try_pop(record)) {
					internal->run_sinks(record);
					popped++;
				}
				while (client->m_Queue.try_pop(record)) {
					client->run_sinks(record);
					popped++;
				}

				bool internal_running = internal->bRunning.load(std::memory_order_acquire);
				bool client_running   = client->bRunning.load(std::memory_order_acquire);

				if (!internal_running && !client_running && popped == 0) break;
				if (popped == 0) std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		});

#ifdef _WIN32
		SetThreadDescription((HANDLE)s_Thread.native_handle(), L"Log Thread");
#endif
	}

	auto Logger::shutdown() -> void {
		auto internal = get(ELogger::internal);
		auto client   = get(ELogger::client);
		internal->bRunning.store(false, std::memory_order_release);
		client->bRunning.store(false, std::memory_order_release);
		if (s_Thread.joinable()) {
			s_Thread.join();
		}
	}

	auto Logger::get_timestamp() -> std::string {
		auto now = std::chrono::system_clock::now();
		auto t   = std::chrono::system_clock::to_time_t(now);
		std::stringstream ss;
		ss << std::put_time(std::localtime(&t), m_TimestampFormat.data());
		return ss.str();
	}

	auto Logger::add_sink(LogSink log_sink) -> size_t {
		size_t id = m_SinkIDPool;
		log_sink.set_id(id);
		m_Sinks.push_back(std::move(log_sink));
		m_SinkIDPool++;
		return id;
	}

	auto Logger::remove_sink(size_t id) -> void {
		auto it = std::find_if(m_Sinks.begin(), m_Sinks.end(), [id = id](const LogSink& sink) {
			return sink.id() == id;
		});
		if (it != m_Sinks.end()) {
			m_Sinks.erase(it);
		} else {
			log_wrn("[logger] attempted to remove sink with id: {}. it does not exist", id);
		}
	}

	auto Logger::run_sinks(const LogRecord& record) -> void {
		for (size_t i = 0; i < m_Sinks.size(); i++) {
			if (!m_Sinks[i]) continue;
			if (record.lvl == ELogLevel::dev || record.lvl == ELogLevel::todo) {
				m_Sinks[i](record);
			} else if (!(record.lvl > m_Sinks[i].level())) {
				m_Sinks[i](record);
			}
		}
	}

	auto Logger::format_depth(size_t depth) -> std::string {
		constexpr auto dim   = "\033[38;2;45;45;45m";
		constexpr auto green = "\033[38;2;181;206;168m";
		constexpr auto reset = "\x1b[0m";

		if (depth < 10) {
			return std::string(dim) + "00" + reset +
			       green + std::to_string(depth) + reset;
		}

		if (depth < 100) {
			return std::string(dim) + "0" + reset +
			       green + std::to_string(depth) + reset;
		}

		return std::format("{}{}{}", green, depth, reset);
	}

	auto Logger::format_fn(std::string_view fn) -> std::string {
		return format_fn(std::string(fn));
	}

	auto Logger::format_type(std::string_view type) -> std::string {
		return format_type(std::string(type));
	}

	auto Logger::format_fn(const std::string& fn) -> std::string {
		constexpr std::string_view ns_color    = "\x1b[38;2;183;200;200m";
		constexpr std::string_view class_color = "\x1b[38;2;62;201;149m";
		constexpr std::string_view fn_color    = "\x1b[38;2;208;220;144m";
		constexpr std::string_view sep_color   = "\x1b[37m";
		constexpr std::string_view reset       = "\x1b[0m";

		std::string out;
		out.reserve(fn.size() + 64);

		size_t last = fn.rfind("::");

		// func
		if (last == std::string::npos) {
			out += fn_color;
			out += fn;
			out += reset;
			return out;
		}

		size_t second_last = fn.rfind("::", last - 1);

		// foo::func
		if (second_last == std::string::npos) {
			auto ns       = fn.substr(0, last);
			auto function = fn.substr(last + 2);

			out += ns_color;
			out += ns;

			out += sep_color;
			out += "::";

			out += fn_color;
			out += function;

			out += reset;
			return out;
		}

		// foo::Class::func
		auto ns       = fn.substr(0, second_last);
		auto cls      = fn.substr(second_last + 2, last - (second_last + 2));
		auto function = fn.substr(last + 2);

		out += ns_color;
		out += ns;

		out += sep_color;
		out += "::";

		out += class_color;
		out += cls;

		out += sep_color;
		out += "::";

		out += fn_color;
		out += function;

		out += reset;

		return out;
	}

	auto Logger::format_type(const std::string& type) -> std::string {
		constexpr std::string_view class_color = "\x1b[38;2;62;201;149m";
		constexpr std::string_view reset       = "\x1b[0m";
		std::string out;
		out += class_color;
		out += type;
		out += reset;
		return out;
	}

	auto Logger::format_header(ELogLevel level) -> std::string {
		auto& info = m_Levels[level];
		if (bTimestamp) {
			return std::format("{} {}{}\x1b[0m", get_timestamp(), info.color, info.prefix);
		}
		return std::format("{}{}\x1b[0m", info.color, info.prefix);
	}

	auto Logger::set_trace(bool enabled) -> Logger* {
		bTrace = enabled;
		return this;
	}

	auto Logger::set_todo(bool enabled) -> Logger* {
		bTodo = enabled;
		return this;
	}

	auto Logger::set_time(bool enabled) -> Logger* {
		bTime             = enabled;
		LogTimer::enabled = enabled;
		return this;
	}

	auto Logger::set_level(ELogLevel lvl) -> Logger* {
		m_Level = lvl;
		return this;
	}

	auto Logger::set_timestamp(bool enabled, std::string_view format) -> Logger* {
		bTimestamp        = enabled;
		m_TimestampFormat = format;
		return this;
	}

	auto Logger::set_level_info(ELogLevel lvl, const LogInfo& info) -> Logger* {
		m_Levels[lvl] = info;
		return this;
	}

	auto Logger::set_color(ELogLevel level, std::string_view color) -> Logger* {
		m_Levels[level].color = color;
		return this;
	}

	auto Logger::set_prefix(ELogLevel level, std::string_view prefix) -> Logger* {
		m_Levels[level].prefix = prefix;
		return this;
	}

	auto Logger::set_stream(ELogLevel level, std::ostream* stream) -> Logger* {
		m_Levels[level].stream = stream;
		return this;
	}

	auto Logger::time(const std::string& name, std::chrono::microseconds us, std::chrono::milliseconds ms) -> void {
		if (!bTime) return;
		auto& info = m_Levels[ELogLevel::log];
		auto msg   = stime(name, us, ms);
		m_Queue.push(LogRecord{
		    .stream = info.stream,
		    .lvl    = ELogLevel::log,
		    .msg    = msg });
	}

	auto Logger::stime(const std::string& name, std::chrono::microseconds us, std::chrono::milliseconds ms) -> std::string {
		auto& info     = m_Levels[ELogLevel::log];
		double seconds = static_cast<double>(us.count()) / 1'000'000.0;
		return std::format("{} {}", format_header(ELogLevel::log), std::format("{} took {:.4f}s", name, seconds));
	}

	auto Logger::strace(const fs::path& file, LogLine line, const std::string& fn) -> std::string {
		auto& info = m_Levels[ELogLevel::trace];
		auto loc   = std::format("{}:({})", file, line);
		return std::format("{} [{}] {}|\x1b[0m {:<" __LOC_FN_SEP_WIDTH__ "} @ {}",
		                   format_header(ELogLevel::trace),
		                   format_depth(LogTraceDepth::value),
		                   info.color, loc, format_fn(fn));
	}

	auto Logger::trace(const fs::path& file, LogLine line, const std::string& fn) -> void {
		if (!bTrace) return;
		auto& info = m_Levels[ELogLevel::trace];
		auto msg   = strace(file, line, fn);
		m_Queue.push(LogRecord{
		    .stream = info.stream,
		    .lvl    = ELogLevel::trace,
		    .msg    = msg });
	}

} // namespace aby::eng

namespace aby::eng {

	LogTimer::LogTimer(std::string name, ...) :
	    name(std::move(name)),
	    start(enabled ? std::chrono::high_resolution_clock::now() : std::chrono::high_resolution_clock::time_point()) {
	}

	LogTimer::~LogTimer() {
		if (enabled) {
			auto end = std::chrono::high_resolution_clock::now();
			auto us  = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			Logger::get(ELogger::client)->time(this->name, us, ms);
		}
	}

} // namespace aby::eng

namespace aby::eng {

	LogSink::LogSink(Callback&& fn, ELogLevel level) :
	    m_Fn(fn),
	    m_Level(level) {
	}

	auto LogSink::operator()(const LogRecord& record) -> void {
		m_Fn(record);
	}

	LogSink::operator bool() const {
		return m_Fn != nullptr;
	}

	auto LogSink::level() const -> ELogLevel {
		return m_Level;
	}

	auto LogSink::id() const -> size_t {
		return m_ID;
	}

	auto LogSink::set_level(ELogLevel level) -> void {
		m_Level = level;
	}

	auto LogSink::set_id(size_t id) -> void {
		m_ID = id;
	}

} // namespace aby::eng
