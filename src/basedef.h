/*
 * basedef.h
 *
 *  Created on: Feb 23, 2018
 *      Author: henry
 *      inspired by, and very similar to, CPW engin, though written without looking at that code
 */

#ifndef SRC_BASEDEF_H_
#define SRC_BASEDEF_H_

#include <cstdlib>
#include <iostream>
#include <string>

enum PieceType {KING = 0, QUEEN = 1, ROOK = 2, BISHOP = 3, KNIGHT = 4, PAWN = 5, NO_PIECE = 6};
enum Player{WHITE = 0, BLACK = 1, NO_PLAYER = 2};
inline Player operator!(const Player p)
{
	return p==NO_PLAYER?
			NO_PLAYER:
			p==WHITE?BLACK:WHITE;
}
static const char UNPLAYERED_PIECE_CHAR_NAMES[7] =
		{'K','Q','R','B','N','P','X'};
static const char PLAYER_CHAR_NAMES[3] =
		{'W','B','Z'};
inline std::ostream& operator<<(std::ostream &o, const Player p)
{
	o<<PLAYER_CHAR_NAMES[p];
	return o;
}
inline std::ostream& operator<<(std::ostream &o, const PieceType p)
{
	o<<UNPLAYERED_PIECE_CHAR_NAMES[p];
	return o;
}

typedef unsigned char U8;
typedef char S8;
typedef unsigned long long U64;

typedef U8 Square;//row*16+col
typedef S8 SquareVec;

inline constexpr Square MK_SQ(int row, int col)//we are using a 0x88 board, note, also is a displacement
{
	return row*16+col;
}
inline constexpr int MK_SQ_INT(int row, int col)//we are using a 0x88 board, note, also is a displacement
{
	return row*16+col;
}
inline constexpr SquareVec MK_SQ_VEC(int row, int col)//we are using a 0x88 board, note, also is a displacement
{
	return row*16+col;
}
inline constexpr bool IS_SQ(Square sq)//we are using a 0x88 board
{
	return !(sq & 0x88);
}
inline constexpr signed int COL(Square sq)//we are using a 0x88 board
{
	return sq & 7;
}
inline constexpr signed int ROW(Square sq)//we are using a 0x88 board
{
	return sq >> 4;
}
inline constexpr signed int MOD(Square sq)
{
	return (ROW(sq)+COL(sq))%2;
}
inline constexpr int MANHATTEN_DIST(Square sq1, Square sq2)//we are using a 0x88 board
{
	return abs(COL(sq1)-COL(sq2))+abs(ROW(sq1)-ROW(sq2));
}
inline constexpr int TO_64(Square sq1)
{
	return ROW(sq1)*8+COL(sq1);
}
inline std::string TO_ST(Square sq)//we are using a 0x88 board, note, also is a displacement
{
	std::string re = "";
	re+=(COL(sq)+'a');
	re+=(ROW(sq)+'1');
	return re;
}
inline Square FROM_ST(std::string str, int start)//we are using a 0x88 board, note, also is a displacement
{
	return MK_SQ(str[start+1]-'1',str[start]-'a');
}

static const SquareVec NORTH = MK_SQ_VEC(1,0);
static const SquareVec SOUTH = MK_SQ_VEC(-1,0);
static const SquareVec EAST = MK_SQ_VEC(0,1);
static const SquareVec WEST = MK_SQ_VEC(0,-1);

static const SquareVec NE = NORTH+EAST;
static const SquareVec NW = NORTH+WEST;//will be overflow
static const SquareVec SE = SOUTH+EAST;
static const SquareVec SW = SOUTH+WEST;//will be overflow

static const SquareVec PLAYER_PAWN_FORWARD_VEC[2] =
		{NORTH,SOUTH};

static const int PLAYER_ROW_INDEX[2][8] =
		{{0,1,2,3,4,5,6,7},{7,6,5,4,3,2,1,0}};
static const int PRI[2][8] =
		{{0,1,2,3,4,5,6,7},{7,6,5,4,3,2,1,0}};
static const int PLAYER_PAWN_HOME_ROW[2] =
		{1,6};
static const int PLAYER_PAWN_PREPROMO_ROW[2] =
		{6,1};
static const int PLAYER_PAWN_PROMO_ROW[2] =
		{7,0};

static const SquareVec PLAYER_PAWN_CAPTURE_VECS[2][2] =
		{{NE,NW},
		{SE,SW}};

///includes NO_PIECE
static const char PLAYER_PIECE_CHAR_NAMES[3][7] =
		{{'K','Q','R','B','N','P','.'},
		{'k','q','r','b','n','p','.'},
		{'?','?','?','?','?','?','.'}};
//KING = 0, QUENN = 1, ROOK = 2, BISHOP = 3, KNIGHT = 4
static const SquareVec PIECE_MOVE_VECS[][16] =
		{/*KING*/{NORTH,NE,EAST,SE,SOUTH,SW,WEST,NW},
		 /*QUEEN*/{NORTH,NE,EAST,SE,SOUTH,SW,WEST,NW},
		 /*ROOK*/{NORTH,EAST,SOUTH,WEST},
		 /*BISHOP*/{NE,SE,SW,NW},
		 /*KNIGHT*/{NORTH+NW,NORTH+NE,EAST+NE,EAST+SE,SOUTH+SE,SOUTH+SW,WEST+SW,WEST+NW}};
static const int PIECE_MOVE_VECS_SIZE[] =
		{/*KING*/8,
		/*QUEEN*/8,
		/*ROOK*/4,
		/*BISHOP*/4,
		/*KNIGHT*/8};
static const bool PIECE_MOVE_VECS_IS_SLIDING[] =
		{/*KING*/false,
		/*QUEEN*/true,
		/*ROOK*/true,
		/*BISHOP*/true,
		/*KNIGHT*/false};

enum NamedSquare {
	A1 = MK_SQ(0,0), B1, C1, D1, E1, F1, G1, H1, INVALID_SQ,/* so it will be within the array*/
	A2 = MK_SQ(1,0), B2, C2, D2, E2, F2, G2, H2,
	A3 = MK_SQ(2,0), B3, C3, D3, E3, F3, G3, H3,
	A4 = MK_SQ(3,0), B4, C4, D4, E4, F4, G4, H4,
	A5 = MK_SQ(4,0), B5, C5, D5, E5, F5, G5, H5,
	A6 = MK_SQ(5,0), B6, C6, D6, E6, F6, G6, H6,
	A7 = MK_SQ(6,0), B7, C7, D7, E7, F7, G7, H7,
	A8 = MK_SQ(7,0), B8, C8, D8, E8, F8, G8, H8,

};

enum Row
{
	ROW_1=ROW(MK_SQ(0,0)),
	ROW_2=ROW(MK_SQ(1,0)),
	ROW_3=ROW(MK_SQ(2,0)),
	ROW_4=ROW(MK_SQ(3,0)),
	ROW_5=ROW(MK_SQ(4,0)),
	ROW_6=ROW(MK_SQ(5,0)),
	ROW_7=ROW(MK_SQ(6,0)),
	ROW_8=ROW(MK_SQ(7,0)),
};

enum Col
{
	COL_1=COL(MK_SQ(0,0)),
	COL_2=COL(MK_SQ(0,1)),
	COL_3=COL(MK_SQ(0,2)),
	COL_4=COL(MK_SQ(0,3)),
	COL_5=COL(MK_SQ(0,4)),
	COL_6=COL(MK_SQ(0,5)),
	COL_7=COL(MK_SQ(0,6)),
	COL_8=COL(MK_SQ(0,7)),
};

static const U8 WHITE_KING_CR=1;
static const U8 WHITE_QUEEN_CR=2;
static const U8 BLACK_KING_CR=4;
static const U8 BLACK_QUEEN_CR=8;
static const U8 WHITE_ALL_CR=WHITE_KING_CR|WHITE_QUEEN_CR;
static const U8 BLACK_ALL_CR=BLACK_KING_CR|BLACK_QUEEN_CR;


#endif /* SRC_BASEDEF_H_ */
