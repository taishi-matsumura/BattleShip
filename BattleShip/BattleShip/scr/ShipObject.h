/**
*@file	ShipObject.h
*@author	Tc
*/

#ifndef _SHIPOBJECT_H_
#define _SHIPOBJECT_H_

#include "gameObject.h"
#include "read_file.h"

#define _SHIP_ARRAY_INDEX_	5	///<	îÌzñw

/**
*@brief	îÌîêNX
*/
class ShipObject:public CGameObject, public ReadFile
{
public:
	/**
	*@brief	îÌíÞñ
	*/
	enum _SHIP_TYPE_NUM_
	{
		TYPE_NONE=-1,
		TYPE_AIRCARRIER,
		TYPE_BATTLESHIP,
		TYPE_CRUISER,
		TYPE_DESTROYER,
		TYPE_SUBMARINE,
		TYPE_MAX,
	};

	enum _SHIP_TABLE_NUM_
	{
		TABLE_SHIP,
		TABLE_ACTION = _SHIP_ARRAY_INDEX_,
		TABLE_SEARCH = TABLE_ACTION+_SHIP_ARRAY_INDEX_,
		TABLE_MOVE = TABLE_SEARCH+_SHIP_ARRAY_INDEX_,
		TABLE_MAX = TABLE_MOVE+_SHIP_ARRAY_INDEX_
	};

private:
	int m_userID;
	
public:
	int m_shipArray[_SHIP_ARRAY_INDEX_][_SHIP_ARRAY_INDEX_];	///<	îÌÀÌzñ
	
protected:
	int m_actionArray[_SHIP_ARRAY_INDEX_][_SHIP_ARRAY_INDEX_];	///<	îÌUÍÍzñ
	int m_searchArray[_SHIP_ARRAY_INDEX_][_SHIP_ARRAY_INDEX_];	///<	îÌõGÍÍzñ
	int m_moveArray[_SHIP_ARRAY_INDEX_][_SHIP_ARRAY_INDEX_];	///<	îÌÚ®ÍÍzñ

public:
	/**
	*@brief	RXgN^
	*@details	îzñÌú»ÆCGameObjectÌRXgN^ðÄÑoµ
	*/
	ShipObject::ShipObject() : CGameObject(){
		for( int iColumn = 0; iColumn < _SHIP_ARRAY_INDEX_; iColumn ){
			for( int iLine = 0; iLine < _SHIP_ARRAY_INDEX_; iLine )	{
				m_shipArray[iColumn][iLine]=0;
				m_actionArray[iColumn][iLine]=0;
				m_searchArray[iColumn][iLine]=0;
				m_moveArray[iColumn][iLine]=0;

			}
		}
	}

	/**
	*@brief	ú»
	*/
	virtual void Init( int _userID ) = 0;

	void Control(){};

	void Draw(){};

	/**
	*@brief	ÇÝæÁ½f[^ðe[uÉZbg
	*@details	îf[^Å
	*/
	void SetTable( char* _p, int _iColumn, int _iLine);

public:
	/**
	*@brief	îÌ¿åIDæ¾
	*/
	int GetUserID(){ return m_userID; }	
};


#endif