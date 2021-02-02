#include "stdafx.h"
#include <thread>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <stdio.h>
#include <cstdlib>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h> 
#include <fstream>
#include <cmath>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

using namespace std;
//prototype
int menuchoice();
void ShowConsoleCursor(bool showFlag);
int resetingball();
void winscreen(int who);
void GotoXY(int x, int y);
void movement();
void draw();
void clrscr(int amountchar, int amountrow, int startingx, int startingy, char x = ' ');
void mainmenu();
void startmenumusicthread();
void menumusic();
void halloffame(int who);
void halloffameprint();
bool sound = true;
int resetingball();
int maxscore;
void Initilizer(int x, int y, int maxscores, int paddlesize, bool ai,int aicount);
int checkingpaddlecollison(int paddle1);
void drawpaddle(int paddlenum);
//prototype end

//structures start
struct Paddle
{

	int length;
	int x;
	int y;
	int dir;
	int counts;

};
struct BALL
{
	int x_;
	int y_;
	int dx;
	int dy;
	int startingposx;
	int startingposy;
};

struct FIELD
{
	int minheight;
	int minwidth;
	int maxheight;
	int maxwidth;
};
//structure end


int player1score, player2score;
//global variable
int sleepeveryround = 100;
thread mainmenumusicthread;
Paddle paddle[2];
BALL ball;
FIELD field;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int maxcounts = 5;
string dificulty;
const int sleepbetween = 1000;

bool flag = false;
//global variable start
int aicount = 0;
int maxaicount = 6;
bool ais;

// Initialize paddles , fields, ai,  and many special values

void Initilizer(int x, int y, int maxscores, int paddlesize,bool ai,int aicount) {
	ais = ai;
	maxscore = maxscores;
	field.minheight = 3;//dont change
	field.minwidth = 3;//dont change
	field.maxheight = y;//untested
	field.maxwidth = x;//untested
	maxaicount = aicount;
	paddle[0].counts = 0;
	paddle[0].dir = 0;
	paddle[0].length = paddlesize;
	paddle[0].x = field.minwidth + 2;
	paddle[0].y = (field.maxheight / 2) - (paddle[0].length / 2);

	paddle[1].counts = 0;
	paddle[1].dir = 0;
	paddle[1].length = paddlesize;
	paddle[1].x = field.maxwidth + field.minwidth;
	paddle[1].y = (field.maxheight / 2) - (paddle[1].length / 2);

	ball.startingposx = (field.maxwidth / 2) + field.minwidth;
	ball.startingposy = (field.maxheight);

	ball.dx = -1;
	ball.dy = -1;
	ball.x_ = ball.startingposx;
	ball.y_ = resetingball();
	srand(time(NULL));



}
//reset ball to the middle usually done during a goal
int resetingball() {
	
	return  (((rand() % (field.maxheight - 1 - field.minheight))) + field.minheight);


}
//turn off or on console cursor
void ShowConsoleCursor(bool showFlag)
{

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);

}
//clear the console in a area
void clrscr(int amountchar, int amountrow, int startingx, int startingy, char x)
{

	static HANDLE hStdout = NULL;
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD startCoords;
	startCoords.X = startingx;
	startCoords.Y = startingy;


	DWORD dummy;


	if (!hStdout)
	{

		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hStdout, &csbi);

	}
	//csbi.dwSize.X * csbi.dwSize.Y
	for (int i = startCoords.Y; i < amountrow; i++)
	{

		FillConsoleOutputCharacter(hStdout, x, amountchar, startCoords, &dummy);
		startCoords.Y = i;

	}

	GotoXY(0, 0);

}


//sets the location of the cursor to output
void GotoXY(int x, int y)	//GotoXY sets location for character through x and y coordinates as parameters 
{

	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	//cout << coord.X << coord.Y;

}

//cpu ai
void cpuai() {
	aicount++;


	/*if (ball.dx == -1)
	{
		if (paddle[1].y == (field.maxheight / 2) - (paddle[1].length / 2))
		{

		}
		else if (paddle[1].y < (field.maxheight / 2) - (paddle[1].length / 2))
		{
			paddle[1].y++;


		}
		else if (paddle[1].y > (field.maxheight / 2) - (paddle[1].length / 2)) {

			paddle[1].y--;

		}
	}
	*/
	if (ball.dx == 1)
	{
		if (ball.dy == -1 &&ball.x_>ball.startingposx) {
			if (paddle[1].y >= field.minheight )
			{
				paddle[1].dir = -1;
				if (aicount<maxaicount) {
					paddle[1].y--;
				}
				else {
					paddle[1].y--;
					if (paddle[1].y >= field.minheight) {
						paddle[1].y--;
					}
					aicount = 0;
				
				}
				paddle[1].counts = 0;
			}
		}
		else if (ball.dy == 1 && ball.x_>ball.startingposx) {
			if ((paddle[1].y < field.maxheight - paddle[1].length - 1))
			{

				paddle[1].dir = 1;
				if (aicount < maxaicount) {
					paddle[1].y++;
			
				}
				else {
					paddle[1].y++;
					if ((paddle[1].y < field.maxheight - paddle[1].length - 1)) {
						paddle[1].y++;
					}
					aicount = 0;
				}
				paddle[1].counts = 0;
			}
		}
		else if(ball.dy == 0){
			if (paddle[1].y<ball.y_ ) {
				paddle[1].y++;
			
			}
			if (paddle[1].y>ball.y_) {
				paddle[1].y--;
			}


		}

	
	/*if (ball.y_ == (field.maxheight / 2) - (paddle[1].length / 2))
	{

	}
	else if (ball.y_ > (field.maxheight / 2) - (paddle[1].length / 2))
	{
		if (paddle[1].y < field.maxheight - paddle[0].length - 1)
		{
			paddle[1].y++;
		}

	}
	else if (ball.y_ < (field.maxheight / 2) - (paddle[1].length / 2)) {

		{
			paddle[1].y--;
		}
	}

}
*/



}
}
//gets the keyboard presses not async
void movement()
{

	if (0 != _kbhit())
	{
		switch (_getch())//object.object.object.variable
		{

		case 119://Moves character up w
			if (paddle[0].y >= field.minheight)
			{
				paddle[0].dir = -1;
				paddle[0].y--;
				paddle[0].counts = 0;
			}
			break;

		case 115://Moves character downs
				 //if (paddle[0].y < field.maxheight - field.minheight - 1)
			if (paddle[0].y < field.maxheight - paddle[0].length - 1)
			{

				paddle[0].dir = 1;

				paddle[0].y++;
				paddle[0].counts = 0;

			}
			break;
		case 72://i
			if (ais==false) {
				if (paddle[1].y >= field.minheight)
				{
					paddle[1].dir = -1;
					paddle[1].y--;
					paddle[1].counts = 0;
				}
			}
			break;
		case 80://k
			if (ais == false) {
				if (paddle[1].y < field.maxheight - paddle[1].length - 1)
				{
					paddle[1].dir = 1;
					paddle[1].y++;
					paddle[1].counts = 0;
				}
			}
			break;

		}
	}
	else
	{
		paddle[0].counts++;
		paddle[1].counts++;
		if (paddle[1].counts > maxcounts)
		{
			paddle[1].dir = 0;
			paddle[1].counts = 0;
		}
		if (paddle[0].counts > maxcounts)
		{
			paddle[0].dir = 0;
			paddle[0].counts = 0;
		}

	}
}
//Starting menu
int menuchoice()
{
	if (0 != _kbhit())
	{
		switch (_getch())//object.object.object.variable
		{

		case 49://1 easy
			sleepeveryround = 100;
			maxscore = 10;
			dificulty = "beginer";
			Initilizer(100, 40, 10, 4,false,0);
			return 1;

		case 50://2 medium
			sleepeveryround = 75;
			Initilizer(70, 20, 15, 3, false,0);
			dificulty = "advance";
			return 1;
		case 51://3 hard
			Initilizer(40, 10, 20, 2, false,0);
			sleepeveryround = 75;
			dificulty = "expert";
			return 1;
		case 52:
			sleepeveryround = 100;
			maxscore = 10;
			dificulty = "beginer";
			Initilizer(100, 40, 10, 4, true,2);
			return 1;
		case 53:
			sleepeveryround = 75;
			Initilizer(70, 20, 15, 3, true,3);
			dificulty = "advance";
			return 1;
		case 54:
			Initilizer(40, 10, 20, 2, true,5);
			sleepeveryround = 75;
			dificulty = "expert";
			return 1;
		case 55:
			system("cls");
			halloffameprint();
			system("cls");
			mainmenu();
			break;
		case 56:
			exit(1);
			break;
		case 57:
			if (sound == true) {
				sound = false;
			}
			else {
				sound = true;
				startmenumusicthread();
			}
			return 0;
			break;
		default:
			return 0;
		}
	}
	else
	{
		return 0;
	}
	return 0;
}

//output the the field paddle and score
void draw()
{
	for (int i = 0; i < field.minheight - 3; i++) {
		cout << endl;

	}
	cout << "Player 1" << setw(field.maxwidth - 10) << "Player 2-" << endl;
	for (int i = 0; i < field.minwidth; i++)
	{
		cout << " ";
	}
	cout << char(201);
	for (int i = 1; i < field.maxwidth + field.minwidth - 1; i++)
	{

		cout << char(205);
	}
	cout << char(187) << endl;
	for (int i = field.minheight; i < field.maxheight; i++)
	{
		for (int i = 0; i < field.minwidth; i++)
		{
			cout << " ";
		}
		cout << char(186) << setw(field.maxwidth + field.minwidth - 1) << char(186) << endl;
	}
	for (int i = field.minheight - 1; i < field.maxheight - 1; i++)
	{
		GotoXY(ball.startingposx, i);
		cout << char(179);

	}
	cout << endl;
	for (int i = 0; i < field.minwidth; i++)
	{
		cout << " ";
	}
	cout << char(200);
	for (int i = 1; i < field.maxwidth + field.minwidth - 1; i++)
	{

		cout << char(205);
	}
	cout << char(188);
	for (int i = paddle[0].y; i < paddle[0].y + paddle[0].length; i++)
	{
		GotoXY(paddle[0].x, i);
		cout << char(221);
	}
	for (int i = paddle[1].y; i < paddle[1].y + paddle[0].length; i++)
	{
		GotoXY(paddle[1].x, i);
		cout << char(221);
	}



	SetConsoleTextAttribute(hConsole, 4);
	GotoXY(8, 0);
	cout << "- " << player1score;
	GotoXY((field.maxwidth), 0);
	SetConsoleTextAttribute(hConsole, 9);
	cout << player2score;



	GotoXY(8, 0);
	cout << "- " << player1score;
	GotoXY((field.maxwidth), 0);

	cout << player2score;
	GotoXY(0, 0);

	GotoXY(0, 0);

}

//checks the paddle collision
int checkingpaddlecollison(int paddle1)
{
	if ((paddle[paddle1].x == ball.x_))
	{
		for (int i = paddle[paddle1].y; i < paddle[paddle1].y + paddle[paddle1].length; i++)
		{
			if ((i == ball.y_))
			{

				ball.dx *= -1;
				if (ball.dy == -1 && paddle[paddle1].dir == 1)
				{
					Beep(100, 100);
					return 1;
				}
				else if (ball.dy == 1 && paddle[paddle1].dir == -1)
				{
					Beep(100, 100);
					return -1;
				}
				else if (ball.dy == -1 && paddle[paddle1].dir == -1)
				{
					Beep(100, 100);
					return -1;
				}
				else if (ball.dy == 1 && paddle[paddle1].dir == 1)
				{
					Beep(100, 100);
					return 1;
				}
				else if (ball.dy == 0 && paddle[paddle1].dir == 1) {
					Beep(100, 100);
					return 1;

				}
				else if (ball.dy == 0 && paddle[paddle1].dir == -1) {
					Beep(100, 100);
					return -1;

				}
				else if (paddle[paddle1].dir == 0) {
					Beep(100, 100);
					return  0;


				}

			}
		}

	}
	else
	{
		return ball.dy;
	}
	return ball.dy;

}
//draw paddles
void drawpaddle(int paddlenum) {
	for (int i = paddle[paddlenum].y; i < paddle[paddlenum].y + paddle[paddlenum].length; i++)
	{
		GotoXY(paddle[paddlenum].x, i);
		cout << char(221);
	}



}
int main()
{

	ShowConsoleCursor(false);

	mainmenu();
	draw();

	GotoXY(ball.x_, ball.y_);
	SetConsoleTextAttribute(hConsole, 13);
	cout << char(233);
	SetConsoleTextAttribute(hConsole, 10);
	Sleep(sleepbetween);

	while (true)
	{

		ball.x_ += ball.dx;
		ball.y_ += ball.dy;
		//if (aicount > maxaicount) {
		//	aicount = 0;
		if (ais == true) {
			cpuai();
		}
		//}
	///	else {
		//	aicount++;

		//}
		movement();
		ball.dy = checkingpaddlecollison(0);
		ball.dy = checkingpaddlecollison(1);


		if (ball.x_ > field.maxwidth + field.minwidth)
		{

			ball.x_ = ball.startingposx;
			ball.y_ = resetingball();
			ball.dx = -1;
			if (ball.y_ > (field.maxheight / 2))
			{
				ball.dy = 1;
			}
			if (ball.y_ < (field.maxheight / 2))
			{
				ball.dy = -1;
			}
			Beep(493, 200);
			Beep(880, 200);
			player1score++;
			if (maxscore <= player1score)
			{
				player1score = 0;
				player2score = 0;
				winscreen(1);
				break;
			}
			GotoXY(ball.x_, ball.y_);
			SetConsoleTextAttribute(hConsole, 13);
			cout << char(233);
			SetConsoleTextAttribute(hConsole, 10);
			Sleep(sleepbetween);



		}
		if (ball.x_ < field.minwidth + 1)
		{

			ball.x_ = ball.startingposx;
			ball.y_ = resetingball();
			ball.dx = 1;
			if (ball.y_ > (field.maxheight / 2))
			{
				ball.dy = 1;
			}
			if (ball.y_ < (field.maxheight / 2))
			{
				ball.dy = -1;
			}

			Beep(493, 200);
			Beep(880, 200);
			player2score++;
			if (maxscore == player2score)
			{
				player1score = 0;
				player2score = 0;
				winscreen(2);
				break;
			}
			GotoXY(ball.x_, ball.y_);
			SetConsoleTextAttribute(hConsole, 13);
			cout << char(233);
			SetConsoleTextAttribute(hConsole, 10);
			Sleep(sleepbetween);



		}



		if (ball.y_ > field.maxheight - 3 || ball.y_ < field.minheight)
		{
			ball.dy *= -1;

		}


		clrscr(field.maxwidth + 1, field.maxheight, field.minwidth + 1, field.minheight - 1);

		GotoXY(ball.x_, ball.y_);
		SetConsoleTextAttribute(hConsole, 13);
		cout << char(233);
		SetConsoleTextAttribute(hConsole, 10);

		drawpaddle(0);
		drawpaddle(1);

		for (int i = field.minwidth - 1; i < field.maxheight - 1; i++)
		{

			GotoXY(ball.startingposx, i);
			cout << char(179);

		}
		if (player1score > player2score) {
			SetConsoleTextAttribute(hConsole, 9);
			GotoXY(8, 0);
			cout << "- " << player1score;
			GotoXY((field.maxwidth), 0);
			SetConsoleTextAttribute(hConsole, 4);
			cout << player2score;
		}
		else if (player1score == player2score) {
			SetConsoleTextAttribute(hConsole, 14);
			GotoXY(8, 0);
			cout << "- " << player1score;
			GotoXY((field.maxwidth), 0);
			cout << player2score;

		}
		else {
			SetConsoleTextAttribute(hConsole, 4);
			GotoXY(8, 0);
			cout << "- " << player1score;
			GotoXY((field.maxwidth), 0);
			SetConsoleTextAttribute(hConsole, 9);
			cout << player2score;

		}



		Sleep(sleepeveryround);

	}

}

void winscreen(int who)
{
	system("cls");
	const int length = 25;
	string winningscreen[length];
	fstream x;
	fstream y;
	fstream z;
	x.open("winnerscreen.txt");
	string hall[100];

	for (int i = 0; i < 18; i++)
	{
		getline(x, winningscreen[i]);
	}


	if (who == 1)
	{
		x.close();

		x.open("player1.txt");
		for (int i = 18 - 1; i < length; i++)
		{
			getline(x, winningscreen[i]);
		}



	}
	else if (who == 2)
	{


		y.open("player2.txt");
		for (int i = 18 - 1; i < length; i++)
		{

			getline(y, winningscreen[i]);

		}

	}
	for (int i = 0; i < length; i++)
	{
		cout << winningscreen[i] << endl;
	}


	for (int i = 0; i < winningscreen[i].length(); i++)
	{
		winningscreen[i] = winningscreen[i].insert(0, 1, winningscreen[i].at(winningscreen[i].length() - 1));
		winningscreen[i].pop_back();
	}
	z.open("halloffame.txt");
	int k = -1;
	for (int i = 0; z.good(); i++) {

		getline(z, hall[i]);
		k++;
	}
	cout << "------Name-------------------Hall of fame------Difficulty---------" << endl;
	int b = 1;
	for (int i = 0; i < k; i++) {
		cout << b++ << ". " << hall[i] << endl;

	}


	for (;;)
	{
		GotoXY(0, 0);

		for (int i = 0; i < length; i++)
		{
			cout << winningscreen[i] << endl;
		}


		for (int i = 0; i < winningscreen[i].length(); i++)
		{
			winningscreen[i] = winningscreen[i].insert(0, 1, winningscreen[i].at(winningscreen[i].length() - 1));
			winningscreen[i].pop_back();
		}
		Sleep(10);
		if (0 != _kbhit()) {
			
			halloffame(who);
			paddle[0].counts = 0;
			paddle[1].counts = 0;
			system("cls");
			main();
		}


	}

}
void halloffameprint() {
	fstream z;
	z.open("halloffame.txt");
	string hall[100];
	int k = -1;
	for (int i = 0; z.good(); i++) {

		getline(z, hall[i]);
		k++;
	}
	cout << "------Name-------------------Hall of fame------Difficulty---------" << endl;
	int b = 1;
	for (int i = 0; i < k; i++) {
		cout << b++ << ". " << hall[i] << endl;

	}
	_getch();

}
void halloffame(int who) {
	system("cls");
	string name, name2;
	ofstream myfile("halloffame.txt", std::ios::app);
	if (who == 1 && ais == true) {
		cout << endl << endl << endl;
		cout << "Enter winners name" << endl;
		cin >> name;
		name2 = "cpu";
		myfile << setw(5) << name << " Won against " << name2 << setw(25) << dificulty << endl;
		myfile.close();
	}
	else if (ais = false) {
		cout << endl << endl << endl;
		cout << "Enter winners name" << endl;
		cin >> name;
		cout << "Enter losers name" << endl;
		cin >> name2;
		myfile << setw(5) << name << " Won against " << name2 << setw(25) << dificulty << endl;
		myfile.close();
	}
}
void startmenumusicthread() {
	if (flag == false) {
		flag = true;
		mainmenumusicthread = thread(menumusic);
	}
}

void mainmenu()
{

	startmenumusicthread();
	int rc;
	int sle = 1000;
	HANDLE hConsole1;
	fstream x;
	hConsole1 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 13);
	string row[12];
	x.open("pongscreen.txt");
	for (int i = 0; i < 12; i++)
	{
		getline(x, row[i]);
	}

	for (int i = 0; i < 12; i++)
	{
		cout << row[i] << endl;
	}
	cout << setw(90) << "    player 1 control                     Press 1 for beginer               " << endl;
	cout << setw(90) << "  up = w    down = s                   Press 2 for advance               " << endl;
	cout << setw(90) << "  player 2 control                     Press 3 for expert try if you dare" << endl;
	cout << setw(91) << "up= up_arrow  down=down arrow         press 4 for beginer with ai        " << endl;
	cout << setw(83) << "press 5 for advance with ai" << endl;
	cout << setw(82) << "press 6 for expert with ai" << endl;
	cout << setw(80) << "Press 7 to hall of fame " << endl;
	cout << setw(71) << "press 8 to quit" << endl;
	cout << setw(84) << "press 9 to mute/unmute sound"<< endl << endl;

	SetConsoleTextAttribute(hConsole, 10);
	for (;;)
	{
		GotoXY(0, 0);

		for (int i = 0; i < 12; i++)
		{
			cout << row[i] << endl;
		}


		for (int i = 0; i < 12; i++)
		{
			row[i] = row[i].insert(0, 1, row[i].at(row[i].length() - 1));
			row[i].pop_back();
		}
		Sleep(10);

		rc = menuchoice();

		if (rc == 1)
		{
			break;
		}

		//	clrscr(78, 12, 0, 0);

	}
	system("cls");
	SetConsoleTextAttribute(hConsole, 15);
}

void menumusic() {
	int music[161][2] = { { 480,200 },{ 1568,200 },{ 1568,200 },{ 1568,200 },{ 739,200 },{ 783,200 },{ 783,200 },{ 783,200 },{ 369, 200 },{ 392 , 200 },{ 369, 200 },{ 392, 200 },{ 392, 400 },
	{ 196,400 },{ 739, 200 },{ 783,200 },{ 783,200 },{ 739,200 },{ 783,200 },{ 783,200 },{ 783,200 },{ 739,200 },{ 83 , 200 },{ 880,200 },{ 830,200 },{ 880, 200 },{ 987, 400 },{ 880,200 },
	{ 783,200 },{ 698,200 },{ 739,200 },{ 783 ,200 },{ 783,200 },{ 739,200 },{ 783 ,200 },{ 783 ,200 },{ 739,200 },{ 783,200 },{ 880 ,200 },{ 830 ,200 },{ 880 ,200 },{ 987 ,400 },{ 1108,10 },
	{ 1174 ,200 },{ 1480 , 10 },{ 1568 ,200 },{ 1108 ,10 },{ 1174,200 },{ 1480,10 },{ 1568,200 },{ 739.200 },{ 783,200 },{ 783,200 },{ 739,200 },{ 783,200 },{ 783,200 },{ 880,200 },{ 830,200 },{ 880,200 },{ 987,400 },
	{ 880,200 },{ 783,200 },{ 698,200 },{ 659,200 },{ 698,200 },{ 698,200 },{ 784, 200 },{ 880,400 },{ 784 ,200 },{ 698 ,200 },{ 659 ,200 },{ 659 ,200 },{ 587 ,200 },{ 659, 200 },{ 784 ,400 },{ 698,200 },
	{ 659 ,200 },{ 587,200 },{ 523,200 },{ 587 ,200 },{ 659 ,200 },{ 698,400 },{ 659,200 },{ 587,200 },{ 493,200 },{ 523 ,200 },{ 349 ,400 },{ 349,400 },{ 392,400 },{ 392 ,200 },{ 329 ,200 },
	{ 523 ,200 },{ 523 ,200 },{ 493 ,200 },{ 466 ,200 },{ 440 ,200 },{ 493 ,200 },{ 523 ,200 },{ 880,200 },{ 880 ,200 },{ 392,200 },{ 440 ,200 },{ 493,200 },{ 783 ,200 },{ 440,200 },{ 783,200 },
	{ 1568 ,200 },{ 392 ,200 },{ 349,200 },{ 392, 200 },{ 440,200 },{ 698,200 },{ 415 ,200 },{ 698,200 },{ 1396 ,200 },{ 349,200 },{ 329 ,200 },{ 329,200 },{ 311 , 200 },{ 329 ,200 },{ 311,200 },
	{ 329 ,200 },{ 659,200 },{ 698,400 },{ 783 ,400 },{ 440,200 },{ 523,200 },{ 880,200 },{ 493 ,200 },{ 880 ,200 },{ 1760 ,200 },{ 440 ,200 },{ 392 ,200 },{ 440 ,200 },{ 392 ,200 },{ 440 ,200 },
	{ 493,200 },{ 783 ,200 },{ 440 ,200 },{ 783 ,200 },{ 1568 ,200 },{ 392 ,200 },{ 349 ,200 },{ 392 ,200 },{ 440 ,200 },{ 698 ,200 },{ 739 ,200 },{ 783,200 },{ 783,200 },{ 392, 200 },{ 392 ,200 },
	{ 392,200 },{392,200 },{ 196,200 },{ 196,200 },{ 185 ,200 },{ 196, 200 },{ 207,200 },{ 220,200 },{ 233,200 },{ 246,200 } };
	for (;;) {

		if(sound == true){
		for (int i = 0; i < 161; i++) {
			Beep(music[i][0], music[i][1]);
			if (sound == false) {
				break;
			
			}
		}

			
		}
	}

}
