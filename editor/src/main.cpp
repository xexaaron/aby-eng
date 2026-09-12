#include "entry_point.hpp"

int main(int argc, char** argv) {
	using namespace aby::eng::editor;
	return EntryPoint::set<EntryPoint>(argc, argv);
}
