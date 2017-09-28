#include "stdafx.h"
#include "ConsumptionItem.h"
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "fbEngine\_Object\_Component\_Physics\SphereCollider.h"
#include "GameObject\Component\ParticleEffect.h"

namespace {
	// 効果を及ぼす人数のテーブル。
	static short effectNumTable[static_cast<int>(ConsumptionItem::Range::Max)] = {
		1,3,5,SHRT_MAX
	};
}

// 一枠の最大所持数。
const int ConsumptionItem::holdMax = 99;

ConsumptionItem::ConsumptionItem(char* name) :HoldItemBase(name)
{
}


ConsumptionItem::~ConsumptionItem()
{
}

void ConsumptionItem::Awake() {
	// ショップで購入したアイテムで枠が追加された場合、Startが呼ばれないのでこちらでも呼ぶ。

	_user = INSTANCE(GameObjectManager)->FindObject("Player");	// とりあえず使用者は固定でプレイヤー。
}

void ConsumptionItem::Start() {
	// Awake時にユーザーが生成されていない可能性があるのでこちらでも呼ぶ。
	Awake();
}


//アイテムを使う。
bool ConsumptionItem::UseItem() {
	vector<GameObject*> targets;	// 消費アイテムを使う対象。

	ParticleEffect* effect = _user->GetComponent<ParticleEffect>();
	if (static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Heel || static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Buff) {
		// 回復もしくはバフアイテム。

		// 使用者自身をターゲットにする。
		CharacterParameter* param = _user->GetComponent<CharacterParameter>();
		Item::ItemInfo* info = static_cast<Item::ItemInfo*>(_Info);

		bool ret = false;
		// 暫定処理。
		// ※とりあえず演出は考慮していない。
		if (param->HeelHP(info->effectValue[CharacterParameter::Param::HP])) {	// HP回復処理。

			//Hp回復のエフェクト。
			effect->HeelHpEffect(_user->transform);
			ret = true;

			if (param->HeelMP(info->effectValue[CharacterParameter::Param::MP])) {	// MP回復処理。

				//Hp回復のエフェクト。
				effect->HeelMpEffect(_user->transform);
				ret = true;
			}

			for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
				int value = info->effectValue[idx];
				if (value > 0) {
					// バフ。

					param->Buff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(value), info->time);
					ret = true;
				}
				else if (value < 0) {
					// デバフ(デメリット)。

					param->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
					ret = true;
				}
			}

			// 暫定処理。
			// ※ゲーム内で何とか効果がないことをお知らせすべき。
			if (!ret) {
				char error[256];
				sprintf(error, "何の成果も得られませんでしたぁっ！！");
				MessageBoxA(0, error, "バフも回復もできないよ！", MB_ICONWARNING);
				targets.clear();
				return false;
			}

			return true;
		}
		else {
			// デバフアイテム。
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
				for (auto coll : hit) {
					// 対象に取得したオブジェクトを追加。
					targets.push_back(coll->gameObject);
				}
			}

		}

		if (targets.size() <= 0 && static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Debuff) {

			// 暫定処理。
			// ※ゲーム内で何とか効果がないことをお知らせすべき。
			{
				char error[256];
				sprintf(error, "何の成果も得られませんでしたぁっ！！");
				MessageBoxA(0, error, "デバフなのに効果範囲内に敵がいないよ", MB_ICONWARNING);
				targets.clear();

				return false;
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
					effect = target->GetComponent<ParticleEffect>();
					effect->DeBuffEffect(target->transform);
					param->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(info->effectValue[idx])), info->time);
				}
			}
		}


		targets.clear();
		return true;
	}
}