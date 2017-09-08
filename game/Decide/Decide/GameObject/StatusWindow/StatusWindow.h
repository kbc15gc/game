/**
* ステータス画面クラスの定義.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include"Parameter\ParameterRender.h"

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
	void Start()override;

	/**
	* 更新.
	*/
	void Update()override;

private:

	/** プレイヤークラスのポインタ. */
	Player* _Player = nullptr;

	/** パラメータリスト. */
	vector<ParameterRender*> _ParameterRenderList;

	/** アイテム表示画面. */
	vector<GameObject*> _ItemWindowList;

};