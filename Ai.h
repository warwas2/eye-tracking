#include <iostream>
#include <cassert>
using namespace std;

// ���� ���� ���� Ŭ����
class Board {

	public:
		static const int WIDTH  = 8;		// Board�� ���� ����
		static const int HEIGHT = 8;		// Board�� ���� ����
		int board[WIDTH][HEIGHT];			// 2���� �迭 (���� ����)
		int height[WIDTH];					// �� column�� �� �ִ� ���� ����
		int count;							// ���� ���� Ƚ��

		// �������̽����� �޾ƿ� ���� ������ ���¸� ī�� (�ʱ�ȭ)
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
		
		// �� ���� ������ x��ǥ�� ���� ��
		void putStone(int x) {
			board[x][height[x]] = 1 + count % 2;
			height[x]++;
			count++;
		}
		
		// �� ���� ������ x��ǥ�� ���� ���� ������ �������� �Ǵ�
		bool isPossibleMove(int col) const {
			return height[col] < HEIGHT;
		}
		
		// �� ���� ������ x��ǥ�� ���� ���� �¸��� �� �ִ��� �Ǵ�
		bool isWinnableMove(int col) const {

			int myColor = 1 + (count % 2);

			// ���η� ���� 4���� ����Ǿ� �¸��ϴ� �� �Ǵ�
			if (height[col] >= 3
				&& board[col][height[col] - 1] == myColor
				&& board[col][height[col] - 2] == myColor
				&& board[col][height[col] - 3] == myColor) {
				return true;
			}

			// ���� Ȥ�� �밢������ ���� 4���� ����Ǵ� �� �Ǵ�
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
		
		// (������) �ʱ�ȭ : ���� ����, ������ �� x��ǥ�� ����, ���� �� Ƚ��
		Board() : board{ 0 }, height{ 0 } {
			count = 0;
		}
};

// Ž�� Ŭ����
class MinMax {

	public:
		int answerStone;		// ���������� ���� ���� x��ǥ
		int depthLimit;			// MinMax�� ���� ���Ѽ�
		int seqExpand[8];		// ��带 Ȯ�� �� ���� (�߾�)
	
		// ����-��Ÿ prunning�� ����Ͽ�, ��带 Ȯ���ϴ� minmax
		int expandNode(const Board &currentNode, int alpha, int beta, int depth) {

			// ���� ���� ���� Ƚ���� 64�� ��� (���尡 �� �� ��Ȳ)
			if (currentNode.count == 8*8) {
				return 0;
			}

			// �� ���� ������ x��ǥ�� ���� ���� �� �ְ�, �¸��� �� ���� ���
			for (int x = 0; x < 8; x++) {
				if (currentNode.isPossibleMove(x) && currentNode.isWinnableMove(x)) {
					if (depth == 1) {
						answerStone = x;
					}
					return (8*8 + 1 - currentNode.count) / 2;
				}
			}

			// beta �� ������ �ִ밪 ���� ū ���
			int scoreMax = (8*8 - 1 - currentNode.count) / 2;
			if (beta > scoreMax) {
				beta = scoreMax;
				if (alpha >= beta) {
					return beta;
				}
			}

			// ��Ʈ���� Ȯ��� �ڽĵ��� ���� ( = ���̰� 1�� ��)
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
			// ��Ʈ�̿��� ��忡�� Ȯ��� �ڽĵ��� ���� ( = ���̰� 1���� Ŭ ��)
			// �ڽ��� ���̸� ��Ÿ���� depth�� ���Ѽ��� �Ѿ ��쿡�� ��带 Ȯ������ ����
			// �� ���̺��� ���̸� �켱�Ͽ� ��带 Ȯ��
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
	
		// ��Ʈ���� ��带 Ȯ��, �� minmax�� ���� (���̰� 1)
		// ��Ʈ�� ���̰� 0 �̹Ƿ�, ��Ʈ���� Ȯ��� �ڽĵ��� ���̴� 1 ( = rootDepth + 1)
		int evaluate(const Board &root) {
			int rootDepth = 0;
			return expandNode(root, -8*8 / 2, 8*8 / 2, rootDepth + 1);
		}
	
		// (������) �ʱ�ȭ : ���������� ���� ���� x��ǥ, MinMax�� ���� ���Ѽ�, ��带 Ȯ���� ����
		MinMax() : seqExpand{ 4, 3, 5, 2, 6, 1, 7, 0 } {
			answerStone = -1;					// -1�� x��ǥ���� ���� ���� �� �����Ƿ�
			depthLimit = 10;					// ��ǻ���� ������ ���� ��� ũ�⸦ Ű���� ����
		}
};