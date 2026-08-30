#include "ui/uitypes.hpp"

namespace aby::eng::ui {

	Border::Border(float scalar, glm::fvec4 color) :
	    top(scalar),
	    right(scalar),
	    bottom(scalar),
	    left(scalar),
	    color(color) {
	}

	Border::Border(float top, float right, float bottom, float left, glm::fvec4 color) :
	    top(top),
	    right(right),
	    bottom(bottom),
	    left(left),
	    color(color) {
	}

	auto Border::set_scalar(float scalar) -> void {
		this->top    = scalar;
		this->right  = scalar;
		this->bottom = scalar;
		this->left   = scalar;
	}

} // namespace aby::eng::ui