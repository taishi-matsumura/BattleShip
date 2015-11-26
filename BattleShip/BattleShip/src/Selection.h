/**
*@file	Selection.h
*@author	亀田
*/

#ifndef _SELECTION_H_
#define _SELECTION_H_

#include "GameState.h"

#define _TAB_WIDTH_		128.f
#define _TAB_HEIGHT_	(float)_TAB_WIDTH_/3

#define _SELECT_LOG_BIT_	0x1
#define _TAB_SELECT_BIT_	0x2
#define _AREA_SELECT_BIT_	0x4

class Selection : public GameState
{
private:
	byte m_selectionFlag;	///<	行動・タブ・行動選択開始ログ表示の管理フラグ	
	_SELECT_NUM_ m_plyaerSelectType;	///< 選択した行動の種類
	int m_arrayCheckResult;		///< 行動選択時の範囲チェックの結果
	
	BoardOfFrame m_actionFrame;	///< 攻撃枠
	BoardOfFrame m_searchFrame;	///< 索敵枠
	BoardOfFrame m_moveFrame;	///< 移動枠

	int(*m_tempArray)[_SHIP_ARRAY_INDEX_];	///<どの配列データとのチェックを行うかの格納用

public:
	/**
	*@brief	コンストラクタ
	*@param	_type	現在選択している艦の種類
	*/
	Selection( ShipObject::_SHIP_TYPE_NUM_& _type, GameLog* _pGameLog ): GameState( _type, _pGameLog ){
		m_selectionFlag = 0x0;
		m_arrayCheckResult = 0;
		m_plyaerSelectType = _SELECT_NONE_;
	}

	~Selection(){};

	/**
	*@brief	戦闘行動選択での初期化
	*/
	bool Init();

	/**
	*@brief	戦闘行動選択でのサブルーチン処理
	*/
	int Control();
	
	/**
	*@brief	戦闘行動選択での描画
	*/
	void Draw();

	/**
	*@brief	選択した行動の種類を取得
	*@details	StateManagerでステージのエフェクトなどにステートが移行した際に、
				選択した行動が分からなければステージ上の演出が出来ないため、
				選択した種類の情報が必要と判断しました。
	*/
	int GetSelectionType(){ return static_cast< int >(m_plyaerSelectType); }

private:

	/**
	*@brief	タブのクリックの判定
	*/
	bool TabCheck();

	/**
	*@brief	行動選択別の配列をセット
	*@details	後にチェックする配列データはタブを選択した際にどれを使うか決まっているため、
				メンバに仮保存する為、ここで一度だけ配列データをセットする。
	*@todo	もし違う行動を選択する場合にはフラグの管理の関係からここはタブを再選択された際に再度通る予定。
	*/
	bool SetTypeArray();

	/**
	*@brief	行動選択時の範囲チェック
	*@details	この関数では選択した行動別に座標を指定するときの分岐や処理を行う
	*/
	int SelectArrayCheck();

	///**
	//*@brief	ステージの範囲チェック
	//*@details	SelectArrayCheck関数に書くと今以上にネストが深くなると思ったため、コチラに処理を分ける事にする。@n
	//			
	//*@param[in]	_id		チェックするステージのプレイヤーID
	//*@param[in]	_column	チェックする行
	//*@param[in]	_line	チェックする列
	//*@param[in]	_array	セットしたい配列データ
	//*/
	//int StageCheck( const int& _id, const int& _column, const int& _line, const int(*_array)[_SHIP_ARRAY_INDEX_]);

	/**
	*@brief	タブの情報をリセット
	*@details	主にタブ情報のカラー値をリセット
	*/
	void ResetTabFrame();

	/**
	*@brief	タブ選択状態リセット
	*/
	void ResetTabSelect();
};

#endif