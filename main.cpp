#include <iostream>
#include "Ai.h"

using namespace std;

void readData();
void saveData();
void hashAnswer();
char hashRead(char key, bool flag);
char hashSave(char key, bool flag);

string line_x, line_y;			// 인터페이스와 Board 클래스의 중간 역할
char answer_x, answer_y;		// 내가 둘 곳의 x좌표, y좌표

int main() {

	Board board;				// 현재 보드 상태 클래스
	MinMax minmax;				// 탐색 클래스

	readData();					// 현재 인터페이스에서 보드 상태를 읽기
	board.initialize(line_x);	// 읽어온 정보를 이용해 보드 만들기
	
	if (line_x.size() >= 25)	// 현재 보드에 돌이 놓여진 수가 많아질 수록 깊게 탐색
		minmax.depthLimit = 20;	// 컴퓨터의 성능에 따라 조절
	if (line_x.size() >= 27)
		minmax.depthLimit = 23;
	if (line_x.size() >= 30)
		minmax.depthLimit = 25;
	if (line_x.size() >= 34)
		minmax.depthLimit = 65;
	
	minmax.evaluate(board);		// 조절된 깊이 제한선을 갖고 탐색 시작
	
	answer_x = char(minmax.answerStone + 49);	// 최종적으로 돌을 놓을 x좌표
	answer_y = '1';								// y좌표는 아직 만들어지지 않은 상태
	
	saveData();									// x좌표와 y좌표를 완성하여 인터페이스에 저장
}

// 인터페이스에서 보드 상태 읽기
void readData() {

	FILE *fin = NULL;
	errno_t err;
	err = fopen_s(&fin, "C:/인공지능/an.sgf", "rt");

	char ch;

	if (err == 0) {
		while (ch = fgetc(fin)) {
			if (ch == EOF) break;							// 종료
			ch = fgetc(fin);								// B or W
			ch = fgetc(fin);								// [
			ch = fgetc(fin); line_x += hashRead(ch, true);	// x 좌표
			ch = fgetc(fin); line_y += hashRead(ch, false);	// y 좌표
			ch = fgetc(fin);								// ]
		}
		fclose(fin);
	}
	else {
		// 파일이 열리지 않았을 때
	}
	return;
}

// 인터페이스에 보드 상태 쓰기
void saveData() {

	FILE *fout = NULL;
	errno_t err;
	err = fopen_s(&fout, "C:/인공지능/re.sgf", "wt");

	if (err == 0) {
		for (unsigned int i = 0; i < line_x.length(); i++) {
			fputc(59, fout);							// ;
			if (i % 2 == 0) fputc(66, fout);			// B (RED)
			else            fputc(87, fout);			// W (YELLOW)
			fputc(91, fout);							// [
			fputc(hashSave(line_x[i], true), fout);		// x 좌표
			fputc(hashSave(line_y[i], false), fout);	// y 좌표
			fputc(93, fout);							// ]
		}

		/* ------------------------------------------------------ */

		hashAnswer();

		fputc(59, fout);		                        // ;
		if (line_x.length() % 2 == 0) fputc(66, fout);	// B (RED)
		else                          fputc(87, fout);	// W (YELLOW)
		fputc(91, fout);								// [
		fputc(answer_x, fout);							// x 좌표
		fputc(answer_y, fout);							// y 좌표
		fputc(93, fout);				                // ]

		fclose(fout);
	}
	else {
		// 파일이 열리지 않았을 때
	}
	return;
}

// 탐색 후 얻은 결과를, 인터페이스의 형식에 맞게 변환
void hashAnswer() {
	for (unsigned int i = 0; i < line_x.length(); i++) {
		if (line_x.at(i) == answer_x) {
			answer_y++;
		}
	}
	answer_x = hashSave(answer_x, true);
	answer_y = hashSave(answer_y, false);
}

// 인터페이스의 형식을, 나의 자료구조 형식에 맞게 변환
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

// 나의 자료구조 형식을, 인터페이스의 형식에 맞게 변환
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