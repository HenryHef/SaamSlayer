/*
 * board.h
 *
 *  Created on: Feb 23, 2018
 *      Author: henry
 */

#ifndef SRC_BOARD_H_
#define SRC_BOARD_H_

#include <iostream>
#include <string>

#include "eval.h"
#include "move.h"

using namespace std;

#include "basedef.h"

struct ZorbistHashStuff
{
	U64 PLAYER_PIECE_SQUARE_HASH_VALS[2][6][120];
	U64 CASTLING_RIGHTS_HASH_VALS[16];
	U64 WHITE_TO_MOVE_HASH_VAL;
	U64 ENP_HASH_VALS[120];
};
extern ZorbistHashStuff z;

void initBoardHashData();

static const int MAX_GAME_LEN = 1000;

struct Board
{
	//Needed Data
	PieceType pieces[120];
	Player colors[120];
	Square enp;//set onl if there is an enpassant possible
	Player toMove;
	U8 castlingRightsFlag;//KING=0, QUEEN=1, as inenum

	//Helped Eval Data
	Value material[2];
	int pieceCount[2][6];
	int kingSq[2];

	int pawnControllMap[2][120];//count of controlling pawns
	int pawnPosControllMap[2][120];//count of controlling pawns
	int pawnPlayerModSqCount[2][2];//player, sq%2 gives if on light or dark square
	int pawnPlayerColCount[2][8];
	int pawnPlayerRowCount[2][8];

	//Hashes for tt
	U64 hash;
	U64 phash;

	//historical info, used for unmake and checking 3 repetition rule
	U64 pastPositionHashes[MAX_GAME_LEN];
	Square pastEnp[MAX_GAME_LEN];
	U8 pastCastlingRightsFlag[MAX_GAME_LEN];
	int histroyStackNextIndex;//==ply+1

	Board();

	void makeMove(const Move &move);
	bool makeStringMove(string str);
	void unmakeMove(const Move &move);
	void setSquare(Square sq,Player player, PieceType piece);//alters tables, and hash by piece hash table, but not else
	void setSquare(Square sq,PieceType piece, Player player)//convinience method, flips parameter order
	{
		setSquare(sq,player,piece);
	}
	bool sqIsAttacked(Square sq, Player p) const;

	//sqIsAttacked(kingSq[p],!p)
	bool isCheck(Player p) const;

	void setStartBoard();

	bool isPiece(Square sq, PieceType pt, Player pl);
};

Move makeMoveNormal(Square _from, Square _to,
			PieceType _piece,
			Player _player);
Move makeMoveCapture(Square _from, Square _to,
		PieceType _fromPiece, PieceType _toPieceOld,
		Player _player);
Move makeMovePromoCapture(Square _from, Square _to,
		PieceType _toPieceOld, PieceType _toPieceNew,
		Player _player);
Move makeMovePromo(Square _from, Square _to,
		PieceType _toPieceNew,
		Player _player);
Move makeMoveCastle(Player _player,bool _kingside);
Move makeMoveNull(Player _player);
Move makeMoveSlow(Board &b, Square to, Square from, char extra);

inline ostream& operator<<(ostream& o, const Board& b)//simple, no metadata
{
	for(int row = 7; row>=0; row--)
	{
		for(int col = 0; col<=7; col++)
		{
			Square sq = MK_SQ(row,col);
			o<<PLAYER_PIECE_CHAR_NAMES[b.colors[sq]][b.pieces[sq]]<<" ";
		}
		o<<endl;
	}
	o<<"Metadata:"<<endl;
	o<<"KingSq:"<<TO_ST(b.kingSq[WHITE])<<" , "<<TO_ST(b.kingSq[BLACK]);
	o<<"enp:"<<TO_ST(b.enp)<<" CR:"<<(int)b.castlingRightsFlag;
	return o;
}
inline ostream& operator<<(ostream& o, const Move& m)//simple, no metadata
{
	o<<"from:"<<TO_ST(m.from)<<" to:"<<TO_ST(m.to)<<" player:"<<m.player
			<<"    fromP:"<<m.fromPiece<<" toPOld:"<<m.toPieceOld<<" toPNew:"<<m.toPieceNew;
	return o;
}




#endif /* SRC_BOARD_H_ */
