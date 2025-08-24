#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <chrono>
#include <sstream>
// ----------------------------------------------------------------------------
using namespace std;
// ----------------------------------------------------------------------------
const int ROW = 42;
const int COL = 80;
const std::string cell = "▄";
const std::string HIDE_CURSOR = "\033[?25l";
const std::string HOME = "\033[H";
const std::string RESET_COLOR = "\033[0m\n";
// ----------------------------------------------------------------------------
struct Point
{
    int x,y;
};

struct RGB
{
	int r,g,b;
};

enum direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

int game_state = 0;
// ----------------------------------------------------------------------------
int get_input();
void enable_special_character();
void show_game();
void show_menu();
std::string get_bg_code(int r, int g, int b);
std::string get_fg_code(int r, int g, int b);
// ----------------------------------------------------------------------------
class Board
{
public:
	RGB board[ROW][COL];

	Board()
	{
		for (int y = 0; y < ROW; y++) {
			for (int x = 0; x < COL; x++) {
				board[y][x].r = 255;
				board[y][x].g = 255;
				board[y][x].b = 255;
			}
		}
	}
	void update_board(const Point src, int type)
	{
		RGB color;
		switch (type)
		{
			case 0:
				color.r = 0;
				color.g = 0;
				color.b = 0;
				break;
			case 1:
				color.r = 255;
				color.g = 255;
				color.b = 255;
				break;
			default: break;
		}
		
		board[src.y][src.x].r = color.r;
		board[src.y][src.x].g = color.g;
		board[src.y][src.x].b = color.b;
	}
	
	void render()
	{
		std::stringstream frame_buffer;
		frame_buffer << HOME;
		for (int row = 0; row < ROW; row+=2) {
			for (int col = 0; col < COL; col++) {
				frame_buffer << get_bg_code(board[row][col].r, board[row][col].g, board[row][col].b);
				frame_buffer << get_fg_code(board[row+1][col].r, board[row+1][col].g, board[row+1][col].b);
				frame_buffer << cell; 
			}
			frame_buffer << RESET_COLOR;
		}
		std::cout << frame_buffer.str();
		std::cout.flush();
	}
};

class CONRAN
{
public:
    Board game_board;
	struct Point A[100];
	Point old;
    int DoDai;
	int speed;
	
    CONRAN()
	{
        DoDai = 3;
		speed = 200;
        A[0].x = 12; A[0].y = 10;
        A[1].x = 11; A[1].y = 10;
        A[2].x = 10; A[2].y = 10;
		old.x = -1;
		old.y = -1;
    }
	
    void Ve()
	{
        for (int i = 0; i < DoDai; i++)
		{
			game_board.update_board(A[i],0);
        }
		if (old.x != -1)
			game_board.update_board(old,1);
		game_board.render();
    }
	
    void DiChuyen(direction dir)
	{
		old.x = A[DoDai-1].x;
		old.y = A[DoDai-1].y;
        for (int i = DoDai-1; i>0;i--)
            A[i] = A[i-1];
		switch (dir)
		{
			case UP:
				A[0].y = A[0].y - 1;
				if (A[0].y < 0) A[0].y = ROW-1;
				break;
			case DOWN:
				A[0].y = A[0].y + 1;
				if (A[0].y >= ROW) A[0].y = 0;
				break;
			case LEFT:
				A[0].x = A[0].x - 1;
				if (A[0].x < 0) A[0].x = COL-1;
				break;
			default: 
				A[0].x = A[0].x + 1;
				if (A[0].x >= COL) A[0].x = 0;
				break;
		}
    }
};
// ----------------------------------------------------------------------------
int main()
{
	
	std::ios_base::sync_with_stdio(false);
	std::cout.tie(nullptr);
	enable_special_character();
    show_menu();
    return 0;
}
// ----------------------------------------------------------------------------
std::string get_bg_code(int r, int g, int b)
	{
		return "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
	}
	
	std::string get_fg_code(int r, int g, int b)
	{
		return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
	}

void enable_special_character()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    SetConsoleOutputCP(CP_UTF8);
}

void show_menu()
{
	std::cout << "write your name: ";
	std::string name;
	std::cin >> name;
	std::cout << "your name: " << name;
	std::cout.flush();
	if (name == "abc")
	{
		game_state=1;
		show_game();
	}
}

void show_game()
{
	std::cout << HIDE_CURSOR;
	CONRAN r;
    int Huong = 0;
	direction dir = RIGHT;
    char t;
	auto last_time = std::chrono::high_resolution_clock::now();
	
	while (game_state==1)
	{
		r.Ve();
        if (kbhit())
		{
            t = getch();
            if (t=='a') dir = LEFT;
            if (t=='w') dir = UP;
            if (t=='d') dir = RIGHT;
            if (t=='s') dir = DOWN;
			if (t=='q') game_state = 0;
			while (_kbhit()) {
				_getch(); // Read and discard the character
			}
        }
        auto current_time = std::chrono::high_resolution_clock::now();
		double delta_time = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(current_time - last_time).count();
		if (delta_time >= r.speed)
		{
			r.DiChuyen(dir);
			last_time = current_time;
		}
    }
}