#include <iostream>
#include <array>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <string>

#define number_cells 9

typedef unsigned short int ushort;

bool START, EXIT;
bool r_start;
bool player_1, player_2;

void filling_array(std::array<char, number_cells> &field, ushort, bool&, bool&, char&);
void drawing(std::array<char, number_cells>, ushort, bool, bool, char, char);
void draw_cell(std::array<char, number_cells>, ushort, ushort, bool);
void draw_field(char, ushort s = 0);
void draw_information_start(ushort);
void draw_information_win(ushort, char);
void draw_information(ushort, bool, bool, char, char);
void draw_init_choice(char, char, char);
void init_choice(ushort);
ushort press_key(std::array<char, number_cells> &field, ushort&, bool&, bool&, char&, char&);
ushort press_key_choice(std::array<char, number_cells> &field, ushort&, bool&, bool&, char&, char&, ushort&);
ushort processing_combinations(std::array<char, number_cells>, char&, char&, bool&);
bool search_win_combination(std::array<char, number_cells>, char);
ushort ai(std::array<char, number_cells> &field, char&);

void drawf(char *text, int count, int color)
{
	while (count--) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		printf("%s", text);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);

	return;
}

void restart(std::array<char, number_cells> &field, ushort& position, bool &busy_cell, bool &pc_move, char &player_move, char &player_winner)
{
	srand((unsigned)time(NULL));
	if (r_start == false)
		r_start = true;

	filling_array(field, 0, busy_cell, pc_move, player_move);
	player_winner = ' ';
	player_move = 'X';
	if (player_1) {
		(rand() & 1) ? pc_move = 1 : pc_move = 0;
	}
	else
		pc_move = false;
	position = 5;

	if (r_start)
		r_start = false;

	return;
}

int main()
{
	system("color f0");
	srand((unsigned)time(NULL));
	setlocale(0, "");
	ushort choise = 1;
	bool busy_cell = false;
	bool pc_move = false;
	char player_move = 'X';
	char player_winner = 0x20;
	std::array<char, number_cells>field = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	filling_array(field, 0, busy_cell, pc_move, player_move);
	ushort position = 5;

	if (player_1) {
		(rand() & 1) ? pc_move = 1 : pc_move = 0;
	}

	while (!EXIT) {
		while (1) {
			init_choice(choise);
			press_key_choice(field, position, busy_cell, pc_move, player_move, player_winner, choise);
			if (START || EXIT)
				break;
		}
		while (START) {
			drawing(field, position, busy_cell, pc_move, player_move, player_winner);
			if (player_1&&pc_move) {
				ushort position_temp = position;
				position = ai(field, player_move);
				filling_array(field, position, busy_cell, pc_move, player_move);
				processing_combinations(field, player_move, player_winner, pc_move);
				pc_move = false;
				Sleep(3000);
				position = position_temp;
				drawing(field, position, busy_cell, pc_move, player_move, player_winner);
			}
			if (busy_cell) {
				busy_cell = false;
				Sleep(1500);
				drawing(field, position, busy_cell, pc_move, player_move, player_winner);
			}
			position = press_key(field, position, busy_cell, pc_move, player_move, player_winner);
			processing_combinations(field, player_move, player_winner, pc_move);
		}
	}

	return 0;
}

void filling_array(std::array<char, number_cells> &field, ushort position, bool &busy_cell, bool &pc_move, char &player_move)
{
	if (player_move == 0x20 && !r_start)
		return;
	char player = player_move;
	if (position != 0) {
		if (field.at(position - 1) != 0x20) {
			busy_cell = true;
		}
		else {
			field.at(position - 1) = player;
			if (player_1&&pc_move == false)
				pc_move = true;
			{
				if (player == 'X')
					player = '0';
				else
					player = 'X';
			}
		}
	}
	else {
		for (int i(0); i<number_cells; ++i)
			field.at(i) = ' ';
	}
	player_move = player;

	return;
}

void drawing(std::array<char, number_cells> field, ushort position, bool busy_cell, bool pc_move, char player_move, char player_winner)
{
	system("cls");
	int counter = 0;
	draw_information_start(24);
	draw_field(0105, 6);
	for (int i(0); i<3; ++i) {
		draw_field(0123, 32);
		draw_cell(field, position, ++counter, pc_move);
		draw_field(0166);
		draw_cell(field, position, ++counter, pc_move);
		draw_field(0166);
		draw_cell(field, position, ++counter, pc_move);
		draw_field(0145);
		if (i<2) {
			draw_field(0123, 32);
			draw_field(0150);
			draw_field(0145);
		}
	}
	draw_information(15, busy_cell, pc_move, player_move, player_winner);

	return;
}

void draw_cell(std::array<char, number_cells> field, ushort position, ushort value, bool pc_move) {
	char *s_r = ">", *s_l = "<", *s_X = "X", *s_0 = "0", *s_p = " ";

	if (position == value && !pc_move) {
		drawf(s_r, 1, 240);
		if (field.at(value - 1) == 'X')
		{
			drawf(s_X, 1, 252);
		}
		else
			if (field.at(value - 1) == '0')
			{
				drawf(s_0, 1, 249);
			}
			else
				if (field.at(value - 1) == ' ')
				{
					drawf(s_p, 1, 240);
				}
		drawf(s_l, 1, 240);
	}
	else {
		drawf(s_p, 1, 240);
		if (field.at(value - 1) == 'X')
		{
			drawf(s_X, 1, 252);
		}
		else
			if (field.at(value - 1) == '0')
			{
				drawf(s_0, 1, 249);
			}
			else
				if (field.at(value - 1) == ' ')
				{
					drawf(s_p, 1, 240);
				}
		drawf(s_p, 1, 240);
	}

	return;
}

void draw_field(char symbol, ushort s)
{
	if (symbol == 'v') {
		printf("%c", 0x7c);
	}
	else
		if (symbol == 'h') {
			printf("%c%c%c", 0x2d, 0x2d, 0x2d);
			printf("%c", 0x2b);
			printf("%c%c%c", 0x2d, 0x2d, 0x2d);
			printf("%c", 0x2b);
			printf("%c%c%c", 0x2d, 0x2d, 0x2d);
		}
		else
			if (symbol == 'e') {
				printf("\n");
			}
			else {
				if (symbol == 'S') {
					for (int i(0); i<s; ++i)
						printf("%c", 0x20);
				}
				else
					if (symbol == 'E') {
						for (int i(0); i<s; ++i)
							printf("\n");
					}
			}

			return;
}

void draw_information_start(ushort s)
{
	for (int i(0); i<s; ++i)
		printf("%c", 0x20);
	drawf("*** Игра крестики-нолики ***", 1, 247);

	return;
}

void draw_information_win(ushort s, char player_winner)
{
	if (player_winner == 'X')
		drawf("Победа крестиков!\n", 1, 252); else
		if (player_winner == '0')
			drawf("Победа ноликов!\n", 1, 249); else
			drawf("Ничья!\n", 1, 247);
	draw_field(0105, 1);
	draw_field(0123, s);
	drawf("Чтобы начать снова, нажмите: ", 1, 247);
	printf("N\n");
	draw_field(0123, s);
	drawf("Чтобы покинуть игру, нажмите: ", 1, 247);
	printf("esc\n");

	return;
}

void draw_information(ushort s, bool busy_cell, bool pc_move, char player_move, char player_winner)
{
	printf("\n");
	for (int i(0); i<s; ++i)
		printf("%c", 0x20);
	if (!busy_cell) {
		if (!player_2) {
			if (player_move != ' '&&player_winner == ' '&&pc_move == 1)
				drawf("Ход компьютера...\n", 1, 247); else
				if (player_move != ' '&&player_winner == ' '&&pc_move == 0)
					drawf("Ваш ход...\n", 1, 247); else
					if (player_move == ' ')
						draw_information_win(15, player_winner);
		}
		else {
			if (player_move == 'X')
				drawf("Ходят крестики...\n", 1, 247); else
				if (player_move == '0')
					drawf("Ходят нолики...\n", 1, 247); else
					if (player_move == ' ')
						draw_information_win(15, player_winner);
		}
	}
	else
		drawf("Эта ячейка уже занята!\n", 1, 247);

	return;
}

void draw_init_choice(char s1, char s2, char s3)
{
	char *game_pc_txt = " игра с компьютером";
	char *game_player_txt = " игра для двух игроков";
	char *game_exit_txt = " выход из игры";

	draw_field(0123, 26);
	if (s1 == '>')
		printf("%c%s\n", s1, game_pc_txt); else
	{
		printf("%c", s1);
		drawf(game_pc_txt, 1, 247);
		draw_field(0145);
	}
	draw_field(0145);

	draw_field(0123, 26);
	if (s2 == '>')
		printf("%c%s\n", s2, game_player_txt); else
	{
		printf("%c", s2);
		drawf(game_player_txt, 1, 247);
		draw_field(0145);
	}
	draw_field(0145);

	draw_field(0123, 26);
	if (s3 == '>')
		printf("%c%s\n", s3, game_exit_txt); else
	{
		printf("%c", s3);
		drawf(game_exit_txt, 1, 247);
		draw_field(0145);
	}
	draw_field(0145);

	return;
}

void init_choice(ushort choise)
{
	system("cls");
	char s_r = '>';
	char s_p = ' ';
	draw_information_start(24);
	draw_field(0105, 5);
	if (choise == 1)
		draw_init_choice(s_r, s_p, s_p); else
		if (choise == 2)
			draw_init_choice(s_p, s_r, s_p); else
			if (choise == 3)
				draw_init_choice(s_p, s_p, s_r);

	return;
}

ushort press_key(std::array<char, number_cells> &field, ushort &position, bool &busy_cell, bool &pc_move, char &player_move, char &player_winner)
{
	int key;
	key = _getch();
	if (key == 72 || key == 80 || key == 75 || key == 77) {
		switch (position) {
		case 1:
		{
			if (key == 72) { position = 7; }
			else
				if (key == 80) { position = 4; }
				else
					if (key == 75) { position = 3; }
					else
						if (key == 77) { position = 2; }
		}; break;
		case 2:
		{
			if (key == 72) { position = 8; }
			else
				if (key == 80) { position = 5; }
				else
					if (key == 75) { position = 1; }
					else
						if (key == 77) { position = 3; }
		}; break;
		case 3:
		{
			if (key == 72) { position = 9; }
			else
				if (key == 80) { position = 6; }
				else
					if (key == 75) { position = 2; }
					else
						if (key == 77) { position = 1; }
		}; break;
		case 4:
		{
			if (key == 72) { position = 1; }
			else
				if (key == 80) { position = 7; }
				else
					if (key == 75) { position = 6; }
					else
						if (key == 77) { position = 5; }
		}; break;
		case 5:
		{
			if (key == 72) { position = 2; }
			else
				if (key == 80) { position = 8; }
				else
					if (key == 75) { position = 4; }
					else
						if (key == 77) { position = 6; }
		}; break;
		case 6:
		{
			if (key == 72) { position = 3; }
			else
				if (key == 80) { position = 9; }
				else
					if (key == 75) { position = 5; }
					else
						if (key == 77) { position = 4; }
		}; break;
		case 7:
		{
			if (key == 72) { position = 4; }
			else
				if (key == 80) { position = 1; }
				else
					if (key == 75) { position = 9; }
					else
						if (key == 77) { position = 8; }
		}; break;
		case 8:
		{
			if (key == 72) { position = 5; }
			else
				if (key == 80) { position = 2; }
				else
					if (key == 75) { position = 7; }
					else
						if (key == 77) { position = 9; }
		}; break;
		case 9:
		{
			if (key == 72) { position = 6; }
			else
				if (key == 80) { position = 3; }
				else
					if (key == 75) { position = 8; }
					else
						if (key == 77) { position = 7; }
		}; break;
		};
	}
	else {
		if (key == 13)
			filling_array(field, position, busy_cell, pc_move, player_move); else
			if (key == 32)
				filling_array(field, position, busy_cell, pc_move, player_move); else
				if (key == 110)
					restart(field, position, busy_cell, pc_move, player_move, player_winner); else
					if (key == 27) {
						r_start = true;
						START = false;
					}
	}

	return position;
}

ushort press_key_choice(std::array<char, number_cells> &field, ushort &position, bool &busy_cell, bool &pc_move, char &player_move, char &player_winner, ushort &choise)
{
	int key;
	key = _getch();
	if (key == 72 && choise>1)
		choise--; else
		if (key == 80 && choise<3)
			choise++; else
			if (key == 13 || key == 32) {
				if (choise == 1) {
					START = 1;
					player_1 = true;
					player_2 = false;
					if (r_start)
						restart(field, position, busy_cell, pc_move, player_move, player_winner);
				}
				else
					if (choise == 2) {
						START = 1;
						player_1 = false;
						player_2 = true;
						if (r_start)
							restart(field, position, busy_cell, pc_move, player_move, player_winner);
					}
					else
						if (choise == 3) {
							EXIT = 1;
						}
			}

	return 0;
}

ushort processing_combinations(std::array<char, number_cells> field, char &player_move, char &player_winner, bool &pc_move)
{
	if (player_move != ' ') {
		bool winning = false;
		winning = search_win_combination(field, player_move);
		if (winning) {
			if (player_move == 'X')
				player_winner = '0';
			else
				player_winner = 'X';
			player_move = ' ';
			pc_move = 0;
			return 0;
		}

		int count = 0;
		for (int i(0); i<number_cells; ++i) {
			if (field.at(i) != ' ') {
				count++;
			}
		}
		if (count == number_cells) {
			player_move = ' ';
			pc_move = 0;
		}
	}

	return 0;
}

bool search_win_combination(std::array<char, number_cells> field, char last_move)
{
	if (last_move == 'X') { last_move = '0'; }
	else
		if (last_move == '0') { last_move = 'X'; }

	if (((field.at(0) == last_move) && (field.at(1) == last_move) && (field.at(2) == last_move)) ||
		((field.at(3) == last_move) && (field.at(4) == last_move) && (field.at(5) == last_move)) ||
		((field.at(6) == last_move) && (field.at(7) == last_move) && (field.at(8) == last_move)))
		return true;

	if (((field.at(0) == last_move) && (field.at(3) == last_move) && (field.at(6) == last_move)) ||
		((field.at(1) == last_move) && (field.at(4) == last_move) && (field.at(7) == last_move)) ||
		((field.at(2) == last_move) && (field.at(5) == last_move) && (field.at(8) == last_move)))
		return true;

	if (((field.at(0) == last_move) && (field.at(4) == last_move) && (field.at(8) == last_move)) ||
		((field.at(2) == last_move) && (field.at(4) == last_move) && (field.at(6) == last_move)))
		return true;

	return false;
}

void overwrite_comb(std::array<char, number_cells> field, char **comb)
{
	comb[0][0] = field.at(0); comb[0][1] = field.at(1); comb[0][2] = field.at(2);
	comb[1][0] = field.at(3); comb[1][1] = field.at(4); comb[1][2] = field.at(5);
	comb[2][0] = field.at(6); comb[2][1] = field.at(7); comb[2][2] = field.at(8);
	comb[3][0] = field.at(0); comb[3][1] = field.at(3); comb[3][2] = field.at(6);
	comb[4][0] = field.at(1); comb[4][1] = field.at(4); comb[4][2] = field.at(7);
	comb[5][0] = field.at(2); comb[5][1] = field.at(5); comb[5][2] = field.at(8);
	comb[6][0] = field.at(2); comb[6][1] = field.at(4); comb[6][2] = field.at(6);
	comb[7][0] = field.at(0); comb[7][1] = field.at(4); comb[7][2] = field.at(8);

	return;
}

void overwrite_index(int **ind)
{
	ind[0][0] = 1; ind[0][1] = 2; ind[0][2] = 3;
	ind[1][0] = 4; ind[1][1] = 5; ind[1][2] = 6;
	ind[2][0] = 7; ind[2][1] = 8; ind[2][2] = 9;
	ind[3][0] = 1; ind[3][1] = 4; ind[3][2] = 7;
	ind[4][0] = 2; ind[4][1] = 5; ind[4][2] = 8;
	ind[5][0] = 3; ind[5][1] = 6; ind[5][2] = 9;
	ind[6][0] = 3; ind[6][1] = 5; ind[6][2] = 7;
	ind[7][0] = 1; ind[7][1] = 5; ind[7][2] = 9;

	return;
}

ushort ai(std::array<char, number_cells> &field, char &player_move)
{
	ushort move;
	srand((unsigned)time(NULL));
	char player, pc;
	if (player_move == 'X')
	{
		pc = 'X'; player = '0';
	}
	else
	{
		pc = '0'; player = 'X';
	}

	int count = 0;
	int array_moves[number_cells - 1];

	int **ind = (int**)malloc((number_cells - 1) * sizeof(int*));
	for (int i(0); i<number_cells - 1; ++i)
		*(ind + i) = (int*)malloc(3 * sizeof(int));

	char **comb = (char**)malloc((number_cells - 1) * sizeof(char*));
	for (int i(0); i<number_cells - 1; ++i)
		*(comb + i) = (char*)malloc(3 * sizeof(char));

	for (int i(0); i<number_cells - 1; ++i) {
		array_moves[i] = NULL;
		for (int j(0); j<3; ++j) {
			ind[i][j] = NULL;
			comb[i][j] = NULL;
		}
	}

	overwrite_index(ind);
	overwrite_comb(field, comb);

	bool move_pc = false;

	if (!move_pc) {
		for (int i(0); i<number_cells - 1; ++i) {
			if (comb[i][0] == pc&&comb[i][1] == pc&&comb[i][2] == ' ')
			{
				array_moves[count++] = ind[i][2]; move_pc = true;
			}
			else
				if (comb[i][0] == pc&&comb[i][1] == ' '&&comb[i][2] == pc)
				{
					array_moves[count++] = ind[i][1]; move_pc = true;
				}
				else
					if (comb[i][0] == ' '&&comb[i][1] == pc&&comb[i][2] == pc)
					{
						array_moves[count++] = ind[i][0]; move_pc = true;
					}
		}
		count = 0;
	}

	if (!move_pc) {
		for (int i(0); i<number_cells - 1; ++i) {
			if (comb[i][0] == player&&comb[i][1] == player&&comb[i][2] == ' ')
			{
				array_moves[count++] = ind[i][2]; move_pc = true;
			}
			else
				if (comb[i][0] == player&&comb[i][1] == ' '&&comb[i][2] == player)
				{
					array_moves[count++] = ind[i][1]; move_pc = true;
				}
				else
					if (comb[i][0] == ' '&&comb[i][1] == player&&comb[i][2] == player)
					{
						array_moves[count++] = ind[i][0]; move_pc = true;
					}
		}
		count = 0;
	}

	if (!move_pc) {
		for (int i(0); i<number_cells - 1; ++i) {
			if (comb[i][0] == pc&&comb[i][1] == ' '&&comb[i][2] == ' ') {
				if (rand() & 1)
				{
					array_moves[count++] = ind[i][1]; move_pc = true;
				}
				else
				{
					array_moves[count++] = ind[i][2]; move_pc = true;
				}
			}
			else
				if (comb[i][0] == ' '&&comb[i][1] == pc&&comb[i][2] == ' ') {
					if (rand() & 1)
					{
						array_moves[count++] = ind[i][0]; move_pc = true;
					}
					else
					{
						array_moves[count++] = ind[i][2]; move_pc = true;
					}
				}
				else
					if (comb[i][0] == ' '&&comb[i][1] == ' '&&comb[i][2] == pc) {
						if (rand() & 1)
						{
							array_moves[count++] = ind[i][0]; move_pc = true;
						}
						else
						{
							array_moves[count++] = ind[i][1]; move_pc = true;
						}
					}
		}
		count = 0;
	}

	if (!move_pc) {
		int val = 0;
		for (int i(0); i<number_cells; ++i) {
			if (field.at(i) == ' ')
				count++;
		}

		if (count == number_cells) {
			count = 0;
			val = rand() % number_cells + 1;
		}
		else
			count = 0;
		for (int i(0); i<number_cells; ++i) {
			if (val&&i == val - 1)
				continue;
			if (field.at(i) == ' ') {
				array_moves[count++] = i + 1;
				move_pc = true;
			}
		}
		count = 0;
	}

	move_pc = false;

	for (int i(0); i<number_cells - 1; ++i)
		free(ind[i]);
	free(ind);
	for (int i(0); i<number_cells - 1; ++i)
		free(comb[i]);
	free(comb);

	bool flag = false;
	int value;
	int tmp[number_cells] = { NULL };
	std::sort(array_moves, array_moves + number_cells - 1);

	for (int i(1); i<number_cells - 1; i++) {
		if (array_moves[i - 1] == array_moves[i] && array_moves[i - 1] != NULL&&array_moves[i] != NULL) {
			flag = true;
			break;
		}
	}

	count = 0;
	for (int i(1); i <= number_cells; i++) {
		for (int n(0); n<number_cells - 1; n++) {
			if (array_moves[n] == i)
				count++;
		}
		if (count>1 && flag)
		{
			tmp[i - 1] = count;
		}
		else
			if (count == 1 && !flag)
			{
				tmp[i - 1] = count;
			}
		count = 0;
	}

	int num_MAX = 0;
	int MAX = tmp[0];
	for (int i(1); i<number_cells; i++) {
		if (tmp[i]>MAX)
			MAX = tmp[i];
	}
	for (int i(0); i<number_cells; i++) {
		if (tmp[i] == MAX) {
			num_MAX += 1;
		}
	}
	{
		int count = 0;
		int *arr = new int[num_MAX];
		for (int i(0); i<number_cells; i++) {
			if (tmp[i] == MAX)
				arr[count++] = i + 1;
		}
		value = arr[rand() % num_MAX];
		delete[] arr;
	}
	move = value;

	return move;
}
