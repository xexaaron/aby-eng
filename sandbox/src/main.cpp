#include "entry_point.hpp"

int main(int argc, char** argv) {
	using namespace aby::eng::sandbox;
	return EntryPoint::set<EntryPoint>(argc, argv);
}
