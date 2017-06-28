#include "stdafx.h"
#include "GameObject\Component\ObjectSpawn.h"
#include "fbEngine\_Support\Time.h"

void Spawner::Update() {
	if (_spawnObject) {
		if (_timeCounter >= _waitTime) {
			// �w�肵�����Ԃ��o�߂����B
			_spawnObject->SetActive(true);	// �A�N�e�B�u������B
			_spawnObject->Start();	// �������B

			// �����������̂Ŏ������폜����B
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
		_timeCounter += Time::DeltaTime();
	}
}

void Spawner::SpawnStandby(GameObject* spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent) {
	spawn->SetActive(false);	// �܂��͔�A�N�e�B�u������B
	_spawnObject = spawn;

	// �ʒu���ݒ�B
	_spawnObject->transform->SetPosition(pos);
	_spawnObject->transform->SetRotation(rot);
	_spawnObject->transform->SetScale(scale);
	_spawnObject->transform->SetParent(parent);

	_waitTime = wait;
	_timeCounter = 0.0f;
}
