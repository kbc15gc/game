/**
* ゲーム管理クラスの定義.
*/
#pragma once

#include"StatusWindow\StatusWindow.h"
#include"History\HistoryBook\HistoryBook.h"
#include"Player\Player.h"
#include"Camera\PlayerCamera.h"
#include "GameObject\WorldMap\WorldMap.h"

/**
* ゲーム管理クラス.
* 色々管理させようかなぁって.
*/
class GameManager : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	GameManager(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~GameManager()
	{
	}

	/**
	* 初期化.
	*/
	void Start()override;

	/**
	* 更新.
	*/
	void Update()override;

private:

	Player* _Player = nullptr;

	/** ステータス画面クラスのポインタ. */
	StatusWindow* _StatusWindow = nullptr;

	/** ヒストリーブック. */
	HistoryBook* _HistoryBook = nullptr;

	PlayerCamera* _PlayerCamera = nullptr;

	// マップ。
	WorldMap* _worldMap = nullptr;
};