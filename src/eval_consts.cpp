/*
 * eval_consts.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: henry
 *
 *      ALL CONSTATS TAKEN FROM THE CPW ENGINE
 */

#ifndef SRC_EVAL_CONSTS_CPP_
#define SRC_EVAL_CONSTS_CPP_

#include "eval_consts.h"

#include <cstdlib>
#include <iostream>

#include "basedef.h"

//All from whiotes perspective
//took initial pawn square tables from CPW engin

/* adjustements of piece value based on the number of own pawns FROM CPW*/
EvalConsts h;


/*****************************************************************
 *                           PAWN PCSQ                            *
 *                                                                *
 *  Unlike TSCP, CPW generally doesn't want to advance its pawns  *
 *  just for the fun of it. It takes into account the following:  *
 *                                                                *
 *  - file-dependent component, encouraging program to capture    *
 *    towards the center                                          *
 *  - small bonus for staying on the 2nd rank                     *
 *  - small bonus for standing on a3/h3                           *
 *  - penalty for d/e pawns on their initial squares              *
 *  - bonus for occupying the center                              *
 *****************************************************************/

Value pawn_pcsq_mg[64] = {
		0,   0,   0,   0,   0,   0,   0,   0,
		-6,  -4,   1,   1,   1,   1,  -4,  -6,
		-6,  -4,   1,   2,   2,   1,  -4,  -6,
		-6,  -4,   2,   8,   8,   2,  -4,  -6,
		-6,  -4,   5,  10,  10,   5,  -4,  -6,
		-4,  -4,   1,   5,   5,   1,  -4,  -4,
		-6,  -4,   1, -24,  -24,  1,  -4,  -6,
		0,   0,   0,   0,   0,   0,   0,   0
};

Value pawn_pcsq_eg[64] = {
		0,   0,   0,   0,   0,   0,   0,   0,
		-6,  -4,   1,   1,   1,   1,  -4,  -6,
		-6,  -4,   1,   2,   2,   1,  -4,  -6,
		-6,  -4,   2,   8,   8,   2,  -4,  -6,
		-6,  -4,   5,  10,  10,   5,  -4,  -6,
		-4,  -4,   1,   5,   5,   1,  -4,  -4,
		-6,  -4,   1, -24,  -24,  1,  -4,  -6,
		0,   0,   0,   0,   0,   0,   0,   0
};

/****************************************************************
 *    KNIGHT PCSQ                                                *
 *                                                               *
 *   - centralization bonus                                      *
 *   - penalty for not being developed                           *
 ****************************************************************/

Value knight_pcsq_mg[64] = {
		-8,  -8,  -8,  -8,  -8,  -8,  -8,  -8,
		-8,   0,   0,   0,   0,   0,   0,  -8,
		-8,   0,   4,   4,   4,   4,   0,  -8,
		-8,   0,   4,   8,   8,   4,   0,  -8,
		-8,   0,   4,   8,   8,   4,   0,  -8,
		-8,   0,   4,   4,   4,   4,   0,  -8,
		-8,   0,   1,   2,   2,   1,   0,  -8,
		-8, -12,  -8,  -8,  -8,  -8, -12,  -8
};

Value knight_pcsq_eg[64] = {
		-8,  -8,  -8,  -8,  -8,  -8,  -8,  -8,
		-8,   0,   0,   0,   0,   0,   0,  -8,
		-8,   0,   4,   4,   4,   4,   0,  -8,
		-8,   0,   4,   8,   8,   4,   0,  -8,
		-8,   0,   4,   8,   8,   4,   0,  -8,
		-8,   0,   4,   4,   4,   4,   0,  -8,
		-8,   0,   1,   2,   2,   1,   0,  -8,
		-8, -12,  -8,  -8,  -8,  -8, -12,  -8
};


/****************************************************************
 *                BISHOP PCSQ                                    *
 *                                                               *
 *   - centralization bonus, smaller than for knight             *
 *   - penalty for not being developed                           *
 *   - good squares on the own half of the board                 *
 ****************************************************************/

Value bishop_pcsq_mg[64] = {
		-4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,
		-4,   0,   0,   0,   0,   0,   0,  -4,
		-4,   0,   2,   4,   4,   2,   0,  -4,
		-4,   0,   4,   6,   6,   4,   0,  -4,
		-4,   0,   4,   6,   6,   4,   0,  -4,
		-4,   1,   2,   4,   4,   2,   1,  -4,
		-4,   2,   1,   1,   1,   1,   2,  -4,
		-4,  -4, -12,  -4,  -4, -12,  -4,  -4
};

Value bishop_pcsq_eg[64] = {
		-4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,
		-4,   0,   0,   0,   0,   0,   0,  -4,
		-4,   0,   2,   4,   4,   2,   0,  -4,
		-4,   0,   4,   6,   6,   4,   0,  -4,
		-4,   0,   4,   6,   6,   4,   0,  -4,
		-4,   1,   2,   4,   4,   2,   1,  -4,
		-4,   2,   1,   1,   1,   1,   2,  -4,
		-4,  -4, -12,  -4,  -4, -12,  -4,  -4
};

/****************************************************************
 *                        ROOK PCSQ                              *
 *                                                               *
 *    - bonus for 7th and 8th ranks                              *
 *    - penalty for a/h columns                                  *
 *    - small centralization bonus                               *
 *****************************************************************/

Value rook_pcsq_mg[64] = {
		5,   5,   5,   5,   5,   5,   5,   5,
		20,  20,  20,  20,  20,  20,  20,  20,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		0,   0,   0,   2,   2,   0,   0,   0
};

Value rook_pcsq_eg[64] = {
		5,   5,   5,   5,   5,   5,   5,   5,
		20,  20,  20,  20,  20,  20,  20,  20,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		-5,   0,   0,   0,   0,   0,   0,  -5,
		0,   0,   0,   2,   2,   0,   0,   0
};

/***************************************************************************
 *                     QUEEN PCSQ                                           *
 *                                                                          *
 * - small bonus for centralization in the endgame                          *
 * - penalty for staying on the 1st rank, between rooks in the midgame      *
 ***************************************************************************/

Value queen_pcsq_mg[64] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 2, 2, 1, 0, 0,
		0, 0, 2, 3, 3, 2, 0, 0,
		0, 0, 2, 3, 3, 2, 0, 0,
		0, 0, 1, 2, 2, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0,
		-5, -5, -5, -5, -5, -5, -5, -5
};

Value queen_pcsq_eg[64] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 2, 2, 1, 0, 0,
		0, 0, 2, 3, 3, 2, 0, 0,
		0, 0, 2, 3, 3, 2, 0, 0,
		0, 0, 1, 2, 2, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0,
		-5, -5, -5, -5, -5, -5, -5, -5
};

//att first centrilzation is bad, but later it is good
Value king_pcsq_mg[64] = {
		-40, -30, -50, -70, -70, -50, -30, -40,
		-30, -20, -40, -60, -60, -40, -20, -30,
		-20, -10, -30, -50, -50, -30, -10, -20,
		-10,   0, -20, -40, -40, -20,   0, -10,
		0,  10, -10, -30, -30, -10,  10,   0,
		10,  20,   0, -20, -20,   0,  20,  10,
		30,  40,  20,   0,   0,  20,  40,  30,
		40,  50,  30,  10,  10,  30,  50,  40
};

Value king_pcsq_eg[64] = {
		-72, -48, -36, -24, -24, -36, -48, -72,
		-48, -24, -12,   0,   0, -12, -24, -48,
		-36, -12,   0,  12,  12,   0, -12, -36,
		-24,   0,  12,  24,  24,  12,   0, -24,
		-24,   0,  12,  24,  24,  12,   0, -24,
		-36, -12,   0,  12,  12,   0, -12, -36,
		-48, -24, -12,   0,   0, -12, -24, -48,
		-72, -48, -36, -24, -24, -36, -48, -72
};

/*****************************************************************
 *                     WEAK PAWNS PCSQ                            *
 *                                                                *
 *  Current version of CPW-engine does not differentiate between  *
 *  isolated  and  backward pawns, using one  generic  cathegory  *
 *  of  weak pawns. The penalty is bigger in the center, on  the  *
 *  assumption  that  weak  central pawns can be  attacked  from  *
 *  many  directions. If the penalty seems too low, please  note  *
 *  that being on a semi-open file will come into equation, too.  *
 *****************************************************************/

Value weak_pawn_pcsq[64] = {
		0,   0,   0,   0,   0,   0,   0,   0,
		-10, -12, -14, -16, -16, -14, -12, -10,
		-10, -12, -14, -16, -16, -14, -12, -10,
		-10, -12, -14, -16, -16, -14, -12, -10,
		-10, -12, -14, -16, -16, -14, -12, -10,
		-8, -12, -14, -16, -16, -14, -12, -10,
		-8, -12, -14, -16, -16, -14, -12, -10,
		0,   0,   0,   0,   0,   0,   0,   0
};

Value passed_pawn_pcsq[64] = {
		0,   0,   0,   0,   0,   0,   0,   0,
		100, 100, 100, 100, 100, 100, 100, 100,
		80,  80,  80,  80,  80,  80,  80,  80,
		60,  60,  60,  60,  60,  60,  60,  60,
		40,  40,  40,  40,  40,  40,  40,  40,
		20,  20,  20,  20,  20,  20,  20,  20,
		20,  20,  20,  20,  20,  20,  20,  20,
		0,   0,   0,   0,   0,   0,   0,   0
};

void setDefaultEvalValues() {

	setBasicValues();
	setUpSquareTables();
}

void setBasicValues() {

	/********************************************************************************
	 *  We use material values by IM Larry Kaufman with additional + 10 for a Bishop *
	 *  and only +30 for a Bishop pair                                                 *
	 ********************************************************************************/

	h.PIECE_VALUE[KING]   = 0;
	h.PIECE_VALUE[QUEEN]  = 975;
	h.PIECE_VALUE[ROOK]   = 500;
	h.PIECE_VALUE[BISHOP] = 335;
	h.PIECE_VALUE[KNIGHT] = 325;
	h.PIECE_VALUE[PAWN]   = 100;

	h.BISHOP_PAIR   = 30;
	h.P_KNIGHT_PAIR = 8;
	h.P_ROOK_PAIR   = 16;

	Value lKAdj[] = { -20, -16, -12, -8, -4,  0,  4,  8, 12};
	Value lRAdj[] =   {  15,  12,   9,  6,  3,  0, -3, -6, -9};
	for(int i = 0; i < 9; i++)
	{
		h.KNIGHT_PAWN_COUNT_ADJUSTMENT[i]=lKAdj[i];
		h.ROOK_PAWN_COUNT_ADJUSTMENT[i]=lRAdj[i];
	}

	/*************************************************
	 *  Calculate  the  value  of  piece  material    *
	 *  at  the  beginning of  the  game,  which  is  *
	 *  used for scaling the king tropism evaluation. *
	 *  This  way we don't have to update a constant  *
	 *  every time we change material values.         *
	 *************************************************/

	h.START_MATERIAL = h.PIECE_VALUE[QUEEN]
					+ 2 * h.PIECE_VALUE[ROOK]
					+ 2 * h.PIECE_VALUE[BISHOP]
					+ 2 * h.PIECE_VALUE[KNIGHT];

	/*************************************************
	 * Values used for sorting captures are the same  *
	 * as normal piece values, except for a king.     *
	 *************************************************/

	/*TODO, implelemtn when move ordering start

	for (int i = 0; i < 6; ++i) {
		SORT_VALUE[i] = PIECE_VALUE[i];
	}
	SORT_VALUE[KING] = SORT_KING;
*/

	/* trapped and blocked pieces */

	h.P_KING_BLOCKS_ROOK   = 24;
	h.P_BLOCK_CENTRAL_PAWN = 24;
	h.P_BISHOP_TRAPPED_A7  = 150;
	h.P_BISHOP_TRAPPED_A6  = 50;
	h.P_KNIGHT_TRAPPED_A8  = 150;
	h.P_KNIGHT_TRAPPED_A7  = 100;

	/* minor penalties */

	h.P_C3_KNIGHT = 5;
	h.P_NO_FIANCHETTO = 4;

	/* king's defence */
	h.SHIELD_1 = 10;
	h.SHIELD_2 = 5;
	h.P_NO_SHIELD = 10;

	/* minor bonuses */

	h.ROOK_OPEN = 10;
	h.ROOK_HALF = 5;
	h.RETURNING_BISHOP = 20;
	h.FIANCHETTO = 4;
	h.TEMPO = 10;

	h.ENDGAME_MAT = 1300;

	h.P_DOUBLED_PAWN = 40;
}


void setUpSquareTables()
{
	Square blackSqOrder[] = {A1, B1, C1, D1, E1, F1, G1, H1,
			A2, B2, C2, D2, E2, F2, G2, H2,
			A3, B3, C3, D3, E3, F3, G3, H3,
			A4, B4, C4, D4, E4, F4, G4, H4,
			A5, B5, C5, D5, E5, F5, G5, H5,
			A6, B6, C6, D6, E6, F6, G6, H6,
			A7, B7, C7, D7, E7, F7, G7, H7,
			A8, B8, C8, D8, E8, F8, G8, H8};
	Square whiteSqOrder[] = {A8, B8, C8, D8, E8, F8, G8, H8,
			A7, B7, C7, D7, E7, F7, G7, H7,
			A6, B6, C6, D6, E6, F6, G6, H6,
			A5, B5, C5, D5, E5, F5, G5, H5,
			A4, B4, C4, D4, E4, F4, G4, H4,
			A3, B3, C3, D3, E3, F3, G3, H3,
			A2, B2, C2, D2, E2, F2, G2, H2,
			A1, B1, C1, D1, E1, F1, G1, H1};
	Square* sqOrders[]={whiteSqOrder,blackSqOrder};

	for(int player = 0; player < 2; player++)
	{
		for(Square kingSq = 0; kingSq<120; kingSq++)
		{
			if(!IS_SQ(kingSq))
				continue;
			for(Square oSq = 0; oSq<120; oSq++)
			{
				if(!IS_SQ(oSq))
					continue;
				if(kingSq+NORTH==oSq ||
						kingSq+EAST==oSq ||
						kingSq+SOUTH==oSq ||
						kingSq+WEST==oSq ||
						kingSq+NE==oSq ||
						kingSq+NW==oSq ||
						kingSq+SE==oSq ||
						kingSq+SW==oSq)//boarder
					h.PLAYER_KING_LOC_NEARBY_SQUARES[player][kingSq][oSq]=1;
				else if(ROW(kingSq)==0 &&  //if on edge, 1 extra square out
						(kingSq+NORTH+NORTH==oSq ||
								kingSq+NORTH+NE==oSq ||
								kingSq+NORTH+NW==oSq))
					h.PLAYER_KING_LOC_NEARBY_SQUARES[player][kingSq][oSq]=1;
				else if(ROW(kingSq)==7 &&
						(kingSq+SOUTH+SOUTH==oSq ||
								kingSq+SOUTH+SE==oSq ||
								kingSq+SOUTH+SW==oSq))
					h.PLAYER_KING_LOC_NEARBY_SQUARES[player][kingSq][oSq]=1;
				else if(COL(kingSq)==0 &&
						(kingSq+EAST+EAST==oSq ||
								kingSq+EAST+NE==oSq ||
								kingSq+EAST+SE==oSq))
					h.PLAYER_KING_LOC_NEARBY_SQUARES[player][kingSq][oSq]=1;
				else if(COL(kingSq)==7 &&
						(kingSq+WEST+WEST==oSq ||
								kingSq+WEST+NW==oSq ||
								kingSq+WEST+SW==oSq))
					h.PLAYER_KING_LOC_NEARBY_SQUARES[player][kingSq][oSq]=1;
				else
					h.PLAYER_KING_LOC_NEARBY_SQUARES[player][kingSq][oSq]=0;
			}
		}
	}
	//piece square tables
	Value* pieceSquareArraysM[]={king_pcsq_mg,queen_pcsq_mg,rook_pcsq_mg,bishop_pcsq_mg,knight_pcsq_mg,pawn_pcsq_mg};
	Value* pieceSquareArraysE[]={king_pcsq_eg,queen_pcsq_eg,rook_pcsq_eg,bishop_pcsq_eg,knight_pcsq_eg,pawn_pcsq_eg};
	for(int player = 0; player < 2; player++)
	{
		for(int piece = 0; piece<6; piece++)
		{
			for(int sqI = 0; sqI<64; sqI++)
			{
				h.PLAYER_PIECE_MGAME_SQUARE_VALUE[player][piece][sqOrders[player][sqI]]=pieceSquareArraysM[piece][sqI];
				/*if(piece==PAWN)
					std::cout<<"SETTING player:"<<player<<" piece:"<<piece<<" sqI:"
					<<(int)sqI<<" sq:"<<TO_ST(sq)<<" accesSq:"<<TO_ST(sqOrders[player][sqI])
						<<"\t  --> "<<pieceSquareArraysM[piece][sqOrders[player][sq]]<<std::endl;*/
				h.PLAYER_PIECE_EGAME_SQUARE_VALUE[player][piece][sqOrders[player][sqI]]=pieceSquareArraysE[piece][sqI];
				if(piece==PAWN)
				{
					h.PLAYER_PAST_PAWN_SQUARE_VALUE[player][sqOrders[player][sqI]]=passed_pawn_pcsq[sqI];
					h.PLAYER_WEEK_PAWN_SQUARE_VALUE[player][sqOrders[player][sqI]]=weak_pawn_pcsq[sqI];
				}
			}
		}
	}

	//set up king tropism tables https://chessprogramming.wikispaces.com/Evaluation%20function%20draft
	/* initializes the table of distances between squares */
	int diag_nw[64] = {
		0, 1, 2, 3, 4, 5, 6, 7,
		1, 2, 3, 4, 5, 6, 7, 8,
		2, 3, 4, 5, 6, 7, 8, 9,
		3, 4, 5, 6, 7, 8, 9,10,
		4, 5, 6, 7, 8, 9,10,11,
		5, 6, 7, 8, 9,10,11,12,
		6, 7, 8, 9,10,11,12,13,
		7, 8, 9,10,11,12,13,14
	};

	int diag_ne[64] = {
		7, 6, 5, 4, 3, 2, 1, 0,
		8, 7, 6, 5, 4, 3, 2, 1,
		9, 8, 7, 6, 5, 4, 3, 2,
		10, 9, 8, 7, 6, 5, 4, 3,
		11,10, 9, 8, 7, 6, 5, 4,
		12,11,10, 9, 8, 7, 6, 5,
		13,12,11,10, 9, 8, 7, 6,
		14,13,12,11,10, 9, 8, 7
	};
	int bonus_dia_distance[15] = {5, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	for (Square kingSq = 0; kingSq < 120; kingSq++)
	{
		for (Square pieceSq = 0; pieceSq < 120; pieceSq++)
		{
			int dist_bonus = 14 - ( abs ( COL(kingSq) - COL(pieceSq) )
				 + abs ( ROW(kingSq) - ROW(pieceSq) ) );
			int king64 = TO_64(kingSq);
			int piece64 = TO_64(pieceSq);
			h.PIECE_KING_SQUARE_LOC_TROPISM_VALUES[QUEEN][kingSq][pieceSq] = (dist_bonus * 5) / 2;
			h.PIECE_KING_SQUARE_LOC_TROPISM_VALUES[ROOK][kingSq][pieceSq] =  dist_bonus / 2;
			h.PIECE_KING_SQUARE_LOC_TROPISM_VALUES[BISHOP][kingSq][pieceSq] =  dist_bonus / 2
					+ bonus_dia_distance[abs(diag_ne[king64] - diag_ne[piece64])]
					+ bonus_dia_distance[abs(diag_nw[king64] - diag_nw[piece64])];
			h.PIECE_KING_SQUARE_LOC_TROPISM_VALUES[KNIGHT][kingSq][pieceSq] =  dist_bonus;
			h.PIECE_KING_SQUARE_LOC_TROPISM_VALUES[KING][kingSq][pieceSq] =  0;
			h.PIECE_KING_SQUARE_LOC_TROPISM_VALUES[PAWN][kingSq][pieceSq] =  dist_bonus;
		}
	}
}


#endif /* SRC_EVAL_CONSTS_CPP_ */
