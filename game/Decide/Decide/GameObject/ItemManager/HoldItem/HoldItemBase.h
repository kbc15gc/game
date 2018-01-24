#pragma once
#include "GameObject\ItemManager\ItemManager.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\HoldItemFactory.h"

//所持アイテムの基底クラス。
class HoldItemBase :public GameObject
{
protected:
	friend class HoldItemFactory;
	HoldItemBase(char* name);
public:
	~HoldItemBase();

	void Start()override;

	// 外部から読み込んだデータを設定。
	// 引数：	CSV読み書き用の所持装備品構造体へのポインタ。
	// ※CSVから読み込んだランダムパラメータ情報や装備情報を使用する際はこの関数でパラメータを設定する。
	virtual void ConfigLoadData(Hold::HoldInfo* info) = 0;

	//構造体の情報を取得。
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	int GetValue()
	{
		return _Info->Value;
	}

	//-50から100の値をランダムで取得。
	//パラメータを補正する値をランダムで算出。
	inline int GetRand_S50to100() {
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

