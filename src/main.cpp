/*
 * main.cpp
 *
 *  Created on: Feb 23, 2018
 *      Author: henry
 */

#include <iostream>
#include <string>

#include "basedef.h"
#include "board.h"
#include "eval.h"
#include "eval_consts.h"
#include "move_gen_sort.h"
#include "SaamSlayer.h"
#include "trans_table.h"

using namespace std;

void playgameHC(Player user, SaamSlayer ss)
{
	Board b = Board();
	b.setStartBoard();
	cout<<"Use algebraic notation. i.e. e2e4, castle is king 2 squest to the side"<<endl<<b<<endl;
	while(true)//TODO get en passant to work
	{
		Move move;
		if(b.toMove==user)
		{
			while(true)
			{
				string mv;
				cout<<"What is your move?"<<endl;
				cin>>mv;
				move=makeMoveSlow(b,FROM_ST(mv,0),FROM_ST(mv,2), mv.size()>4?mv[4]:' ');
				if(testMoveIsLegalSlow(b,move))
					break;
				else
					cout<<"ILLEGAL MOVE, try AGAIN"<<endl;
			}

		}
		else
		{
			move = ss.getMove(b);
		}
		cout<<"Playing:" <<move<<endl;
		if(move.isNull)
			break;
		b.makeMove(move);
		cout<<endl<<endl<<b<<endl;
		cout<<"Value of:"<<eval(b,true)<<endl;
		cout<<"SAVED EVAL"<<endl;
		printSavedEval();
	}
}
void playgameCC()
{
	SaamSlayer s1, s2;
	Board b = Board();
	b.setStartBoard();
	while(true)
	{
		Move move;
		if(b.toMove==WHITE)
		{
			move = s1.getMove(b);
		}
		else
		{
			move = s2.getMove(b);
		}
		cout<<move<<endl;
		if(move.isNull)
			break;
		b.makeMove(move);

		eval(b);
		cout<<"SAVED EVAL"<<endl;
		printSavedEval();
		cout<<endl<<endl<<"==============="<<endl<<b<<endl<<"==============="<<endl;
	}
}

int main(int argc, char* argv[])
{
	setDefaultEvalValues();
	initBoardHashData();
	oh.initVals();

	//playgameCC();
	playgameHC(BLACK,SaamSlayer());
	return 0;
}
