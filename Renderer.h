#pragma once
#include "GlobalVariable.h"

// Lớp Renderer: Chịu trách nhiệm hiển thị tất cả các đối tượng
class Renderer
{
public:
	Renderer()
	{
		// Khởi tạo kích thước screenBuffer
		screenBuffer.resize(BOARD_HEIGHT, std::vector<Color>(BOARD_WIDTH, DEFAULT_BG_COLOR));
		// Ngắt đồng bộ hóa giữa C và C++ standard streams để tăng tốc độ xuất
		std::ios_base::sync_with_stdio(false);
		std::cout.tie(nullptr);
		// Thiết lập console để hỗ trợ UTF-8 và ANSI escape codes
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut != INVALID_HANDLE_VALUE)
		{
			DWORD dwMode = 0;
			if (GetConsoleMode(hOut, &dwMode))
			{
				dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				SetConsoleMode(hOut, dwMode); // Bật ANSI escape codes
				SetConsoleOutputCP(CP_UTF8); // Thiết lập mã hóa UTF-8
			}
		}
		std::cout << HIDE_CURSOR;	// Ẩn con trỏ
		std::cout.flush();	// Đảm bảo tất cả các thay đổi được áp dụng ngay lập tức
	}

	// Hàm hủy mặc định để giải phóng bộ nhớ (nếu cần)
	~Renderer() = default;

	// hàm xóa màn hình
	void clearScreen();

	// hàm vẽ một khối tại vị trí với loại khối cụ thể
	void drawBlock(const Point& position, CELL_TYPE type);
	// hàm vẽ một ký tự tại vị trí với màu cụ thể
	void drawChar(const std::string& text, const Point& position, const Color& color);
	// hàm vẽ một chuỗi ký tự tại vị trí với màu cụ thể
	void drawText(const std::string& text, const Point& position, const Color& color);

	// hàm hiển thị những thông tin đã xử lý ra màn hình
	void present();

	// hàm điền nền cho menu
	void fillMenu();

private:
	// bảng lưu màu sắc và ký tự của từng ô trong bảng
	// sử dụng ký tự "▄" đại diện cho 2 ô (nửa trên và nửa dưới) để tăng độ phân giải
	// vì ô trên trống nên màu ô trên sẽ là màu nền, màu ô dưới sẽ là màu ký tự
	std::vector<std::vector<Color>> screenBuffer;

};