#include <aby-eng/core/app.hpp>
#include <aby-eng/core/entry.hpp>

class EntryPoint final : public aby::eng::EntryPoint {
public:
	EntryPoint(int argc, char** argv) :
	    m_Argc(argc),
	    m_Argv(argv) {
	}

	auto init() -> aby::eng::AppInfo {
		return aby::eng::AppInfo{
			.name = "aby-eng-sandbox",
			.argc = m_Argc,
			.argv = m_Argv
		};
	}

	auto on_exec(aby::eng::App& app) -> void {
	}

	auto on_exit() -> void {
	}
private:
	int m_Argc;
	char** m_Argv;
};

int main(int argc, char** argv) {
	EntryPoint::set<EntryPoint>(argc, argv);
	return 0;
}
