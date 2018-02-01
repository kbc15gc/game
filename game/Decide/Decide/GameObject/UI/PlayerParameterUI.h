/**
* プレイヤーパラメータUIクラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"fbEngine\_Object\_GameObject\ImageObject.h"

#include"../Player/Player.h"

/**
* プレイヤーパラメータUIクラス.
*/
class PlayerParameterUI : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	PlayerParameterUI(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~PlayerParameterUI()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* 更新.
	*/
	void LateUpdate()override;

private:

	Player* _Player = nullptr;
	
	TextObject* _LvText = nullptr;

	ImageObject* _HpBar = nullptr;
	ImageObject* _ExpBar = nullptr;

	float _NextHpRate = 0.0f;
	float _NowHpRate = 0.0f;

	float _NextExpRate = 0.0f;
	float _NowExpRate = 0.0f;
};