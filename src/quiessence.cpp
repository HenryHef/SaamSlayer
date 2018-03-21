/*
 * quiessence.cpp
 *
 *  Created on: Feb 26, 2018
 *      Author: henry
 */

#ifndef SRC_QUIESSENCE_CPP_
#define SRC_QUIESSENCE_CPP_

#include "basedef.h"
#include "board.h"
#include "eval.h"
#include "eval_consts.h"
#include "move_gen_sort.h"
#include "trans_table.h"

int qct = 0;
void printQCT()
{
	cout<<"QCT:"<<qct<<endl;
	qct=0;
}
Value quiessence(Board &b, Value alpha, Value beta, int ply, int &nodect) {
	nodect++;
	qct++;
	bool isInCheck = b.isCheck(b.toMove);
    Value val = eval(b);
    if( val >= beta )
        return val;

    // The next three lines test if alpha can be improved by greatest
    // possible matrial swing.

    int BIG_DELTA = h.PIECE_VALUE[QUEEN]; // queen value
    if ( b.pawnPlayerRowCount[b.toMove][PLAYER_PAWN_PREPROMO_ROW[b.toMove]]>0)
    	BIG_DELTA += h.PIECE_VALUE[QUEEN]-h.PIECE_VALUE[PAWN];

    if (!isInCheck && val < alpha - BIG_DELTA ) {
       return alpha;
    }

    if(!isInCheck && alpha < val )//TODO check that this check on check is right
       alpha = val;


    TTEntry ent;//TODO read rom tble

    MoveGenerator gen;
    gen.genPLegalCaptures(b,b.toMove);
    gen.preSortQuiessence(b,ent,ply);
    for(int i = 0; i < gen.ct; i++)
    {
    	Move cmove = gen.loadNextMove(b,i);
        b.makeMove(cmove);

        val = -quiessence(b, -beta, -alpha, ply+1, nodect);

        b.unmakeMove(cmove);

        if( val >= beta )
            return beta;
        if( val > alpha )
           alpha = val;
    }
    return alpha;
}



#endif /* SRC_QUIESSENCE_CPP_ */
