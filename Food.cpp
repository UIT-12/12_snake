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

void Food::spawnBonus(const std::vector<std::vector<CELL_TYPE>>& grid)
{
}

Point Food::refreshBonus(double elapsedMs)
{
	return Point();
}
