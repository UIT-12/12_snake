#include "Food.h"

/*
 * Hàm giúp xuất hiện thức ăn một cách ngẫu nhiên tại nhiều vị trí khác nhau trên bản đồ
 */
void Food::spawnFood(const std::vector<std::vector<CELL_TYPE>>& grid)
{
	// Thiết kế bộ sinh số ngẫu nhiên
	std::random_device rd;
	std::mt19937 gen(rd());
	// Khởi tạo một danh sách các ô trống
	std::deque<Point> remain;
	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[0].size(); ++x)
		{
			if (grid[y][x] == CELL_TYPE::NONE)
				remain.push_back({ x, y });
		}
	}
	// Lựa chọn ngẫu nhiên một ô còn trống.
	std::uniform_int_distribution<> dis(0, remain.size() - 1);
	// Đặt thức ăn vào một vị trí trên bản đồ
	foodPosition = remain[dis(gen)];
}

/*
 * Hàm tạo điểm cộng tại các vị trí ngẫu nhiên trên bản đồ
 * Điểm cộng chỉ xuất hiện trong một khoảng thời gian nhất định
 */
void Food::spawnBonus(const std::vector<std::vector<CELL_TYPE>>& grid)
{
	// Tạo ra thiết bị sinh số ngẫu nhiên
	std::random_device rd;
	std::mt19937 gen(rd());
	// Tạo ra danh sách các ô trống
	std::deque<Point> remain;
	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[0].size(); ++x)
		{
			if (grid[y][x] == CELL_TYPE::NONE)
				remain.push_back({ x, y });
		}
	}
	// Chọn ra ngẫu nhiên một ô trống
	std::uniform_int_distribution<> dis(0, remain.size() - 1);
	// Đặt vị trí điểm cộng
	bonusPosition = remain[dis(gen)];
	// Khởi động bộ đếm thời gian tồn tại của điểm cộng
	bonusTimeRemaining = bonusDurationMs;
	// Đánh dấu điểm cộng đang hoạt động
	bonusActive = true;
}

/*
 * Hàm cập nhật thời gian tồn tại của điểm cộng
 * Nếu hết thời gian thì hủy điểm cộng
 * Trả về vị trí điểm cộng nếu hết thời gian, ngược lại trả về {-1,-1}
 */
Point Food::refreshBonus(double elapsedMs)
{
	// Lấy vị trí điểm cộng hiện tại
	Point bonus_pos = bonusPosition;

	if (bonusActive)
	{
		// Cập nhật thời gian tồn tại của điểm cộng
		bonusTimeRemaining -= elapsedMs;
		// Nếu hết thời gian, hủy điểm cộng
		if (bonusTimeRemaining <= 0)
		{

			deactivateBonus();
			return bonus_pos;
		}
	}
	// Nếu điểm cộng vẫn còn thời gian, trả về {-1,-1}
	return Point{ -1,-1 };
}
