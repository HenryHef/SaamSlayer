/*
 * board_move.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: henry
 */

#include <cmath>
#include <cstdlib>
#include <random>

#include "board.h"
#include "eval_consts.h"
#include "move_gen_sort.h"

ZorbistHashStuff z;
/*
static U64 PLAYER_PIECE_SQUARE_HASH_VALS[2][6][120];
static U64 CASTLING_RIGHTS_HASH_VALS[16];
static U64 WHITE_TO_MOVE_HASH_VAL;
static U64 ENP_HASH_VAL[120];
 */

constexpr int HMS = pow(2,15);

Board hashMap[HMS];

void initBoardHashData()
{
	int SEED = 123456;

	std::mt19937_64 e2(SEED);

	std::uniform_int_distribution<unsigned long long int> dist;

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			for(int k = 0; k < 120; k++)
			{
				z.PLAYER_PIECE_SQUARE_HASH_VALS[i][j][k]=dist(e2);
			}
		}
	}
	for(int i = 0; i < 16; i++)
	{
		z.CASTLING_RIGHTS_HASH_VALS[i]=dist(e2);
	}
	z.WHITE_TO_MOVE_HASH_VAL=dist(e2);
	for(int i = 0; i < 120; i++)
	{
		z.ENP_HASH_VALS[i]=dist(e2);
	}
}


Move makeMoveNormal(Square _from, Square _to,
			PieceType _piece,
			Player _player)
{
	return Move(_from,_to,
			_piece,NO_PIECE,_piece,
			false,false,false,
			false,false,false,
			_player);
}
Move makeMoveCapture(Square _from, Square _to,
		PieceType _fromPiece, PieceType _toPieceOld,
		Player _player)
{
	return Move(_from,_to,
			_fromPiece,_toPieceOld,_fromPiece,
			false,false,true,
			false,false,false,
			_player);
}
Move makeMovePromoCapture(Square _from, Square _to,
		PieceType _toPieceOld, PieceType _toPieceNew,
		Player _player)
{
	return Move(_from,_to,
			PAWN,_toPieceOld,_toPieceNew,
			false,true,true,
			false,false,false,
			_player);
}
Move makeMovePromo(Square _from, Square _to, PieceType _toPieceNew,
		Player _player)
{
	return Move(_from,_to,
			PAWN,NO_PIECE,_toPieceNew,
			false,true,false,
			false,false,false,
			_player);
}
static const Move CASTLING_MOVE_WHITE_KINGSIDE =
		Move(E1,G1,//WK
			KING,NO_PIECE,KING,
			false,false,false,
			true,true,false,
			WHITE);
static const Move CASTLING_MOVE_WHITE_QUEENSIDE =
		Move(E1,C1,//WQ
			KING,NO_PIECE,KING,
			false,false,false,
			true,false,false,
			WHITE);
static const Move CASTLING_MOVE_BLACK_KINGSIDE =
		Move(E8,G8,//BK
			KING,NO_PIECE,KING,
			false,false,false,
			true,true,false,
			BLACK);
static const Move CASTLING_MOVE_BLACK_QUEENSIDE =
		Move(E8,C8,//BQ
			KING,NO_PIECE,KING,
			false,false,false,
			true,false,false,
			BLACK);

//WK,WQ,BK,BQ   NOTE: index != enum vals
Move makeMoveCastle(Player _player,bool _kingside)
{
	//cout<<"adding castle for player "<<_player<<" kingside?"<<_kingside<<endl;
	if(_player==WHITE)
	{
		if(_kingside)
			return CASTLING_MOVE_WHITE_KINGSIDE;
		else//(_kingside)
			return CASTLING_MOVE_WHITE_QUEENSIDE;
	}
	else if(_player==BLACK)
	{
		if(_kingside)
			return CASTLING_MOVE_BLACK_KINGSIDE;
		else//(_kingside)
			return CASTLING_MOVE_BLACK_QUEENSIDE;
	}
	else
	{
		cout<<"INVADING INPUT TO makeMoveCastle"<<endl;
		return Move();
	}
}
Move makeMoveNull(Player _player)
{
	return Move(INVALID_SQ,INVALID_SQ,
			NO_PIECE,NO_PIECE,NO_PIECE,
			false,false,false,
			false,false,true,
			_player);
}
Move makeMoveSlow(Board &b, Square from, Square to, char extra)
{
	Player player = b.colors[from];
	PieceType pieceToOld = b.pieces[to];
	PieceType pieceFrom = b.pieces[from];
	bool capture=false;
	if(pieceFrom==KING && abs(COL(from)-COL(to))==2)//castle
	{
		return makeMoveCastle(player,COL(to)==6);
	}
	if(pieceToOld!=NO_PIECE)
		capture=true;
	if(pieceFrom==PAWN && (ROW(to)==0||ROW(to)==7))//capture
	{
		PieceType pp = QUEEN;
		if(extra=='r'||extra=='R')
			pp=ROOK;
		else if(extra=='b'||extra=='B')
			pp=BISHOP;
		else if(extra=='n'||extra=='N')
			pp=KNIGHT;
		if(capture)
			return makeMovePromoCapture(from, to, pieceToOld, pp, player);
		else
			return makeMovePromo(from, to, pp, player);
	}
	if(capture)
		return makeMoveCapture(from, to, pieceFrom, pieceToOld, player);
	else
		return makeMoveNormal(from, to, pieceFrom, player);
}

Board::Board()
{
	for(int i = 0; i < 120; i++)
	{
		pieces[i]=NO_PIECE;
		colors[i]=NO_PLAYER;
	}
	enp=INVALID_SQ;//set onl if there is an enpassant possible
	toMove=WHITE;
	castlingRightsFlag=0;//WHITE_KING_CR|WHITE_QUEEN_CR|BLACK_KING_CR|BLACK_QUEEN_CR;//KING=0, QUEEN=1, as inenum

	for(int i =0; i < 2; i++)
	{
		material[i]=0;

		for(int p = 0; p < 6; p++)
			pieceCount[i][p]=0;
		kingSq[i]=INVALID_SQ;

		for(int p = 0; p < 120; p++)
			pawnControllMap[i][p]=0;
		for(int p = 0; p < 2; p++)
			pawnPlayerModSqCount[i][p]=0;
		for(int p = 0; p < 8; p++)
			pawnPlayerColCount[i][p]=0;
		for(int p = 0; p < 8; p++)
			pawnPlayerRowCount[i][p]=0;
	}

	hash=z.WHITE_TO_MOVE_HASH_VAL;
	hash^=z.CASTLING_RIGHTS_HASH_VALS[castlingRightsFlag];
	hash^=z.ENP_HASH_VALS[enp];
	phash=0;

	for(int i =0; i < MAX_GAME_LEN; i++)
	{
		pastPositionHashes[i]=0;
		pastEnp[i]=INVALID_SQ;
		pastCastlingRightsFlag[i]=0;
	}
	histroyStackNextIndex=1;
}

void Board::setStartBoard()
{
	toMove=WHITE;
	hash=z.WHITE_TO_MOVE_HASH_VAL;

	for(int i =0; i < 8; i++)
	{
		setSquare(MK_SQ(1,i),WHITE,PAWN);
		setSquare(MK_SQ(6,i),BLACK,PAWN);
		if(i==0||i==7)
		{
			setSquare(MK_SQ(0,i),WHITE,ROOK);
			setSquare(MK_SQ(7,i),BLACK,ROOK);
		}
		else if(i==1||i==6)
		{
			setSquare(MK_SQ(0,i),WHITE,KNIGHT);
			setSquare(MK_SQ(7,i),BLACK,KNIGHT);
		}
		else if(i==2||i==5)
		{
			setSquare(MK_SQ(0,i),WHITE,BISHOP);
			setSquare(MK_SQ(7,i),BLACK,BISHOP);
		}
		else if(i==3)
		{
			setSquare(MK_SQ(0,i),WHITE,QUEEN);
			setSquare(MK_SQ(7,i),BLACK,QUEEN);
		}
		else if(i==4)
		{
			setSquare(MK_SQ(0,i),WHITE,KING);
			setSquare(MK_SQ(7,i),BLACK,KING);
		}
	}
	//redo catling rights, now as they have been changed
	castlingRightsFlag= WHITE_KING_CR | WHITE_QUEEN_CR | BLACK_KING_CR | BLACK_QUEEN_CR;//change flag
	hash^=z.CASTLING_RIGHTS_HASH_VALS[castlingRightsFlag];//apply new

	enp=INVALID_SQ;
	hash^=z.ENP_HASH_VALS[enp];

	pastPositionHashes[0]=hash;
	pastEnp[0]=enp;
	pastCastlingRightsFlag[0]=castlingRightsFlag;

	histroyStackNextIndex=1;
}
void Board::setSquare(Square sq,Player player, PieceType piece)
///alters pieces, colors, hash, phash, material, pieceCount, kingSq
{
	//first clear the square
	if(pieces[sq]!=NO_PIECE)//undo onl ownership hashes
	{
		Player ocolor = colors[sq];
		PieceType opiece = pieces[sq];
		hash^=z.PLAYER_PIECE_SQUARE_HASH_VALS[ocolor][opiece][sq];
		if(pieces[sq]==PAWN)
		{
			phash^=z.PLAYER_PIECE_SQUARE_HASH_VALS[ocolor][opiece][sq];

			for(int i = 0; i < 2; i++)
			{

				Square ts = sq+PLAYER_PAWN_CAPTURE_VECS[ocolor][i];
				if(IS_SQ(ts))
					pawnControllMap[ocolor][ts]--;
			}
			pawnPlayerModSqCount[ocolor][MOD(sq)]--;
			pawnPlayerColCount[ocolor][COL(sq)]--;
			pawnPlayerRowCount[ocolor][ROW(sq)]--;
		}
		material[ocolor]-=h.PIECE_VALUE[opiece];
		pieceCount[ocolor][opiece]--;
		if(opiece==KING)
		{
			//unsetting king square
			kingSq[ocolor]=INVALID_SQ;
		}
	}

	//set new data
	pieces[sq]=piece;
	colors[sq]=player;

	if(piece!=NO_PIECE)//undo onl ownership hashes
	{
		hash^=z.PLAYER_PIECE_SQUARE_HASH_VALS[player][piece][sq];
		if(pieces[sq]==PAWN)
		{
			phash^=z.PLAYER_PIECE_SQUARE_HASH_VALS[player][piece][sq];

			for(int i = 0; i < 2; i++)
			{
				Square ts = sq+PLAYER_PAWN_CAPTURE_VECS[player][i];
				if(IS_SQ(ts))
					pawnControllMap[player][ts]++;
			}
			pawnPlayerModSqCount[player][MOD(sq)]++;
			pawnPlayerColCount[player][COL(sq)]++;
			pawnPlayerRowCount[player][ROW(sq)]++;
		}
		material[player]+=h.PIECE_VALUE[piece];
		pieceCount[player][piece]++;
		if(piece==KING)
		{
			//setting king square
			kingSq[player]=sq;
		}
	}
}
bool Board::makeStringMove(string str)
{
	Move move=makeMoveSlow(*this,FROM_ST(str,0),FROM_ST(str,2), str.size()>4?str[4]:' ');
	if(!testMoveIsLegalSlow(*this,move))
		return false;
	makeMove(move);
	return true;
}

void Board::makeMove(const Move &move)
{
	//first store state in history
	pastPositionHashes[histroyStackNextIndex]=hash;
	pastEnp[histroyStackNextIndex]=enp;
	pastCastlingRightsFlag[histroyStackNextIndex]=castlingRightsFlag;
	histroyStackNextIndex++;

	setSquare(move.from,NO_PIECE,NO_PLAYER);//MUST CLEAR FIRST, OR KING HAS PROBLEMS
	setSquare(move.to,move.toPieceNew,move.player);

	toMove=!toMove;
	hash^=z.WHITE_TO_MOVE_HASH_VAL;

	//set enp if possible
	hash^=z.ENP_HASH_VALS[enp];
	Square enpTarget = move.from+PLAYER_PAWN_FORWARD_VEC[!move.player];
	if(move.toPieceNew==PAWN//if pawn double move and square for one from start is controlled by oponent
			&& abs(ROW(move.to)-ROW(move.from))==2
			&& pawnControllMap[!move.player][enpTarget] !=0)
	{
		enp=enpTarget;//set only if there is an en passant possible
	}
	else
		enp=INVALID_SQ;
	hash^=z.ENP_HASH_VALS[enp];

	if(move.isCastle)
	{
		if(move.castleKingside)
		{
			if(move.player==WHITE)
			{
				setSquare(F1,ROOK,WHITE);
				setSquare(H1,NO_PIECE,NO_PLAYER);
			}
			else
			{
				setSquare(F8,ROOK,BLACK);
				setSquare(H8,NO_PIECE,NO_PLAYER);
			}
		}
		else
		{
			if(move.player==WHITE)
			{
				setSquare(D1,ROOK,WHITE);
				setSquare(A1,NO_PIECE,NO_PLAYER);
			}
			else
			{
				setSquare(D8,ROOK,BLACK);
				setSquare(A8,NO_PIECE,NO_PLAYER);
			}
		}
		//cout<<"UNMAKING CASTLE "<<move.player<<" "<<move.castleKingside<<endl;
	}
	if(move.isEnp)
	{
		setSquare(MK_SQ(PLAYER_ROW_INDEX[move.player][4],COL(move.to)),NO_PIECE,NO_PLAYER);//was piece of !move.player
	}

	//update castling rights
	hash^=z.CASTLING_RIGHTS_HASH_VALS[castlingRightsFlag];
	if(move.to==A1||move.from==A1)//rook moved or was captured
	{
		castlingRightsFlag&=~WHITE_QUEEN_CR;
	}
	else if(move.to==H1||move.from==H1)
		castlingRightsFlag&=~WHITE_KING_CR;
	else if(move.to==A8||move.from==A8)
		castlingRightsFlag&=~BLACK_QUEEN_CR;
	else if(move.to==H8||move.from==H8)
		castlingRightsFlag&=~BLACK_KING_CR;

	if(move.to==A1||move.from==A1)//rook moved or was captured
		castlingRightsFlag&=~WHITE_QUEEN_CR;
	else if(move.to==H1||move.from==H1)
		castlingRightsFlag&=~WHITE_KING_CR;
	else if(move.to==A8||move.from==A8)
		castlingRightsFlag&=~BLACK_QUEEN_CR;
	else if(move.to==H8||move.from==H8)
		castlingRightsFlag&=~BLACK_KING_CR;

	if(move.from==E1)
		castlingRightsFlag&=~WHITE_ALL_CR;
	else if(move.from==E8)
		castlingRightsFlag&=~BLACK_ALL_CR;
	hash^=z.CASTLING_RIGHTS_HASH_VALS[castlingRightsFlag];
	hashMap[hash%HMS]=*this;
}

void Board::unmakeMove(const Move &move)
{
	histroyStackNextIndex--;

	setSquare(move.to,move.toPieceOld,!move.player);
	setSquare(move.from,move.fromPiece,move.player);//MUST DO LAST, SO WE SET NEW KING POSITION

	if(move.isCastle)
	{
		if(move.castleKingside)
		{
			if(move.player==WHITE)
			{
				setSquare(H1,ROOK,WHITE);
				setSquare(F1,NO_PIECE,NO_PLAYER);
			}
			else
			{
				setSquare(H8,ROOK,BLACK);
				setSquare(F8,NO_PIECE,NO_PLAYER);
			}
		}
		else
		{
			if(move.player==WHITE)
			{
				setSquare(A1,ROOK,WHITE);
				setSquare(D1,NO_PIECE,NO_PLAYER);
			}
			else
			{
				setSquare(A8,ROOK,BLACK);
				setSquare(D8,NO_PIECE,NO_PLAYER);
			}
		}
		//cout<<"UNMAKING CASTLE "<<move.player<<" "<<move.castleKingside<<endl;
	}
	if(move.isEnp)
	{
		setSquare(MK_SQ(PLAYER_ROW_INDEX[move.player][4],COL(move.to)),PAWN,!move.player);//was piece of !move.player
	}

	toMove=!toMove;
	hash^=z.WHITE_TO_MOVE_HASH_VAL;

	//set enp if possible
	hash^=z.ENP_HASH_VALS[enp];
	enp=pastEnp[histroyStackNextIndex];
	hash^=z.ENP_HASH_VALS[enp];

	//update castling rights
	hash^=z.CASTLING_RIGHTS_HASH_VALS[castlingRightsFlag];
	castlingRightsFlag=pastCastlingRightsFlag[histroyStackNextIndex];
	hash^=z.CASTLING_RIGHTS_HASH_VALS[castlingRightsFlag];

	//cout<<"HASH "<<hash<<endl;
	if(pastPositionHashes[histroyStackNextIndex]!=hash)
	{
		cout<<"HASH END  "<<hash<<endl;
		cout<<"pastPositionHashes[histroyStackNextIndex]  "<<pastPositionHashes[histroyStackNextIndex]<<endl;
		std::cout<<"ERROR ON UNDO of "<<move<<"    did not undo hash"<<std::endl;
		cout<<"WAS POST UNDO"<<endl<<*this<<endl;
		cout<<"WAS PRE MAKE"<<endl<<hashMap[pastPositionHashes[histroyStackNextIndex]%HMS]<<endl;
	}
	//else
	//std::cout<<"SUCCESS ON UNDO of "<<move<<"    to "<<hash<<std::endl;
}

bool Board::sqIsAttacked(Square sq, Player attackingPlayer) const
{
	if(pawnControllMap[attackingPlayer][sq]>0)
	{
		return true;
	}
	for(int piece = 0; piece<5; piece++)//trace backward from square
	{
		for(int vecI = 0; vecI< PIECE_MOVE_VECS_SIZE[piece]; vecI++)
		{
			if(PIECE_MOVE_VECS_IS_SLIDING[piece])
			{
				Square target = sq;
				//do sliding
				while(true)
				{
					target=target+PIECE_MOVE_VECS[piece][vecI];
					if(!IS_SQ(target))
						break;
					if(pieces[target]!=NO_PIECE)//then either break or return tru if piece of other color
					{
						if(pieces[target]==piece && colors[target]== attackingPlayer)
						{
							return true;
						}
						break;
					}
				}
			}
			else
			{
				Square target=sq+PIECE_MOVE_VECS[piece][vecI];
				if(IS_SQ(target)&&pieces[target]==piece&&colors[target] == attackingPlayer)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Board::isCheck(Player p) const
{
	return sqIsAttacked(kingSq[p],!p);
}
bool Board::isPiece(Square sq, PieceType pt, Player pl)
{
	return pieces[sq]==pt && colors[sq]==pl;
}



