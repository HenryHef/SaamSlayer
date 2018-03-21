/*
 * trans_table.h
 *
 *  Created on: Feb 26, 2018
 *      Author: henry
 */

#ifndef SRC_TRANS_TABLE_H_
#define SRC_TRANS_TABLE_H_

#include <cmath>

#include "basedef.h"
#include "board.h"
#include "eval.h"
#include "eval_consts.h"

struct PawnTTEntry
{
	U64 hash;
	Value eval;
	Value PLAYER_WEEK_SQUARES_BONUS[2][120];//for no pawn attackable squares
	Value PLAYER_POST_SQUARES_BONUS[2][120];//for squares protected by a pawn not attacked by an oponent pawn
};

class PawnTable
{
public:
	static constexpr int size = pow(2,20);
	PawnTTEntry table[size];

	bool getEval(U64 hash, Value &e)
	{
		e=table[hash%size].eval;
		return table[hash%size].hash==hash;
	}
	void storeEval(U64 hash, Value e)
	{
		table[hash%size].hash=hash;
		table[hash%size].eval=e;
	}
};

struct ValueHash
{
	U64 hash;
	Value eval;
};

class EvalTable
{
public:
	static constexpr int size = pow(2,20);
	ValueHash table[size];

	bool getEval(U64 hash, Value &e)
	{
		e=table[hash%size].eval;
		return table[hash%size].hash==hash;
	}
	void storeEval(U64 hash, Value e)
	{
		table[hash%size].hash=hash;
		table[hash%size].eval=e;
	}
};


enum NodeType { LOWER_BOUND, UPPER_BOUND, EXACT};

struct TTEntry
{
	U64 hash;
	NodeType type;
	int depth;
	Value iterVal;
	Move bestMove;///hashmove
};

class TransTable
{
public:
	static constexpr int size = pow(2,20);
	TTEntry table[size];

	bool get(U64 hash, TTEntry& e)
	{
		e=table[hash%size];
		bool re = e.hash==hash;
		if(re)
		{
			//cout<<"hit for hash "<<_hash<<" with depth "<<e.depth<<endl;
		}
		return re;
	}
	void store(U64 hash, TTEntry e)
	{
		if(table[hash%size].hash!=hash || table[hash%size].depth<e.depth)//check depth
		{
			table[hash%size]=e;
		}
	}
};

extern TransTable tt;
extern EvalTable te;

struct OrderHelper
{
	int history[128][128];//from, to
	Move killers[MAX_GAME_LEN][2];//ply, index (2 possible)

	int SORT_CAPTURE=100000000;
	int SORT_HASH  = 200000000;
	int SORT_PROMO=90000000;
	int SORT_KILL=80000000;
	int SORT_VALUE[6];//for each piece


	void nextPly();
	void setKiller(Move killer, int ply);
	void initVals()
	{
		for (int i = 0; i < 6; ++i) {
			SORT_VALUE[i] = h.PIECE_VALUE[i];
		}
		SORT_VALUE[KING] = 400000000;//SORT_KING;
		for(int i = 0; i < 128; i++)
		{
			for(int j = 0; j < 128; j++)
			{
				history[i][j]=0;
			}
		}
		for(int i = 0; i < MAX_GAME_LEN; i++)
		{
			for(int j = 0; j < 2; j++)
			{
				killers[i][j]=Move();
			}
		}
	}
};
extern OrderHelper oh;



#endif /* SRC_TRANS_TABLE_H_ */
