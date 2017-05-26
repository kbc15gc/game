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
			// 寿命が無限でない。

			// 削除。
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
	else {
		// 寿命がまだある。

		// 衝突判定。
		// ※衝突しているコリジョンをすべて取得する。
		btAlignedObjectArray<btCollisionObject*> collisions = _Gost->GetPairCollisions();
		for (int idx = 0; idx < collisions.size();idx++) {
			switch (_master) {
			case CollisionMaster::Player:
				// プレイヤーが生成した攻撃。
				if (collisions[idx]->getUserIndex() == Collision_ID::ENEMY) {
					// 敵と衝突した。
					static_cast<Player*>(static_cast<Collision*>(collisions[idx]->getUserPointer())->gameObject)->HitAttackCollision(this);
				}
				break;
			case CollisionMaster::Enemy:
				// 雑魚敵が生成した攻撃。
				if (collisions[idx]->getUserIndex() == Collision_ID::PLAYER) {
					// プレイヤーと衝突した。
					static_cast<EnemyCharacter*>(static_cast<Collision*>(collisions[idx]->getUserPointer())->gameObject)->HitAttackCollision(this);
				}
				break;
			case CollisionMaster::Other:
				// その他が生成した攻撃。
				// ※暫定。とりあえず何もしない。
				break;
			}
		}
	}
}

