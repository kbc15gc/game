#include "stdafx.h"
#include "GameObject\Enemy\SordShock.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "GameObject\ParticleParamTable.h"

void SordShock::Awake() {
	ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// 衝撃波が通り過ぎた後のちろちろのパーティクル設定。
	_initTirotiroParticleParam.Init();
	_initTirotiroParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::TiroTiroViolet];
	p->Init(_initTirotiroParticleParam);
	p->SetEmitFlg(false);
	_tirotiroEmitter = p;


	p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// 衝撃波のパーティクル設定。
	_initShockParticleParam.Init();
	_initShockParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::ShockViolet];
	p->Init(_initShockParticleParam);
	p->SetEmitFlg(false);
	_shockParticleEmitter = p;
}

void SordShock::Create(EnemyCharacter* obj, const Vector3& emitPosLocal, const Vector3& speed, const float range) {
	BreathObject::Create(obj);
	_emitPos = emitPosLocal;
	_speed = speed;
	_range = range;
}

void SordShock::Update() {
	if(_isShot){
		// 衝撃波を進める。
		_shockParticleEmitter->transform->SetPosition(_shockParticleEmitter->transform->GetPosition() + _speed * Time::DeltaTime());
		Vector3 work = _shockParticleEmitter->transform->GetPosition() - _emitPos;	// 初期位置から現在の衝撃波の位置までのベクトル。

		// ちろちろの長さを変更。
		_initTirotiroParticleParam.initPositionRandomMargin.z = work.Length() * 0.5f;
		_attack[1]->ReSize(Vector3(0.5f, 0.5f, _initTirotiroParticleParam.initPositionRandomMargin.z * 2.0f));

		_tirotiroEmitter->SetParam(_initTirotiroParticleParam);
		// 長さを変更したので位置をずらす。
		Vector3 lpos = _tirotiroEmitter->transform->GetLocalPosition();
		_tirotiroEmitter->transform->SetLocalPosition(lpos.x, lpos.y,(work.Length() * 0.5f) * -1.0f);

		if (work.Length() >= _range) {
			// 衝撃波が飛距離まで飛んだ。

			_isShot = false;
			_isAutoRange = true;
			_initShockParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::TiroTiroTowerViolet];
			_shockParticleEmitter->ResetParameterAlreadyCreated(_initShockParticleParam);
			// 光の柱のコリジョン作成。
			INSTANCE(GameObjectManager)->AddRemoveList(_attack[0]);
			_attack[0] = _enemyObject->CreateAttack(Vector3(0.0f, 2.0f, 0.0f), Quaternion::Identity, Vector3(1.0f, 4.0f, 1.0f), -1.0f, _shockParticleEmitter->transform, false, true, AttackCollision::ReactionType::NotAction);
		}
		else if (_attack[0]->GetIsHit()) {
			// 何かと衝突。

			_isShot = false;
			_isBomb = true;
			_initShockParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::BombViolet];
			_shockParticleEmitter->SetParam(_initShockParticleParam);
			INSTANCE(GameObjectManager)->AddRemoveList(_attack[0]);

			//攻撃コリジョン作成。
			_enemyObject->CreateAttack(Vector3(0.0f, 1.0f, 0.0f), Quaternion::Identity, Vector3(2.0f, 2.0f, 2.0f), 0.2f, _shockParticleEmitter->transform, false, false, AttackCollision::ReactionType::Blown)->RemoveParent();
		}
	}
	else {
		if (_isBomb && _timeCounter >= 0.2f) {
			// 爆発終了。
			_isBomb = false;
			_initShockParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::TiroTiroTowerViolet];
			_shockParticleEmitter->SetParam(_initShockParticleParam);

			// 光の柱のコリジョン作成。
			//INSTANCE(GameObjectManager)->AddRemoveList(_attack[0]);
			_attack[0] = _enemyObject->CreateAttack(Vector3(0.0f, 2.0f, 0.0f), Quaternion::Identity, Vector3(1.0f, 4.0f, 1.0f), -1.0f, _shockParticleEmitter->transform, false, true, AttackCollision::ReactionType::NotAction,10);
		}

		if (_timeCounter >= (_interval + 10.0f)) {
			// 光の柱を消したいタイミングで削除。
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
		if (_tirotiroEmitter && (_timeCounter >= _interval)) {
			// ちろちろは柱より先に消す。
			INSTANCE(GameObjectManager)->AddRemoveList(_tirotiroEmitter);
			_tirotiroEmitter = nullptr;
			INSTANCE(GameObjectManager)->AddRemoveList(_attack[1]);
			_attack[1] = nullptr;
		}
		_timeCounter += Time::DeltaTime();
	}
}

void SordShock::_BreathStartSubClass() {
	_shockParticleEmitter->transform->SetParent(_enemyObject->transform);
	_shockParticleEmitter->transform->SetLocalPosition(_emitPos);
	_shockParticleEmitter->transform->SetLocalRotation(Quaternion::Identity);
	_shockParticleEmitter->transform->SetParent(nullptr);
	_emitPos = _shockParticleEmitter->transform->GetPosition();

	_tirotiroEmitter->transform->SetParent(_shockParticleEmitter->transform);
	_tirotiroEmitter->transform->SetLocalPosition(Vector3(0.0f,-0.5f,0.0f));
	_tirotiroEmitter->transform->SetLocalRotation(Quaternion::Identity);

	_shockParticleEmitter->SetEmitFlg(true);
	_tirotiroEmitter->SetEmitFlg(true);

	//衝撃波の攻撃コリジョン作成。
	AttackCollision* attack = _enemyObject->CreateAttack(Vector3(0.0f, 0.5f, 0.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 1.0f), -1.0f, _shockParticleEmitter->transform);
	_attack.push_back(attack);

	//ちろちろの攻撃コリジョン作成。
	attack = _enemyObject->CreateAttack(Vector3(0.0f,0.0f,0.0f), Quaternion::Identity, Vector3(0.5f, 0.5f, 0.0f), -1.0f, _tirotiroEmitter->transform,false,true,AttackCollision::ReactionType::NotAction,25);
	_attack.push_back(attack);

	_isShot = true;

	_timeCounter = 0.0f;
}

void SordShock::BreathStop() {
	if (_shockParticleEmitter) {
		_shockParticleEmitter->SetActive(false);
	}
	if (_tirotiroEmitter) {
		_tirotiroEmitter->SetActive(false);
	}

	BreathObject::BreathStop();
};

void SordShock::BreathPlay() {
	if (_shockParticleEmitter) {
		_shockParticleEmitter->SetActive(true);
	}
	if (_tirotiroEmitter) {
		_tirotiroEmitter->SetActive(true);
	}

	BreathObject::BreathPlay();
};
