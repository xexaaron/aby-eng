#include "widgets/editor_logs.hpp"

#include "log.hpp"

#include <aby-rhi/aby-rhi.hpp>
#include <qnamespace.h>

namespace aby::eng::editor::detail {

	void qt_message_handler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

}

namespace aby::eng::editor {

	Logs::Logs(const std::filesystem::path& rel_path_log_file, QWidget* parent) :
	    Console(parent),
	    m_Sink([this](const LogRecord& record) {
		    std::lock_guard lock(m_StreamMutex);
		    m_Stream << record.msg << "\n";
		    m_Stream.flush();
	    }, ELogLevel::err),
	    m_Stream(rel_path_log_file, std::ios::trunc | std::ios::out) {
		auto* io = rhi::Context::get().file_io();
		auto cwd = io->cwd();

		qInstallMessageHandler(detail::qt_message_handler);

		const auto command = std::format("clear; tail -f {}", rel_path_log_file.string());

		setWindowTitle("Logs");

		term()->setShellProgram("/bin/bash -c "
		                        "\""
		                        "alias ls='eza -al --color=always --group-directories-first'; "
		                        "\"");
		term()->setWorkingDirectory(QString::fromStdString(cwd.string()));
		term()->setWindowTitle("Logs");
		term()->sendText(QString::fromStdString(command));
		term()->sendKeyEvent(new QKeyEvent(
		    QEvent::KeyPress,
		    Qt::Key_Enter,
		    Qt::NoModifier));
		term()->sendKeyEvent(new QKeyEvent(
		    QEvent::KeyRelease,
		    Qt::Key_Enter,
		    Qt::NoModifier));

		term()->setEnabled(false);
		term()->setKeyboardCursorShape(QTermWidget::KeyboardCursorShape::UnderlineCursor);
		Logger::get(ELogger::client)
		    ->add_sink(m_Sink);
#ifndef _NDEBUG
		Logger::get(ELogger::internal)->add_sink(m_Sink);
#endif
	}

	Logs::~Logs() {
		std::lock_guard lock(m_StreamMutex);
		m_Stream.close();
	}

} // namespace aby::eng::editor

namespace aby::eng::editor::detail {

	void qt_message_handler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
		switch (type) {
			case QtDebugMsg:
				log_dev("[qt] {}", msg.toStdString());
				break;
			case QtInfoMsg:
				log_inf("[qt] {}", msg.toStdString());
				break;
			case QtWarningMsg:
				log_wrn("[qt] {}", msg.toStdString());
				break;
			case QtCriticalMsg:
				log_err("[qt] {}", msg.toStdString());
				break;
			case QtFatalMsg:
				Logger::get(ELogger::internal)->log(ELogLevel::ast, "[qt] {}:({}) @ {}", context.file, context.line, context.function);
				Logger::get(ELogger::internal)->log(ELogLevel::ast, "[qt] {}", msg.toStdString());
				break;
		}
	}

} // namespace aby::eng::editor::detail
