/*
 * eval.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: henry
 */

#include "eval.h"

#include <iostream>

#include "board.h"
#include "eval_consts.h"
#include "pawn_eval.h"
#include "trans_table.h"

int PIECE_GAME_PHASE_CONSTS[] = {0,4,2,1,1,0};
static const int TOTAL_PHASE =
		PIECE_GAME_PHASE_CONSTS[PAWN]*16 +
		PIECE_GAME_PHASE_CONSTS[KNIGHT]*4 +
		PIECE_GAME_PHASE_CONSTS[BISHOP]*4 +
		PIECE_GAME_PHASE_CONSTS[ROOK]*4 +
		PIECE_GAME_PHASE_CONSTS[QUEEN]*2;
static const int PHASE_MID = TOTAL_PHASE*3/4;
static const int PHASE_END = TOTAL_PHASE*1/8;
static const int PHASE_RANGE = PHASE_MID-PHASE_END;
int calcGamePhase(Board &b)
{
	int phase = TOTAL_PHASE;

	phase -= (b.pieceCount[WHITE][PAWN]+b.pieceCount[BLACK][PAWN]) * PIECE_GAME_PHASE_CONSTS[PAWN];
	phase -= (b.pieceCount[WHITE][KNIGHT]+b.pieceCount[BLACK][KNIGHT]) * PIECE_GAME_PHASE_CONSTS[KNIGHT];
	phase -= (b.pieceCount[WHITE][BISHOP]+b.pieceCount[BLACK][BISHOP]) * PIECE_GAME_PHASE_CONSTS[BISHOP];
	phase -= (b.pieceCount[WHITE][ROOK]+b.pieceCount[BLACK][ROOK]) * PIECE_GAME_PHASE_CONSTS[ROOK];
	phase -= (b.pieceCount[WHITE][QUEEN]+b.pieceCount[BLACK][QUEEN]) * PIECE_GAME_PHASE_CONSTS[QUEEN];

	if(phase>PHASE_MID)
		phase=PHASE_MID;
	if(phase<PHASE_END)
		phase=PHASE_END;
	phase-=PHASE_END;

	return phase;
}

EvalHelper savedEval;
bool verbose = false;

Value eval(Board& b, bool redo)
{
	verbose=redo;
	Value re;
	if(!redo && te.getEval(b.hash,re))
		return re;
	//cout<<"EVALING:"<<b<<endl;
	EvalHelper e;
	evalPawns(b,e);

	evalMaterial(b,e);
	/*Value PLAYER_PAST_PAWN_SQUARE_VALUE[2][120];
	Value PLAYER_WEEK_PAWN_SQUARE_VALUE[2][120];*/

	//loop over board, if piece, add piece square value, and then ...
	/**/

	for(Square sq  =0; sq < 120; sq++)
	{
		//DONE Value P_BLOCK_CENTRAL_PAWN;
		//DONE Value PIECE_KING_SQUARE_LOC_TROPISM_VALUES[6][120][120];
		//DONE Value PLAYER_PIECE_MGAME_SQUARE_VALUE[2][6][120];
		//DONE Value PLAYER_PIECE_EGAME_SQUARE_VALUE[2][6][120];

		if(!IS_SQ(sq))
			continue;
		Player color = b.colors[sq];
		PieceType piece = b.pieces[sq];
		if(piece==NO_PIECE)
			continue;
		//cout<<"is piece e.midPSVal[WHITE]="<<e.midPSVal[WHITE]<<endl;
		//first
		e.midPSVal[color]+=h.PLAYER_PIECE_MGAME_SQUARE_VALUE[color][piece][sq];
		e.endPSVal[color]+=h.PLAYER_PIECE_EGAME_SQUARE_VALUE[color][piece][sq];
		//cout<<"val:"<<h.PLAYER_PIECE_MGAME_SQUARE_VALUE[WHITE][PAWN][E4]<<endl;

		if(ROW(sq)==PLAYER_ROW_INDEX[color][2] &&
				b.pieces[sq-PLAYER_PAWN_FORWARD_VEC[color]]==PAWN)//both middle game
			e.matVal[color]-=h.P_BLOCK_CENTRAL_PAWN;

		e.tropism[color]+=h.PIECE_KING_SQUARE_LOC_TROPISM_VALUES[piece][b.kingSq[color]][sq];
		if(piece==BISHOP)
			evalBishop(b,e,sq);
		else if(piece==KNIGHT)
			evalKinght(b,e,sq);
		else if(piece==ROOK)
			evalRook(b,e,sq);
		else if(piece==QUEEN)
			evalQueen(b,e,sq);
		else if(piece==KING)
			evalKing(b,e,sq);
	}

	evalKingSafety(b,e);
	evalOtherLast(b,e);

	int midVal = e.midPSVal[WHITE]-e.midPSVal[BLACK];
	int endVal = e.endPSVal[WHITE]-e.endPSVal[BLACK];
	int gamePhase = calcGamePhase(b);//from 0...PHASE_RANGE
	int mixedEval=(midVal*(PHASE_RANGE-gamePhase)+endVal*gamePhase)/PHASE_RANGE;

	mixedEval+=e.matVal[WHITE]+e.pawnVal[WHITE]+e.kingSafety[WHITE]+e.otherVal[WHITE];
	mixedEval-=e.matVal[BLACK]+e.pawnVal[BLACK]+e.kingSafety[BLACK]+e.otherVal[BLACK];

	re = mixedEval*(b.toMove==WHITE?1:-1);
	te.storeEval(b.hash,re);

	savedEval=e;

	//cout<<e.matVal[WHITE]<<"  "<< e.pawnVal[WHITE]<<"  "<<e.matVal[BLACK]<<"  "<<e.pawnVal[BLACK]<<endl;
	return re;//put in play to moves perspective for negamax

	//TODO, add pawn bishop coordination
	//pawn evaluation TODO improve
	//TODO mobility, safe mobility
	//TODO center controll
}

void printSavedEval()
{
	cout<<savedEval.tropism[WHITE]<<"  "<<savedEval.tropism[BLACK]<<endl;
}

void evalMaterial(Board& b, EvalHelper& e)
{
	for(int player = 0; player < 2; player++)
	{
		e.matVal[player]=b.material[player];

		if(b.pieceCount[player][BISHOP]==2)
			e.matVal[player]+=h.BISHOP_PAIR;
		if(b.pieceCount[player][KNIGHT]==2)
			e.matVal[player]-=h.P_KNIGHT_PAIR;
		if(b.pieceCount[player][ROOK]==2)
			e.matVal[player]-=h.P_ROOK_PAIR;
		e.matVal[player]+=h.KNIGHT_PAWN_COUNT_ADJUSTMENT[b.pieceCount[player][PAWN]]*b.pieceCount[player][KNIGHT];
		e.matVal[player]+=h.ROOK_PAWN_COUNT_ADJUSTMENT[b.pieceCount[player][PAWN]]*b.pieceCount[player][ROOK];
	}
}

void evalBishop(Board& b, EvalHelper& e, Square sq)
{
	Player p = b.colors[sq];
	//TODO add pawn mod(square) good vs. bad bishop evaluation
	//add pin vs rook, king, queen, unprotected other piece lookup
	/*Value P_BISHOP_TRAPPED_A7;
	Value P_BISHOP_TRAPPED_A6;
	Value FIANCHETTO;*/
	if(p==WHITE &&
			((sq==A7 && b.isPiece(B6,PAWN,BLACK)) ||
			(sq==H7 && b.isPiece(G6,PAWN,BLACK))))
		e.matVal[WHITE]-=h.P_BISHOP_TRAPPED_A7;
	else if(p==WHITE &&
			((sq==A6 && b.isPiece(B5,PAWN,BLACK) && b.isPiece(C6,PAWN,BLACK)) ||
			(sq==H6 && b.isPiece(G5,PAWN,BLACK) && b.isPiece(F6,PAWN,BLACK))))
		e.matVal[WHITE]-=h.P_BISHOP_TRAPPED_A6;
	else if(p==BLACK &&
			((sq==A2 && b.isPiece(B3,PAWN,WHITE)) ||
			(sq==H2 && b.isPiece(G3,PAWN,WHITE))))
		e.matVal[BLACK]-=h.P_BISHOP_TRAPPED_A7;
	else if(p==BLACK &&
			((sq==A3 && b.isPiece(B4,PAWN,WHITE) && b.isPiece(C3,PAWN,WHITE)) ||
			(sq==H3 && b.isPiece(G4,PAWN,WHITE) && b.isPiece(F3,PAWN,WHITE))))
		e.matVal[BLACK]-=h.P_BISHOP_TRAPPED_A6;

	if(p==WHITE &&
			((sq==B2 && b.isPiece(B3,PAWN,WHITE)) ||
			(sq==G2 && b.isPiece(G3,PAWN,WHITE))))
	{
		e.matVal[WHITE]+=h.FIANCHETTO;
		e.fienCt[WHITE]++;
	}
	if(p==BLACK &&
			((sq==B7 && b.isPiece(B6,PAWN,BLACK)) ||
			(sq==G7 && b.isPiece(G6,PAWN,BLACK))))
	{
		e.matVal[BLACK]+=h.FIANCHETTO;
		e.fienCt[BLACK]++;
	}

	//mobility evaluation
	int safeMobility = 0;
	for(int vecI = 0; vecI< PIECE_MOVE_VECS_SIZE[BISHOP]; vecI++)
	{
		//is sliding piece
		Square target = sq;
		//do sliding
		while(true)
		{
			target=target+PIECE_MOVE_VECS[BISHOP][vecI];
			if(!IS_SQ(target))
				break;
			if((b.pieces[target]==NO_PIECE||b.colors[target]==!p) && b.pawnControllMap[!p][target]==0)//add capture
			{
				/*if(verbose)
					cout<<"BISHOP on "<<TO_ST(sq)<<" has msafe mobility on sq "<<TO_ST(target)<<endl;*/
				safeMobility++;
			}
			else if(b.pieces[target]!=NO_PIECE)
				break;
		}
	}//TODO add pins
	e.matVal[p]+=h.BISHOP_MOBILITY_VALUES[safeMobility];
}
void evalKinght(Board& b, EvalHelper& e, Square sq)
{
	Player p = b.colors[sq];
	//TODO
	/*Value P_KNIGHT_TRAPPED_A8;
	Value P_KNIGHT_TRAPPED_A7;
	Value P_C3_KNIGHT;*/

	if(p==WHITE &&
			((sq==H8 && (b.isPiece(H7,PAWN,BLACK) || b.isPiece(F7,PAWN,BLACK))) ||
			(sq==A8 && (b.isPiece(A7,PAWN,BLACK) || b.isPiece(C7,PAWN,BLACK)))))
		e.matVal[WHITE]-=h.P_KNIGHT_TRAPPED_A8;
	else if(p==WHITE &&
			((sq==A7 && b.isPiece(B7,PAWN,BLACK) && (b.isPiece(A6,PAWN,BLACK) || b.isPiece(C6,PAWN,BLACK))) ||
			(sq==H7 && b.isPiece(G7,PAWN,BLACK) && (b.isPiece(H6,PAWN,BLACK) || b.isPiece(F6,PAWN,BLACK)))))
		e.matVal[WHITE]-=h.P_KNIGHT_TRAPPED_A7;
	else if(p==BLACK &&
			((sq==H1 && (b.isPiece(H2,PAWN,WHITE) || b.isPiece(F2,PAWN,WHITE))) ||
			(sq==A1 && (b.isPiece(A2,PAWN,WHITE) || b.isPiece(C2,PAWN,WHITE)))))
		e.matVal[BLACK]-=h.P_KNIGHT_TRAPPED_A8;
	else if(p==BLACK &&
			((sq==A2 && b.isPiece(B2,PAWN,WHITE) && (b.isPiece(A3,PAWN,WHITE) || b.isPiece(C3,PAWN,WHITE))) ||
			(sq==H2 && b.isPiece(G2,PAWN,WHITE) && (b.isPiece(H3,PAWN,WHITE) || b.isPiece(F3,PAWN,WHITE)))))
		e.matVal[BLACK]-=h.P_KNIGHT_TRAPPED_A7;

	if(p==WHITE && sq==C3 && b.isPiece(C2,PAWN,WHITE))
			e.matVal[WHITE]-=h.P_C3_KNIGHT;
	else if(p==BLACK && sq==C6 && b.isPiece(C7,PAWN,BLACK))
			e.matVal[BLACK]-=h.P_C3_KNIGHT;

	int safeMobility = 0;
	for(int vecI = 0; vecI< PIECE_MOVE_VECS_SIZE[KNIGHT]; vecI++)
	{
		//is sliding piece
		Square target=sq+PIECE_MOVE_VECS[KNIGHT][vecI];
		if(!IS_SQ(target))
			continue;
		if((b.pieces[target]==NO_PIECE||b.colors[target]==!p) && b.pawnControllMap[!p][target]==0)//add capture
		{
			safeMobility++;
		}
	}
	e.matVal[p]+=h.KNIGHT_MOBILITY_VALUES[safeMobility];
}
void evalRook(Board& b, EvalHelper& e, Square sq)
{
	Player p = b.colors[sq];
	if(p==WHITE &&
			(sq==H1 || sq==H2 || sq==G1 || sq==G2) &&
			(b.kingSq[WHITE]==F1 || b.kingSq[WHITE]==G1))
		e.matVal[WHITE]-=h.P_KING_BLOCKS_ROOK;
	else if(p==WHITE &&
			(sq==A1 || sq==A2 || sq==B1 || sq==B2) &&
			(b.kingSq[WHITE]==B1 || b.kingSq[WHITE]==C1))//TODO include D1?
		e.matVal[WHITE]-=h.P_KING_BLOCKS_ROOK;
	else if(p==BLACK &&
			(sq==H8 || sq==H7 || sq==G8 || sq==G7) &&
			(b.kingSq[BLACK]==F8 || b.kingSq[BLACK]==G8))
		e.matVal[BLACK]-=h.P_KING_BLOCKS_ROOK;
	else if(p==BLACK &&
			(sq==A8 || sq==A7 || sq==B8 || sq==B7) &&
			(b.kingSq[BLACK]==B8 || b.kingSq[BLACK]==C8))//TODO include D1?
		e.matVal[BLACK]-=h.P_KING_BLOCKS_ROOK;

	if(b.pawnPlayerColCount[p][COL(sq)]==0)//then either open or half open
	{
		if(b.pawnPlayerColCount[!p][COL(sq)]==0)//open
		{
			e.matVal[p]+=h.ROOK_OPEN;
		}
		else//half open
		{
			e.matVal[p]+=h.ROOK_HALF;
		}
	}

	int safeMobility;
	for(int vecI = 0; vecI< PIECE_MOVE_VECS_SIZE[ROOK]; vecI++)
	{
		//is sliding piece
		Square target = sq;
		//do sliding
		while(true)
		{
			target=target+PIECE_MOVE_VECS[ROOK][vecI];
			if(!IS_SQ(target))
				break;
			if((b.pieces[target]==NO_PIECE||b.colors[target]==!p) && b.pawnControllMap[!p][target]==0)//add capture
			{
				/*if(verbose)
					cout<<"BISHOP on "<<TO_ST(sq)<<" has msafe mobility on sq "<<TO_ST(target)<<endl;*/
				safeMobility++;
			}
			else if(b.pieces[target]!=NO_PIECE)
				break;
		}
	}//TODO add pins
	e.matVal[p]+=h.ROOK_MOBILITY_VALUES[safeMobility];
	//TODO
	/*Value ROOK_OPEN;
	Value ROOK_HALF;
	Value P_KING_BLOCKS_ROOK;*/
	/*
	 *
    a white rook on h1/g1/h2/g2 with a white king on f1 or g1 (perhaps -40,
    to prevent pseudo-castling with a rook hemmed in)
	 *
	 */
}

static const Square PLAYER_MINOR_PIECE_SQUARES[2][4] = {{B1,C1,F1,G1},{B8,C8,F8,G8}};
static const Square QUEEN_HOME_SQ[2] = {D1,D8};
static const Square PLAYER_MINOR_PIECE_PIECES[4] = {KNIGHT,BISHOP,BISHOP,KNIGHT};
void evalQueen(Board& b, EvalHelper& e, Square sq)
{
	Player p = b.colors[sq];
	int undevMinorCt = 0;
	for(int i = 0; i < 4; i++)
	{
		if(b.pieces[PLAYER_MINOR_PIECE_SQUARES[p][i]]==PLAYER_MINOR_PIECE_PIECES[i] &&
				b.colors[PLAYER_MINOR_PIECE_SQUARES[p][i]]==p)
			undevMinorCt++;
	}
	if(sq==QUEEN_HOME_SQ[p])
	{
		Value boost[] = {0,10,20,30,40};

		//cout<<hit[undevMinorCt]<<endl;
		e.matVal[p]+=boost[undevMinorCt];
	}
}
void evalKing(Board& b, EvalHelper& e, Square sq)
{
	//do nothing
}
void evalKingSafety(Board& b, EvalHelper& e)
{
	for(int player = 0; player<2; player++)
	{
		Square ksq = b.kingSq[player];
		SquareVec forw =PLAYER_PAWN_FORWARD_VEC[player];

		/*
		 * x 0 x
		 * 3 2 1
		 * x K x
		 *
		 * having pawns on 0123 squares matters, will be stored in a bool[4]
		 */
		bool pawn[4];
		pawn[0]=(b.pieces[ksq+ forw*2]==PAWN && b.colors[ksq+forw*2]==player);
		pawn[1]=(b.pieces[ksq+ forw+EAST]==PAWN && b.colors[ksq+forw+EAST]==player);
		pawn[2]=(b.pieces[ksq+ forw]==PAWN && b.colors[ksq+forw]==player);
		pawn[3]=(b.pieces[ksq+ forw+WEST]==PAWN && b.colors[ksq+forw+WEST]==player);
		if(COL(ksq)==0)
		{
			if(pawn[2] && pawn[1])
				e.kingSafety[player]+=h.SHIELD_2;
			else if(pawn[2])
				e.kingSafety[player]+=h.SHIELD_1;

			//TODO add open and semiopen penalties
		}
		else if(COL(ksq)==7)
		{
			if(pawn[2] && pawn[3])
				e.kingSafety[player]+=h.SHIELD_2;
			else if(pawn[2])
				e.kingSafety[player]+=h.SHIELD_1;

			//TODO add open and semiopen penalties
		}
		else if(!(COL(ksq)==4||COL(ksq)==5))//dont count shielding in the center
		{
			if((pawn[1] && pawn[2] && pawn[3])||(pawn[1] && pawn[0] && pawn[3]))
				e.kingSafety[player]+=h.SHIELD_2;
			else if((pawn[2] && pawn[3])||(pawn[2] && pawn[1]))
				e.kingSafety[player]+=h.SHIELD_1;

			//TODO add open and semiopen penalties
		}


	}
}
void evalOtherLast(Board& b, EvalHelper& e)
{
	if(e.fienCt[WHITE]==0)
		e.otherVal[WHITE]-=h.P_NO_FIANCHETTO;
	if(e.fienCt[BLACK]==0)
		e.otherVal[BLACK]-=h.P_NO_FIANCHETTO;
	e.otherVal[b.toMove]+=h.TEMPO;
}


