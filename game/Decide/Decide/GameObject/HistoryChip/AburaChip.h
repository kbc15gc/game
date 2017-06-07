#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class Player;
class HistoryMenuSelect;

class AburaChip : public GameObject
{
public:
	AburaChip(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
private:
	//プレイヤーオブジェクト
	Player*		_Player;
	//ヒストリーメニューセレクトオブジェクト
	HistoryMenuSelect* _HistoryMenuSelect;
	//サウンド
	SoundSource* _SE;
};
#pragma once
