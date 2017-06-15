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
	//vector<Collision*> collisions = INSTANCE(PhysicsWorld)->AllHitsContactTest(this->_Gost);

	btAlignedObjectArray<btCollisionObject*> collisions = _Gost->GetPairCollisions();

	for (int idx = 0; idx < collisions.size(); idx++) {
		// 取得したコリジョンの情報を参照して対応するコールバックを呼び出す。

		switch (_master) {
		case CollisionMaster::Player:
			// プレイヤーが生成した攻撃。
			if (collisions[idx]->getUserIndex() == Collision_ID::ENEMY) {
				// 敵と衝突した。
				static_cast<EnemyCharacter*>(static_cast<Collision*>(collisions[idx]->getUserPointer())->gameObject)->HitAttackCollision(this);
			}
			break;
		case CollisionMaster::Enemy:
			// 雑魚敵が生成した攻撃。
			if (collisions[idx]->getUserIndex() == Collision_ID::PLAYER) {
				// プレイヤーと衝突した。
				static_cast<Player*>(static_cast<Collision*>(collisions[idx]->getUserPointer())->gameObject)->HitAttackCollision(this);
			}
			break;
		case CollisionMaster::Other:
			// その他が生成した攻撃。
			// ※暫定。とりあえず何もしない。
			break;
		}
	}

	
}

GostCollision* AttackCollision::Create(int attack, const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master, float lifeTime, Transform* Parent) {
	_lifeTime = lifeTime;	// 寿命を保存。
	_master = master;		// コリジョンの生成者を保存。
	_AttackDamage = attack;		// 攻撃力を保存。
	static_cast<BoxCollider*>(_Colider)->Create(size);		// コライダー生成(※とりあえず暫定的にボックス固定)。
	if (Parent) {
		transform->SetParent(Parent);
	}
	transform->SetLocalPosition(pos);
	transform->SetLocalRotation(rotation);

	transform->UpdateTransform();
	_Gost->Create(_Colider, Collision_ID::ATTACK);	// ゴーストコリジョン生成。
	return _Gost;
}