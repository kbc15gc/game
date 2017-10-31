#include "stdafx.h"
#include "GameObject\Component\ObjectSpawn.h"
#include "fbEngine\_Support\Time.h"
#include "GameObject\SplitSpace.h"

SplitSpace* Spawner::_splitSpace = nullptr;

// �������u�B
void Spawner::Create(GameObject* spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent) {
	spawn->SetActive(false);	// �܂��͔�A�N�e�B�u������B

	_spawnObject = spawn;

	// �ʒu���ݒ�B
	_spawnObject->transform->SetPosition(pos);
	_spawnObject->transform->SetRotation(rot);
	_spawnObject->transform->SetScale(scale);
	if (parent) {
		_spawnObject->transform->SetParent(parent);
	}

	_spawnObject->Start();	// �������B

	if (_splitSpace) {
		// ���������I�u�W�F�N�g����Ԃɓo�^�B
		_splitSpace->AddObjectHitSpace(*_spawnObject);
	}

	_waitTime = wait;
	_timeCounter = 0.0f;
}

bool Spawner::MyUpdate() {
	if (!_isRemoveComponent) {
		return _ClosedUpdate();
	}
	return false;
}

void Spawner::Update() {
	if (_isRemoveComponent) {
		_ClosedUpdate();
	}
}

bool Spawner::_ClosedUpdate() {
	if (_spawnObject) {
		if (_timeCounter >= _waitTime) {
			// �w�肵�����Ԃ��o�߂����B
 			_spawnObject->SetActive(true);	// �A�N�e�B�u������B

			// �����������̂Ŏ������폜����B
			INSTANCE(GameObjectManager)->AddRemoveList(this);
			return true;
		}
		_timeCounter += Time::DeltaTime();
	}

	return false;
}


// ObjectSpawn�R���|�[�l���g�B
void ObjectSpawn::Update() {
	for (auto itr = _spawner.begin(); itr != _spawner.end();) {
		if ((*itr)->MyUpdate()) {
			// �I�u�W�F�N�g�𐶐������B

			if (*itr == _respawner) {
				// ���������������u���������u�������B
				_respawner = nullptr;
			}
			// �������u���X�g����O���B
			itr = _spawner.erase(itr);
		}
		else {
			itr++;
		}
	}
}
