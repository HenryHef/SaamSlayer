#include "ChessGame.h"

#include "board.h"

ChessGame::ChessGame()
{
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

bool ChessGame::isValidNextBoard(std::vector<int> piece, std::vector<int> color)//row*8+col
{
	if(piece.size()!=64||color.size()!=64)
	{
		cout<<"MUST HAVE ARRAY SIZE 64"<<endl;
		return false;
	}
	for(int row = 0; row < 8; row++)
	{
		for(int col = 0; col < 8; col++)
		{
			Square sq120 = MK_SQ(row,col);
			int sq64 = row*8+col;
			if(b.pieces[sq120]!=piece[sq64] /*pieces are different*/ ||
					(b.pieces[sq120]!=NO_PIECE &&
							b.colors[sq120]!=color[sq64]))
			{
				return false;
			}
		}
	}
	return true;
}
