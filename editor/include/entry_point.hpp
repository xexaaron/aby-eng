#include "argparse/argparse.hpp"
#include "editor_app.hpp"
#include "editor_window.hpp"
#include "window.hpp"

#include <QApplication>
#include <aby-eng/core/app.hpp>
#include <aby-eng/core/entry.hpp>
#include <aby-win/aby-win.hpp>

namespace aby::eng::editor {

	class EntryPoint final : public eng::EntryPoint {
	public:
		EntryPoint(int argc, char** argv);

		auto on_cmdl(argparse::ArgumentParser& parser) -> void override;
		auto on_exec() -> void override;
		auto on_create() -> void override;
		auto on_exit() -> void override;
	private:
		ref<Application> m_App;
	};

} // namespace aby::eng::editor
