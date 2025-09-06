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

void Renderer::drawBlock(const Point& position, CELL_TYPE type)
{
	return;
}

void Renderer::drawText(const std::string& text, const Point& position, const Color& color)
{
	return;
}

void Renderer::drawChar(const std::string& text, const Point& position, const Color& color)
{
	return;
}

void Renderer::fillMenu()
{
	return;
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