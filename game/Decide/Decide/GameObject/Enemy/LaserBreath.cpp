#include "stdafx.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyCharacter.h"

void LaserBreath::Awake() {
	ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);

	// �U�������Ɏg�p����p�[�e�B�N���ݒ�B
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
	// �p�[�e�B�N���̔�ԕ������G�l�~�[�̌����ɍĐݒ�B
	Vector3 initVelocity = _enemyObject->transform->GetForward();
	initVelocity.Normalize();
	_initParticleParam.initVelocity = initVelocity * _particleEmitter->GetInitVelocity().Length();
	_particleEmitter->SetParam(_initParticleParam);

	// �쐬�����p�[�e�B�N�������W�B
	_particleList.reset(new vector<Particle*>);
	_particleEmitter->AchievedCreateParticleStart(_particleList.get());
	_particleEmitter->SetEmitFlg(true);

	//�U���R���W�����쐬�B
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
				// �p�[�e�B�N������������Ă���B

				start = *(list->begin());	// �ŏ��ɐ������ꂽ�p�[�e�B�N�����擾�B
				Particle* end = *(--list->end());	// �Ō�ɐ������ꂽ�p�[�e�B�N�����擾�B
				if (start != end) {
					// �p�[�e�B�N���̐擪�ƏI�[������̃p�[�e�B�N���ł͂Ȃ��B

					Vector3 breathEndPos = start->transform->GetPosition();	// �u���X�̏I�[�ʒu�͍ŏ��ɐ������ꂽ�p�[�e�B�N���̈ʒu�B
					Vector3 breathStartPos = end->transform->GetPosition(); // �u���X�̊J�n�ʒu�͍Ō�ɐ������ꂽ�p�[�e�B�N���̈ʒu�B

					// �u���X�J�n�ʒu����I�[�ʒu�܂ł̋������R���W�����̉��s�T�C�Y�Ƃ���B
					float sizeZ = Vector3(breathEndPos - breathStartPos).Length();

					// ����R���W�����̃T�C�Y��ύX�B
					if (start != _start || end != _end) {
						// �u���X�̐擪���I�[���ύX���Ă���B

						// ���O�̒l��j�����A���݂̒l��V�����ۑ��B
						_start = start;
						_end = end;
					}

					float sizeXYOrigin = 4.0f;
					static_cast<BoxCollider*>(const_cast<Collider*>(Gost->GetShape()))->Resize(Vector3(sizeXYOrigin * start->transform->GetScale().y, sizeXYOrigin * start->transform->GetScale().y, sizeZ));
					//const_cast<Collider*>(Gost->GetShape())->RenderDisable();

					// �ʒu�ݒ�B
					Gost->transform->SetPosition(breathEndPos);
					// �ʒu���T�C�Y�̔����������炷���ƂŃR���W�����̒��S���w�肷��B
					Vector3 pos = Gost->transform->GetLocalPosition();
					Vector3 dir = start->GetVelocity();
					dir.Normalize();
					pos -= dir * sizeZ * 0.5f;
					Gost->transform->SetLocalPosition(pos);
				}

				if (start->transform->GetLocalScale().y <= 0.0f) {
					// ���[�U�[�����ł��Ă���B

					INSTANCE(GameObjectManager)->AddRemoveList(this);
				}


			}
		}
	}
}
