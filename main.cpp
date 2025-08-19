#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <chrono>
// ----------------------------------------------------------------------------
using namespace std;
// ----------------------------------------------------------------------------
const int ROW = 23;
const int COL = 80;
bool active_board[ROW][COL];
int game_state = 0;
// ----------------------------------------------------------------------------
struct Point
{
    int x,y;
};
// ----------------------------------------------------------------------------
int get_input();
void enable_special_character();
void gotoxy( int column, int line );
void show_game();
void show_menu();
// ----------------------------------------------------------------------------
class CONRAN
{
public:
    struct Point A[100];
	Point old;
    int DoDai;
	
    CONRAN()
	{
        DoDai = 3;
        A[0].x = 10; A[0].y = 10;
        A[1].x = 11; A[1].y = 10;
        A[2].x = 12; A[2].y = 10;
    }
    void Ve()
	{
        for (int i = 0; i < DoDai; i++)
		{
			if (active_board[A[i].y][A[i].x]) continue;
            gotoxy(A[i].x,A[i].y);
            cout<<"▄";
			active_board[A[i].y][A[i].x] = true;
        }
    }
	void Update()
	{
		if (!active_board[A[0].y][A[0].x]) 
		{
			gotoxy(A[0].x,A[0].y);
			cout<<"▄";
			active_board[A[0].y][A[0].x] = true;
		}
		
		if (active_board[old.y][old.x]) 
		{
			gotoxy(old.x,old.y);
			cout<<" ";
			active_board[old.y][old.x] = false;
		}
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
	enable_special_character();
	for (int i = 0; i < ROW; ++i)
	{
		for (int j = 0; j < COL; ++j)
		{
			active_board[i][j] = false;
		}
	}
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
	std::cout << "your name: " << name << '\n';
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