#pragma once
#include "GlobalVariable.h"
#include "Renderer.h"
#include "Snake.h"
#include "Food.h"

// rút gọn tên kiểu thời gian
using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
using t_cast = std::chrono::duration<double, std::milli>;

// Lớp Game: Lớp quản lý chính của trò chơi
class Game
{
public:
	Game()
	{
		this->renderer = new Renderer();
		this->snake = new Snake(BOARD_WIDTH / 2, BOARD_HEIGHT / 2);
		this->food = new Food();

		this->highScoreFilename = "high_scores.dat";									// Đặt tên file điểm cao
		this->currentState = State::MENU;												// trạng thái ban đầu là menu
		this->scoreManager.highScore = readDataFromBinaryFile(this->highScoreFilename); // Tải điểm cao từ file
		this->currentDifficulty = 1;													// độ khó ban đầu là 1 (dễ)

		this->difficultSelect = 1;              // lựa chọn độ khó ban đầu là 1 (dễ) - dùng trong render menu chọn độ khó
		this->menuSelection = 0;                // lựa chọn menu ban đầu là 0 (trò chơi mới) - dùng trong render menu chính

		this->lastTime = std::chrono::high_resolution_clock::now(); // thời điểm bắt đầu
		this->currentTime = lastTime;   // thời điểm hiện tại

		this->minSpeed = 80;            // tốc độ tối thiểu (tốc độ càng thấp thì rắn càng nhanh)
		this->currentSpeed = minSpeed;  // tốc độ hiện tại (cập nhật trong quá trình chơi game)
	}

	// hàm hủy để giải phóng bộ nhớ
	~Game()
	{
		if (renderer) delete renderer;
		if (snake) delete snake;
		if (food) delete food;

		renderer = nullptr;
		snake = nullptr;
		food = nullptr;
	}

	// Vòng lặp chính của trò chơi
	void gameLoop();
private:
	Renderer* renderer;		// đối tượng Renderer để hiển thị
	Snake* snake;			// đối tượng Snake đại diện cho con rắn
	Food* food;				// đối tượng Food đại diện cho thức ăn và điểm cộng

	std::vector<std::vector<CELL_TYPE>> canvas;     // bảng trò chơi

	State currentState;                             // trạng thái hiện tại của trò chơi
	highScoreEntry scoreManager;                    // quản lý điểm cao
	int currentDifficulty;                          // độ khó hiện tại (1-2-3)
	std::string highScoreFilename;                   // Tên file để lưu điểm cao

	int difficultSelect, menuSelection;             // lựa chọn độ khó và lựa chọn menu hiện tại

	time_point lastTime, currentTime;               // thời điểm lần cập nhật logic cuối và thời điểm hiện tại

	int minSpeed, currentSpeed;                     // tốc độ tối thiểu và tốc độ hiện tại (tốc độ càng thấp thì rắn càng nhanh)
	std::deque<Direction> inputQueue;               // bộ nhớ tạm hướng đi của người dùng

	double timeNow();                                   // hàm tính thời gian trôi qua kể từ last_time
	KeyInput getKey();                                  // hàm bắt các phím người dùng nhập vào bàn phím và trả về tên gọi của chúng
	void checkCollision(const Point& point);            // hàm kiểm tra va chạm của rắn với các vật thể khác

	void handleInput();                                 // hàm xử lý các phím người dùng nhập vào tùy theo trạng thái hiện tại
	void handleMenuInput(KeyInput key);                 // hàm xử lý phím trong trạng thái MENU
	void handleDifficultSelectionInput(KeyInput key);   // hàm xử lý phím trong trạng thái DIFFICULTY_SELECTION
	void handlePlayingInput(KeyInput key);              // hàm xử lý phím trong trạng thái PLAYING

	void updateLogic(double deltaTime);                 // hàm xử lý logic game tùy theo trạng thái hiện tại
	void updatePlayingLogic(double deltaTime);          // hàm xử lý logic game trong trạng thái PLAYING

	void render();					// hàm hiển thị màn hình trò chơi tùy theo trạng thái hiện tại
	void renderMenu();				// hàm hiển thị màn hình menu 
	void renderPlaying();			// hàm hiển thị màn hình trò chơi
	void renderGameOver();			// hàm hiển thị màn hình kết thúc trò chơi
	void renderHighScores();		// hàm hiển thị màn hình điểm cao
	void renderDifficulty();		// hàm hiển thị màn hình lựa chọn độ khó
	void renderAbout();				// hàm hiển thị màn hình giới thiệu

	void resetGame();				// hàm đặt lại trò chơi về trạng thái ban đầu
	void vietSub();					// hàm việt hóa các ký tự tiếng việt

	// hàm ghi dữ liệu vào file nhị phân
	bool writeDataToBinaryFile(const std::string& filename, const std::vector<std::pair<std::string, int>>& data);
	// hàm đọc dữ liệu từ file nhị phân
	std::vector<std::pair<std::string, int>> readDataFromBinaryFile(const std::string& filename);
};