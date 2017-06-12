#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\Village\HistoryInfo.h"

class Player;
class HistoryMenuSelect;

class Chips : public GameObject
{
public:
	Chips(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	
	void SetChipID(const ChipID& id);
private:
	//プレイヤーオブジェクト
	Player*		_Player = nullptr;
	//ヒストリーメニューセレクトオブジェクト
	HistoryMenuSelect* _HistoryMenuSelect = nullptr;
	//サウンド
	SoundSource* _SE = nullptr;
	//自分のID
	ChipID _ChipID;
};