
#ifndef SRC_CHESSGAME_H_
#define SRC_CHESSGAME_H_

#include <vector>
#include "SaamSlayer.h"

class CGBoard
{

};

class ChessGame
{
	Board b;
	SaamSlayer ss;

public:
	ChessGame();
	Move getAndApplyMove();//searches for, finds, and applys next move
	void printBoard();
	//should be len 64 vectors. board as array, piece encoding same
	bool isValidNextBoard(std::vector<int> piece, std::vector<int> colors);
};



#endif /* SRC_CHESSGAME_H_ */
