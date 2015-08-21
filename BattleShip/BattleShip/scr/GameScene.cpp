/**
*@file	GameScene.cpp
*@author	亀田
*/

#include "GameScene.h"

/**
*@brief	ゲームシーン初期化
*@details	現在メンバクラスの初期化関数の引数の問題もあり、ここで管理クラスなどをセットしてやる必要がある。@n

*/
bool GameScene::Init()
{
	for( int iCount=0; iCount<_PLAYER_NUM_; iCount++ ) {
		m_Player.push_back( new Player( iCount ));	///<	プレイヤーの初期化
	}
	m_background.Init();	///<	背景オブジェクトの初期化

	m_pStageObject = new( StageObject );		///<	StageObjectオブジェクトを生成
	m_pStageObject->Init();
	m_stateManager = new StateManager(m_Player[0], m_Player[1], m_pStageObject, _PLAYER_ID_);	///<	StateManagerオブジェクトを初期化
		
	//	ここは後にメンバのオブジェクトにも管理クラスをセットする必要があるので、
	//	順番に注意
	m_stateManager->SetDraw( m_pDrawManager );	///<	描画管理クラスのアドレスセット
	m_stateManager->SetMouse( m_pMouse );		///<	マウス管理クラスのアドレスセット
	m_stateManager->StateInit();

	return true;
}


int GameScene::Control()
{
	float tempX, tempY;
	m_background.GetPosition( &tempX, &tempY);
	m_pDrawManager->VertexDraw( _TEX_BACKGROUND_, tempX, tempY, 
		m_background.GetWidth(),  m_background.GetHeight(),
		0.f, 0.f, 1.f, 1.f);
	m_stateManager->StateCotrol();
	
	return 0;
}

void GameScene::Draw()
{

	m_stateManager->StateDraw( m_pDrawManager );
}

//	解放処理
bool GameScene::Free()
{
	for( int iCount = 0; iCount < m_Player.size(); iCount++ ){
		m_Player[iCount]->Free();
		delete m_Player[iCount];
	}
	m_pStageObject->Free();
	delete m_pStageObject;
	m_stateManager->Free();
	delete m_stateManager;

	return true;
}
