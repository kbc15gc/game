#include "stdafx.h"
#include "ConsumptionItem.h"
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "fbEngine\_Object\_Component\_Physics\SphereCollider.h"
#include "GameObject\Component\ParticleEffect.h"
#include"GameObject\Enemy\EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

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
	_NoEffectTextColor = Color::red;
}


ConsumptionItem::~ConsumptionItem()
{
}

void ConsumptionItem::Awake() {
	// ショップで購入したアイテムで枠が追加された場合、Startが呼ばれないのでこちらでも呼ぶ。

	_user = _Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");	// とりあえず使用者は固定でプレイヤー。
}

void ConsumptionItem::Start() {
	// Awake時にユーザーが生成されていない可能性があるのでこちらでも呼ぶ。
	Awake();
}


//アイテムを使う。
bool ConsumptionItem::UseItem() {

	if (static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Heel || static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Buff) {
		// 回復もしくはバフアイテム。

		// 使用者自身をターゲットにする。
		Item::ItemInfo* info = static_cast<Item::ItemInfo*>(_Info);

		// プレイヤーにアイテム効果発動.
		bool ret = _Player->ItemEffect(info);

		// 暫定処理。
		// ※ゲーム内で何とか効果がないことをお知らせすべき。
		if (!ret) {
			//char error[256];
			//sprintf(error, "何の成果も得られませんでしたぁっ！！");
			//MessageBoxA(0, error, "バフも回復もできないよ！", MB_ICONWARNING);
			return false;
		}

		return true;
	}
	else 
	{
		vector<EnemyCharacter*> targets;	// 消費アイテムを使う対象。

		// デバフアイテム。
		// 効果範囲内のエネミーを取得。
		int attr = Collision_ID::ENEMY;
		vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>> hit;
		fbPhysicsCallback::AllHitsContactResultCallback callback;
		INSTANCE(PhysicsWorld)->AllHitsContactTest(_gost->GetAttachCollision(), &hit,&callback, attr);

		if (hit.size() > effectNumTable[static_cast<int>(static_cast<Item::ItemInfo*>(_Info)->rangeStrength)]) {
			// 衝突した対象がアイテムの効果範囲より多い。

			for (int idx = 0; idx < effectNumTable[static_cast<int>(static_cast<Item::ItemInfo*>(_Info)->rangeStrength)]; idx++) {

				// 衝突した対象の中の近いものから取得していく。
				float min = FLT_MAX;
				GameObject* minObj = nullptr;
				int eraseIdx = 0;	// 先頭から何番目の要素か。
				for (int idx = 0; idx < hit.size(); idx++) {
					if (Vector3(_user->transform->GetPosition() - hit[idx]->collision->transform->GetPosition()).Length() < min) {
						minObj = hit[idx]->collision->gameObject;
						eraseIdx = idx;
					}
				}

				// 取得したコリジョンは配列から省く。
				hit.erase(hit.begin() + eraseIdx);

				// 対象に取得したオブジェクトを追加。
				targets.push_back((EnemyCharacter*)minObj);
			}
		}
		else {
			for (auto& info : hit) {
				// 対象に取得したオブジェクトを追加。
				targets.push_back(static_cast<EnemyCharacter*>(info->collision->gameObject));
			}
		}

		if (targets.size() <= 0) {

			// 暫定処理。
			// ※ゲーム内で何とか効果がないことをお知らせすべき。
			//char error[256];
			//sprintf(error, "何の成果も得られませんでしたぁっ！！");
			//MessageBoxA(0, error, "デバフなのに効果範囲内に敵がいないよ", MB_ICONWARNING);

			return false;
		}
		else {
			// 複合アイテムを考慮してとりあえず全部実行する。
			for (auto target : targets) {
				Item::ItemInfo* info = static_cast<Item::ItemInfo*>(_Info);

				target->ItemEffect(info);

			}
			targets.clear();
		}

		return true;
	}
}