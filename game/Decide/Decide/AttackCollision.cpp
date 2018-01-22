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
	if (_lifeTime >= 0.0f && _time > _lifeTime)
	{
		// 寿命が無限でないかつ寿命を過ぎた。

		if (_isLifeOverDelete) {
			// 削除。
			INSTANCE(GameObjectManager)->AddRemoveList(this);
			_isAlive = false;
		}
		else {
			this->SetActive(false);
			_isAlive = false;
		}
	}
	else {
		// 寿命がまだある。

		// 衝突判定。
		DetectionCollision();
	}
}

void AttackCollision::LateUpdate()
{
}

void AttackCollision::DetectionCollision() {
	// 衝突しているコリジョンをすべて取得する。
	vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>> hit;
	int attr = Collision_ID::ENEMY | Collision_ID::BOSS | Collision_ID::PLAYER | Collision_ID::GROUND | Collision_ID::BUILDING | Collision_ID::DRARIAN;
	fbPhysicsCallback::AllHitsContactResultCallback callback;
	INSTANCE(PhysicsWorld)->AllHitsContactTest(_Gost, &hit, &callback, attr);

	//btAlignedObjectArray<btCollisionObject*> collisions = _Gost->GetPairCollisions();
	int max = static_cast<int>(_hitInfos.size());
	for (int num = 0; num < max; ) {
		// 登録されているオブジェクト分検索。

		bool isHit = false;	// 登録しているオブジェクトが現在も衝突しているか。

		if (_hitInfos[num]->coll->getUserPointer() == nullptr)
		{
			// 登録されていたオブジェクトが削除された。

			_hitInfos[num]->object = nullptr;
		}
		else {

			// フラグ初期化。
			_hitInfos[num]->isCallStay = false;

			int max2 = static_cast<int>(hit.size());
			for (int idx = 0; idx < max2; idx++) {
				// 新しく取得した衝突コリジョン分検索。

				GameObject* obj = _CollisionObjectToGameObject(hit[idx]->collision->GetCollisionObj());
				if (obj && _hitInfos[num]->object == obj) {
					// 登録されているオブジェクトが衝突していた。

					if (!_hitInfos[num]->isCallStay) {
						// ステイコールバックがまだ呼ばれていない。

						// 衝突している間呼び続けるコールバック処理。
						_CallBackStay(_hitInfos[num]->object);
						_hitInfos[num]->isCallStay = true;
					}

					// 登録されているものは新しく取得した配列から除く。
					hit.erase(hit.begin() + idx);
					max2--;
					isHit = true;
				}
			}
		}

		if (!isHit) {
			// 登録されていたオブジェクトが衝突していなかった。

			if (_hitInfos[num]->object) {
				// 衝突を外れた時に呼び出すコールバック処理。
				_CallBackExit(_hitInfos[num]->object);
			}

			_hitInfos.erase(_hitInfos.begin() + num);

			// 要素数を減らす。
			max--;
		}
		else {
			// 次の要素に進む。
			num++;
		}
	}


	vector<GameObject*> add;
	for (int idx = 0; idx < static_cast<int>(hit.size()); idx++) {
		// 新しく衝突したオブジェクト。

		if (!_isHit) {
			_isHit = true;
		}

		GameObject* obj = _CollisionObjectToGameObject(hit[idx]->collision->GetCollisionObj());

		bool flg = true;
		for (auto& a : add) {

			if (a == obj) {
				// 追加済みオブジェクトと同じものは無視。
				flg = false;
				break;
			}
		}

		if (flg) {
			if (hit[idx]->collision->GetID()) {
				// 衝突した瞬間に呼び出すコールバック処理。
				_CallBackEnter(hit[idx]->collision->GetCollisionObj());

				// 衝突リストに追加。
				unique_ptr<HitObjectInfo> info(new HitObjectInfo);

				info->coll = hit[idx]->collision->GetCollisionObj_shared();

				info->object = obj;
				_hitInfos.push_back(move(info));
				add.push_back(obj);
			}

		}
	}

	hit.clear();
	add.clear();

}

void AttackCollision::_CallBackEnter(btCollisionObject* coll) {
	GameObject* obj = _CollisionObjectToGameObject(coll);
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// プレイヤーが生成した攻撃。
			if (coll->getUserIndex() == Collision_ID::ENEMY || coll->getUserIndex() == Collision_ID::BOSS || coll->getUserIndex() == Collision_ID::DRARIAN) {
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
void AttackCollision::_CallBackStay(GameObject* obj) {
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// プレイヤーが生成した攻撃。
			if (obj->GetAttachCollision()->GetID() == Collision_ID::ENEMY || obj->GetAttachCollision()->GetID() == Collision_ID::BOSS) {
				// 敵と衝突した。
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionStay(this);
			}
			break;
		case CollisionMaster::Enemy:
			// 雑魚敵が生成した攻撃。
			if (obj->GetAttachCollision()->GetID() == Collision_ID::PLAYER) {
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

void AttackCollision::_CallBackExit(GameObject* obj) {
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// プレイヤーが生成した攻撃。
			if (obj->GetAttachCollision()->GetID() == Collision_ID::ENEMY || obj->GetAttachCollision()->GetID() == Collision_ID::BOSS) {
				// 敵と衝突した。
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionExit(this);
			}
			break;
		case CollisionMaster::Enemy:
			// 雑魚敵が生成した攻撃。
			if (obj->GetAttachCollision()->GetID() == Collision_ID::PLAYER) {
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

void AttackCollision::Create(unique_ptr<CharacterParameter::DamageInfo> info, const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master, float lifeTime, ReactionType reactionType, Transform* Parent, bool isLifeOverDelete) {

	_DamageInfo = move(info);

	_lifeTime = lifeTime;	// 寿命を保存。
	_master = master;		// コリジョンの生成者を保存。
	_reactionType = reactionType;
	_isLifeOverDelete = isLifeOverDelete;
	_Colider->Create(size);		// コライダー生成(※とりあえず暫定的にボックス固定)。

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
	_isAlive = true;
	transform->UpdateTransform();
	_Gost->Create(_Colider, Collision_ID::ATTACK);	// ゴーストコリジョン生成。
	_time = 0.0f;
}
