/*
 * eval.h
 *
 *  Created on: Feb 24, 2018
 *      Author: henry
 */

#ifndef EVAL_H_
#define EVAL_H_

#include "basedef.h"

struct Board;
typedef int Value;//in centipawns


struct EvalHelper
{
	int fienCt[2] = {0,0};
	int tropism[2] = {0,0};
	//alterations is material value
	Value matVal[2] = {0,0};
	Value pawnVal[2] = {0,0};
	Value otherVal[2] = {0,0};
	Value kingSafety[2] = {0,0};

	Value midPSVal[2] = {0,0};
	Value endPSVal[2] = {0,0};

	Value WEEK_SQUARE_BONUS_TABLE[2][120];
	Value POST_SQUARE_BONUS_TABLE[2][120];


	EvalHelper()
	{

	}
};

//enum PieceType {KING = 0, QUEEN = 1, ROOK = 2, BISHOP = 3, KNIGHT = 4, PAWN = 5, NO_PIECE = 6};
static const Value CHECKMATE_MAX_VAL = 200*100;
static const Value INF = CHECKMATE_MAX_VAL+1;
static const Value MAX_TURNS_UNTIL_CHECKMATE = 100;
static const Value CHECKMATE_MIN_VAL = CHECKMATE_MAX_VAL-MAX_TURNS_UNTIL_CHECKMATE;

Value eval(Board& b, bool redo = false);
void printSavedEval();

void evalMaterial(Board& b, EvalHelper& e);
void evalBishop(Board& b, EvalHelper& e, Square sq);
void evalKinght(Board& b, EvalHelper& e, Square sq);
void evalRook(Board& b, EvalHelper& e, Square sq);
void evalQueen(Board& b, EvalHelper& e, Square sq);
void evalKing(Board& b, EvalHelper& e, Square sq);
void evalKingSafety(Board& b, EvalHelper& e);
void evalOtherLast(Board& b, EvalHelper& e);

#endif /* EVAL_H_ */
