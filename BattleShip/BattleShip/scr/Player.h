#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "ShipObject.h"
class Player
{
private:
	int m_userID;
	ShipObject	m_pShip[ShipObject::TYPE_MAX];

public:
	/**
	*@brief	初期化
	*/
	void Init( int _userID );

	/**
	*@brief	駒情報取得
	*/
	ShipObject* GetShip( _SHIP_TYPE_NUM_ _shipType ) { return m_pShip[_shipType]; }
	
	/**
	*@brief	ユーザーID取得
	*/
	int GetUserID(){ return m_userID; }
};

#endif 
