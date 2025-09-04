#include "Renderer.h"

/*
 * hàm xóa màn hình
 * đổi tất cả các ô trong screenBuffer về màu nền mặc định
 */
void Renderer::clearScreen()
{
	for (int y = 0; y < BOARD_HEIGHT; ++y)
		for (int x = 0; x < BOARD_WIDTH; ++x)
			screenBuffer[y][x] = DEFAULT_BG_COLOR;
}

/*
 * hàm vẽ một khối tại vị trí cụ thể với màu cụ thể
 *
 * nếu vị trí y là số lẻ thì vẽ nửa trên của khối ở vị trí (x, y-1)
 * vẽ nửa dưới của khối ở vị trí (x, y)
 */
void Renderer::drawBlock(const Point& position, CELL_TYPE type)
{
	// Vẽ một khối tại vị trí cụ thể
	Color color;
	switch (type)
	{
	case CELL_TYPE::WALL:
		color = DEFAULT_WALL_COLOR;
		break;
	case CELL_TYPE::SNAKE:
		color = DEFAULT_SNAKE_COLOR;
		break;
	case CELL_TYPE::SNAKE_HEAD:
		color = DEFAULT_SNAKE_HEAD_COLOR;
		break;
	case CELL_TYPE::POINT:
		color = DEFAULT_FOOD_COLOR;
		break;
	case CELL_TYPE::BONUS:
		color = DEFAULT_BONUS_COLOR;
		break;
	default:
		color = DEFAULT_BG_COLOR;
		break;
	}
	if (position.y % 2 != 0) screenBuffer[position.y - 1][position.x].character = color.character;
	screenBuffer[position.y][position.x] = color;
}


/*
 * hàm vẽ một chuỗi ký tự tại vị trí cụ thể với màu cụ thể
 *
 * nếu vị trí vượt quá kích thước bảng thì dừng vẽ
 */
void Renderer::drawText(const std::string& text, const Point& position, const Color& color)
{
	// In văn bản tại vị trí cụ thể
	for (int i = 0; i < text.length(); ++i)
	{
		if (position.x + i >= BOARD_WIDTH) return;
		screenBuffer[position.y + 1][position.x + i] = color;
		screenBuffer[position.y][position.x + i].character = text.substr(i, 1);
		screenBuffer[position.y + 1][position.x + i].character = text.substr(i, 1);
	}
}

/*
 * hàm vẽ một ký tự tại vị trí cụ thể với màu cụ thể
 *
 * nếu vị trí vượt quá kích thước bảng thì dừng vẽ
 */
void Renderer::drawChar(const std::string& text, const Point& position, const Color& color)
{
	if (position.x >= BOARD_WIDTH) return;
	screenBuffer[position.y + 1][position.x] = color;
	screenBuffer[position.y][position.x].character = text;
	screenBuffer[position.y + 1][position.x].character = text;
}

/*
 * hàm điền hình nền menu vào screenBuffer
 *
 * sử dụng mảng 2D bg_menu_r, bg_menu_g, bg_menu_b để lấy giá trị màu tương ứng
 */
void Renderer::fillMenu()
{
	// Vẽ menu
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			screenBuffer[y][x].r = bg_menu_r[y][x];
			screenBuffer[y][x].g = bg_menu_g[y][x];
			screenBuffer[y][x].b = bg_menu_b[y][x];
		}
	}
}

/*
 * hàm hiển thị nội dung trong screen buffer lên màn hình console
 *
 */
void Renderer::present()
{
	// Cập nhật hiển thị
	std::stringstream frame_buffer;
	// Di chuyển con trỏ về góc trên bên trái
	frame_buffer << HOME;
	for (int row = 0; row < BOARD_HEIGHT; row += 2)
	{
		for (int col = 0; col < BOARD_WIDTH; col++)
		{
			int next_row = row + 1;
			Point first_point = { col, row };	// nửa trên
			Point last_point = { col, row + 1 };// nửa dưới
			// thay đổi màu nền nửa trên với background (nửa trên trống)
			frame_buffer << BG;
			frame_buffer << std::to_string(screenBuffer[first_point.y][first_point.x].r) << ";";
			frame_buffer << std::to_string(screenBuffer[first_point.y][first_point.x].g) << ";";
			frame_buffer << std::to_string(screenBuffer[first_point.y][first_point.x].b) << "m";
			// thay đổi màu nền nửa dưới với foreground (nửa dưới chứa ký tự)
			frame_buffer << FG;
			frame_buffer << std::to_string(screenBuffer[last_point.y][last_point.x].r) << ";";
			frame_buffer << std::to_string(screenBuffer[last_point.y][last_point.x].g) << ";";
			frame_buffer << std::to_string(screenBuffer[last_point.y][last_point.x].b) << "m";
			frame_buffer << screenBuffer[first_point.y][first_point.x].character;
		}
		// khôi phục màu nền và xuống dòng
		frame_buffer << RESET_COLOR;
	}
	// In toàn bộ frame_buffer
	std::cout << frame_buffer.str();
	std::cout.flush();
}