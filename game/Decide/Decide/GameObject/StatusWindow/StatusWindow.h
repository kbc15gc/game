/**
* ステータス画面クラスの定義.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include"Parameter\ParameterRender.h"
#include"fbEngine\_Object\_Component\_2D\Sprite.h"

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

	/** ウィンドウ表示クラス. */
	Sprite* _WindowSprite = nullptr;

	/** パラメータリスト. */
	vector<ParameterRender*> _ParameterRenderList;

};