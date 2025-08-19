#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
// ----------------------------------------------------------------------------
using namespace std;
// ----------------------------------------------------------------------------
struct Point
{
    int x,y;
};
// ----------------------------------------------------------------------------
int get_input();
void enable_special_character();
void gotoxy( int column, int line );
// ----------------------------------------------------------------------------
class CONRAN
{
public:
    struct Point A[100];
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
            gotoxy(A[i].x,A[i].y);
            cout<<"▄";
        }
    }
    void DiChuyen(int Huong)
	{
        for (int i = DoDai-1; i>0;i--)
            A[i] = A[i-1];
        if (Huong==0) A[0].x = A[0].x + 1;
        if (Huong==1) A[0].y = A[0].y + 1;
        if (Huong==2) A[0].x = A[0].x - 1;
        if (Huong==3) A[0].y = A[0].y - 1;
    }
};
// ----------------------------------------------------------------------------
int main()
{
	enable_special_character();
    CONRAN r;
    int Huong = 0;
    char t;

    while (1)
	{
        if (kbhit())
		{
            t = getch();
            if (t=='a') Huong = 2;
            if (t=='w') Huong = 3;
            if (t=='d') Huong = 0;
            if (t=='s') Huong = 1;
        }
        system("cls");
        r.Ve();
        r.DiChuyen(Huong);
        Sleep(300);
    }
	
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