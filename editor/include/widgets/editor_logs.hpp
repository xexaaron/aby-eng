#pragma once

#include "misc/qt_header_guard_start.inl"
#include "widgets/editor_console.hpp"

#include <fstream>
#include <mutex>

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

#include "misc/qt_header_guard_end.inl"

