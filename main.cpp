#include <iostream>
#include "Ai.h"

using namespace std;

void readData();
void saveData();
void hashAnswer();
char hashRead(char key, bool flag);
char hashSave(char key, bool flag);

string line_x, line_y;			// �������̽��� Board Ŭ������ �߰� ����
char answer_x, answer_y;		// ���� �� ���� x��ǥ, y��ǥ

int main() {

	Board board;				// ���� ���� ���� Ŭ����
	MinMax minmax;				// Ž�� Ŭ����

	readData();					// ���� �������̽����� ���� ���¸� �б�
	board.initialize(line_x);	// �о�� ������ �̿��� ���� �����
	
	if (line_x.size() >= 25)	// ���� ���忡 ���� ������ ���� ������ ���� ��� Ž��
		minmax.depthLimit = 20;	// ��ǻ���� ���ɿ� ���� ����
	if (line_x.size() >= 27)
		minmax.depthLimit = 23;
	if (line_x.size() >= 30)
		minmax.depthLimit = 25;
	if (line_x.size() >= 34)
		minmax.depthLimit = 65;
	
	minmax.evaluate(board);		// ������ ���� ���Ѽ��� ���� Ž�� ����
	
	answer_x = char(minmax.answerStone + 49);	// ���������� ���� ���� x��ǥ
	answer_y = '1';								// y��ǥ�� ���� ��������� ���� ����
	
	saveData();									// x��ǥ�� y��ǥ�� �ϼ��Ͽ� �������̽��� ����
}

// �������̽����� ���� ���� �б�
void readData() {

	FILE *fin = NULL;
	errno_t err;
	err = fopen_s(&fin, "C:/�ΰ�����/an.sgf", "rt");

	char ch;

	if (err == 0) {
		while (ch = fgetc(fin)) {
			if (ch == EOF) break;							// ����
			ch = fgetc(fin);								// B or W
			ch = fgetc(fin);								// [
			ch = fgetc(fin); line_x += hashRead(ch, true);	// x ��ǥ
			ch = fgetc(fin); line_y += hashRead(ch, false);	// y ��ǥ
			ch = fgetc(fin);								// ]
		}
		fclose(fin);
	}
	else {
		// ������ ������ �ʾ��� ��
	}
	return;
}

// �������̽��� ���� ���� ����
void saveData() {

	FILE *fout = NULL;
	errno_t err;
	err = fopen_s(&fout, "C:/�ΰ�����/re.sgf", "wt");

	if (err == 0) {
		for (unsigned int i = 0; i < line_x.length(); i++) {
			fputc(59, fout);							// ;
			if (i % 2 == 0) fputc(66, fout);			// B (RED)
			else            fputc(87, fout);			// W (YELLOW)
			fputc(91, fout);							// [
			fputc(hashSave(line_x[i], true), fout);		// x ��ǥ
			fputc(hashSave(line_y[i], false), fout);	// y ��ǥ
			fputc(93, fout);							// ]
		}

		/* ------------------------------------------------------ */

		hashAnswer();

		fputc(59, fout);		                        // ;
		if (line_x.length() % 2 == 0) fputc(66, fout);	// B (RED)
		else                          fputc(87, fout);	// W (YELLOW)
		fputc(91, fout);								// [
		fputc(answer_x, fout);							// x ��ǥ
		fputc(answer_y, fout);							// y ��ǥ
		fputc(93, fout);				                // ]

		fclose(fout);
	}
	else {
		// ������ ������ �ʾ��� ��
	}
	return;
}

// Ž�� �� ���� �����, �������̽��� ���Ŀ� �°� ��ȯ
void hashAnswer() {
	for (unsigned int i = 0; i < line_x.length(); i++) {
		if (line_x.at(i) == answer_x) {
			answer_y++;
		}
	}
	answer_x = hashSave(answer_x, true);
	answer_y = hashSave(answer_y, false);
}

// �������̽��� ������, ���� �ڷᱸ�� ���Ŀ� �°� ��ȯ
char hashRead(char key, bool isX) {
	char value;
	if (isX) {
		switch (key) {
		case 'a':
			value = '1';
			break;
		case 'b':
			value = '2';
			break;
		case 'c':
			value = '3';
			break;
		case 'd':
			value = '4';
			break;
		case 'e':
			value = '5';
			break;
		case 'f':
			value = '6';
			break;
		case 'g':
			value = '7';
			break;
		case 'h':
			value = '8';
			break;
		}
	}
	else {
		switch (key) {
		case 'a':
			value = '8';
			break;
		case 'b':
			value = '7';
			break;
		case 'c':
			value = '6';
			break;
		case 'd':
			value = '5';
			break;
		case 'e':
			value = '4';
			break;
		case 'f':
			value = '3';
			break;
		case 'g':
			value = '2';
			break;
		case 'h':
			value = '1';
			break;
		}
	}
	return value;
}

// ���� �ڷᱸ�� ������, �������̽��� ���Ŀ� �°� ��ȯ
char hashSave(char key, bool isX) {
	char value;
	if (isX) {
		switch (key) {
		case '1':
			value = 'a';
			break;
		case '2':
			value = 'b';
			break;
		case '3':
			value = 'c';
			break;
		case '4':
			value = 'd';
			break;
		case '5':
			value = 'e';
			break;
		case '6':
			value = 'f';
			break;
		case '7':
			value = 'g';
			break;
		case '8':
			value = 'h';
			break;
		}
	}
	else {
		switch (key) {
		case '1':
			value = 'h';
			break;
		case '2':
			value = 'g';
			break;
		case '3':
			value = 'f';
			break;
		case '4':
			value = 'e';
			break;
		case '5':
			value = 'd';
			break;
		case '6':
			value = 'c';
			break;
		case '7':
			value = 'b';
			break;
		case '8':
			value = 'a';
			break;
		}
	}
	return value;
}