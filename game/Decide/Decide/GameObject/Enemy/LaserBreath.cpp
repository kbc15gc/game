#include "stdafx.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyCharacter.h"

void LaserBreath::Awake() {
	ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// 攻撃処理に使用するパーティクル設定。
	_initParticleParam.Init();
	_initParticleParam.texturePath = "t1.png";
	_initParticleParam.alphaBlendMode = 1;
	_initParticleParam.addVelocityRandomMargih = Vector3::zero;
	_initParticleParam.brightness = 7.0f;
	_initParticleParam.fadeTime = 0.2f;
	_initParticleParam.gravity = 0.0f;
	_initParticleParam.initAlpha = 1.0f;
	_initParticleParam.initPositionRandomMargin = Vector3::zero;
	_initParticleParam.initVelocity = Vector3::front * 10.0f;
	_initParticleParam.initVelocityVelocityRandomMargin = Vector3::zero;
	_initParticleParam.intervalTime = 0.001f;
	_initParticleParam.isBillboard = true;
	_initParticleParam.isFade = true;
	_initParticleParam.life = -1.0f;
	_initParticleParam.size = Vector2(0.5f, 0.5f);
	p->Init(_initParticleParam);
	p->SetEmitFlg(false);

	_particleEmitter = p;
}

void LaserBreath::Init(EnemyCharacter* obj, const Vector3& emitPosLocal){
	BreathObject::Init(obj, emitPosLocal);
	_particleEmitter->transform->SetParent(_enemyObject->transform);
	_particleEmitter->transform->SetLocalPosition(emitPosLocal);
}

void LaserBreath::Update() {

	ParticleParameter param = _particleEmitter->GetParam();
	param.size.y -= 0.3f * Time::DeltaTime();
	_particleEmitter->SetParam(param);

	if (_particleList) {
		for (auto particle : *_particleList) {
			//Vector3 size = particle->transform->GetLocalScale();
			//size.y -= 0.1f * Time::DeltaTime();
			//particle->transform->SetLocalScale(size);
			particle->transform->SetScale(Vector3(param.size));
		}
	}

	_UpdateCollision();
}

void LaserBreath::BreathStart(){
	// パーティクルの飛ぶ方向をエネミーの向きに再設定。
	Vector3 initVelocity = _enemyObject->transform->GetForward();
	initVelocity.Normalize();
	_initParticleParam.initVelocity = initVelocity * _particleEmitter->GetInitVelocity().Length();
	_particleEmitter->SetParam(_initParticleParam);

	// 作成したパーティクルを収集。
	_particleList.reset(new vector<Particle*>);
	_particleEmitter->AchievedCreateParticleStart(_particleList.get());
	_particleEmitter->SetEmitFlg(true);

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
					static_cast<BoxCollider*>(const_cast<Collider*>(Gost->GetShape()))->Resize(Vector3(sizeXYOrigin * start->transform->GetScale().y, sizeXYOrigin * start->transform->GetScale().y, sizeZ));
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
