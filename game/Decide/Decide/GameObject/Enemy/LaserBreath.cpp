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
					static_cast<BoxCollider*>(const_cast<Collider*>(Gost->GetShape()))->Resize(Vector3(sizeXYOrigin * start->transform->GetLocalScale().y, sizeXYOrigin * start->transform->GetLocalScale().y, sizeZ));
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
