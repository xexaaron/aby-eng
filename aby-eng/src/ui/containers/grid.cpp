#include "ui/containers/grid.hpp"

#include "core/renderer.hpp"
#include "log.hpp"

namespace aby::eng::ui {

	Grid::Grid(Transform2D transform, size_t cols, size_t rows, float spacing, float padding, const Border& border, EStretch stretch) :
	    Container(transform, ELayout::horizontal, EDirection::forward, spacing, padding, border, stretch),
	    m_Columns(cols),
	    m_Rows(rows) {
		m_Children.resize(rows * cols, std::make_shared<Element>());
	}

	auto Grid::create(Transform2D transform, size_t cols, size_t rows, float spacing, float padding, const Border& border, EStretch stretch) -> ref<Grid> {
		return std::make_shared<Grid>(transform, cols, rows, spacing, padding, border, stretch);
	}

	auto Grid::on_tick(const Time& deltatime) -> void {
		const Rect2D rect = available_rect();

		if (m_Columns == 0 || m_Rows == 0) {
			Element::on_tick(deltatime);
			return;
		}

		const float cell_width  = (rect.size.x - m_Spacing * static_cast<float>(m_Columns - 1)) / static_cast<float>(m_Columns);
		const float cell_height = (rect.size.y - m_Spacing * static_cast<float>(m_Rows - 1)) / static_cast<float>(m_Rows);

		for (size_t row = 0; row < m_Rows; ++row) {
			for (size_t col = 0; col < m_Columns; ++col) {
				auto& child = m_Children[grid_idx(col, row)];

				if (!child)
					continue;

				const glm::fvec2 cell_pos = {
					rect.pos.x + col * (cell_width + m_Spacing),
					rect.pos.y + row * (cell_height + m_Spacing)
				};

				const Rect2D cell_rect(cell_pos, { cell_width, cell_height });

				child->set_resolved_layout(resolve_child_layout(*child, cell_rect));
			}
		}

		Element::on_tick(deltatime);
	}

	auto Grid::on_render() -> void {
		const auto rect = available_rect();

		if (m_Columns == 0 || m_Rows == 0) {
			Container::on_render();
			return;
		}

		// Render outer border.
		Container::on_render();

		if (m_Border.top <= 0.f &&
		    m_Border.right <= 0.f &&
		    m_Border.bottom <= 0.f &&
		    m_Border.left <= 0.f) {
			return;
		}

		const float cell_width  = (rect.size.x - m_Spacing * static_cast<float>(m_Columns - 1)) / static_cast<float>(m_Columns);
		const float cell_height = (rect.size.y - m_Spacing * static_cast<float>(m_Rows - 1)) / static_cast<float>(m_Rows);

		Material2D mat(m_Border.color);

		// Vertical grid lines.
		for (size_t col = 1; col < m_Columns; ++col) {
			const float x =
			    rect.pos.x +
			    static_cast<float>(col) * cell_width +
			    static_cast<float>(col - 1) * m_Spacing +
			    m_Spacing * 0.5f;

			Transform2D transform(
			    { x - m_Border.left * 0.5f, rect.pos.y },
			    { m_Border.left, rect.size.y },
			    1.f);

			Renderer2D::quad(transform, mat);
		}

		// Horizontal grid lines.
		for (size_t row = 1; row < m_Rows; ++row) {
			const float y =
			    rect.pos.y +
			    static_cast<float>(row) * cell_height +
			    static_cast<float>(row - 1) * m_Spacing +
			    m_Spacing * 0.5f;

			Transform2D transform(
			    { rect.pos.x, y - m_Border.top * 0.5f },
			    { rect.size.x, m_Border.top },
			    1.f);

			Renderer2D::quad(transform, mat);
		}
	}

	auto Grid::add_child(ref<Element> element, size_t col, size_t row) -> void {
		expect(col < m_Columns, "grid column out of bounds");
		expect(row < m_Rows, "grid row out of bounds");

		m_Children[grid_idx(col, row)] = std::move(element);
	}

	auto Grid::remove_child(size_t col, size_t row) -> void {
		expect(col < m_Columns, "grid column out of bounds");
		expect(row < m_Rows, "grid row out of bounds");

		m_Children[grid_idx(col, row)].reset();
	}

	auto Grid::add_row() -> void {
		resize_grid(m_Columns, m_Rows + 1);
	}

	auto Grid::add_column() -> void {
		resize_grid(m_Columns + 1, m_Rows);
	}

	auto Grid::set_columns(size_t cols) -> void {
		resize_grid(cols, m_Rows);
	}

	auto Grid::set_rows(size_t rows) -> void {
		resize_grid(m_Columns, rows);
	}

	auto Grid::set_dimensions(size_t cols, size_t rows) -> void {
		resize_grid(cols, rows);
	}

	auto Grid::grid_idx(size_t col, size_t row) const -> size_t {
		return row * m_Columns + col;
	}

	auto Grid::resize_grid(size_t cols, size_t rows) -> void {
		std::vector<ref<Element>> children(cols * rows);

		const size_t copy_cols = std::min(m_Columns, cols);
		const size_t copy_rows = std::min(m_Rows, rows);

		for (size_t row = 0; row < copy_rows; ++row) {
			for (size_t col = 0; col < copy_cols; ++col) {
				const size_t old_idx = grid_idx(col, row);
				const size_t new_idx = row * cols + col;

				children[new_idx] = std::move(m_Children[old_idx]);
			}
		}

		for (auto& child : children) {
			if (!child) {
				child = std::make_shared<Element>();
			}
		}

		m_Children = std::move(children);
		m_Columns  = cols;
		m_Rows     = rows;
	}

	auto Grid::columns() const -> size_t {
		return m_Columns;
	}

	auto Grid::rows() const -> size_t {
		return m_Rows;
	}

} // namespace aby::eng::ui
