#include "ChessGame.h"

#include <iostream>

#include "eval_consts.h"
#include "move_gen_sort.h"
#include "trans_table.h"

ChessGame::ChessGame()
{
	setDefaultEvalValues();
	initBoardHashData();
	oh.initVals();

	ss = SaamSlayer();
	b=Board();
	b.setStartBoard();
}

Move ChessGame::getAndApplyMove()//searches for, finds, and applys next move
{
	Move m = ss.getMove(b);
	b.makeMove(m);
	return m;
}

void ChessGame::printBoard()
{
	cout<<b<<endl;
}
bool ChessGame::whiteToMove()
{
	return b.toMove==WHITE;
}


OwnedPiece ChessGame::get(int row, int col)
{
	Square sq = MK_SQ(row,col);
//	cout<<"Sq:"<<(int)sq<<","<<row<<","<<col<<"  Val:"<<b.pieces[sq]<<","<<b.colors[sq]<<endl;
	return OwnedPiece(b.pieces[sq],b.colors[sq]).cleaned();
}

BoardStateList ChessGame::getValidChildStates()
{
	vector<BoardState> re = vector<BoardState>();
	MoveGenerator gen;
	BoardState s = BoardState();

	gen.genPLegalMoves(b,b.toMove);
	for(int i = 0; i < gen.ct; i++)
	{
		Move move=gen.moves[i].m;
		b.makeMove(move);
		if(!b.isCheck(!b.toMove) && (!move.isPromo || move.toPieceNew==QUEEN))
		{
			s=BoardState(b,move);
			re.push_back(s);
		}
		b.unmakeMove(move);
	}
	return BoardStateList(re);
}

//for now, always promot to a queen
bool ChessGame::isValidMoveAlgebraic(int rowStart, int colStart, int rowEnd, int colEnd)
{
	Move m = makeMoveSlow(b,MK_SQ(rowStart,colStart),MK_SQ(rowEnd,colEnd),'Q');
	return(testMoveIsLegalSlow(b,m));
}
bool ChessGame::applyMoveAlgebraic(int rowStart, int colStart, int rowEnd, int colEnd)
{
	Move m = makeMoveSlow(b,MK_SQ(rowStart,colStart),MK_SQ(rowEnd,colEnd),'Q');
	bool re = testMoveIsLegalSlow(b,m);
	if(re)
		b.makeMove(m);
	return re;
}

///returns true if this turn castling would be a valid move (considering both castling rights,
//occupation of inbetween squares, and attack of inbetween squares)
bool ChessGame::shouldConsiderCastlePlayerKingsideQ(Player player, bool kingsideQ)
{
	return testMoveIsLegalSlow(b,makeMoveCastle(player,kingsideQ));
}

//
//bool ChessGame::isValidNextBoard(std::vector<int> piece, std::vector<int> color)//row*8+col
//{
//	if(piece.size()!=64||color.size()!=64)
//	{
//		cout<<"MUST HAVE ARRAY SIZE 64"<<endl;
//		return false;
//	}
//	for(int row = 0; row < 8; row++)
//	{
//		for(int col = 0; col < 8; col++)
//		{
//			Square sq120 = MK_SQ(row,col);
//			int sq64 = row*8+;
//			if(b.pieces[sq120]!=piece[sq64] /*pieces are different*/ ||
//					(b.pieces[sq120]!=NO_PIECE &&
//							b.colors[sq120]!=color[sq64]))
//			{
//				return false;
//			}
//		}
//	}
//	return true;
//}


