/*
 * move.h
 *
 *  Created on: Mar 13, 2018
 *      Author: henry
 */

#ifndef SRC_MOVE_H_
#define SRC_MOVE_H_

#include "basedef.h"


struct Move
{

	Square from=INVALID_SQ, to=INVALID_SQ;
	PieceType fromPiece=NO_PIECE/*player*/, toPieceOld=NO_PIECE/*!player*/, toPieceNew=NO_PIECE/*player*/;
	bool isEnp=false;
	bool isPromo=false;
	bool isCapture=false;
	bool isCastle=false;
	bool castleKingside=false;
	bool isNull=true;
	Player player=NO_PLAYER;
	Move()
	{

	}
	Move(Square _from, Square _to,
			PieceType _fromPiece, PieceType _toPieceOld, PieceType _toPieceNew,
			bool _isEnp, bool _isPromo, bool _isCapture,
			bool _isCastle, bool _castleKingside, bool _isNull,
			Player _player):
					from(_from), to(_to),
					fromPiece(_fromPiece), toPieceOld(_toPieceOld), toPieceNew(_toPieceNew),
					isEnp(_isEnp), isPromo(_isPromo), isCapture(_isCapture),
					isCastle(_isCastle), castleKingside(_castleKingside), isNull(_isNull),
					player(_player)
	{}

	bool getIsEnp()
	{
		return isEnp;
	}
	bool getIsPromo()
	{
		return isPromo;
	}
	bool getIsCapture()
	{
		return isCapture;
	}
	bool getIsCastle()
	{
		return isCastle;
	}
	bool getIsKingside()
	{
		return castleKingside;
	}

	int getFromRow()
	{
		return ROW(from);
	}
	int getFromCol()
	{
		return COL(from);
	}

	int getToRow()
	{
		return ROW(to);
	}
	int getToCol()
	{
		return COL(to);
	}
};





#endif /* SRC_MOVE_H_ */
