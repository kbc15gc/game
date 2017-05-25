#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
class HistoryMenuSelect;

class FireChip : public GameObject
{
public:
	FireChip(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
private:
	//プレイヤーオブジェクト
	Player*		_Player;
	//ヒストリーメニューセレクトオブジェクト
	HistoryMenuSelect* _HistoryMenuSelect;
};
