#pragma once
#include "GameObject\ItemManager\ItemManager.h"

namespace {
	//アイテムごとの所持数(書き出しと読み込みに使用)。
	struct HoldNum
	{
		HoldNum() {

		}
		HoldNum(int TypeID,int ID,int HoldNum) {
			_TypeID = TypeID;
			_ID = ID;
			_HoldNum = HoldNum;
		}
		int _TypeID;		//アイテムの種類(消費アイテムか武器か防具か)。
		int _ID;			//アイテムの通し番号(TypeIDの中でユニーク)。
		int _HoldNum;	//所持数。
	};
	static Support::DATARECORD HoldItemData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldNum,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldNum,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct HoldNum,_HoldNum),		sizeof(int) },
	};
}

//所持アイテムの基底クラス。
class HoldItemBase
{
public:
	// 引数：	アイテムの情報構造体。
	HoldItemBase(Item::BaseInfo* info);
	virtual ~HoldItemBase();

	//構造体に情報を設定。
	inline void SetInfo(Item::BaseInfo* info) {
		_Info = info;
	}

	//構造体の情報を取得。
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	//所持数を更新(減らす場合は引数にマイナスを設定)。
	inline void UpdateHoldNum(int add = 1) {
		_HoldNum += add;
	}

	//所持数を取得。
	inline int GetHoldNum() {
		return  _HoldNum;
	}

private:
	int _HoldNum = 0;
	Item::BaseInfo* _Info;
};