/**
*@file	SetShip.cpp
*@author	亀田
*/

#include "SetShip.h"

bool SetShip::Init()
{
	m_StateCompFlag = false;
	return false;
}

//	
int SetShip::Control()
{
	m_pStage->ResetSelect();	///<	ステージの選択状態をリセット
	m_tempX = (float)m_pMouse->GetCursorPosX();	///<	マウス座標の更新
	m_tempY = (float)m_pMouse->GetCursorPosY();	///<	マウス座標の更新
	m_tempShip = m_pPlayer[m_playerID-1]->GetShip( (ShipObject::_SHIP_TYPE_NUM_)m_ShipCount );
			
	if( !m_StateCompFlag )
	{
		int iCheckResult = 0;
		//	駒が置ける置けない関係なく、右クリックで駒を回転させる
		if( m_pMouse->MouseStCheck( MOUSE_R, PUSH ) ) {
			m_tempShip->RotationShip( 0, true );
		}
		iCheckResult = CheckBoard();

		if( iCheckResult == 2 )
			m_ShipCount++;
	}

	if( m_ShipCount >= ShipObject::TYPE_MAX ){
		m_StateCompFlag = true;
		m_pStage->ResetSelect();
	}
	return m_ShipCount;
}

//
int SetShip::CheckBoard()
{
	bool tempFlag = false;	///<	ネストが深くなる事を防止するために、ブロックのクリック判定がtrue

	//	行
	for( int iColumn=0; iColumn<_STAGE_COLUMN_MAX_; iColumn++ ){	
		//	列
		for( int iLine=0; iLine<_STAGE_LINE_MAX_; iLine++ ){
			
			if( m_pStage->m_stageBlock[m_playerID-1][iColumn][iLine].HitBlockCheck( m_tempX, m_tempY ))
			{
				int iCheckResult=0;
				
				//	ステージブロックのチェック
				iCheckResult = m_pStage->CheckStageBlock( m_playerID, iColumn, iLine, m_tempShip, ShipObject::ARRAY_TYPE_SHIP );
				
				if( iCheckResult != 0 )	///<駒を置けるマスじゃなかった。
				{	
					//	置けない範囲だった場合も、置けないという情報をステージにセットする
					m_pStage->SetRange( m_playerID, iColumn, iLine, m_tempShip->m_shipArray, 2 );
					return 1;
				}
				else ///<置けるマス。
				{
					m_pStage->SetRange( m_playerID, iColumn, iLine, m_tempShip->m_shipArray, 1);
					//	駒が置けるマスであり、左クリックを押した時
					if( m_pMouse->MouseStCheck( MOUSE_L, PUSH )) {
						m_pStage->SetShip( m_playerID, iColumn, iLine, m_tempShip );
						m_tempShip->SetArrayPos( iColumn, iLine );
						m_tempShip->SetDeadFlag( false );///<駒を設置したのでオブジェクトの死亡フラグを下げる
						
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

	return 0;
}

//	
void SetShip::Draw()
{
	if( m_ShipCount < ShipObject::TYPE_MAX )
	{
		m_pDrawManager->VertexTransform( m_ShipCount + _TEX_AIRCARRIER_, m_tempShip->m_vertex, m_tempX, m_tempY, 1.f, 1.f, m_tempShip->GetDirection()*90.f );
	}
}

//	
void SetShip::ComStandby()
{

}