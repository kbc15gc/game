#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
class Player;

//消費アイテムのクラス。
class ConsumptionItem :public HoldItemBase
{
public:
	ConsumptionItem(const char* name);
	~ConsumptionItem();

	//アイテムを使う。
	void UseItem(Item::ItemInfo* item);
private:
	Player* Target = nullptr;	//消費アイテムを使う対象。
};