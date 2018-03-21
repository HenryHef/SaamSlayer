/*
 * trans_table.cpp
 *
 *  Created on: Feb 26, 2018
 *      Author: henry
 */

#include "trans_table.h"

TransTable tt;
EvalTable te;
OrderHelper oh;

void OrderHelper::nextPly()
{
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			history[i][j] = history[i][j] / 8;
		}
	}
	for(int i = 1; i < MAX_GAME_LEN; i++)
	{
		killers[i-1][0]=killers[i][0];
		killers[i-1][1]=killers[i][1];
	}
}

void OrderHelper::setKiller(Move killer, int ply)
{
	if (!killer.isCapture)
	{
		/* make sure killer moves will be different
		   before saving secondary killer move */
		if (killer.from != killers[ply][0].from || killer.to   != killers[ply][0].to)
			killers[ply][1] = killers[ply][0];

		/* save primary killer move */
		killers[ply][0] = killer;
	}
}
