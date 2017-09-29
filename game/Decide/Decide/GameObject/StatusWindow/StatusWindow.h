/**
* ステータス画面クラスの定義.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include"ParameterRender.h"
#include"ItemWindow.h"
#include "GameObject\Village\EventManager.h"

/**
* ステータス画面クラス.
*/
class StatusWindow : public GameObject
{

public:

	/**
	* コンストラクタ.
	*/
	StatusWindow(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~StatusWindow()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* 更新.
	*/
	void Update()override;

	void OnDisable()override
	{
		INSTANCE(EventManager)->NotifyEndEvent();
	}
private:

	/** プレイヤークラスのポインタ. */
	Player* _Player = nullptr;

	/** アイテム表示画面. */
	vector<ItemWindow*> _ItemWindowList;

	int _WindowCount = 3;
	int _NowSelectWindow = 0;

	ParameterRender* _MoneyRender = nullptr;	// お金の表示。
	ImageObject* _MoneyFrame = nullptr;			// お金の枠表示。

public:
	static const int WindowBackPriorty;
};