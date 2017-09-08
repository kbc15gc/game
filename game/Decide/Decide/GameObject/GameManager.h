/**
* ゲーム管理クラスの定義.
*/
#pragma once

#include"StatusWindow\StatusWindow.h"

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

	/** ステータス画面クラスのポインタ. */
	StatusWindow* _StatusWindow = nullptr;

};