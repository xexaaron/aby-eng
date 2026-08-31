#pragma once
#include "ui/containers/container.hpp"

namespace aby::eng::ui {

	class ABY_API Grid : public Container {
	public:
		Grid(Transform2D transform, size_t cols, size_t rows,
		     float spacing = 0.f, float padding = 0.f,
		     const Border& border = Border(),
		     EStretch stretch     = EStretch::fill);

		static auto create(Transform2D transform, size_t cols, size_t rows,
		                   float spacing = 0.f, float padding = 0.f,
		                   const Border& border = Border(),
		                   EStretch stretch     = EStretch::fill) -> ref<Grid>;

		auto on_tick(const Time& deltatime) -> void override;
		auto on_render() -> void override;

		auto add_child(ref<Element> element, size_t col, size_t row) -> void;

		auto remove_child(size_t col, size_t row) -> void;
		auto fill(std::function<ref<Element>()> factory) -> void;

		auto add_row() -> void;
		auto add_column() -> void;

		auto set_columns(size_t cols) -> void;
		auto set_rows(size_t rows) -> void;
		auto set_dimensions(size_t cols, size_t rows) -> void;

		auto columns() const -> size_t;
		auto rows() const -> size_t;
	protected:
		auto grid_idx(size_t col, size_t row) const -> size_t;
		auto resize_grid(size_t cols, size_t rows) -> void;
	protected:
		size_t m_Columns;
		size_t m_Rows;
	};

} // namespace aby::eng::ui
