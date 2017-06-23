/**
* チップクラスの定義.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "HistoryInfo.h"

class Player;
class HistoryMenuSelect;

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
	
	/**
	* チップIDを設定.
	*/
	void SetChipID(ChipID chipID);

private:

	/** チップID. */
	ChipID _ChipID;

	/** プレイヤーのポインタ. */
	Player*	_Player = nullptr;

	/** サウンドのポインタ. */
	SoundSource* _SE = nullptr;

};