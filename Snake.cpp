#include "Snake.h"

/*
 * Hàm thực hiện việc di chuyển rắn trên bản đồ
 *
 */
void Snake::move()
{
	// Cập nhật hướng di chuyển
	direction = nextDirection;
	// Tính toán vị trí đầu mới
	Point newHead = body.front();
	switch (direction)
	{
	case Direction::UP:    newHead.y--; break;
	case Direction::DOWN:  newHead.y++; break;
	case Direction::LEFT:  newHead.x--; break;
	case Direction::RIGHT: newHead.x++; break;
	case Direction::NONE:  break;
	}
	// Thêm đầu mới vào thân rắn
	body.push_front(newHead);
	// Nếu rắn đói (point=0) thì xóa đuôi rắn và đặt isMove=true để cho phép di chuyển tiếp
	if (point > 0) point--;
	else
	{
		body.pop_back();
		isMove = true;
	}
}

/*
 * Hàm kiểm tra rắn có tự cắn hay không
 */
bool Snake::checkSelfCollision() const
{
	if (body.size() < 4) return false;	// rắn 3 khúc không thể tự cắn được 
	// Lấy vị trí đầu rắn 
	Point head = body.front();
	// Nếu đầu rắn vượt quá phạm vi chơi thì trả về va chạm 
	if (head.x < 0 || head.y < 0) return true;
	if (head.x >= BOARD_WIDTH || head.y >= BOARD_HEIGHT) return true;
	// Nếu đầu rắn trùng với thân rắn thì tự cắn 
	for (size_t i = 1; i < body.size(); ++i)
	{
		if (head == body[i])
		{
			return true;
		}
	}
	return false;
}
