/*
 * move_gen_sort.h
 *
 *  Created on: Feb 24, 2018
 *      Author: henry
 */

#ifndef SRC_MOVE_GEN_SORT_H_
#define SRC_MOVE_GEN_SORT_H_

#include "basedef.h"
#include "board.h"

struct TTEntry;

struct Board;
struct Move;

const static int MOVE_ARRAY_ALLOC_SIZE = 256;

struct ScoredMove
{
	Move m;
	int score;
	ScoredMove& operator= ( const Move& move)
	{
		m=move;
		return *this;
	}
};


class MoveGenerator
{
public:
	ScoredMove moves[MOVE_ARRAY_ALLOC_SIZE];
	int ct;

	void genPLegalMoves(const Board &b, Player player);
	void genPLegalCaptures(const Board &b, Player player);
	void preSort(Board &b, TTEntry &ent, int ply);
	void preSortQuiessence(Board &b, TTEntry &ent, int ply);
	Move loadNextMove(Board &b, int newIndex);//orders and sorts move
};
bool testMoveIsLegalSlow(Board &b, Move move);
///optionally may assume MOVE_ARRAY_ALLOC_SIZE is actual size


/*
 *  score for capture:
        Add the value of the captured piece and the id of the attacking piece.
        So that if two pieces can attack the same target, the one with the higher id (eg. Pawn=5) gets searched first.
    if (piece_cap != PIECE_EMPTY)
        m[movecount].score = SORT_CAPT + e.SORT_VALUE[piece_cap] + piece_from;

    //score for ep-capture
    if ((piece_from == PAWN) && (to == b.ep)) {
        m[movecount].score = SORT_CAPT + e.SORT_VALUE[PAWN] + 5;
        m[movecount].flags = MFLAG_EPCAPTURE;
    }

    if ((piece_from == PAWN) && ( (ROW(to)==0)||(ROW(to)==7) )) {
        m[movecount].flags |= MFLAG_PROMOTION;

        for (char prompiece = QUEEN; prompiece <= KNIGHT; prompiece++) {
            m[movecount+prompiece-1] = m[movecount];
            m[movecount+prompiece-1].piece_to = prompiece;
            m[movecount+prompiece-1].score += SORT_PROM + e.SORT_VALUE[prompiece];
            m[movecount+prompiece-1].id = movecount+prompiece-1;
        }
        movecount += 3;
    }
 */
#endif /* SRC_MOVE_GEN_SORT_H_ */
