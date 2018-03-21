/*
 * eval_consts.h
 *
 *  Created on: Feb 25, 2018
 *      Author: henry
 */

#ifndef SRC_EVAL_CONSTS_H_
#define SRC_EVAL_CONSTS_H_

#include "eval.h"

static const Value SafetyTable[100] = {
    0,  0,   1,   2,   3,   5,   7,   9,  12,  15,
  18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
  68,  75,  82,  85,  89,  97, 105, 113, 122, 131,
 140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
 260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
 377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
 494, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500
};

struct EvalConsts
{
	Value PIECE_VALUE[6];
	Value BISHOP_PAIR;
	Value P_KNIGHT_PAIR;
	Value P_ROOK_PAIR;

	Value KNIGHT_PAWN_COUNT_ADJUSTMENT[9];
	Value ROOK_PAWN_COUNT_ADJUSTMENT[9];

	Value START_MATERIAL;
	Value P_KING_BLOCKS_ROOK;
	Value P_BLOCK_CENTRAL_PAWN;
	Value P_BISHOP_TRAPPED_A7;
	Value P_BISHOP_TRAPPED_A6;
	Value P_KNIGHT_TRAPPED_A8;
	Value P_KNIGHT_TRAPPED_A7;
	Value P_C3_KNIGHT;
	Value P_NO_FIANCHETTO;
	Value SHIELD_1;
	Value SHIELD_2;
	Value P_NO_SHIELD;
	Value ROOK_OPEN;
	Value ROOK_HALF;
	Value RETURNING_BISHOP;
	Value FIANCHETTO;
	Value TEMPO;
	Value ENDGAME_MAT;

	Value PLAYER_KING_LOC_NEARBY_SQUARES[2][120][120];
	Value PLAYER_PIECE_MGAME_SQUARE_VALUE[2][6][120];
	Value PLAYER_PIECE_EGAME_SQUARE_VALUE[2][6][120];

	Value PIECE_KING_SQUARE_LOC_TROPISM_VALUES[6][120][120];

	Value PLAYER_PAST_PAWN_SQUARE_VALUE[2][120];
	Value PLAYER_WEEK_PAWN_SQUARE_VALUE[2][120];
	Value P_DOUBLED_PAWN;

	Value BISHOP_MOBILITY_VALUES[16]  = { -10, -6, 0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8 };
	Value ROOK_MOBILITY_VALUES[16]  = { -4,  -2, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4 };
	Value KNIGHT_MOBILITY_VALUES[9] = { -6,  -4, 0, 2, 4, 5, 6, 7, 8};
	//no queen or pawn mobility values
};
extern EvalConsts h;

void setDefaultEvalValues();
void setBasicValues();
void setUpSquareTables();

#endif /* SRC_EVAL_CONSTS_H_ */
