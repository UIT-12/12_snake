#include "Food.h"

/*
 * hàm sinh ra thức ăn ở vị trí ngẫu nhiên trên bản đồ
 */
void Food::spawnFood(const std::vector<std::vector<CELL_TYPE>>& grid)
{
	// Tạo thiết bị sinh số ngẫu nhiên
	std::random_device rd;
	std::mt19937 gen(rd());
	// Tạo danh sách các ô trống
	std::deque<Point> remain;
	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[0].size(); ++x)
		{
			if (grid[y][x] == CELL_TYPE::NONE)
				remain.push_back({ x, y });
		}
	}
	// Chọn ngẫu nhiên một ô trống
	std::uniform_int_distribution<> dis(0, remain.size() - 1);
	// Đặt vị trí thức ăn
	foodPosition = remain[dis(gen)];
}

/*
 * hàm sinh ra điểm cộng ở vị trí ngẫu nhiên trên bản đồ
 * điểm cộng chỉ xuất hiện trong một khoảng thời gian nhất định
 */
void Food::spawnBonus(const std::vector<std::vector<CELL_TYPE>>& grid)
{
	// Tạo thiết bị sinh số ngẫu nhiên
	std::random_device rd;
	std::mt19937 gen(rd());
	// Tạo danh sách các ô trống
	std::deque<Point> remain;
	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[0].size(); ++x)
		{
			if (grid[y][x] == CELL_TYPE::NONE)
				remain.push_back({ x, y });
		}
	}
	// Chọn ngẫu nhiên một ô trống
	std::uniform_int_distribution<> dis(0, remain.size() - 1);
	// Đặt vị trí điểm cộng
	bonusPosition = remain[dis(gen)];
	// Khởi động bộ đếm thời gian tồn tại của điểm cộng
	bonusTimeRemaining = bonusDurationMs;
	// Đánh dấu điểm cộng đang hoạt động
	bonusActive = true;
}

/*
 * hàm cập nhật thời gian tồn tại của điểm cộng
 * nếu hết thời gian thì hủy điểm cộng
 * trả về vị trí điểm cộng nếu hết thời gian, ngược lại trả về {-1,-1}
 */
Point Food::refreshBonus(double elapsedMs)
{
	// lấy vị trí điểm cộng hiện tại
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