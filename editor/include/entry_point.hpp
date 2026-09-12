#include "argparse/argparse.hpp"

#include <aby-eng/core/app.hpp>
#include <aby-eng/core/entry.hpp>

namespace aby::eng::editor {

	class EntryPoint final : public eng::EntryPoint {
	public:
		EntryPoint(int argc, char** argv);

		auto on_cmdl(argparse::ArgumentParser& parser) -> void override;
		auto on_exec() -> void override;
		auto on_exit() -> void override;
	private:
		AppInfo m_AppInfo;
	};

} // namespace aby::eng::editor
