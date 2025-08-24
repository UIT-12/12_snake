#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <chrono>
#include <sstream>
// ----------------------------------------------------------------------------
using namespace std;
// ----------------------------------------------------------------------------
int game_state = 0;
// ----------------------------------------------------------------------------
struct Point
{
    int x,y;
};
struct RGB
{
	int r,g,b;
};
const int ROW = 42;
const int COL = 80;
// ----------------------------------------------------------------------------
int get_input();
void enable_special_character();
void gotoxy( int column, int line );
void show_game();
void show_menu();
// ----------------------------------------------------------------------------
class Board
{
public:
	const std::string item = "▄";	// y0 = bg, y1 = fg
	RGB board[ROW][COL];
	bool active_board[ROW][COL];

	Board()
	{
		for (int y = 0; y < ROW; y++) {
			for (int x = 0; x < COL; x++) {
				board[y][x].r = 255;
				board[y][x].g = 255;
				board[y][x].b = 255;
				active_board[y][x] = false;
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
		std::stringstream frameBuffer;
		frameBuffer << "\033[H";
		for (int row = 0; row < ROW; row+=2) {
			for (int col = 0; col < COL; col++) {
				frameBuffer << "\033[48;2;" << std::to_string(board[row][col].r) << ";" << std::to_string(board[row][col].g) << ";" << std::to_string(board[row][col].b) << "m" << "\033[38;2;" << std::to_string(board[row+1][col].r) << ";" << std::to_string(board[row+1][col].g) << ";" << std::to_string(board[row+1][col].b) << "m" << item; 
			}
			frameBuffer << "\033[0m\n";
		}
		std::cout << frameBuffer.str();
		std::cout.flush();
	}
};
class CONRAN
{
public:
    struct Point A[100];
	Board game_board;
	Point old;
    int DoDai;
	
    CONRAN()
	{
        DoDai = 3;
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
			if (game_board.active_board[A[i].y][A[i].x]) continue;
			game_board.update_board(A[i],0);
			game_board.active_board[A[i].y][A[i].x] = true;
        }
		game_board.render();
    }
	void Update()
	{
		if (game_board.active_board[A[0].y][A[0].x] == false) 
		{
			game_board.update_board(A[0],0);
			game_board.active_board[A[0].y][A[0].x] = true;
		}
		
		if (game_board.active_board[old.y][old.x]) 
		{
			game_board.update_board(old,1);
			game_board.active_board[old.y][old.x] = false;
		}
		game_board.render();
	}
    void DiChuyen(int Huong)
	{
		old.x = A[DoDai-1].x;
		old.y = A[DoDai-1].y;
        for (int i = DoDai-1; i>0;i--)
            A[i] = A[i-1];
        if (Huong==0)
		{
			A[0].x = A[0].x + 1;
			if (A[0].x >= COL) A[0].x = 0;
		}
		if (Huong==1)
		{
			A[0].y = A[0].y + 1;
			if (A[0].y >= ROW) A[0].y = 0;
		}
        if (Huong==2)
		{
			A[0].x = A[0].x - 1;
			if (A[0].x < 0) A[0].x = COL-1;
		}
        if (Huong==3)
		{
			A[0].y = A[0].y - 1;
			if (A[0].y < 0) A[0].y = ROW-1;
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
void gotoxy( int column, int line )
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
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
		Sleep(2000);
		system("cls");
		game_state=1;
		show_game();
	}
}

void show_game()
{
	std::cout << "\033[?25l";
	CONRAN r;
    int Huong = 0;
    char t;
	auto now = std::chrono::high_resolution_clock::now();

    // Cast the duration since the epoch to milliseconds
    auto duration = now.time_since_epoch();
    long long prev = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	system("cls");
	r.Ve();
	while (game_state==1)
	{
        if (kbhit())
		{
            t = getch();
            if (t=='a') Huong = 2;
            if (t=='w') Huong = 3;
            if (t=='d') Huong = 0;
            if (t=='s') Huong = 1;
			if (t=='q') game_state = 0;
			while (_kbhit()) {
				_getch(); // Read and discard the character
			}
        }
        
		auto nows = std::chrono::high_resolution_clock::now();

		// Cast the duration since the epoch to milliseconds
		
		long long noow = std::chrono::duration_cast<std::chrono::milliseconds>(nows.time_since_epoch()).count();
		if (noow - prev >= 300)
		{
			r.DiChuyen(Huong);
			prev = noow;
		}
        r.Update();
    }
}