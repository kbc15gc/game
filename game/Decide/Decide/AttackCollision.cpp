#include"stdafx.h"
#include "AttackCollision.h"
#include "GameObject\Player\Player.h"
#include "GameObject\Enemy\EnemyCharacter.h"

void AttackCollision::Awake()
{
	// ※とりあえず暫定。
	// 箱形状のコライダーを生成。
	_Colider = AddComponent<BoxCollider>();
	_Gost = AddComponent<GostCollision>();
}

void AttackCollision::Update()
{
	time += Time::DeltaTime();
	if (_lifeTime >= 0.0f && time > _lifeTime)
	{
		// 寿命が無限でないかつ寿命を過ぎた。
		// かつ寿命が無限でない。

		// 削除。
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
	else {
		// 寿命がまだある。

		// 衝突判定。
		DetectionCollision();
	}
}

void AttackCollision::LateUpdate()
{
	btAlignedObjectArray<btCollisionObject*> colls = _Gost->GetPairCollisions();

	FOR(i, colls.size())
		((Collision*)(colls.at(i)->getUserPointer()))->gameObject;
}

void AttackCollision::DetectionCollision() {
	// 衝突しているコリジョンをすべて取得する。
	btAlignedObjectArray<btCollisionObject*> collisions = _Gost->GetPairCollisions();

	for (auto itr = _HitCollisions.begin(); itr != _HitCollisions.end();) {
		bool isHit = false;	// 登録しているコリジョンが現在も衝突しているか。
		for (int idx = 0; idx < collisions.size(); idx++) {
			if (*itr == collisions[idx]->getUserPointer()) {
				// 既に登録されているコリジョンが衝突している。
				isHit = true;

				// 衝突している間呼び続けるコールバック処理。
				_CallBackStay(*itr);
				// コールバックを呼んだものは配列から除く。
				collisions.remove(collisions[idx]);
				break;
			}
			else if (collisions[idx]->getUserIndex() != Collision_ID::PLAYER && 
				collisions[idx]->getUserIndex() != Collision_ID::ENEMY &&
				collisions[idx]->getUserIndex() != Collision_ID::BOSS) {
				// コールバックを呼ばないコリジョンだった。
				collisions.remove(collisions[idx]);
			}
		}

		if (!isHit) {
			// 登録されていたコリジョンが衝突していなかった。
			// 衝突を外れた時に呼び出すコールバック処理。
			_CallBackExit(*itr);
			itr = _HitCollisions.erase(itr);
		}
		else {
			itr++;
		}
	}

	for (int idx = 0; idx < collisions.size();idx++) {
		// 衝突をした瞬間に呼び出すコールバック処理。
		_CallBackEnter(static_cast<Collision*>(collisions[idx]->getUserPointer()));
		// 衝突リストに追加。
		_HitCollisions.push_back(static_cast<Collision*>(collisions[idx]->getUserPointer()));
	}
}

void AttackCollision::_CallBackEnter(Collision* coll) {
	switch (_master) {
	case CollisionMaster::Player:
		// プレイヤーが生成した攻撃。
		if (coll->GetID() == Collision_ID::ENEMY) {
			// 敵と衝突した。
			static_cast<EnemyCharacter*>(coll->gameObject)->HitAttackCollisionEnter(this);
		}
		break;
	case CollisionMaster::Enemy:
		// 雑魚敵が生成した攻撃。
		if (coll->GetID() == Collision_ID::PLAYER) {
			// プレイヤーと衝突した。
			static_cast<Player*>(coll->gameObject)->HitAttackCollisionEnter(this);
		}
		break;
	case CollisionMaster::Other:
		// その他が生成した攻撃。
		// ※暫定。とりあえず何もしない。
		break;
	}
}

// 衝突している間呼び続けるコールバック処理。
void AttackCollision::_CallBackStay(Collision* coll) {
	switch (_master) {
	case CollisionMaster::Player:
		// プレイヤーが生成した攻撃。
		if (coll->GetID() == Collision_ID::ENEMY) {
			// 敵と衝突した。
			static_cast<EnemyCharacter*>(coll->gameObject)->HitAttackCollisionStay(this);
		}
		break;
	case CollisionMaster::Enemy:
		// 雑魚敵が生成した攻撃。
		if (coll->GetID() == Collision_ID::PLAYER) {
			// プレイヤーと衝突した。
			static_cast<Player*>(coll->gameObject)->HitAttackCollisionStay(this);
		}
		break;
	case CollisionMaster::Other:
		// その他が生成した攻撃。
		// ※暫定。とりあえず何もしない。
		break;
	}
}

void AttackCollision::_CallBackExit(Collision* coll) {
	switch (_master) {
	case CollisionMaster::Player:
		// プレイヤーが生成した攻撃。
		if (coll->GetID() == Collision_ID::ENEMY) {
			// 敵と衝突した。
			static_cast<EnemyCharacter*>(coll->gameObject)->HitAttackCollisionExit(this);
		}
		break;
	case CollisionMaster::Enemy:
		// 雑魚敵が生成した攻撃。
		if (coll->GetID() == Collision_ID::PLAYER) {
			// プレイヤーと衝突した。
			static_cast<Player*>(coll->gameObject)->HitAttackCollisionExit(this);
		}
		break;
	case CollisionMaster::Other:
		// その他が生成した攻撃。
		// ※暫定。とりあえず何もしない。
		break;
	}
}

GostCollision* AttackCollision::Create(int damage,const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master, float lifeTime, Transform* Parent) {
	_lifeTime = lifeTime;	// 寿命を保存。
	_master = master;		// コリジョンの生成者を保存。
	static_cast<BoxCollider*>(_Colider)->Create(size);		// コライダー生成(※とりあえず暫定的にボックス固定)。
	if (Parent) {
		transform->SetParent(Parent);
	}
	transform->SetLocalPosition(pos);
	transform->SetLocalRotation(rotation);

	transform->UpdateTransform();
	_Gost->Create(_Colider, Collision_ID::ATTACK);	// ゴーストコリジョン生成。

	_Damage = damage;
	return _Gost;
}