#include "stdafx.h"
#include "GameObject\Enemy\LastBossMagic.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyCharacter.h"

void LastBossMagic::Awake() {
	ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// 攻撃処理に使用するパーティクル設定。
	_initParticleParam.Init();
	_initParticleParam.texturePath = "MurasakiHonoo.png";
	//_initParticleParam.alphaBlendMode = 0;
	_initParticleParam.alphaBlendMode = 1;
	_initParticleParam.addVelocityRandomMargih = Vector3(0.5f,0.5f,0.5f);
	//_initParticleParam.brightness = 4.0f;
	_initParticleParam.brightness = 0.7f;
	_initParticleParam.fadeTime = 0.2f;
	_initParticleParam.gravity = 0.0f;
	//_initParticleParam.initAlpha = 0.7f;
	_initParticleParam.initAlpha = 0.7f;
	_initParticleParam.initPositionRandomMargin = Vector3(0.4f, 0.4f, 0.4f);
	_initParticleParam.initVelocity = Vector3::zero;
	_initParticleParam.initVelocityVelocityRandomMargin = Vector3(0.02f, 0.02f, 0.02f);
	_initParticleParam.intervalTime = 0.007f;
	_initParticleParam.isBillboard = true;
	_initParticleParam.isFade = true;
	_initParticleParam.life =0.1f;
	_initParticleParam.size = Vector2(0.5f, 0.5f);
	//_initParticleParam.mulColor = Color(1.5f, 1.0f, 1.5f, 1.0f);
	_initParticleParam.mulColor = Color(1.2f,1.0f,1.2f,1.0f);
	p->Init(_initParticleParam);
	p->SetEmitFlg(false);

	_particleEmitter = p;
}

void LastBossMagic::Init(EnemyCharacter* obj, const Vector3& emitPosLocal) {
	BreathObject::Init(obj, emitPosLocal);
	_initEmitPos = emitPosLocal;
}

void LastBossMagic::Update() {
	_particleEmitter->transform->SetPosition(_particleEmitter->transform->GetPosition() + (_direction * 5.0f * Time::DeltaTime()));

	if (_timeCounter >= _interval) {
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
	else {
		_timeCounter += Time::DeltaTime();
	}
}

void LastBossMagic::BreathStart() {
	_particleEmitter->transform->SetParent(_enemyObject->transform);
	_particleEmitter->transform->SetLocalPosition(_initEmitPos);
	_particleEmitter->transform->SetParent(nullptr);

	// パーティクルの飛ぶ方向をエネミーの向きに再設定。
	Vector3 initVelocity = _enemyObject->transform->GetForward();
	initVelocity.Normalize();
	_direction = initVelocity;
	_particleEmitter->SetEmitFlg(true);

	//攻撃コリジョン作成。
	AttackCollision* attack = _enemyObject->CreateAttack(Vector3(0.0f, 0.0f, 0.0f), Quaternion::Identity, Vector3(0.0f, 0.0f, 0.0f), -1.0f, _particleEmitter->transform);
	attack->RemoveParent();
	_attack.push_back(attack);

	_timeCounter = 0.0f;
}
