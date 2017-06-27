#pragma once
#include "fbEngine\_Object\_Component\Component.h"
//#include "fbEngine\_Object\_GameObject\GameObjectManager.h"
//#include "fbEngine\_Object\_GameObject\GameObject.h"

// 登録したGameObjectを一旦非アクティブにし、指定したタイミングでアクティブ化させる。
class Spawner : public GameObject {
	Spawner(char* name) : GameObject(name) {};
	~Spawner() {};

	void Update()override;

	// 発生予告。
	// 引数：	復活させるオブジェクト。
	//			何秒後に復活するか。
	//			復活後の位置(ワールド座標)。
	//			復活後の回転(ワールド座標)。
	//			復活後の拡縮(ワールド座標)。
	//			復活後に設定する親(デフォルトはNull)。
	void SpawnStandby(GameObject* spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent);
private:
	GameObject* _spawnObject = nullptr;	// 発生させるオブジェクト。
	float _waitTime = 0.0f;	// 待ち時間。
	float _timeCounter = 0.0f;	// カウンター。
};

// このコンポーネントをアタッチすると指定した位置に指定したタイミングでゲームオブジェクトを発生させる。
class ObjectSpawn :public Component {
public:
	ObjectSpawn(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~ObjectSpawn() {};
	// アタッチされているオブジェクトを死亡させた後、復活させる関数。
	// 引数：	何秒後に復活するか。
	//			復活後の位置(ワールド座標)。
	//			復活後の回転(ワールド座標)。
	//			復活後の拡縮(ワールド座標)。
	//			復活後に設定する親(デフォルトはNull)。
	// ※オブジェクトマネージャーに登録されているもののみ有効。
	template<class T>
	void ObjectSpawn::DeathObject(float wait,const Vector3& pos,const Quaternion& rot,const Vector3& scale,Transform* parent = nullptr) {
		if (_spawner == nullptr) {
			INSTANCE(GameObjectManager)->AddRemoveList(gameObject);
			_spawner = INSTANCE(GameObjectManager)->AddNew<Spawner>("Spawner", gameObject->_Pri);
			_spawner->SpawnStandby(INSTANCE(GameObjectManager)->AddNew<T*>(gameObject->_Name, gameObject->_Pri), wait,pos,rot,scale, parent);
		}
	}
private:
	Spawner* _spawner = nullptr;	// 登録したオブジェクトを発生させるオブジェクト。
};

