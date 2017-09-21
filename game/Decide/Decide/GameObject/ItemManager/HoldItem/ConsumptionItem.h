#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
class Player;

//消費アイテムのクラス。
class ConsumptionItem :public HoldItemBase
{
public:
	ConsumptionItem(Item::BaseInfo* info);
	~ConsumptionItem();

	//アイテムを使う。
	void UseItem(Item::ItemInfo* item);

	//所持数を更新(減らす場合は引数にマイナスを設定)。
	inline void UpdateHoldNum(int add = 1) {
		_HoldNum += add;
	}

	//所持数を取得。
	inline int GetHoldNum() {
		return  _HoldNum;
	}
private:
	Player* _UseTarget = nullptr;	//消費アイテムを使う対象。

	int _HoldNum = 0;
};