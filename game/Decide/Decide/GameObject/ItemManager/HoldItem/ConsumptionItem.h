#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include "fbEngine\_Object\_GameObject\CollisionObject.h"

class GameObject;

//消費アイテムのクラス。
class ConsumptionItem :public HoldItemBase
{
public:
	enum class EffectType{Heel = 0,Buff,Debuff};
	// アイテムが及ぼす対象。
	enum class Range{
		I = 0,	// 1体。
		III,	// 3体。
		V,		// 5体。
		All,	// 範囲内すべて。
		Max
	};
public:
	ConsumptionItem(char* name);
	~ConsumptionItem();

	void Start()override;

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
	GameObject* _user = nullptr;	// 使用者。
	vector<GameObject*> _targets;	// 消費アイテムを使う対象。

	float _range = 10.0f;	// 効果を及ぼす範囲(この範囲外のものにはアイテムは影響を及ぼさない)。
	int _HoldNum = 0;	// 所持数。

	CollisionObject* _gost = nullptr;	// 対象の探索に必要。
};