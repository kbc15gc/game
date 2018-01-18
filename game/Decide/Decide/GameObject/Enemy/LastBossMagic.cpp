#include "stdafx.h"
#include "GameObject\Enemy\LastBossMagic.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "GameObject\ParticleParamTable.h"

void LastBossMagic::Awake() {
	ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// 攻撃処理に使用するパーティクル設定。
	_initParticleParam.Init();
	_initParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::FoxFireViolet];
	p->Init(_initParticleParam);
	p->SetEmitFlg(false);

	_particleEmitter = p;
}

void LastBossMagic::Create(EnemyCharacter* obj, const Vector3& emitPosLocal, const Quaternion& rot, const float speed) {
	BreathObject::Create(obj);
	_initEmitPos = emitPosLocal;
	_speed = speed;
	_rotation = rot;
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

			// 爆発エフェクト。
			_initParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::BombViolet];

			//攻撃コリジョン作成。
			if (_enemyObject) {
				AttackCollision* attack = _enemyObject->CreateAttack(Vector3(0.0f, 0.0f, 0.0f), Quaternion::Identity, Vector3(2.0f, 2.0f, 2.0f), 0.2f, _particleEmitter->transform, true, false, AttackCollision::ReactionType::Blown);
				attack->RemoveParent();
				_attack.push_back(attack);
			}
		}
		else {
			_margin -= _margin * 0.5f * Time::DeltaTime();
			if (_margin <= 0.5f) {
				_margin = 0.5f;
			}

			_initParticleParam.initVelocity = (Vector3::up * 3.0f * _margin) + (Vector3::back * 6.0f);

			_particleEmitter->transform->SetPosition(_particleEmitter->transform->GetPosition() + (_direction * _speed * Time::DeltaTime()));
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
	_particleEmitter->transform->SetLocalRotation(Quaternion::Identity);
	_particleEmitter->transform->SetParent(nullptr);
	_direction = _rotation.RotationVector3(_particleEmitter->transform->GetForward());	// エミッターの飛ぶ方向を算出。
	// パーティクルの発生方向を飛んでいく方向とは逆方向にしたいのでエミッターを回転。
	Quaternion rot = _particleEmitter->transform->GetRotation();
	_particleEmitter->transform->SetRotation(rot * _rotation);

	_particleEmitter->SetParam(_initParticleParam);
	_particleEmitter->SetEmitFlg(true);

	//攻撃コリジョン作成。
	AttackCollision* attack = _enemyObject->CreateAttack(Vector3(0.0f, 0.0f, 0.0f), Quaternion::Identity, Vector3(1.0f, 1.5f, 1.0f), -1.0f, _particleEmitter->transform,true);
	_attack.push_back(attack);

	_timeCounter = 0.0f;
}

void LastBossMagic::BreathStop() {
	if (_particleEmitter) {
		_particleEmitter->SetActive(false);
	}
	BreathObject::BreathStop();
};

void LastBossMagic::BreathPlay() {
	if (_particleEmitter) {
		_particleEmitter->SetActive(true);
	}
	BreathObject::BreathPlay();
};
