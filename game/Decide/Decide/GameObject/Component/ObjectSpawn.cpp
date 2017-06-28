#include "stdafx.h"
#include "GameObject\Component\ObjectSpawn.h"
#include "fbEngine\_Support\Time.h"

void Spawner::Update() {
	if (_spawnObject) {
		if (_timeCounter >= _waitTime) {
			// 指定した時間が経過した。
			_spawnObject->SetActive(true);	// アクティブ化する。
			_spawnObject->Start();	// 初期化。

			// 復活させたので自分を削除する。
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
		_timeCounter += Time::DeltaTime();
	}
}

void Spawner::SpawnStandby(GameObject* spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent) {
	spawn->SetActive(false);	// まずは非アクティブ化する。
	_spawnObject = spawn;

	// 位置情報設定。
	_spawnObject->transform->SetPosition(pos);
	_spawnObject->transform->SetRotation(rot);
	_spawnObject->transform->SetScale(scale);
	_spawnObject->transform->SetParent(parent);

	_waitTime = wait;
	_timeCounter = 0.0f;
}
