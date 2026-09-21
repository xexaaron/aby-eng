#include "editor_common.hpp"

namespace aby::eng::editor {

	auto operator""_q(const char* s, std::size_t n) -> QString {
		return QString::fromUtf8(s, qsizetype(n));
	}

} // namespace aby::eng::editor
