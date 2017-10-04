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
	_time += Time::DeltaTime();
	if (_isCreateCollision) {
		// コリジョンが生成されている。
		if (_lifeTime >= 0.0f && _time > _lifeTime)
		{
			// 寿命が無限でないかつ寿命を過ぎた。

			// 削除。
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
		else {
			// 寿命がまだある。

			// 衝突判定。
			DetectionCollision();
		}
	}
	else {
		// コリジョンが生成されていない。
		CreateCollision();
	}
}

void AttackCollision::LateUpdate()
{
}

void AttackCollision::DetectionCollision() {
	// 衝突しているコリジョンをすべて取得する。
	btAlignedObjectArray<btCollisionObject*> collisions = _Gost->GetPairCollisions();

	for (auto itr = _HitCollisions.begin(); itr != _HitCollisions.end();) {
		bool isHit = false;	// 登録しているコリジョンが現在も衝突しているか。
		for (int idx = 0; idx < collisions.size(); idx++) {
			if ((*itr).get() == collisions[idx]) {
				// 既に登録されているコリジョンが衝突している。
				isHit = true;

				// 衝突している間呼び続けるコールバック処理。
				_CallBackStay((*itr).get());
				// コールバックを呼んだものは配列から除く。
				collisions.remove(collisions[idx]);
				break;
			}
			else if (collisions[idx]->getUserIndex() != Collision_ID::PLAYER && 
				collisions[idx]->getUserIndex() != Collision_ID::ENEMY) {
				// コールバックを呼ばないコリジョンだった。
				collisions.remove(collisions[idx]);
			}
		}

		if (!isHit) {
			// 登録されていたコリジョンが衝突していなかった。
			// 衝突を外れた時に呼び出すコールバック処理。
			_CallBackExit((*itr).get());
			//(*itr).reset();	// シェアードポインタの参照カウンタを明示的に下げる(必要？)。
			itr = _HitCollisions.erase(itr);
		}
		else {
			itr++;
		}
	}

	for (int idx = 0; idx < collisions.size();idx++) {
		// 衝突をした瞬間に呼び出すコールバック処理。
		_CallBackEnter(collisions[idx]);
		// 衝突リストに追加。
		_HitCollisions.push_back(static_cast<Collision*>(collisions[idx]->getUserPointer())->GetCollisionObj_shared());
	}
}

void AttackCollision::_CallBackEnter(btCollisionObject* coll) {
	GameObject* obj = _CollisionObjectToGameObject(coll);
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// プレイヤーが生成した攻撃。
			if (coll->getUserIndex() == Collision_ID::ENEMY) {
				// 敵と衝突した。
				// ゲームオブジェクトがある。
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionEnter(this);
			}
			break;
		case CollisionMaster::Enemy:
			// 雑魚敵が生成した攻撃。
			if (coll->getUserIndex() == Collision_ID::PLAYER) {
				// プレイヤーと衝突した。
				static_cast<Player*>(obj)->HitAttackCollisionEnter(this);
			}
			break;
		case CollisionMaster::Other:
			// その他が生成した攻撃。
			// ※暫定。とりあえず何もしない。
			break;
		}
	}
}

// 衝突している間呼び続けるコールバック処理。
void AttackCollision::_CallBackStay(btCollisionObject* coll) {
	GameObject* obj = _CollisionObjectToGameObject(coll);
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// プレイヤーが生成した攻撃。
			if (coll->getUserIndex() == Collision_ID::ENEMY) {
				// 敵と衝突した。
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionStay(this);
			}
			break;
		case CollisionMaster::Enemy:
			// 雑魚敵が生成した攻撃。
			if (coll->getUserIndex() == Collision_ID::PLAYER) {
				// プレイヤーと衝突した。
				static_cast<Player*>(obj)->HitAttackCollisionStay(this);
			}
			break;
		case CollisionMaster::Other:
			// その他が生成した攻撃。
			// ※暫定。とりあえず何もしない。
			break;
		}
	}
}

void AttackCollision::_CallBackExit(btCollisionObject* coll) {
	GameObject* obj = _CollisionObjectToGameObject(coll);
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// プレイヤーが生成した攻撃。
			if (coll->getUserIndex() == Collision_ID::ENEMY) {
				// 敵と衝突した。
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionExit(this);
			}
			break;
		case CollisionMaster::Enemy:
			// 雑魚敵が生成した攻撃。
			if (coll->getUserIndex() == Collision_ID::PLAYER) {
				// プレイヤーと衝突した。
				static_cast<Player*>(obj)->HitAttackCollisionExit(this);
			}
			break;
		case CollisionMaster::Other:
			// その他が生成した攻撃。
			// ※暫定。とりあえず何もしない。
			break;
		}
	}
}

void AttackCollision::Create(unique_ptr<CharacterParameter::DamageInfo> info, const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master, float lifeTime, float waitTime, Transform* Parent) {

	_DamageInfo = move(info);

	_lifeTime = lifeTime;	// 寿命を保存。
	_master = master;		// コリジョンの生成者を保存。
	_waitTime = waitTime;	// 生成待ち時間保存。

	static_cast<BoxCollider*>(_Colider)->Create(size);		// コライダー生成(※とりあえず暫定的にボックス固定)。

	// コリジョンに設定するTransform情報設定。
	if (Parent) {
		transform->SetParent(Parent);
	}
	transform->SetLocalPosition(pos);
	transform->SetLocalRotation(rotation);

	// コリジョン生成。
	CreateCollision();
}

void AttackCollision::CreateCollision() {
	if (_time >= _waitTime) {
		// 待ち時間をカウントし終えた。
		transform->UpdateTransform();
		_Gost->Create(_Colider, Collision_ID::ATTACK);	// ゴーストコリジョン生成。
		_isCreateCollision = true;
		_time = 0.0f;
	}
}
