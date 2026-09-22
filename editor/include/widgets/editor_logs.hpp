#pragma once

#include "widgets/editor_console.hpp"

#include <fstream>
#include <mutex>

#ifdef emit
#	define __qt_emit_stored__ emit
#	undef emit
#	include <aby-eng/log.hpp>
#else
#	include <aby-eng/log.hpp>
#endif

namespace aby::eng::editor {

	class Logs : public Console {
		Q_OBJECT
	public:
		Logs(const std::filesystem::path& rel_path_log_file, QWidget* parent = nullptr);
		~Logs();
	private:
		LogSink m_Sink;
		std::ofstream m_Stream;
		std::mutex m_StreamMutex;
	};

} // namespace aby::eng::editor

#ifdef __qt_emit_stored__
#	define emit __qt_emit_stored__
#	undef __qt_emit_stored__
#endif

