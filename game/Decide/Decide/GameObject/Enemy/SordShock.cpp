#include "stdafx.h"
#include "GameObject\Enemy\SordShock.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "GameObject\ParticleParamTable.h"

void SordShock::Awake() {
	ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// �Ռ��g���ʂ�߂�����̂��낿��̃p�[�e�B�N���ݒ�B
	_initTirotiroParticleParam.Init();
	_initTirotiroParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::TiroTiroViolet];
	p->Init(_initTirotiroParticleParam);
	p->SetEmitFlg(false);
	_tirotiroEmitter = p;


	p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// �Ռ��g�̃p�[�e�B�N���ݒ�B
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
		// �Ռ��g��i�߂�B
		_shockParticleEmitter->transform->SetPosition(_shockParticleEmitter->transform->GetPosition() + _speed * Time::DeltaTime());
		Vector3 work = _shockParticleEmitter->transform->GetPosition() - _emitPos;	// �����ʒu���猻�݂̏Ռ��g�̈ʒu�܂ł̃x�N�g���B

		// ���낿��̒�����ύX�B
		_initTirotiroParticleParam.initPositionRandomMargin.z = work.Length() * 0.5f;
		_attack[1]->ReSize(Vector3(0.5f, 0.5f, _initTirotiroParticleParam.initPositionRandomMargin.z * 2.0f));

		_tirotiroEmitter->SetParam(_initTirotiroParticleParam);
		// ������ύX�����̂ňʒu�����炷�B
		Vector3 lpos = _tirotiroEmitter->transform->GetLocalPosition();
		_tirotiroEmitter->transform->SetLocalPosition(lpos.x, lpos.y,(work.Length() * 0.5f) * -1.0f);

		if (work.Length() >= _range) {
			// �Ռ��g���򋗗��܂Ŕ�񂾁B

			_isShot = false;
			_isAutoRange = true;
			_initShockParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::TiroTiroTowerViolet];
			_shockParticleEmitter->ResetParameterAlreadyCreated(_initShockParticleParam);
			// ���̒��̃R���W�����쐬�B
			INSTANCE(GameObjectManager)->AddRemoveList(_attack[0]);
			_attack[0] = _enemyObject->CreateAttack(Vector3(0.0f, 2.0f, 0.0f), Quaternion::Identity, Vector3(1.0f, 4.0f, 1.0f), -1.0f, _shockParticleEmitter->transform, false, true, AttackCollision::ReactionType::NotAction);
		}
		else if (_attack[0]->GetIsHit()) {
			// �����ƏՓˁB

			_isShot = false;
			_isBomb = true;
			_initShockParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::BombViolet];
			_shockParticleEmitter->SetParam(_initShockParticleParam);
			INSTANCE(GameObjectManager)->AddRemoveList(_attack[0]);

			//�U���R���W�����쐬�B
			_enemyObject->CreateAttack(Vector3(0.0f, 1.0f, 0.0f), Quaternion::Identity, Vector3(2.0f, 2.0f, 2.0f), 0.2f, _shockParticleEmitter->transform, false, false, AttackCollision::ReactionType::Blown)->RemoveParent();
		}
	}
	else {
		if (_isBomb && _timeCounter >= 0.2f) {
			// �����I���B
			_isBomb = false;
			_initShockParticleParam = ParticleParamTable::Params[ParticleParamTable::Type::TiroTiroTowerViolet];
			_shockParticleEmitter->SetParam(_initShockParticleParam);

			// ���̒��̃R���W�����쐬�B
			//INSTANCE(GameObjectManager)->AddRemoveList(_attack[0]);
			_attack[0] = _enemyObject->CreateAttack(Vector3(0.0f, 2.0f, 0.0f), Quaternion::Identity, Vector3(1.0f, 4.0f, 1.0f), -1.0f, _shockParticleEmitter->transform, false, true, AttackCollision::ReactionType::NotAction,10);
		}

		if (_timeCounter >= (_interval + 10.0f)) {
			// ���̒������������^�C�~���O�ō폜�B
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
		if (_tirotiroEmitter && (_timeCounter >= _interval)) {
			// ���낿��͒�����ɏ����B
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

	//�Ռ��g�̍U���R���W�����쐬�B
	AttackCollision* attack = _enemyObject->CreateAttack(Vector3(0.0f, 0.5f, 0.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 1.0f), -1.0f, _shockParticleEmitter->transform);
	_attack.push_back(attack);

	//���낿��̍U���R���W�����쐬�B
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
