// XOgame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <random>	// for Random
#include <chrono>	// for Random
#include <stdlib.h> // for Random
#include <Windows.h>

using namespace std;

#define SIZE 3
enum XO_Symbol {
	E = 32, // Empty cell
	X = 88,
	O = 79
};
enum GameStatus {
	IN_PROGRESS,
	PLAYER_WON,
	COMP_WON,
	DRAW
};

#pragma pack(push,1)
struct Game
{
	XO_Symbol field[SIZE][SIZE] = { E,E,E,E,E,E,E,E,E }; // Empty fieldVolumes
	XO_Symbol player;
	XO_Symbol comp;
	uint8_t turn = 0;
	GameStatus status = IN_PROGRESS;
};
#pragma pack(pop)

// Clear screen function
#ifdef _WIN32 || _WIN64
//#include <windows.h>
void clear_screen() {
	system("cls"); // for windows
}
#else
#include <unistd.h>
void clear_screen()
{
	write(1, "\E[H\E[2J", 7);
}
#endif

// std::cin check function for integer number
int getIntNumber(string sQuestion)
{
	while (true) // цикл продолжается до тех пор, пока пользователь не введёт корректное значение
	{
		cout << sQuestion;
		int num;
		cin >> num;
		if (cin.fail()) // если предыдущее извлечение оказалось неудачным,
		{
			cout << "Invalid input. Please enter an integer number!" << endl;
			cin.clear(); // то возвращаем cin в 'обычный' режим работы
			cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
		}
		else // если всё хорошо, то возвращаем num
		{
			return num;
		}
	}
}
// Рандом
int32_t getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

// Начало игры
void initGame(Game& match) {
	if (getRandomNum(0, 1000) > 500)
	{
		match.player = X;
		match.comp = O;
		match.turn = 0;
	}
	else
	{
		match.player = O;
		match.comp = X;
		match.turn = 1;
	}
}

// Рисование поля красивой рамкой
void __fastcall fieldDraw(const Game& match) {
	// Инструкция
	SetConsoleOutputCP(1251); // Для русского языка
	cout << "Ведите номер клетки 1-9, расположение клеток соответствует NUM-Клавиатуре" << endl;
	cout << "7_|_8_|_9" << endl;
	cout << "4_|_5_|_6" << endl;
	cout << "1 | 2 | 3" << endl;
	SetConsoleOutputCP(866); // установка кодовой страницы cp866 в поток вывода для красивых рамок
	cout << char(218) << char(196) << char(196) << char(196) << char(194) << char(196) << char(196) << char(196) << char(194) << char(196) << char(196) << char(196) << char(191) << endl;
	cout << char(179) << char(32) << char(match.field[0][0]) << char(32) << char(179) << char(32) << char(match.field[0][1]) << char(32) << char(179) << char(32) << char(match.field[0][2]) << char(32) << char(179) << endl;
	cout << char(195) << char(196) << char(196) << char(196) << char(197) << char(196) << char(196) << char(196) << char(197) << char(196) << char(196) << char(196) << char(180) << endl;
	cout << char(179) << char(32) << char(match.field[1][0]) << char(32) << char(179) << char(32) << char(match.field[1][1]) << char(32) << char(179) << char(32) << char(match.field[1][2]) << char(32) << char(179) << endl;
	cout << char(195) << char(196) << char(196) << char(196) << char(197) << char(196) << char(196) << char(196) << char(197) << char(196) << char(196) << char(196) << char(180) << endl;
	cout << char(179) << char(32) << char(match.field[2][0]) << char(32) << char(179) << char(32) << char(match.field[2][1]) << char(32) << char(179) << char(32) << char(match.field[2][2]) << char(32) << char(179) << endl;
	cout << char(192) << char(196) << char(196) << char(196) << char(193) << char(196) << char(196) << char(196) << char(193) << char(196) << char(196) << char(196) << char(217) << endl;
	SetConsoleOutputCP(1251); // возврат обратно кодовой страницы win-cp 1251 в поток вывода
	cout << "Компьютер играет: " << char(match.comp) << endl;
	cout << "Вы играете: " << char(match.player) << endl;
}

//Проверка статуса игры
GameStatus winCheck(Game& match) {
	for (size_t i = 0; i < SIZE; i++) {
		if (match.field[i][0] != E && match.field[i][0] == match.field[i][1] && match.field[i][0] == match.field[i][2]) {
			if (match.field[i][0] == match.player)
				return PLAYER_WON;
			else
				return COMP_WON;
		}
		if (match.field[0][i] != E && match.field[0][i] == match.field[1][i] && match.field[0][i] == match.field[2][i]) {
			if (match.field[0][i] == match.player)
				return PLAYER_WON;
			else
				return COMP_WON;
		}
	}
	if (match.field[0][0] != E && match.field[0][0] == match.field[1][1] && match.field[0][0] == match.field[2][2])
		if (match.field[0][0] == match.player)
			return PLAYER_WON;
		else
			return COMP_WON;
	if (match.field[0][2] != E && match.field[0][2] == match.field[1][1] && match.field[0][2] == match.field[2][0])
		if (match.field[0][2] == match.player)
			return PLAYER_WON;
		else
			return COMP_WON;
	if (match.player == X && match.turn == 8 || match.player == O && match.turn == 9)
		return DRAW;
	return IN_PROGRESS;
}

// Ход игрока
void playerTurn(Game& match) {
	int playerChoice;
	bool turnCheck = false;
	do
	{
		//cout << "Ваш ход: ";
		playerChoice = getIntNumber("Ваш ход: ");
		switch (playerChoice) {
		case 1:
			if (match.field[2][0] == E) {
				match.field[2][0] = match.player;
				turnCheck = true;
			}
			break;
		case 2:
			if (match.field[2][1] == E) {
				match.field[2][1] = match.player;
				turnCheck = true;
			}
			break;
		case 3:
			if (match.field[2][2] == E) {
				match.field[2][2] = match.player;
				turnCheck = true;
			}
			break;
		case 4:
			if (match.field[1][0] == E) {
				match.field[1][0] = match.player;
				turnCheck = true;
			}
			break;
		case 5:
			if (match.field[1][1] == E) {
				match.field[1][1] = match.player;
			turnCheck = true;
			}
			break;
		case 6:
			if (match.field[1][2] == E) {
				match.field[1][2] = match.player;
				turnCheck = true;
			}
			break;
		case 7:
			if (match.field[0][0] == E) {
				match.field[0][0] = match.player;
				turnCheck = true;
			}
			break;
		case 8:
			if (match.field[0][1] == E) {
				match.field[0][1] = match.player;
				turnCheck = true;
			}
			break;
		case 9:
			if (match.field[0][2] == E) {
				match.field[0][2] = match.player;
				turnCheck = true;
			}
			break;
		default:
			cout << "Введите правильную ячейку: " << endl;
		}
	} while (playerChoice < 1 || playerChoice > 9 || turnCheck == false);
}

// Ход компа
void compTurn(Game& match) {
	//comp win possibility check 
	for (size_t i = 0; i < SIZE; i++)	{
		for (size_t j = 0; j < SIZE; j++)		{
			if (match.field[i][j] == E) {
				match.field[i][j] = match.comp;
				if (winCheck(match) == COMP_WON || winCheck(match) == DRAW) 
					return;
				else
					match.field[i][j] = E;
			}
		}
	}
	//player win possibility check
	for (size_t i = 0; i < SIZE; i++) {
		for (size_t j = 0; j < SIZE; j++) {
			if (match.field[i][j] == E) {
				match.field[i][j] = match.player;
				if (winCheck(match) == PLAYER_WON) {
					match.field[i][j] = match.comp;
					return;
				}
				else
					match.field[i][j] = E;
			}
		}
	}
	// Если свободный центр
	if (match.field[1][1] == E) {
		match.field[1][1] = match.comp;
		return;
	}
	// Если свободны углы
	if (match.field[0][0] == E || match.field[2][2] == E || match.field[2][0] == E || match.field[0][2] == E) {
		bool cornCheck = false;
		do
		{
			size_t cornRand = getRandomNum(0, 3);
			if (cornRand == 0 && match.field[0][0] == E)
			{
				match.field[0][0] = match.comp;
				cornCheck = true;
			}
			else if (cornRand == 1 && match.field[2][2] == E)
			{
				match.field[2][2] = match.comp;
				cornCheck = true;
			}
			else if (cornRand == 2 && match.field[2][0] == E)
			{
				match.field[2][0] = match.comp;
				cornCheck = true;
			}
			else if (cornRand == 3 && match.field[0][2] == E)
			{
				match.field[0][2] = match.comp;
				cornCheck = true;
			}
		} while (cornCheck == false);
		return;
	}
	// Если свободны стороны
	if (match.field[0][1] == E || match.field[1][2] == E || match.field[2][1] == E || match.field[1][0] == E) {
		bool sideCheck = false;
		do
		{
			size_t sideRand = getRandomNum(0, 3);
			if (sideRand == 0 && match.field[0][1] == E)
			{
				match.field[0][1] = match.comp;
				sideCheck = true;
			}
			else if (sideRand == 1 && match.field[1][2] == E)
			{
				match.field[1][2] = match.comp;
				sideCheck = true;
			}
			else if (sideRand == 2 && match.field[2][1] == E)
			{
				match.field[2][1] = match.comp;
				sideCheck = true;
			}
			else if (sideRand == 3 && match.field[1][0] == E)
			{
				match.field[1][0] = match.comp;
				sideCheck = true;
			}
		} while (sideCheck == false);
		return;
	}
}

int main()
{
	Game match;
	initGame(match);
	clear_screen();
	fieldDraw(match);
	do
	{
		if (match.turn % 2 == 0)
		{
			//Ход человека
			playerTurn(match);
		}
		else
		{
			//Ход компьютера
			compTurn(match);
		}
		clear_screen();
		fieldDraw(match);
		match.status = winCheck(match);
		match.turn++;
	} while (match.status == IN_PROGRESS);
	if (match.status == PLAYER_WON)
		cout << "Вы победили, поздравляем!";
	else if (match.status == COMP_WON)
		cout << "Победил компьютер";
	else
		cout << "Ничья!";
	cout << endl;
	Sleep(5000);
}