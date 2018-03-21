/*
 * search.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: henry
 */

#include "search_base.h"

#include <sys/time.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "basedef.h"
#include "board.h"
#include "eval_consts.h"
#include "move_gen_sort.h"
#include "quiessence.h"
#include "trans_table.h"


void searchProgressive(Board &b, int min_depth, double min_time, Move& re)
{
	clock_t ti,tl;
	ti=clock();
	int i = 2;
	while(i <= min_depth || ((float)clock()-(float)ti)/CLOCKS_PER_SEC < min_time)
	{
		int nodect=0;
		searchRootNode(b,i,re,nodect);
		clock_t c = clock();
		float secTot = ((float)c-(float)ti)/CLOCKS_PER_SEC;
		float secL = ((float)c-(float)tl)/CLOCKS_PER_SEC;
		tl=c;
		cout<<i<<" nodeCt:"<<nodect<<"\t\tround:"<<secL<<"s\t\ttotal:"<<secTot<<"s"<<endl;
		i++;
	}
}
Value searchRootNode(Board &b, int depth, Move& re, int &nodect)
{
	nodect++;
	Value alpha = -INF, beta= INF;
	Value best = -INF;
	Move bestM;
	bool isValidMove = false;

	TTEntry ent;
	tt.get(b.hash,ent);

	MoveGenerator gen;
	gen.genPLegalMoves(b,b.toMove);
	gen.preSort(b,ent,0);

	for(int i = 0; i < gen.ct; i ++)
	{
		Move cmove = gen.loadNextMove(b,i);
		b.makeMove(cmove);

		if(b.isCheck(!b.toMove))
		{
			b.unmakeMove(cmove);
			continue;
		}
		isValidMove=true;
		Value ne = -searchN(b, -beta, -alpha, depth - 1, 1, true, true, nodect);
		//cout<<"Alpha: "<<alpha<<" Beta: "<<beta<<"  Move: "<<cmove<<"  Val:"<<ne<<endl;

		b.unmakeMove(cmove);

		if(ne>best)
		{
			best=ne;
			bestM=cmove;
		}
		alpha = alpha> ne?alpha:ne;
	}
	int falsect = 0;
	//cout<<"SearchN:"<<searchN(b,-INF,INF,depth,0,true,falsect)<<endl;
	if(!isValidMove)//checkmate or stalemate, no valid moves
	{
		re=Move();
		if(b.isCheck(b.toMove))//checkmate
		{
			return -(CHECKMATE_MAX_VAL);
		}
		else
		{
			return 0;//TODO add contempt!!!
		}
	}
	else//found valid
	{
		re=bestM;
		return best;
	}
}

Value searchN(Board &b, Value alpha, Value beta, int depth, int ply, bool isPV, bool can_null, int &nodect)
{
	bool increasedAlpha = false;
	bool wasValidMove = false;

	bool isInCheck = b.isCheck(b.toMove);

	if(depth==0 && isInCheck)
		depth++;

	if(depth==0)
		return quiessence(b,alpha,beta,ply,nodect);

	nodect++;

	Move bestMove = Move();

	TTEntry ent;
	if(tt.get(b.hash,ent))
	{
		if(ent.depth>=depth)
		{
			//cout<<"valid depth for des dpeth "<<depth<<endl;
			if (!isPV||ent.type == EXACT)
				return ent.iterVal;
			else if (ent.type == LOWER_BOUND)
			{
				increasedAlpha=alpha>=ent.iterVal;
				alpha = max(alpha, ent.iterVal);
			}
			else if (ent.type == UPPER_BOUND)
				beta = min(beta, ent.iterVal);

			if (alpha >= beta)
				return ent.iterVal;
		}
		//else
		//cout<<"invalid depth for des dpeth "<<depth<<endl;
	}

	MoveGenerator gen;
	gen.genPLegalMoves(b,b.toMove);
	gen.preSort(b,ent,ply);

	ent=TTEntry();
	ent.hash = b.hash;
	ent.depth = depth;
	ent.type = UPPER_BOUND;

	Value val;

	//if ( b.isRepetition() ) return 0;//contempt();


	static const int STATIC_NULL_MOVE_CONST = 120;

	if (depth < 3
			&& (!isPV)
			&& (!isInCheck)
			&& (abs(beta - 1) > -CHECKMATE_MIN_VAL))
	{
		int static_eval = eval(b);

		int eval_margin = STATIC_NULL_MOVE_CONST * depth;
		if (static_eval - eval_margin >= beta)
			return static_eval - eval_margin;
	}

	/************************************************************************
	 *  Here  we introduce  NULL MOVE PRUNING. It  means  allowing opponent  *
	 *  to execute two moves in a row, i.e. capturing something and escaping *
	 *  a recapture. If this cannot  wreck our position, then it is so good  *
	 *  that there's  no  point in searching further. The flag "can_null"    *
	 *  ensures we don't do  two null moves in a row. Null move is not used  *
	 *  in  the endgame because of the risk of zugzwang.                     *
	 ************************************************************************/

	if ( ( depth > 2)
			&&     ( can_null )
			&&   (!isPV)
			&&     ( eval(b) >= beta ) //should be >=, see post
			&&     ( b.material[b.toMove] > h.ENDGAME_MAT )
			&&     ( !isInCheck )  )
	{
		b.makeMove(Move());
		/********************************************************************
		 *  We use so-called adaptative null move pruning. Size of reduction *
		 *  depends on remaining  depth.                                     *
		 ********************************************************************/

		char R = 2;
		if ( depth > 6 ) R = 3;

		val = -searchN(b, -beta, -beta+1,  depth - R - 1, ply, false,false,nodect);

		b.unmakeMove(Move());

		if (val >= beta) return beta;
	}

	/************************************************************************
	 *  Decide  if FUTILITY PRUNING  is  applicable. If we are not in check, *
	 *  not searching for a checkmate and eval is below  (alpha - margin),   *
	 *  it  might  mean that searching non-tactical moves at  low depths     *
	 *  is futile, so we set a flag allowing this pruning.                   *
	 ************************************************************************/

	int fmargin[4] = {0, 200, 300, 500};
	bool f_prune = false;
	if ( depth <= 3
			&&     !isPV
			&&     !isInCheck
			&&     abs(alpha) < 9000
			&&     eval(b) + fmargin[depth] <= alpha )
		f_prune = true;

	int validCt = 0;
	for(int i = 0; i < gen.ct; i ++)
	{
		Move cmove = gen.loadNextMove(b,i);
		b.makeMove(cmove);
		//cout<<endl<<"Move:"<<moves[i]<<endl<<" --> "<<endl<<b<<endl;
		if(b.isCheck(!b.toMove))
		{
			b.unmakeMove(cmove);
			continue;
		}
		validCt++;
		//512890
		if ( f_prune
				&&   wasValidMove
				&&  !cmove.isCapture
				&&    !cmove.isPromo
				&&  !b.isCheck(b.toMove)) {
			b.unmakeMove(cmove);
			continue;
		}




		int reduction_depth = 0;   // this move has not been reduced yet
		int new_depth = depth - 1; // decrease depth by one ply

		/********************************************************************
		 *  Late move reduction. Typically a cutoff occurs on trying one of  *
		 *  the first moves. If it doesn't, we are probably in an all-node,  *
		 *  which means that all moves will fail low. So we might as well    *
		 *  spare some effort, searching to reduced depth. Of course this is *
		 *  not a foolproof method, but it works more often than not. Still, *
		 *  we  need to exclude certain moves from reduction, in  order  to  *
		 *  filter out tactical moves that may cause a late cutoff.          *
		 ********************************************************************/

		if (!isPV
				&& new_depth > 3
				&& wasValidMove
				&& validCt > 3
				&& !b.isCheck(b.toMove)
				&& !isInCheck
				&& (cmove.from != oh.killers[ply][0].from || cmove.to != oh.killers[ply][0].to)
				&& (cmove.from != oh.killers[ply][0].from || cmove.to != oh.killers[ply][0].to)
				&& !cmove.isCapture
				&& !cmove.isPromo) {

			/****************************************************************
			 * Real programs tend use more advanced formulas to calculate    *
			 * reduction depth. Typically they calculate it from both        *
			 * remaining depth and move count. Formula used here is very     *
			 * basic and gives only a minimal improvement over uniform       *
			 * one ply reduction, and is included for the sake of complete-  *
			 * ness only.                                                    *
			 ****************************************************************/

			reduction_depth = 1;
			if (validCt > 8) reduction_depth += 1;

			new_depth -= reduction_depth;
		}

		re_search:

		/********************************************************************
		 *  The code below introduces principal variation search. It  means  *
		 *  that once we are in a PV-node (indicated by IS_PV flag) and  we  *
		 *  have  found a move that raises alpha, we assume that  the  rest  *
		 *  of moves ought to be refuted. This is done  relatively  cheaply  *
		 *  by using  a null-window search centered around alpha.  Only  if  *
		 *  this search fails high, we are forced repeat it with full window.*
		 *                                                                   *
		 *  Understanding the shorthand in the first two lines is a bit      *
		 *  tricky. If alpha has not been raised, we might be either in      *
		 *  a  zero window (scout) node or in an open window (pv)  node,     *
		 *  entered after a scout search failed high. In both cases, we      *
		 *  need to search with the same alpha, the same beta AND the same   *
		 *  node type.                                                       *
		 ********************************************************************/

		if (!increasedAlpha)
			val = -searchN(b,-beta, -alpha, new_depth, ply+1, isPV, true, nodect);
		else {
			// first try to refute a move - if this fails, do a real search
			if (-searchN(b, -alpha - 1, -alpha, new_depth, ply+1, false, true, nodect) > alpha)
				val = -searchN(b, -beta, -alpha, new_depth, ply+1, true, true, nodect);
		}

		/********************************************************************
		 *  Sometimes reduced search brings us above alpha. This is unusual, *
		 *  since we expected reduced move to be bad in first place. It is   *
		 *  not certain now, so let's search to the full, unreduced depth.   *
		 ********************************************************************/

		if (reduction_depth && val > alpha) {
			new_depth += reduction_depth;
			reduction_depth = 0;
			goto re_search;
		}

		b.unmakeMove(cmove);






		/********************************************************************
		 * FROM CPW ENGINE
		 *  We can improve over alpha, so we change the node value together  *
		 *  with  the expected move. Also the raised_alpha flag, needed  to  *
		 *  control PVS, is set. In case of a beta cuoff, when our position  *
		 *  is  so good that the score will not be accepted one ply before,  *
		 *  we return it immediately.                                        *
		 ********************************************************************/

		wasValidMove=true;

		if ( val > alpha ) {//if increasedAlpha, val==alpha

			ent.bestMove = cmove;
			bestMove = cmove;

			if ( val >= beta ) {

				/*************************************************************
				 *  On a quiet move update killer moves and history table     *
				 *  in order to enhance move ordering.                        *
				 *************************************************************/

				if (!cmove.isCapture
						&& !cmove.isPromo)
				{
					oh.setKiller(cmove, ply);
					oh.history[cmove.from][cmove.to] += depth*depth;

					/*********************************************************
					 *  With super deep search history table would overflow   *
					 *  - let's prevent it.                                   *
					 *********************************************************/

					if (oh.history[cmove.from][cmove.to] > oh.SORT_KILL) {
						for (int a = 0; a < 128; a++)
							for (int b = 0; b < 128; b++) {
								oh.history[a][b] = oh.history[a][b] / 2;
							}
					}
				}
				ent.type = LOWER_BOUND;
				alpha = beta;
				break; // no need to search any further
			}

			increasedAlpha = true;
			ent.type = EXACT;
			alpha = val;

		} // changing the node value is finished
	}


	if (!wasValidMove )
	{
		ent.bestMove=Move();

		if (isInCheck)
			alpha = -CHECKMATE_MAX_VAL + ply;
		else
			alpha = 0;
	}

	ent.iterVal=alpha;
	//cout<<"storing hash "<<b.hash<<" for depth "<<depth<<endl;
	tt.store(b.hash,ent);//store relative to this node
	return alpha;
}


