#pragma once
#include "GameObject\ItemManager\ItemManager.h"

//所持アイテムの基底クラス。
class HoldItemBase :public GameObject
{
public:
	HoldItemBase(const char* name);
	~HoldItemBase();
	

	//ショップでアイテムを売った数分所持数を減らす。
	void ItemSub(int sub = 1);

	//構造体に情報を設定。
	inline void SetInfo(Item::BaseInfo* info) {
		_Info = info;
	}

	//構造体の情報を取得。
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	//所持数を増やす。
	inline void AddHoldNum(int add = 1) {
		_HoldNum = add;
	}

private:
	Item::BaseInfo* _Info = nullptr;	//アイテムの構造体。
	int _HoldNum;						//所持数。
};