
#ifndef SRC_CHESSGAMEBOARDSTATE_H_
#define SRC_CHESSGAMEBOARDSTATE_H_

#include <vector>

#include "basedef.h"
#include "board.h"
#include "move.h"

class OwnedPiece
{
private:
	PieceType piece;
	Player player;
public:
	OwnedPiece(PieceType _piece, Player _player)
	{
		piece=_piece;
		player=_player;
	}
	OwnedPiece()
	{
		piece=NO_PIECE;
		player=NO_PLAYER;
	}

	bool isNoPiece()
	{
		return piece==NO_PIECE;
	}

	PieceType getPiece()
	{
		return piece;
	}
	Player getPlayer()
	{
		return player;
	}

	OwnedPiece cleaned()
	{
		OwnedPiece n = OwnedPiece(piece,player);
		if(piece==NO_PIECE)
			n.player=NO_PLAYER;
		if(player==NO_PLAYER)
			n.piece=NO_PIECE;
		return n;
	}
};

struct TupleInt4
{
	int v[4];
	TupleInt4(int a,int b, int c, int d)
	{
		v[0]=a;
		v[1]=b;
		v[2]=c;
		v[3]=d;
	}

	int __getitem__(int i)
	{
		return v[i];
	}
};

class BoardState
{
private:
	OwnedPiece pieces[64];
	int startRow = -1, endRow = -1, startCol = -1, endCol = -1;
public:
	BoardState(Board &_b,Move move)
	{
		for(int i = 0; i < 8; i++)
		{
			for(int j = 0; j < 8; j++)
			{
				Square sq = MK_SQ(i,j);
				pieces[i*8+j]=OwnedPiece(_b.pieces[sq],_b.colors[sq]).cleaned();
			}
		}
		startRow = ROW(move.from);
		endRow = ROW(move.to);
		startCol = COL(move.from);
		endCol = COL(move.to);
	}
	BoardState()
	{

	}

	PieceType getPiece(int i, int j)
	{
		return pieces[i*8+j].getPiece();
	}
	Player getPlayer(int i, int j)
	{
		return pieces[i*8+j].getPlayer();
	}

	TupleInt4 getMoveSSEE()
	{
		return TupleInt4(startRow,startCol,endRow,endCol);
	}
};

class BoardStateList
{
	vector<BoardState> ls;
public:
	BoardStateList(vector<BoardState> _ls)
	{
		ls=_ls;
	}

	BoardState get(int i)
	{
		return ls[i];
	}

	int size()
	{
		return ls.size();
	}
};

#endif /* SRC_CHESSGAMEBOARDSTATE_H_ */
