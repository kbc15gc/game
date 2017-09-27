#pragma once
#include "GameObject\ItemManager\ItemManager.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Inventory\Inventory.h"

//所持アイテムの基底クラス。
class HoldItemBase :public ImageObject
{
public:
	// 引数：	アイテムの情報構造体。
	HoldItemBase(char* name);
	~HoldItemBase();

	void Start()override;

	//構造体の情報を取得。
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	//-50から100の値をランダムで取得。
	inline float GetRand_S50to100() {
		return (rand() % 151) - 50;
	}


	virtual inline void SetInfo(Item::BaseInfo* info) {
		_Info = info;
	}

	static bool SortID(HoldItemBase& left, HoldItemBase& right) 
	{
		return left.GetInfo()->ID > right.GetInfo()->ID;
	}

protected:
	Item::BaseInfo* _Info = nullptr;
};

