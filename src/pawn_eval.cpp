/*
 * pawn_eval.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: henry
 */

#include <algorithm>

#include "basedef.h"
#include "board.h"
#include "eval.h"
#include "eval_consts.h"

void evalPawns(Board& b, EvalHelper& e)
{
	int minPR[2][8];
	for(int col = 0; col < 8; col++)
	{
		minPR[WHITE][col]=7;
		minPR[BLACK][col]=0;
		for(int row = 0; row < 8; row++)
		{
			Square sq = MK_SQ(row,col);
			if(b.pieces[sq]==PAWN)
			{
				if(b.colors[sq]==WHITE)
				{
					minPR[WHITE][col]=min(minPR[WHITE][col],row);
				}
				else
				{
					minPR[BLACK][col]=max(minPR[BLACK][col],row);
				}
			}
		}
	}
	//now pass over grid, looking for isolated and backward pawns
	for(int col = 0; col < 8; col++)
	{
		//first penalize doubled pawns
		for(int player = 0; player<2; player++)
		{
			if(b.pawnPlayerColCount[player][col]>=2)
			{
				//cout<<"DOUBLED PAWN FOR "<<player<<" COL:"<<col<<endl;
				e.pawnVal[player]-=(b.pawnPlayerColCount[player][col]-1)*h.P_DOUBLED_PAWN;
			}
		}

		//then search for backward, isolated (both week), and passed pawns.
		for(int row = 0; row < 8; row++)
		{
			Square sq = MK_SQ(row,col);
			if(b.pieces[sq]==PAWN)//evaluate pawn
			{
				Player pl = b.colors[sq];
				if(((col==7 || b.pawnPlayerColCount[pl][col+1]==0)
						&& (col==0 || b.pawnPlayerColCount[pl][col-1]==0)))//issolated
				{
					//cout<<"ISSOLATED PAWN ON "<<row<<","<<col<<endl;
					e.pawnVal[pl]+=h.PLAYER_WEEK_PAWN_SQUARE_VALUE[pl][sq];
				}

				if((col==7 || PRI[pl][minPR[pl][col+1]] > PRI[pl][row]) &&
					(col==0 || PRI[pl][minPR[pl][col-1]] > PRI[pl][row]))
				{
					//cout<<"BACKWARD PAWN ON "<<row<<","<<col<<endl;
					e.pawnVal[pl]+=h.PLAYER_WEEK_PAWN_SQUARE_VALUE[pl][sq];//backward
				}

				if(PRI[pl][minPR[!pl][col]]<=PRI[pl][row] &&
						(col==7 || PRI[pl][minPR[!pl][col+1]]<=PRI[pl][row]) &&
						 (col==0 || PRI[pl][minPR[!pl][col-1]]<=PRI[pl][row]))//passed
				{
					//cout<<"PASSED PAWN ON "<<row<<","<<col<<endl;
					e.pawnVal[pl]+=h.PLAYER_PAST_PAWN_SQUARE_VALUE[pl][sq];
				}
			}
		}
	}
}


