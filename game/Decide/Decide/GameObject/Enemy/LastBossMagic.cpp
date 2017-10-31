#include "stdafx.h"
#include "GameObject\Enemy\LastBossMagic.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyCharacter.h"

void LastBossMagic::Awake() {
	ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// 攻撃処理に使用するパーティクル設定。
	_initParticleParam.Init();
	_initParticleParam.texturePath = "MurasakiHonoo.png";
	_initParticleParam.alphaBlendMode = 1;
	_initParticleParam.addVelocityRandomMargih = Vector3(0.4f,0.8f,0.4f);
	_initParticleParam.brightness = 1.1f;
	_initParticleParam.fadeTime = 0.2f;
	_initParticleParam.gravity = 0.0f;
	//_initParticleParam.initAlpha = 0.7f;
	_initParticleParam.initAlpha = 1.0f;

	_initParticleParam.initPositionRandomMargin = Vector3(0.3f, 0.3f, 0.4f);
	//_initParticleParam.initPositionRandomMargin = Vector3(0.8f, 0.8f, 0.4f);
	_initParticleParam.initVelocity = /*Vector3::back*/Vector3::up * 3.0f;
	_initParticleParam.initVelocityVelocityRandomMargin = Vector3(0.0f, 0.0f, 0.0f);
	_initParticleParam.intervalTime = 0.008f;
	_initParticleParam.isBillboard = true;
	_initParticleParam.isFade = true;
	_initParticleParam.life =0.1f;
	_initParticleParam.size = Vector2(0.5f, 0.5f);
	//_initParticleParam.size = Vector2(1.0f, 1.0f);
	_initParticleParam.mulColor = Color(1.3f,1.0f,1.3f,1.0f);
	_initParticleParam.isParent = true;
	p->Init(_initParticleParam);
	p->SetEmitFlg(false);

	_particleEmitter = p;
}

void LastBossMagic::Create(EnemyCharacter* obj, const Vector3& emitPosLocal, const Vector3& speed) {
	BreathObject::Create(obj);
	_initEmitPos = emitPosLocal;
	_speed = speed;
}

void LastBossMagic::Update() {
	if (_isShot) {
		if (_attack[0]->GetIsHit() && !_isBomb) {
			// 何かに当たった。
			
			// 爆発させる。
			_isShot = false;
			_isBomb = true;

			INSTANCE(GameObjectManager)->AddRemoveList(_attack[0]);
			_attack.clear();

			//// 真ん中に塊、周辺にかけらのエフェクト。
			//// ※使えるかもしれないので置いておく。
			//_initParticleParam.addVelocityRandomMargih = Vector3(2.0f, 2.0f, 2.0f);
			//_initParticleParam.brightness = 0.55f;
			//_initParticleParam.fadeTime = 0.5f;
			//_initParticleParam.initPositionRandomMargin = Vector3(0.1f, 0.1f, 0.1f);
			//_initParticleParam.initVelocity = Vector3::zero;
			//_initParticleParam.initVelocityVelocityRandomMargin = Vector3(3.0f, 3.0f, 3.0f);
			//_initParticleParam.intervalTime = 0.008f;
			//_initParticleParam.life = 0.2f;
			//_initParticleParam.size = Vector2(0.5f, 0.5f);
			//_initParticleParam.mulColor = Color(1.3f, 1.0f, 1.3f, 1.0f);

			// 爆発エフェクト。
			_initParticleParam.addVelocityRandomMargih = Vector3(2.0f, 2.0f, 2.0f);
			_initParticleParam.brightness = 1.1f;
			_initParticleParam.fadeTime = 0.5f;
			_initParticleParam.initPositionRandomMargin = Vector3(0.1f, 0.1f, 0.1f);
			_initParticleParam.initVelocity = Vector3::zero;
			_initParticleParam.initVelocityVelocityRandomMargin = Vector3(6.0f, 6.0f, 6.0f);
			_initParticleParam.intervalTime = 0.005f;
			_initParticleParam.life = 0.002f;
			_initParticleParam.size = Vector2(2.0f, 2.0f);
			_initParticleParam.mulColor = Color(1.3f, 1.0f, 1.3f, 1.0f);
			_initParticleParam.isParent = false;

			//攻撃コリジョン作成。
			AttackCollision* attack = _enemyObject->CreateAttack(Vector3(0.0f, 0.0f, 0.0f), Quaternion::Identity, Vector3(2.0f, 2.0f, 2.0f), 0.2, _particleEmitter->transform, true,false);
			attack->RemoveParent();
			_attack.push_back(attack);
		}
		else {
			_margin -= _margin * 0.5f * Time::DeltaTime();
			if (_margin <= 0.5f) {
				_margin = 0.5f;
			}

			_initParticleParam.initVelocity = (Vector3::up * 3.0f * _margin) + (Vector3::back * 6.0f);

			_particleEmitter->transform->SetPosition(_particleEmitter->transform->GetPosition() + (_speed * Time::DeltaTime()));
		}

		_particleEmitter->SetParam(_initParticleParam);
	}
	if (!_isBomb) {
		if (_timeCounter >= _interval) {
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
		else {
			_timeCounter += Time::DeltaTime();
		}
	}
	else {
		if (_attack[0]->IsDeath()) {
			// コリジョンの寿命が尽きたので削除。


			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
	}
}

void LastBossMagic::_BreathStartSubClass() {
	_particleEmitter->transform->SetParent(_enemyObject->transform);
	_particleEmitter->transform->SetLocalPosition(_initEmitPos);
	_particleEmitter->transform->SetParent(nullptr);

	// パラメータのベロシティをエネミーの正面から見た方向に変換。
	//Vector3 work = Vector3::zero;
	//work += (_enemyObject->transform->GetRight() * _initParticleParam.initVelocity.x);
	//work += (_enemyObject->transform->GetUp() * _initParticleParam.initVelocity.y);
	//work += (_enemyObject->transform->GetForward() * _initParticleParam.initVelocity.z);

	//_initParticleParam.initVelocity = work;

	_particleEmitter->SetParam(_initParticleParam);
	_particleEmitter->SetEmitFlg(true);

	//攻撃コリジョン作成。
	AttackCollision* attack = _enemyObject->CreateAttack(Vector3(0.0f, 0.0f, -0.9f), Quaternion::Identity, Vector3(1.0f, 1.0f, 1.8f), -1.0f, _particleEmitter->transform,true);
	//attack->RemoveParent();
	_attack.push_back(attack);

	_timeCounter = 0.0f;
}
