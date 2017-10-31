#include "stdafx.h"
#include "GameObject\Enemy\SordShock.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyCharacter.h"

void SordShock::Awake() {
	ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// �Ռ��g���ʂ�߂�����̂��낿��̃p�[�e�B�N���ݒ�B
	_initTirotiroParticleParam.Init();
	_initTirotiroParticleParam.texturePath = "MurasakiHonoo.png";
	_initTirotiroParticleParam.alphaBlendMode = 1;
	_initTirotiroParticleParam.addVelocityRandomMargih = Vector3(0.0f, 0.8f, 0.0f);
	_initTirotiroParticleParam.brightness = 1.1f;
	_initTirotiroParticleParam.fadeTime = 0.2f;
	_initTirotiroParticleParam.gravity = 0.0f;
	_initTirotiroParticleParam.initAlpha = 1.0f;
	_initTirotiroParticleParam.initPositionRandomMargin = Vector3(0.25f, 0.0f, 0.0f);
	_initTirotiroParticleParam.initVelocity = Vector3::front * 6.0f;
	_initTirotiroParticleParam.initVelocityVelocityRandomMargin = Vector3(0.0f, 4.0f, 0.0f);
	_initTirotiroParticleParam.intervalTime = 0.008f;
	_initTirotiroParticleParam.isBillboard = true;
	_initTirotiroParticleParam.isFade = true;
	_initTirotiroParticleParam.life = 0.1f;
	_initTirotiroParticleParam.size = Vector2(0.5f, 0.5f);
	_initTirotiroParticleParam.mulColor = Color(1.3f, 1.0f, 1.3f, 1.0f);
	_initTirotiroParticleParam.isParent = false;
	p->Init(_initTirotiroParticleParam);
	p->SetEmitFlg(false);
	_tirotiroEmitter = p;


	p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// �Ռ��g�̃p�[�e�B�N���ݒ�B
	_initShockParticleParam.Init();
	_initShockParticleParam.texturePath = "MurasakiHonoo.png";
	_initShockParticleParam.alphaBlendMode = 1;
	_initShockParticleParam.addVelocityRandomMargih = Vector3(0.0f, 0.8f, 0.0f);
	_initShockParticleParam.brightness = 1.1f;
	_initShockParticleParam.fadeTime = 0.2f;
	_initShockParticleParam.gravity = 0.0f;
	_initShockParticleParam.initAlpha = 1.0f;
	_initShockParticleParam.initPositionRandomMargin = Vector3(0.25f, 0.0f, 0.0f);
	_initShockParticleParam.initVelocity = Vector3::up * 1.0f;
	_initShockParticleParam.initVelocityVelocityRandomMargin = Vector3(0.0f, 4.0f, 0.0f);
	_initShockParticleParam.intervalTime = 0.008f;
	_initShockParticleParam.isBillboard = true;
	_initShockParticleParam.isFade = true;
	_initShockParticleParam.life = 0.1f;
	_initShockParticleParam.size = Vector2(0.5f, 0.5f);
	_initShockParticleParam.mulColor = Color(1.3f, 1.0f, 1.3f, 1.0f);
	_initShockParticleParam.isParent = true;
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
	if(_shockParticleEmitter->GetActive()){
		// �Ռ��g��i�߂�B
		_shockParticleEmitter->transform->SetPosition(_shockParticleEmitter->transform->GetPosition() + _speed * Time::DeltaTime());
		Vector3 work = Vector3(_shockParticleEmitter->transform->GetPosition() - _emitPos);	// �����ʒu���猻�݂̏Ռ��g�̈ʒu�܂ł̃x�N�g���B

		// ���낿��̒�����ύX�B
		Vector3 dir = _speed;
		dir.Normalize();
		_initTirotiroParticleParam.initPositionRandomMargin = dir * work.Length();
		_initTirotiroParticleParam.initPositionRandomMargin += _shockParticleEmitter->transform->GetRight() * 0.25f;
		//_initTirotiroParticleParam.initPositionRandomMargin += _shockParticleEmitter->transform->GetForward() * work.Length();

		//Quaternion rot;
		//rot.CreateVector3ToVector3(Vector3(0.0f, 0.0f, work.Length()), _shockParticleEmitter->transform->GetForward());
		//_initTirotiroParticleParam.initPositionRandomMargin = rot.RotationVector3(Vector3(0.25f, 0.0f, work.Length()));

		_tirotiroEmitter->SetParam(_initTirotiroParticleParam);
		// ������ύX�����̂ňʒu�����炷�B
		_tirotiroEmitter->transform->SetLocalPosition(0.0f,0.0f,(work.Length() * 0.5f) * -1.0f);

		if (work.Length() >= _range) {
			// �Ռ��g���򋗗��܂Ŕ�񂾁B

			_shockParticleEmitter->SetActive(false);
		}
	}

	if (_timeCounter >= _interval) {
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
	else {
		_timeCounter += Time::DeltaTime();
	}
}

void SordShock::_BreathStartSubClass() {
	_shockParticleEmitter->transform->SetParent(_enemyObject->transform);
	_shockParticleEmitter->transform->SetLocalPosition(_emitPos);
	_shockParticleEmitter->transform->SetParent(nullptr);
	_emitPos = _shockParticleEmitter->transform->GetPosition();

	_tirotiroEmitter->transform->SetParent(_shockParticleEmitter->transform);

	//// �p�����[�^�̃x���V�e�B���G�l�~�[�̐��ʂ��猩�������ɕϊ��B
	//Vector3 work = Vector3::zero;
	//work += (_enemyObject->transform->GetRight() * _initParticleParam.initVelocity.x);
	//work += (_enemyObject->transform->GetUp() * _initParticleParam.initVelocity.y);
	//work += (_enemyObject->transform->GetForward() * _initParticleParam.initVelocity.z);

	//_initParticleParam.initVelocity = work;

	//_shockParticleEmitter->SetParam(_initParticleParam);
	_shockParticleEmitter->SetEmitFlg(true);
	_tirotiroEmitter->SetEmitFlg(true);

	////�U���R���W�����쐬�B
	//AttackCollision* attack = _enemyObject->CreateAttack(Vector3(0.0f, 0.0f, -0.9f), Quaternion::Identity, Vector3(1.0f, 1.0f, 1.8f), -1.0f, _particleEmitter->transform, true);
	////attack->RemoveParent();
	//_attack.push_back(attack);

	_timeCounter = 0.0f;
}
