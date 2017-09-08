#include "stdafx.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyCharacter.h"

void LaserBreath::Start() {
}

void LaserBreath::Update() {
	if (_particleList) {
		for (auto particle : *_particleList) {
			Vector3 size = particle->transform->GetLocalScale();
			size.y -= 0.1f * Time::DeltaTime();
			particle->transform->SetLocalScale(size);
		}
	}

	_UpdateCollision();
}

void LaserBreath::BreathStart(){
	//攻撃コリジョン作成。
	AttackCollision* attack = _enemyObject->CreateAttack(Vector3(0.0f, 0.0f, 0.0f), Quaternion::Identity, Vector3(0.0f, 0.0f, 0.0f), -1.0f, _enemyObject->transform);
	attack->RemoveParent();
	_attack.push_back(attack);
}

void LaserBreath::_UpdateCollision() {
	if (_attack.size() >= 1) {
		GostCollision* Gost = _attack[0]->GetGostCollision();
		if (Gost) {
			vector<Particle*>* list = _particleList.get();
			Particle* start = nullptr;
			if (list->size() >= 1) {
				// パーティクルが生成されている。

				start = *(list->begin());	// 最初に生成されたパーティクルを取得。
				Particle* end = *(--list->end());	// 最後に生成されたパーティクルを取得。
				if (start != end) {
					// パーティクルの先頭と終端が同一のパーティクルではない。

					Vector3 breathEndPos = start->transform->GetPosition();	// ブレスの終端位置は最初に生成されたパーティクルの位置。
					Vector3 breathStartPos = end->transform->GetPosition(); // ブレスの開始位置は最後に生成されたパーティクルの位置。

					// ブレス開始位置から終端位置までの距離をコリジョンの奥行サイズとする。
					float sizeZ = Vector3(breathEndPos - breathStartPos).Length();

					// 判定コリジョンのサイズを変更。
					if (start != _start || end != _end) {
						// ブレスの先頭か終端が変更している。

						// 直前の値を破棄し、現在の値を新しく保存。
						_start = start;
						_end = end;
					}

					float sizeXYOrigin = 4.0f;
					static_cast<BoxCollider*>(const_cast<Collider*>(Gost->GetShape()))->Resize(Vector3(sizeXYOrigin * start->transform->GetLocalScale().y, sizeXYOrigin * start->transform->GetLocalScale().y, sizeZ));
					//const_cast<Collider*>(Gost->GetShape())->RenderDisable();

					// 位置設定。
					Gost->transform->SetPosition(breathEndPos);
					// 位置をサイズの半分だけずらすことでコリジョンの中心を指定する。
					Vector3 pos = Gost->transform->GetLocalPosition();
					Vector3 dir = start->GetVelocity();
					dir.Normalize();
					pos -= dir * sizeZ * 0.5f;
					Gost->transform->SetLocalPosition(pos);
				}

				if (start->transform->GetLocalScale().y <= 0.0f) {
					// レーザーが消滅している。

					INSTANCE(GameObjectManager)->AddRemoveList(this);
				}


			}
		}
	}
}
