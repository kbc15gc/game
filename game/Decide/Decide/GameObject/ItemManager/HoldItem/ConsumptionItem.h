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
	void Awake()override;
	void Start()override;

	//アイテムを使う。
	// 戻り値：	アイテムを使用できたか。
	bool UseItem();

	//所持数を加算。
	// 引数：	増やす量。
	// 戻り値：	入りきらなかった値。
	inline int AddHoldNum(int add){
		_HoldNum += add;
		if (_HoldNum > holdMax) {
			int ret = _HoldNum - holdMax;
			_HoldNum = holdMax;
			return ret;
		}
		return 0;
	}

	//所持数を減算。
	// 引数：	減らす量。
	// 戻り値：	引数のあまり(このアイテムの所持数が0になっても引数の値が残っている場合は0以外が帰ってくる)。
	inline int SubHoldNum(int sub) {
		_HoldNum -= sub;
		if (_HoldNum < 0) {
			int ret = abs(_HoldNum);
			_HoldNum = 0;
			return ret;
		}
		return 0;
	}

	//所持数を取得。
	inline int GetHoldNum() {
		return  _HoldNum;
	}

	inline void SetInfo(Item::BaseInfo* info)override {
		if (_user) {
			if (_Info) {
				if (static_cast<Item::ItemInfo*>(_Info)->type == static_cast<int>(ConsumptionItem::EffectType::Debuff)) {
					_gost = INSTANCE(GameObjectManager)->AddNew<CollisionObject>("ItemRange", 5);	// アイテムの効果範囲コリジョン。
					_gost->transform->SetParent(_user->transform);
					_gost->transform->SetLocalPosition(Vector3::zero);
					_gost->Create(Collision_ID::ITEMRANGE, Vector3(_range, _range, _range), false);
#ifdef _DEBUG
					_gost->GetAttachCollision()->GetShape()->RenderDisable();
#endif // _DEBUG
				}
			}
		}
	}
private:

	void SetParticleBuffParam();

	GameObject* _user = nullptr;	// 使用者。

	float _range = 10.0f;	// 効果を及ぼす範囲(この範囲外のものにはアイテムは影響を及ぼさない)。
	int _HoldNum = 0;	// 所持数。

	CollisionObject* _gost = nullptr;	// 対象の探索に必要。

	const static int holdMax;
};