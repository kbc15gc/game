#include "stdafx.h"
#include "ConsumptionItem.h"
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "fbEngine\_Object\_Component\_Physics\SphereCollider.h"

namespace {
	// 効果を及ぼす人数のテーブル。
	static short effectNumTable[static_cast<int>(ConsumptionItem::Range::Max)] = {
		1,3,5,SHRT_MAX
	};
}

ConsumptionItem::ConsumptionItem(char* name) :HoldItemBase(name)
{
}


ConsumptionItem::~ConsumptionItem()
{
}

void ConsumptionItem::Start() {
	_user = INSTANCE(GameObjectManager)->FindObject("Player");	// とりあえず使用者は固定でプレイヤー。
	_gost = INSTANCE(GameObjectManager)->AddNew<CollisionObject>("ItemRange",5);	// アイテムの効果範囲コリジョン。

	_gost->transform->SetParent(_user->transform);
	_gost->transform->SetLocalPosition(Vector3::zero);
	_gost->Create(Collision_ID::ITEMRANGE,Vector3(_range,_range,_range),false);	

	_Effect = AddComponent<ParticleEffect>();

}


//アイテムを使う。
void ConsumptionItem::UseItem() {
	vector<GameObject*> targets;	// 消費アイテムを使う対象。

	if (static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Heel || static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Buff) {
		// 回復もしくはバフアイテム。

		// 使用者自身をHP回復処理。
		CharacterParameter* param = _user->GetComponent<CharacterParameter>();
		Item::ItemInfo* info = static_cast<Item::ItemInfo*>(_Info);
		// 暫定処理。
		// ※とりあえず演出は考慮していない。
		_Effect->HeelEffect(_user->transform);

		param->HeelHP(info->effectValue[CharacterParameter::Param::HP]);	// HP回復処理。
		param->HeelMP(info->effectValue[CharacterParameter::Param::MP]);	// MP回復処理。

		for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
			int value = info->effectValue[idx];
			if (value > 0) {
				// バフ。
				
				_Effect->BuffEffect(_user->transform);
				param->Buff(static_cast<CharacterParameter::Param>(idx),static_cast<unsigned short>(value),info->time);
			}
			else if (value < 0) {
				// デバフ(デメリット)。

				param->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
			}
		}
	}
	else {
		// デバフアイテム。
		_Effect->DeBuffEffect(_user->transform);
		// 効果範囲内のエネミーを取得。
		int attr = Collision_ID::ENEMY | Collision_ID::BOSS;
		vector<Collision*> hit;
		INSTANCE(PhysicsWorld)->AllHitsContactTest(_gost->GetAttachCollision(), hit, attr);

		if (hit.size() > effectNumTable[static_cast<int>(static_cast<Item::ItemInfo*>(_Info)->rangeStrength)]) {
			// 衝突した対象がアイテムの効果範囲より多い。

			for (int idx = 0; idx < effectNumTable[static_cast<int>(static_cast<Item::ItemInfo*>(_Info)->rangeStrength)]; idx++) {

				// 衝突した対象の中の近いものから取得していく。
				float min = FLT_MAX;
				GameObject* minObj = nullptr;
				int eraseIdx = 0;	// 先頭から何番目の要素か。
				for (int idx = 0; idx < hit.size(); idx++) {
					if (Vector3(_user->transform->GetPosition() - hit[idx]->transform->GetPosition()).Length() < min) {
						minObj = hit[idx]->gameObject;
						eraseIdx = idx;
					}
				}

				// 取得したコリジョンは配列から省く。
				hit.erase(hit.begin() + eraseIdx);

				// 対象に取得したオブジェクトを追加。
				targets.push_back(minObj);
			}
		}
		else {
			for (auto coll: hit) {
				// 対象に取得したオブジェクトを追加。
				targets.push_back(coll->gameObject);
			}
		}

	}

	// 複合アイテムを考慮してとりあえず全部実行する。
	for (auto target : targets) {
		CharacterParameter* param = target->GetComponent<CharacterParameter>();
		Item::ItemInfo* info = static_cast<Item::ItemInfo*>(_Info);
		// 暫定処理。
		// ※とりあえず演出は考慮していない。
		for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
			if (param) {
				param->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(info->effectValue[idx])), info->time);
			}
		}
	}

	if (targets.size() <= 0 && static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Debuff) {

		// 暫定処理。
		// ※ゲーム内で何とか効果がないことをお知らせすべき。
		char error[256];
		sprintf(error, "デバフなのに効果範囲内に敵がいないよ");
		MessageBoxA(0, error, "何の成果も得られませんでしたぁっ！！", MB_ICONWARNING);
	}

	targets.clear();
}
