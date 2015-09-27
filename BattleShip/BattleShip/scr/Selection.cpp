/**
*@file	Selection.cpp
*@author	亀田
*/

#include "Selection.h"

bool Selection::Init()
{
	m_StateCompFlag = false;
	m_tabSelectFlag = false;
	m_areaSelectFlag= false;
	m_arrayCheckResult = 0;

	m_tempShip = m_pPlayer[m_playerID-1]->GetShip( (ShipObject::_SHIP_TYPE_NUM_)(m_ShipCount) );
		
	m_actionFrame.Init( m_tempShip->GetPositionX(), m_tempShip->GetPositionY()+_TAB_HEIGHT_*2, 
						_TAB_WIDTH_, _TAB_HEIGHT_ );
	m_actionFrame.SetColor( 255, 255, 255, 255 );
	
	m_searchFrame.Init( m_tempShip->GetPositionX(), m_tempShip->GetPositionY()+_TAB_HEIGHT_,
						_TAB_WIDTH_, _TAB_HEIGHT_ );
	m_searchFrame.SetColor( 255, 255, 255, 255 );

	m_moveFrame.Init( m_tempShip->GetPositionX(), m_tempShip->GetPositionY(),
						_TAB_WIDTH_, _TAB_HEIGHT_ );
	m_moveFrame.SetColor( 255, 255, 255, 255 );

	return false;
	
}

//	
int Selection::Control()
{
	m_tempX = (float)m_pMouse->GetCursorPosX();
	m_tempY = (float)m_pMouse->GetCursorPosY();

	if( !m_StateCompFlag )
	{
		if( !m_tabSelectFlag ){	
			m_tabSelectFlag = TabCheck();
		}else if( !m_areaSelectFlag ){
			m_areaSelectFlag = SetTypeArray();
			if( !m_areaSelectFlag )	///< 選択した範囲にデータが無い（orその行動は出来ない）場合、タブの選択も解除する。
				m_tabSelectFlag = false;
		}else{
			m_pStage->ResetSelect();
			SelectArrayCheck();
		}
	}

	
	return m_ShipCount;
}


bool Selection::TabCheck()
{
	ResetTabFrame();
	bool tempFlag = false;

	//	選択タブごとのマウスとの接触判定
	if( m_actionFrame.HitBlockCheck( m_tempX, m_tempY ) )
	{
		m_actionFrame.SetColor( 255, 150, 150, 150 );
		if( m_pMouse->MouseStCheck( MOUSE_L, PUSH )){
			m_selectType = _SELECT_ACTION_;
			tempFlag = true;
		}
	}
	if( m_searchFrame.HitBlockCheck( m_tempX, m_tempY ) )
	{
		m_searchFrame.SetColor( 255, 150, 150, 150 );
		if( m_pMouse->MouseStCheck( MOUSE_L, PUSH )){
			m_selectType = _SELECT_SEARCH_;
			tempFlag = true;
		}
	}
	if( m_moveFrame.HitBlockCheck( m_tempX, m_tempY ) )
	{
		m_moveFrame.SetColor( 255, 150, 150, 150 );
		if( m_pMouse->MouseStCheck( MOUSE_L, PUSH )){
			m_selectType = _SELECT_MOVE_;
			tempFlag = true;
		}
	}

	return tempFlag;
}


bool Selection::SetTypeArray()
{
	switch( m_selectType )
	{
	case _SELECT_ACTION_:
		m_tempArray = m_tempShip->m_actionArray;
		break;
	case _SELECT_SEARCH_:
		m_tempArray = m_tempShip->m_searchArray;
		break;
	case _SELECT_MOVE_:
		m_tempArray = m_tempShip->m_moveArray;
		
		break;
	}

	for( int iColumn = 0; iColumn < _SHIP_ARRAY_INDEX_; iColumn++ ){
		for( int iLine = 0; iLine < _SHIP_ARRAY_INDEX_; iLine++ ){
		if( m_tempArray[iColumn][iLine] != 0)
			return true;
		}
	}

	MessageBoxA(0,"この行動は選択出来ない。\n申し訳ないが、他の行動を選択してくれ",NULL,MB_OK);
	return false;
}



int Selection::SelectArrayCheck( )
{
	int tempID = m_playerID;	///<どちらのプレイヤーのステージ配列を示すかの判定用にコピー
	int (*tempArray)[_SHIP_ARRAY_INDEX_];
	int iCheckResult = 0;
	int iColumn;	
	int iLine;

	//	攻撃と索敵なら相手側ID、移動なら自分側のIDを使うつもり
	switch( m_selectType )
	{
	case _SELECT_ACTION_:
	case _SELECT_SEARCH_:
		if( --tempID <= 0 )	///<チェックするステージ配列の指数の判定
			tempID+=2;
		//	行
		for( iColumn=0; iColumn<_STAGE_COLUMN_MAX_; iColumn++ ){	
			//	列
			for( iLine=0; iLine<_STAGE_LINE_MAX_; iLine++ ){
				
				if( m_pStage->m_stageBlock[tempID-1][iColumn][iLine].HitBlockCheck( m_tempX, m_tempY ))
				{
					//	ステージブロックのチェック
					switch( m_selectType )
					{
					case _SELECT_ACTION_:
						iCheckResult = m_pStage->CheckStageBlock( tempID, iColumn, iLine, m_tempShip, ShipObject::ARRAY_TYPE_ACTION, m_ShipCount );
						tempArray = m_tempShip->m_actionArray;
						break;
					case _SELECT_SEARCH_:
						iCheckResult = m_pStage->CheckStageBlock( tempID, iColumn, iLine, m_tempShip, ShipObject::ARRAY_TYPE_SEARCH, m_ShipCount );
						tempArray = m_tempShip->m_searchArray;
						break;
					}

					if( iCheckResult == -1 )	///<駒を置けるマスじゃなかった。
					{	
						return -1;
					}
					else ///<置けるマス。
					{
						m_pStage->SetRange( tempID, iColumn, iLine, tempArray, 1);
						//	駒が置けるマスであり、左クリックを押した時
						if( m_pMouse->MouseStCheck( MOUSE_L, PUSH )) {
							m_pStage->SetRange( tempID, iColumn, iLine, tempArray, 4 );
							
							//	駒の基準点（中心点）を予め算出させておく
							float tempW = _BLOCK_WIDTH_SIZE_;		///<	ステージ上の1コマのサイズの入力を簡略化
							float tempH = _BLOCK_HEIGHT_SIZE_;		///<	ステージ上の1コマのサイズの入力を簡略化
							m_tempX = iLine*tempW + tempW*1.5f ;		
							m_tempY = iColumn*tempH + tempH*1.5f;
							m_tempShip->SetPosition( m_tempX, m_tempY, 0.5f );
							return 2;
						}
					}
				}
			}
		}
		break;

	case _SELECT_MOVE_:
		tempArray = m_tempShip->m_moveArray;
		m_pStage->SetStageToRange( tempID, m_tempShip, tempArray, m_ShipCount );
		
		iCheckResult =  m_pStage->CheckRangeOnStage( iColumn, iLine, tempID, m_tempX, m_tempY, m_tempShip, ShipObject::ARRAY_TYPE_SHIP );

		if( iCheckResult == -1 )	///<駒を置けるマスじゃなかった。
		{	
			return -1;
		}
		else if( iCheckResult == 1  )	///<置けるマス。
		{
			float tempW = _BLOCK_WIDTH_SIZE_;		///<	ステージ上の1コマのサイズの入力を簡略化
			float tempH = _BLOCK_HEIGHT_SIZE_;		///<	ステージ上の1コマのサイズの入力を簡略化
			m_tempX = iLine*tempW + tempW*1.5f ;		
			m_tempY = iColumn*tempH + tempH*1.5f;
			m_tempShip->SetPosition( m_tempX, m_tempY, 0.5f );
			
			if( m_pMouse->MouseStCheck( MOUSE_L, PUSH )) 
			{
				m_pStage->RelocationShip( tempID, iColumn, iLine, m_tempShip, m_ShipCount );
				return 2;
			}
			return 1;
		}
		else
		{
			m_tempShip->GetArrayPos( iColumn, iLine );

			float tempW = _BLOCK_WIDTH_SIZE_;		///<	ステージ上の1コマのサイズの入力を簡略化
			float tempH = _BLOCK_HEIGHT_SIZE_;		///<	ステージ上の1コマのサイズの入力を簡略化
			m_tempX = iLine*tempW + tempW*1.5f ;		
			m_tempY = iColumn*tempH + tempH*1.5f;
			m_tempShip->SetPosition( m_tempX, m_tempY, 0.5f );
		}
		
		break;
	}

	

	return 0;
}


void Selection::ResetTabFrame()
{
	m_actionFrame.SetColor( 255, 255, 255, 255 );
	m_searchFrame.SetColor( 255, 255, 255, 255 );
	m_moveFrame.SetColor( 255, 255, 255, 255 );

}

//	
void Selection::Draw()
{
	if( !m_tabSelectFlag ){
		m_pDrawManager->VertexDraw( _TEX_TABFRAME_, m_actionFrame.GetPositionX(), m_actionFrame.GetPositionY(),
			m_actionFrame.GetWidth(), m_actionFrame.GetHeight(),
			0.f, (_TAB_HEIGHT_*2)/_TAB_WIDTH_, 1.f, 1.f,
			m_actionFrame.GetAlpha(), m_actionFrame.GetRed(), m_actionFrame.GetGreen(), m_actionFrame.GetBlue() );
		
		m_pDrawManager->VertexDraw( _TEX_TABFRAME_, m_searchFrame.GetPositionX(), m_searchFrame.GetPositionY(),
			m_searchFrame.GetWidth(), m_searchFrame.GetHeight(),
			0.f, _TAB_HEIGHT_/_TAB_WIDTH_, 1.f, (_TAB_HEIGHT_*2)/_TAB_WIDTH_,
			m_searchFrame.GetAlpha(), m_searchFrame.GetRed(), m_searchFrame.GetGreen(), m_searchFrame.GetBlue() );
		
		m_pDrawManager->VertexDraw( _TEX_TABFRAME_, m_moveFrame.GetPositionX(), m_moveFrame.GetPositionY(),
			m_moveFrame.GetWidth(), m_moveFrame.GetHeight(), 
			0.f, 0.f, 1.f, _TAB_HEIGHT_/_TAB_WIDTH_,
			m_moveFrame.GetAlpha(), m_moveFrame.GetRed(), m_moveFrame.GetGreen(), m_moveFrame.GetBlue() );

	}
	else{
		
	}
}

//	
void Selection::ComStandby()
{

}