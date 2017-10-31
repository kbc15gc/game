#include "stdafx.h"
#include "GameObject\Component\ObjectSpawn.h"
#include "fbEngine\_Support\Time.h"
#include "GameObject\SplitSpace.h"

SplitSpace* Spawner::_splitSpace = nullptr;

// 発生装置。
void Spawner::Create(GameObject* spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent) {
	spawn->SetActive(false);	// まずは非アクティブ化する。

	_spawnObject = spawn;

	// 位置情報設定。
	_spawnObject->transform->SetPosition(pos);
	_spawnObject->transform->SetRotation(rot);
	_spawnObject->transform->SetScale(scale);
	if (parent) {
		_spawnObject->transform->SetParent(parent);
	}

	_spawnObject->Start();	// 初期化。

	if (_splitSpace) {
		// 生成したオブジェクトを空間に登録。
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
			// 指定した時間が経過した。
 			_spawnObject->SetActive(true);	// アクティブ化する。

			// 発生させたので自分を削除する。
			INSTANCE(GameObjectManager)->AddRemoveList(this);
			return true;
		}
		_timeCounter += Time::DeltaTime();
	}

	return false;
}


// ObjectSpawnコンポーネント。
void ObjectSpawn::Update() {
	for (auto itr = _spawner.begin(); itr != _spawner.end();) {
		if ((*itr)->MyUpdate()) {
			// オブジェクトを生成した。

			if (*itr == _respawner) {
				// 生成した発生装置が復活装置だった。
				_respawner = nullptr;
			}
			// 発生装置リストから外す。
			itr = _spawner.erase(itr);
		}
		else {
			itr++;
		}
	}
}
