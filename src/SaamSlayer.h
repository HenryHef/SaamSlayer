/*
 * SaamSlayer.h
 *
 *  Created on: Feb 24, 2018
 *      Author: henry
 */

#ifndef SRC_SAAMSLAYER_H_
#define SRC_SAAMSLAYER_H_

#include "board.h"
#include "quiessence.h"
#include "search_base.h"
#include "trans_table.h"

struct Board;
struct Move;

class SaamSlayer
{
public:
	/*Move getMove(Board &b)
	{
		Move moves[MOVE_ARRAY_ALLOC_SIZE];
		int ct;
		genPLegalMoves(b,moves,ct,b.toMove);
		shuffle(moves,ct);
		for(int i = 0; i < ct; i ++)
		{
			b.makeMove(moves[i]);
			//cout<<endl<<"Move:"<<moves[i]<<endl<<" --> "<<endl<<b<<endl;
			if(b.isCheck(!b.toMove))
			{
				b.unmakeMove(moves[i]);
			}
			else
			{
				b.unmakeMove(moves[i]);
				return moves[i];
			}
		}
		cout<<"NO VALID MOVE FOUND"<<endl;
		return Move();
	}
	 */Move getMove(Board &b)
	 {
		 oh.nextPly();

		 Move re;
		 searchProgressive(b,7,6,re);
		 printQCT();
		 //cout<<"RAW SEARCH:  "<<searchN(b, -INF, INF,6, 0)<<endl;
		 return re;
	 }
};



#endif /* SRC_SAAMSLAYER_H_ */
