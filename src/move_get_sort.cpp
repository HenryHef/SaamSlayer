/*
 * move_get_sort.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: henry
 */

#ifndef SRC_MOVE_GET_SORT_CPP_
#define SRC_MOVE_GET_SORT_CPP_

#include "basedef.h"
#include "board.h"
#include "move_gen_sort.h"
#include "trans_table.h"
#include "Utils.h"

struct Board;
struct Move;

void MoveGenerator::genPLegalMoves(const Board &b, Player player)
{
	ct=0;
	for(int square = 0; square<120; square++)
	{
		if(!IS_SQ(square))
			continue;
		PieceType piece = b.pieces[square];
		Player color = b.colors[square];
		if(piece==NO_PIECE)
			continue;
		if(color!=player)
			continue;
		else if(piece==PAWN)
		{
			Square target = square+PLAYER_PAWN_FORWARD_VEC[color];
			if(IS_SQ(target)&&b.pieces[target]==NO_PIECE)//forward one
			{
				if(ROW(square)==PLAYER_PAWN_PREPROMO_ROW[color])
				{
					//add promo forward move
					moves[ct]=makeMovePromo(square,target,KNIGHT,color);	ct++;
					moves[ct]=makeMovePromo(square,target,BISHOP,color);	ct++;
					moves[ct]=makeMovePromo(square,target,ROOK,color);		ct++;
					moves[ct]=makeMovePromo(square,target,QUEEN,color);		ct++;
				}
				else
				{
					//add normal forward move
					moves[ct]=makeMoveNormal(square,target,PAWN,color);		ct++;
				}

				//then check forward two move
				if(ROW(square)==PLAYER_PAWN_HOME_ROW[color])
				{
					Square target2 = square+PLAYER_PAWN_FORWARD_VEC[color]*2;
					if(IS_SQ(target2)&&b.pieces[target2]==NO_PIECE)//forward two
					{
						//add forward 2
						moves[ct]=makeMoveNormal(square,target2,PAWN,color);		ct++;
					}
				}
			}

			//check captures
			for(int i = 0; i < 2; i++)
			{
				target = square+PLAYER_PAWN_CAPTURE_VECS[color][i];
				if(IS_SQ(target)
						&& b.pieces[target]!=NO_PIECE
						&& b.colors[target]== !color)
				{
					if(ROW(square)==PLAYER_PAWN_PREPROMO_ROW[color])
					{
						//add capture promo left
						moves[ct]=makeMovePromoCapture(square,target,b.pieces[target],KNIGHT,color);	ct++;
						moves[ct]=makeMovePromoCapture(square,target,b.pieces[target],BISHOP,color);	ct++;
						moves[ct]=makeMovePromoCapture(square,target,b.pieces[target],ROOK,color);		ct++;
						moves[ct]=makeMovePromoCapture(square,target,b.pieces[target],QUEEN,color);		ct++;
					}
					else
					{
						moves[ct]=makeMoveCapture(square,target,PAWN,b.pieces[target],color);		ct++;
						//add normal forward move
					}
				}
			}


		}
		else//piece is not PAWN
		{
			for(int vecI = 0; vecI< PIECE_MOVE_VECS_SIZE[piece]; vecI++)
			{
				if(PIECE_MOVE_VECS_IS_SLIDING[piece])
				{
					Square target = square;
					//do sliding
					while(true)
					{
						target=target+PIECE_MOVE_VECS[piece][vecI];
						if(!IS_SQ(target))
							break;
						if(b.pieces[target]!=NO_PIECE)//add capture
						{
							if(b.colors[target]==!color)
							{
								moves[ct]=makeMoveCapture(square,target,piece,b.pieces[target],color);
								ct++;
							}
							break;
						}
						else//add normal move
						{
							moves[ct]=makeMoveNormal(square,target,piece,color);
							ct++;
						}
					}
				}
				else//non sliding
				{
					Square target=square+PIECE_MOVE_VECS[piece][vecI];
					if(IS_SQ(target))
					{
						if(b.pieces[target]==NO_PIECE)
						{
							moves[ct]=makeMoveNormal(square,target,piece,color);
							ct++;
						}
						else if(b.colors[target]==!color)
						{
							moves[ct]=makeMoveCapture(square,target,piece,b.pieces[target],color);
							ct++;
						}
					}
				}
			}
		}
	}
	if(player==WHITE)
	{
		//cout<<"Board:"<<b<<endl<<"Castling rights:"<<(int)b.castlingRightsFlag<<endl;
		if((b.castlingRightsFlag&WHITE_KING_CR) &&
				b.pieces[F1]==NO_PIECE &&
				b.pieces[G1]==NO_PIECE &&
				!b.sqIsAttacked(F1,BLACK) &&
				!b.sqIsAttacked(G1,BLACK) &&
				!b.sqIsAttacked(E1,BLACK))
		{
			moves[ct]=makeMoveCastle(WHITE,true);
			ct++;
		}
		if((b.castlingRightsFlag&WHITE_QUEEN_CR) &&
				b.pieces[D1]==NO_PIECE &&
				b.pieces[C1]==NO_PIECE &&
				b.pieces[B1]==NO_PIECE &&
				!b.sqIsAttacked(D1,BLACK) &&
				!b.sqIsAttacked(C1,BLACK) &&
				!b.sqIsAttacked(E1,BLACK))
		{
			moves[ct]=makeMoveCastle(WHITE,false);
			ct++;
		}
	}
	else if(player==BLACK)
	{
		if((b.castlingRightsFlag&BLACK_KING_CR) &&
				b.pieces[F8]==NO_PIECE &&
				b.pieces[G8]==NO_PIECE &&
				!b.sqIsAttacked(F8,WHITE) &&
				!b.sqIsAttacked(G8,WHITE) &&
				!b.sqIsAttacked(E8,WHITE))
		{
			moves[ct]=makeMoveCastle(BLACK,true);
			ct++;
		}
		if((b.castlingRightsFlag&BLACK_QUEEN_CR) &&
				b.pieces[D8]==NO_PIECE &&
				b.pieces[C8]==NO_PIECE &&
				b.pieces[B8]==NO_PIECE &&
				!b.sqIsAttacked(D8,WHITE) &&
				!b.sqIsAttacked(C8,WHITE) &&
				!b.sqIsAttacked(E8,WHITE))
		{
			moves[ct]=makeMoveCastle(BLACK,false);
			ct++;
		}
	}
	//TODO append castling if applicable

	shuffle(moves,ct);
}
void MoveGenerator::genPLegalCaptures(const Board &b, Player player)
{
	ct=0;
	for(int square = 0; square<120; square++)
	{
		if(!IS_SQ(square))
			continue;
		PieceType piece = b.pieces[square];
		Player color = b.colors[square];
		if(piece==NO_PIECE)
			continue;
		if(color!=player)
			continue;
		else if(piece==PAWN)
		{
			Square target;
			//check captures
			for(int i = 0; i < 2; i++)
			{
				target = square+PLAYER_PAWN_CAPTURE_VECS[color][i];
				if(IS_SQ(target)
						&& b.pieces[target]!=NO_PIECE
						&& b.colors[target]== !color)
				{
					if(ROW(square)==PLAYER_PAWN_PREPROMO_ROW[color])
					{
						//add capture promo left
						moves[ct]=makeMovePromoCapture(square,target,b.pieces[target],KNIGHT,color);	ct++;
						moves[ct]=makeMovePromoCapture(square,target,b.pieces[target],BISHOP,color);	ct++;
						moves[ct]=makeMovePromoCapture(square,target,b.pieces[target],ROOK,color);		ct++;
						moves[ct]=makeMovePromoCapture(square,target,b.pieces[target],QUEEN,color);		ct++;
					}
					else
					{
						moves[ct]=makeMoveCapture(square,target,PAWN,b.pieces[target],color);		ct++;
					}
				}
			}
		}
		else//piece is not PAWN
		{
			for(int vecI = 0; vecI< PIECE_MOVE_VECS_SIZE[piece]; vecI++)
			{
				if(PIECE_MOVE_VECS_IS_SLIDING[piece])
				{
					Square target = square;
					//do sliding
					while(true)
					{
						target=target+PIECE_MOVE_VECS[piece][vecI];
						if(!IS_SQ(target))
							break;
						if(b.pieces[target]!=NO_PIECE)//add capture
						{
							if(b.colors[target]==!color)
							{
								moves[ct]=makeMoveCapture(square,target,piece,b.pieces[target],color);
								ct++;
							}
							break;
						}
					}
				}
				else//non sliding
				{
					Square target=square+PIECE_MOVE_VECS[piece][vecI];
					if(IS_SQ(target))
					{
						if(b.pieces[target]!=NO_PIECE && b.colors[target]==!color)
						{
							moves[ct]=makeMoveCapture(square,target,piece,b.pieces[target],color);
							ct++;
						}
					}
				}
			}
		}
	}
	//TODO append castling if applicable

	shuffle(moves,ct);
}
void MoveGenerator::preSortQuiessence(Board &b, TTEntry &ent, int ply)
{
	//TODO use killer moves
	//taken from CPW Engine
	for(int i = 0; i < ct; i++)
	{
		moves[i].score = oh.history[moves[i].m.from][moves[i].m.to];

		if (moves[i].m.isCapture)
		{
			moves[i].score = oh.SORT_CAPTURE + oh.SORT_VALUE[moves[i].m.toPieceOld] + moves[i].m.fromPiece;
		}

		//score for ep-capture
		if ((moves[i].m.fromPiece == PAWN) && (moves[i].m.to == b.enp)) {
			moves[i].score = oh.SORT_CAPTURE + oh.SORT_VALUE[PAWN] + PAWN;
		}

		if (moves[i].m.isPromo)
		{
			moves[i].score += oh.SORT_PROMO + oh.SORT_VALUE[moves[i].m.toPieceNew];
		}

		if(moves[i].m.to==ent.bestMove.to && moves[i].m.from==ent.bestMove.from)
			moves[i].score=oh.SORT_HASH;
	}
}
void MoveGenerator::preSort(Board &b, TTEntry &ent, int ply)
{
	//TODO use killer moves
	//taken from CPW Engine
	for(int i = 0; i < ct; i++)
	{
	    moves[i].score = oh.history[moves[i].m.from][moves[i].m.to];

		if (moves[i].m.isCapture)
		{
			moves[i].score = oh.SORT_CAPTURE + oh.SORT_VALUE[moves[i].m.toPieceOld] + moves[i].m.fromPiece;
		}

		//score for ep-capture
		if ((moves[i].m.fromPiece == PAWN) && (moves[i].m.to == b.enp)) {
			moves[i].score = oh.SORT_CAPTURE + oh.SORT_VALUE[PAWN] + PAWN;
		}

		if (moves[i].m.isPromo)
		{
			moves[i].score += oh.SORT_PROMO + oh.SORT_VALUE[moves[i].m.toPieceNew];
		}

		if(moves[i].m.to==ent.bestMove.to && moves[i].m.from==ent.bestMove.from)
			moves[i].score=oh.SORT_HASH;
	}
	for(int i = 0; i < ct; i++)
	{
		if ((moves[i].m.from == oh.killers[ply][1].from )
				&& (moves[i].m.to   == oh.killers[ply][1].to   )
				&& (moves[i].score < oh.SORT_KILL-1 ) ) {
			moves[i].score = oh.SORT_KILL-1;
		}

		if ((moves[i].m.from == oh.killers[ply][0].from )
				&& (moves[i].m.to   == oh.killers[ply][0].to   )
				&& (moves[i].score < oh.SORT_KILL ) ) {
			moves[i].score = oh.SORT_KILL;
		}
	}
}

Move MoveGenerator::loadNextMove(Board &b, int newIndex)
{
	int bestI = newIndex;
	for(int i =newIndex; i < ct; i++)
	{
		if(moves[bestI].score<moves[i].score)
		{
			bestI=i;
		}
	}
	ScoredMove tmp = moves[newIndex];
	moves[newIndex]=moves[bestI];
	moves[bestI]=tmp;
	/*
	 *
104
725
2102
24454
108468
CHECKMATE!!
CHECKMATE!!
CHECKMATE!!
707709
	 */
	return moves[newIndex].m;
}

bool testMoveIsLegalSlow(Board &b, Move move)
{
	MoveGenerator gen;
	gen.genPLegalMoves(b,b.toMove);
	for(int i = 0; i < gen.ct; i++)
	{
		if(move.to==gen.moves[i].m.to && move.from==gen.moves[i].m.from)
		{
			b.makeMove(move);
			if(b.isCheck(!b.toMove))
			{
				b.unmakeMove(move);
				return false;
			}
			else
			{
				b.unmakeMove(move);
				return true;
			}
		}
	}
	return false;
}

#endif /* SRC_MOVE_GET_SORT_CPP_ */
