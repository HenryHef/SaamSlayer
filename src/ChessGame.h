
#ifndef SRC_CHESSGAME_H_
#define SRC_CHESSGAME_H_

#include <vector>

#include "basedef.h"
#include "board.h"
#include "ChessGameBoardState.h"
#include "SaamSlayer.h"

class ChessGame
{
private:
	Board b;
	SaamSlayer ss;
public:
	ChessGame();
	Move getAndApplyMove();//searches for, finds, and applys next move
	bool whiteToMove();

	void printBoard();

	OwnedPiece get(int row, int col);
	BoardStateList getValidChildStates();

	//for now, always promot to a queen
	bool isValidMoveAlgebraic(int rowStart, int colStart, int rowEnd, int colEnd);
	bool applyMoveAlgebraic(int rowStart, int colStart, int rowEnd, int colEnd);

	///returns true if this turn castling would be a valid move (considering both castling rights,
	//occupation of inbetween squares, and attack of inbetween squares)
	bool shouldConsiderCastlePlayerKingsideQ(Player player, bool kingsideQ);


	//should be len 64 vectors. board as array, piece encoding same
	//bool isValidNextBoard(std::vector<int> piece, std::vector<int> colors);
};

#endif /* SRC_CHESSGAME_H_ */
