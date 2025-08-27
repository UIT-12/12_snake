#pragma once
#include "GlobalVariable.h"

// Lớp Food: Quản lý vị trí của thức ăn
class Food
{
public:
	// hàm tạo vị trí thức ăn ban đầu
	void spawnFood(const std::vector<std::vector<CELL_TYPE>>& grid);

	// hàm tạo vị trí điểm cộng (bonus) mới
	void spawnBonus(const std::vector<std::vector<CELL_TYPE>>& grid);
	// hàm làm mới trạng thái điểm cộng, trả về vị trí điểm cộng nếu hết thời gian
	Point refreshBonus(double elapsedMs);

	// lấy vị trí thức ăn
	Point getFoodPosition() const { return foodPosition; }

	// kiểm tra trạng thái điểm cộng
	bool isBonusActive() const { return bonusActive; }
	// hủy điểm cộng
	void deactivateBonus() { bonusActive = false; bonusPosition = { 0,0 }; }
	// lấy vị trí điểm cộng
	Point getBonusPosition() const { return bonusPosition; }

private:
	Point foodPosition = { 0,0 };   // Vị trí thức ăn
	Point bonusPosition = { 0,0 };  // Vị trí điểm cộng
	double bonusTimeRemaining = 0;  // Thời gian còn lại cho điểm cộng
	double bonusDurationMs = 8000;	// Bonus tồn tại trong 8 giây
	bool bonusActive = false;		// Trạng thái điểm cộng
};