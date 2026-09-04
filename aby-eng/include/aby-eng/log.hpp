#pragma once
#include "common.hpp"

#include <aby-rhi/interfaces/default_job_system.hpp>
#include <chrono>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <print>
#include <thread>
#include <unordered_map>

#define __LOC_FN_SEP_WIDTH__ "70"
#define __TIMESTAMP_FORMAT__ "\033[1;30m%H:%M:%S\033[0m"

#define client_inf(fmt, ...) ::aby::eng::Logger::get(::aby::eng::ELogger::client)->log(::aby::eng::ELogLevel::log, fmt __VA_OPT__(, ) __VA_ARGS__);
#define client_wrn(fmt, ...) ::aby::eng::Logger::get(::aby::eng::ELogger::client)->log(::aby::eng::ELogLevel::warn, fmt __VA_OPT__(, ) __VA_ARGS__);
#define client_err(fmt, ...) ::aby::eng::Logger::get(::aby::eng::ELogger::client)->log(::aby::eng::ELogLevel::err, fmt __VA_OPT__(, ) __VA_ARGS__);

#ifndef _NDEBUG
#	define mconcat_impl(x, y) x##y
#	define mconcat(x, y) mconcat_impl(x, y)
#	define TIME_SCOPE(...) ::aby::eng::LogTimer mconcat(__scoped_timer__, __LINE__)(__VA_ARGS__ __VA_OPT__(, ) __FUNCTION__)
#	define TRACE_SCOPE()                                           \
		::aby::eng::LogTraceDepth mconcat(__trc_depth__, __LINE__); \
		::aby::eng::Logger::get(::aby::eng::ELogger::internal)->trace(::aby::eng::fs::path(__FILE__), ::aby::eng::LogLine{ __LINE__ }, __FUNCTION__)
#	define log_trc(...) ::aby::eng::Logger::get(::aby::eng::ELogger::internal)->trace(__FILE__, ::aby::eng::LogLine{ __LINE__ }, __FUNCTION__ __VA_OPT__(, ) __VA_ARGS__)
#	define log_inf(fmt, ...) ::aby::eng::Logger::get(::aby::eng::ELogger::internal)->log(::aby::eng::ELogLevel::log, fmt __VA_OPT__(, ) __VA_ARGS__)
#	define log_wrn(fmt, ...) ::aby::eng::Logger::get(::aby::eng::ELogger::internal)->log(::aby::eng::ELogLevel::warn, fmt __VA_OPT__(, ) __VA_ARGS__)
#	define log_err(fmt, ...) ::aby::eng::Logger::get(::aby::eng::ELogger::internal)->log(::aby::eng::ELogLevel::err, fmt __VA_OPT__(, ) __VA_ARGS__)
#	define log_dev(fmt, ...) ::aby::eng::Logger::get(::aby::eng::ELogger::internal)->log(::aby::eng::ELogLevel::dev, fmt __VA_OPT__(, ) __VA_ARGS__)
#	define log_tdo(fmt, ...) ::aby::eng::Logger::get(::aby::eng::ELogger::internal)->todo(::aby::eng::fs::path(__FILE__), ::aby::eng::LogLine{ __LINE__ }, fmt __VA_OPT__(, ) __VA_ARGS__)
#	define log_ast(expr, fmt, ...)                                                                                                                                                                 \
		do {                                                                                                                                                                                        \
			::aby::eng::Logger::get(::aby::eng::ELogger::internal)->log(::aby::eng::ELogLevel::ast, "{}:({}) @ {}", ::aby::eng::fs::path(__FILE__), ::aby::eng::LogLine{ __LINE__ }, __FUNCTION__); \
			::aby::eng::Logger::get(::aby::eng::ELogger::internal)->log(::aby::eng::ELogLevel::ast, "Trace Depth: {}", ::aby::eng::Logger::format_depth(::aby::eng::LogTraceDepth::value));         \
			::aby::eng::Logger::get(::aby::eng::ELogger::internal)->log(::aby::eng::ELogLevel::ast, "Expression: {}", #expr);                                                                       \
			::aby::eng::Logger::get(::aby::eng::ELogger::internal)->log(::aby::eng::ELogLevel::ast, fmt __VA_OPT__(, ) __VA_ARGS__);                                                                \
		} while (0)
#	define expect(expr, fmt, ...)                             \
		do {                                                   \
			if (!(expr)) {                                     \
				log_ast(expr, fmt __VA_OPT__(, ) __VA_ARGS__); \
				ABY_ENG_DEBUG_BREAK();                         \
			}                                                  \
		} while (0)
#else
#	define TIME_SCOPE(...)
#	define TRACE_SCOPE() (void)0
#	define log_trc(...)
#	define log_tdo(...)
#	define log_inf(...)
#	define log_err(...)
#	define log_wrn(...)
#	define log_ast(...)
#	define log_dev(...)
#	define expect(...)
#endif

namespace aby::eng {

	namespace fs = std::filesystem;

	enum class ELogger {
		internal,
		client,
	};

	enum class ELogLevel {
		todo  = -2, // Athough not affected by log level, we still support prefix, color, & stream settings
		trace = -1, // Athough not affected by log level, we still support prefix, color, & stream settings
		log   = 0,
		warn  = 1,
		err   = 2,
		ast   = 3,
		dev   = 4,
	};

	struct LogLocation {
		uint64_t line;
		uint64_t col;
	};

	struct LogLine {
		uint64_t value;
	};

	struct LogTraceDepth {
		LogTraceDepth() {
			value++;
		}
		~LogTraceDepth() {
			value--;
		}

		thread_local static inline size_t value = 0;
	};

	struct LogTimer {
		LogTimer(std::string name, ...); // Variadic to allow the macro to default param as __FILE__
		~LogTimer();

		std::string name;
		std::chrono::high_resolution_clock::time_point start;
	private:
		friend class Logger;
		static inline bool enabled = false;
	};

	struct LogInfo {
		std::string color    = "";
		std::string prefix   = "";
		std::ostream* stream = &std::cout;
	};

	struct LogRecord {
		std::ostream* stream;
		ELogLevel lvl;
		std::string msg;
	};

	class ABY_API LogSink {
	public:
		using Callback = std::function<void(const LogRecord&)>;

		LogSink(Callback&& fn, ELogLevel level = ELogLevel::err);

		auto level() const -> ELogLevel;
		auto id() const -> size_t;
		auto set_level(ELogLevel level) -> void;

		auto operator()(const LogRecord& record) -> void;
		explicit operator bool() const;
	private:
		auto set_id(size_t id) -> void;
		friend class Logger;
	private:
		Callback m_Fn;
		ELogLevel m_Level;
		size_t m_ID;
	};

	class ABY_API Logger {
	private:
		template <typename K, typename V>
		using MapTy = std::unordered_map<K, V>;
		template <typename T>
		using QueueTy = rhi::ConcurrentQueue<T>;
		template <typename T>
		using ContainerTy = std::vector<T>;
	public:
		static auto get(ELogger logger) -> std::shared_ptr<Logger>;
		static auto run() -> void;
		static auto shutdown() -> void;

		auto get_timestamp() -> std::string;
		auto add_sink(LogSink log_sink) -> size_t;
		auto remove_sink(size_t id) -> void;

		static auto format_fn(std::string_view fn) -> std::string;
		static auto format_type(std::string_view type) -> std::string;
		static auto format_fn(const std::string& fn) -> std::string;
		static auto format_type(const std::string& type) -> std::string;
		static auto format_depth(size_t depth) -> std::string;
		auto format_header(ELogLevel level) -> std::string;

		template <typename... Args>
		auto log(ELogLevel lvl, std::format_string<Args...> fmt, Args&&... args) -> void;
		template <typename... Args>
		auto slog(ELogLevel lvl, std::format_string<Args...> fmt, Args&&... args) -> std::string;

		template <typename... Args>
		auto strace(const fs::path& file, LogLine line, const std::string& fn, std::format_string<Args...> fmt, Args&&... args) -> std::string;
		auto strace(const fs::path& file, LogLine line, const std::string& fn) -> std::string;
		template <typename... Args>
		auto trace(const fs::path& file, LogLine line, const std::string& fn, std::format_string<Args...> fmt, Args&&... args) -> void;
		auto trace(const fs::path& file, LogLine line, const std::string& fn) -> void;

		template <typename... Args>
		auto todo(const fs::path& file, LogLine line, std::format_string<Args...> fmt, Args&&... args) -> void;
		template <typename... Args>
		auto stodo(const fs::path& file, LogLine line, std::format_string<Args...> fmt, Args&&... args) -> std::string;

		auto time(const std::string& name, std::chrono::microseconds us, std::chrono::milliseconds ms) -> void;
		auto stime(const std::string& name, std::chrono::microseconds us, std::chrono::milliseconds ms) -> std::string;

		auto set_trace(bool enabled) -> Logger*;
		auto set_todo(bool enabled) -> Logger*;
		auto set_time(bool enabled) -> Logger*;
		auto set_level(ELogLevel lvl) -> Logger*;
		auto set_timestamp(bool enabled, std::string_view format = __TIMESTAMP_FORMAT__) -> Logger*;
		auto set_level_info(ELogLevel lvl, const LogInfo& info) -> Logger*;
		auto set_color(ELogLevel level, std::string_view color) -> Logger*;
		auto set_prefix(ELogLevel level, std::string_view prefix) -> Logger*;
		auto set_stream(ELogLevel level, std::ostream* stream) -> Logger*;
	private:
		auto run_sinks(const LogRecord& record) -> void;
	private:
		bool bTimestamp                    = false;
		bool bTrace                        = false;
		bool bTodo                         = false;
		bool bTime                         = false;
		std::string m_TimestampFormat      = __TIMESTAMP_FORMAT__;
		ELogLevel m_Level                  = ELogLevel::dev;
		MapTy<ELogLevel, LogInfo> m_Levels = {};
		ContainerTy<LogSink> m_Sinks       = {};
		size_t m_SinkIDPool                = 0;
		std::atomic<bool> bRunning         = { true };
		QueueTy<LogRecord> m_Queue;
	private:
		static MapTy<ELogger, std::shared_ptr<Logger>> s_Loggers;
		static std::thread s_Thread;
		static inline bool s_Running = false;
	};

} // namespace aby::eng

namespace std {

	template <>
	struct formatter<filesystem::path, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const filesystem::path& path, FmtContext& ctx) const {
			string str = "\033[34m\033]8;;\033\\";
			str.append(path.string());
			str.append("\033]8;;\033\\\033[0m");
			return ranges::copy(str, ctx.out()).out;
		}
	};

	template <>
	struct formatter<aby::eng::LogLocation, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::LogLocation& loc, FmtContext& ctx) const {
			return format_to(ctx.out(), "\033[38;2;181;206;168m{}\x1b[0m,\033[38;2;181;206;168m{}\x1b[0m", loc.line, loc.col);
		}
	};

	template <>
	struct formatter<aby::eng::LogLine, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::LogLine& line, FmtContext& ctx) const {
			return format_to(ctx.out(), "\033[38;2;181;206;168m{}\x1b[0m", line.value);
		}
	};

} // namespace std

namespace aby::eng {

	template <typename... Args>
	auto Logger::slog(ELogLevel lvl, std::format_string<Args...> fmt, Args&&... args) -> std::string {
		expect(lvl >= ELogLevel::log, "Use appropraite logger function instead. (trace|todo)");
		auto& info = m_Levels[lvl];
		return std::format("{} {}", format_header(lvl), std::format(fmt, std::forward<Args>(args)...));
	}

	template <typename... Args>
	auto Logger::strace(const fs::path& file, LogLine line, const std::string& fn, std::format_string<Args...> fmt, Args&&... args) -> std::string {
		auto& info = m_Levels[ELogLevel::trace];
		auto loc   = std::format("{}:({})", file, line);
		return std::format("{} [{}] {}|\x1b[0m {:<" __LOC_FN_SEP_WIDTH__ "} @ {} | {}",
		                   format_header(ELogLevel::trace), format_depth(LogTraceDepth::value),
		                   info.color, loc, format_fn(fn),
		                   std::format(fmt, std::forward<Args>(args)...));
	}

	template <typename... Args>
	auto Logger::stodo(const fs::path& file, LogLine line, std::format_string<Args...> fmt, Args&&... args) -> std::string {
		auto& info = m_Levels[ELogLevel::todo];
		return std::format("{} {}:({}): {}",
		                   format_header(ELogLevel::todo),
		                   fs::relative(file), line,
		                   std::format(fmt, std::forward<Args>(args)...));
	}

	template <typename... Args>
	auto Logger::log(ELogLevel lvl, std::format_string<Args...> fmt, Args&&... args) -> void {
		expect(lvl >= ELogLevel::log, "Use appropraite logger function instead. (trace|todo)");
		if (lvl > m_Level) return;
		auto& info = m_Levels[lvl];
		auto msg   = slog(lvl, fmt, std::forward<Args>(args)...);
		LogRecord record{
			.stream = info.stream,
			.lvl    = lvl,
			.msg    = msg
		};

		if (lvl == ELogLevel::ast) {
			for (auto& sink : m_Sinks) {
				sink(record);
			}
			return;
		}

		m_Queue.push(record);
	}

	template <typename... Args>
	auto Logger::trace(const fs::path& file, LogLine line, const std::string& fn, std::format_string<Args...> fmt, Args&&... args) -> void {
		if (!bTrace) return;
		auto& info = m_Levels[ELogLevel::trace];
		auto msg   = strace(file, line, fn, fmt, std::forward<Args>(args)...);
		m_Queue.push(LogRecord{
		    .stream = info.stream,
		    .lvl    = ELogLevel::trace,
		    .msg    = msg });
	}

	template <typename... Args>
	auto Logger::todo(const fs::path& file, LogLine line, std::format_string<Args...> fmt, Args&&... args) -> void {
		if (!bTodo) return;
		auto& info = m_Levels[ELogLevel::todo];
		auto msg   = stodo(file, line, fmt, std::forward<Args>(args)...);
		m_Queue.push(LogRecord{
		    .stream = info.stream,
		    .lvl    = ELogLevel::todo,
		    .msg    = msg });
	}

} // namespace aby::eng
