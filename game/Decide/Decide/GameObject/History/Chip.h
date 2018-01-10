/**
* チップクラスの定義.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "HistoryInfo.h"

class Player;
class HistoryMenuSelect;

static const float ChipTimer = 4.0f;

/**
* チップクラス.
* 歴史のかけら.
*/
class Chip : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	Chip(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~Chip()
	{
	}

	/**
	* コンストラクタ後の初期化.
	*/
	void Awake()override;

	/**
	* 初期化.
	*/
	void Start()override;

	/**
	* 更新.
	*/
	void Update()override;

	/*
	* 描画.
	*/
	void Render()override;
	
	/**
	* チップIDを設定.
	*/
	void SetChipID(ChipID chipID);

	/**
	* 敵ドロップのチップIDを設定.
	*/
	void SetDropChipID(ChipID chipID, const Vector3& pos);


	/**
	* 取得できるまでの時間を設定.
	*/
	inline void SetGetTime(float time)
	{
		_GetTime = time;
	}
private:

	/** チップID. */
	ChipID _ChipID;

	/** プレイヤーのポインタ. */
	Player*	_Player = nullptr;

	/** サウンドのポインタ. */
	SoundSource* _SE = nullptr;

	//モデル
	SkinModel* _Model = nullptr;
	//マテリアル
	Material* _Material = nullptr;

	//取得できるようになるまでの時間
	float _GetTime;
	//取得できるようになるまでの時間を計測
	float _GetTimer;

};