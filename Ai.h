#include <iostream>
#include <cassert>
using namespace std;

// 현재 보드 상태 클래스
class Board {

	public:
		static const int WIDTH  = 8;		// Board의 가로 길이
		static const int HEIGHT = 8;		// Board의 세로 길이
		int board[WIDTH][HEIGHT];			// 2차원 배열 (현재 상태)
		int height[WIDTH];					// 각 column에 차 있는 돌의 높이
		int count;							// 돌이 놓인 횟수

		// 인터페이스에서 받아온 현재 게임의 상태를 카피 (초기화)
		bool initialize(string init) {
			for (unsigned int i = 0; i < init.size(); i++) {
				int col = init[i] - '1';
				if (col < 0 || col >= WIDTH || !isPossibleMove(col) || isWinnableMove(col)) {
					return false;
				}
				putStone(col);
			}
			return true;
		}
		
		// 현 상태 보드의 x좌표에 돌을 둠
		void putStone(int x) {
			board[x][height[x]] = 1 + count % 2;
			height[x]++;
			count++;
		}
		
		// 현 상태 보드의 x좌표에 돌을 놓는 행위가 가능한지 판단
		bool isPossibleMove(int col) const {
			return height[col] < HEIGHT;
		}
		
		// 현 상태 보드의 x좌표에 돌을 놓아 승리할 수 있는지 판단
		bool isWinnableMove(int col) const {

			int myColor = 1 + (count % 2);

			// 세로로 돌이 4개가 연결되어 승리하는 것 판단
			if (height[col] >= 3
				&& board[col][height[col] - 1] == myColor
				&& board[col][height[col] - 2] == myColor
				&& board[col][height[col] - 3] == myColor) {
				return true;
			}

			// 가로 혹은 대각선으로 돌이 4개가 연결되는 것 판단
			for (int winY = -1; winY <= 1; winY++) {
				int stone = 0;
				for (int winX = -1; winX <= 1; winX += 2) {
					for (int x = col + winX, y = height[col] + winX*winY;
						x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && board[x][y] == myColor;
						stone++) {
						x += winX;
						y += winX*winY;
					}
				}
				if (stone >= 3) {
					return true;
				}
			}
			return false;
		}
		
		// (생성자) 초기화 : 상태 보드, 보드의 각 x좌표의 높이, 돌을 둔 횟수
		Board() : board{ 0 }, height{ 0 } {
			count = 0;
		}
};

// 탐색 클래스
class MinMax {

	public:
		int answerStone;		// 최종적으로 돌을 놓을 x좌표
		int depthLimit;			// MinMax의 깊이 제한선
		int seqExpand[8];		// 노드를 확장 할 순서 (중앙)
	
		// 알파-베타 prunning을 사용하여, 노드를 확장하는 minmax
		int expandNode(const Board &currentNode, int alpha, int beta, int depth) {

			// 현재 돌이 놓인 횟수가 64일 경우 (보드가 꽉 찬 상황)
			if (currentNode.count == 8*8) {
				return 0;
			}

			// 현 상태 보드의 x좌표에 돌을 놓을 수 있고, 승리할 수 있을 경우
			for (int x = 0; x < 8; x++) {
				if (currentNode.isPossibleMove(x) && currentNode.isWinnableMove(x)) {
					if (depth == 1) {
						answerStone = x;
					}
					return (8*8 + 1 - currentNode.count) / 2;
				}
			}

			// beta 가 점수의 최대값 보다 큰 경우
			int scoreMax = (8*8 - 1 - currentNode.count) / 2;
			if (beta > scoreMax) {
				beta = scoreMax;
				if (alpha >= beta) {
					return beta;
				}
			}

			// 루트에서 확장된 자식들의 동작 ( = 깊이가 1일 때)
			if (depth == 1) {
				int scoreAtDepthOne[8];
				for (int x = 0; x < 8; x++) {
					if (currentNode.isPossibleMove(seqExpand[x])) {
						Board nextNode(currentNode);
						nextNode.putStone(seqExpand[x]);
						int score = -expandNode(nextNode, -beta, -alpha, depth + 1);
						scoreAtDepthOne[seqExpand[x]] = score;
						if (score >= beta) {
							if (depth == 1) {
								answerStone = seqExpand[x];
							}
							return score;
						}
						if (score > alpha) {
							alpha = score;
						}
					}
					else {
						scoreAtDepthOne[seqExpand[x]] = -100;
					}
				}
				int bestScore = scoreAtDepthOne[seqExpand[0]];
				answerStone = seqExpand[0];
				for (int i = 1; i < 8; i++) {
					if (bestScore < scoreAtDepthOne[seqExpand[i]]) {
						bestScore = scoreAtDepthOne[seqExpand[i]];
						answerStone = seqExpand[i];
					}
					
				}
			}
			// 루트이외의 노드에서 확장된 자식들의 동작 ( = 깊이가 1보다 클 때)
			// 자신의 깊이를 나타내는 depth가 제한선을 넘어설 경우에는 노드를 확장하지 않음
			// 즉 깊이보다 넓이를 우선하여 노드를 확장
			else if (depth < depthLimit) {
				for (int x = 0; x < 8; x++) {
					if (currentNode.isPossibleMove(seqExpand[x])) {
						Board nextNode(currentNode);
						nextNode.putStone(seqExpand[x]);
						int score = -expandNode(nextNode, -beta, -alpha, depth + 1);
						if (score >= beta) {
							return score;
						}
						if (score > alpha) {
							alpha = score;
						}
					}
				}
			}
			return alpha;
		}
	
		// 루트에서 노드를 확장, 즉 minmax의 시작 (깊이가 1)
		// 루트의 깊이가 0 이므로, 루트에서 확장된 자식들의 깊이는 1 ( = rootDepth + 1)
		int evaluate(const Board &root) {
			int rootDepth = 0;
			return expandNode(root, -8*8 / 2, 8*8 / 2, rootDepth + 1);
		}
	
		// (생성자) 초기화 : 최종적으로 돌을 놓을 x좌표, MinMax의 깊이 제한선, 노드를 확장할 순서
		MinMax() : seqExpand{ 4, 3, 5, 2, 6, 1, 7, 0 } {
			answerStone = -1;					// -1의 x좌표에는 돌을 놓을 수 없으므로
			depthLimit = 10;					// 컴퓨터의 성능이 좋을 경우 크기를 키워도 가능
		}
};