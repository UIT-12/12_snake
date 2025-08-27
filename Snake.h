#pragma once
#include "GlobalVariable.h"

// Lớp Snake: Quản lý con rắn
class Snake
{
public:
	Snake(int startX, int startY)
	{
		// Khởi tạo rắn với 3 đoạn, đầu rắn ở vị trí (startX, startY)
		body.push_front({ startX - 2, startY });
		body.push_front({ startX - 1, startY });
		body.push_front({ startX, startY });
		// Khởi đầu rắn di chuyển sang phải
		direction = Direction::RIGHT;
		// Hướng tiếp theo mặc định là sang phải
		nextDirection = Direction::RIGHT;
		// Điểm ban đầu và trạng thái di chuyển
		// điểm đại diện cho độ dài rắn bổ sung khi ăn thức ăn
		point = 0;
		// trạng thái đuôi rắn có di chuyển hay không
		// nếu rắn đang tiêu hóa thức ăn thì đuôi không di chuyển (rắn dài thêm 1 đoạn)
		isMove = false;
	}

	// hàm di chuyển rắn
	void move();
	// hàm kiểm tra rắn có đang tiêu hóa thức ăn hay không
	bool isSnakeMove() { return isMove; }
	// hàm đặt lại trạng thái di chuyển sau khi đã xóa đuôi rắn
	void moveDone() { this->isMove = false; }
	// hàm cộng điểm (độ dài rắn) khi ăn thức ăn
	void addPoint(int point) { this->point += point; }
	// hàm kiểm tra rắn có tự cắn vào mình không
	bool checkSelfCollision() const;

	// lấy vị trí đầu và đuôi rắn
	Point getHead() const { return body.front(); }
	Point getTail() const { return body.back(); }

	// lấy toàn bộ thân rắn
	const std::deque<Point>& getBody() const { return body; }

	// hàm đặt hướng di chuyển mới cho rắn
	void setDirection(Direction newDirection) { nextDirection = newDirection; }
	// lấy hướng hiện tại của rắn
	Direction getDirection() const { return direction; }

private:
	std::deque<Point> body;	// danh sách các đoạn của rắn
	Direction direction;	// hướng hiện tại của rắn
	Direction nextDirection;// hướng tiếp theo của rắn
	int point;				// điểm (độ dài rắn bổ sung) khi ăn thức ăn
	bool isMove;			// trạng thái đuôi rắn có di chuyển hay không
};