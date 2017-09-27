/**
* ステータス画面クラスの定義.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include"ParameterRender.h"
#include"ItemWindow.h"

/**
* ステータス画面クラス.
*/
class StatusWindow : public GameObject
{
public:
	enum ShowStatus { LV = 0, HP, MP, ATK, MAT, DEF, MDE, DEX, MONEY, MAX };

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
	// ステータス表示作成。
	void _CreateShowStatus();
	// パラメータ表示クラスのインスタンスに値を設定。
	void _ConfigParamRender();
private:

	/** プレイヤークラスのポインタ. */
	Player* _Player = nullptr;
	// プレイヤーのレベル。
	int _playerLevel = 0;
	// ゲージ。
	ParameterBar* _ExpBar = nullptr;
	ParameterBar* _HpBar = nullptr;
	ParameterBar* _MpBar = nullptr;

	/** パラメータリスト. */
	vector<ParameterRender*> _ParameterRenderList;

	/** アイテム表示画面. */
	vector<ItemWindow*> _ItemWindowList;

	int _WindowCount = 3;
	int _NowSelectWindow = 0;

};