#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class SplitSpace;


// オブジェクト発生装置。
// 登録したGameObjectを一旦非アクティブにし、指定したタイミングでアクティブ化させる。
class Spawner : public GameObject {
public:
	Spawner(char* name) : GameObject(name) {
	};
	~Spawner() {};

	// 発生装置生成告。
	// 引数：	発生させるオブジェクト。
	//			何秒後に発生するか。
	//			発生後の位置(ワールド座標)。
	//			発生後の回転(ワールド座標)。
	//			発生後の拡縮(ワールド座標)。
	//			発生後に設定する親。
	// 戻り値： 発生予告したオブジェクト。
	void Create(GameObject* spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent);

	// スポナーを更新。
	// 戻り値：	発生登録したオブジェクトが生成されたか(trueで生成済み)。
	// ※発生装置が存在するか、復活装置が存在するかをコンポーネント側で判定したいため、この関数が必要。
	bool MyUpdate();

	// スポナーを更新。
	// ※ただしオブジェクトが破棄されるとコンポーネントは削除されるため、削除後の更新処理を委譲するためにこちらも必要。
	void Update()override;

	inline GameObject* GetSpawnObject()const {
		return _spawnObject;
	}

	inline void SetIsRemoveComponent(bool flg) {
		_isRemoveComponent = flg;
	}

private:
	bool _ClosedUpdate();
private:
	GameObject* _spawnObject = nullptr;	// 発生させるオブジェクト。
	float _waitTime = 0.0f;	// 待ち時間。
	float _timeCounter = 0.0f;	// カウンター。
	bool _isRemoveComponent = false;	// このクラスのインスタンスを保持しているコンポーネントが削除されたか。
public:
	static vector<SplitSpace*> _splitSpace;	// 空間分割(復活したエネミーを登録するために保持)。
};

// このコンポーネントをアタッチすると指定した位置に指定したタイミングでゲームオブジェクトを発生させる。
class ObjectSpawn :public Component {
public:
	ObjectSpawn(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~ObjectSpawn() {
		_respawner = nullptr;
		for (auto spawner : _spawner) {
			spawner->SetIsRemoveComponent(true);
		}
		_spawner.clear();
	};

	void Update()override;

	// アタッチされているオブジェクトを死亡させた後、復活させる関数。
	// テンプレート引数：	復活させたいオブジェクトのクラス名(基本的にアタッチされたゲームオブジェクトのクラス名)。
	// 引数：	復活したオブジェクトへのポインタが入る(Null以外の値の入ったポインタを入力すると上書きされ、もともと入っていたアドレス情報はロストするため注意、不要な場合は直接Nullを入力する)。
	//			何秒後に復活するか。
	//			復活後の位置(ワールド座標)。
	//			復活後の回転(ワールド座標)。
	//			復活後の拡縮(ワールド座標)。
	//			復活後に設定する親(デフォルトはNull)。
	// 戻り値：	復活したオブジェクトへのポインタ(第一引数と同じ)。
	// ※GameObjectManagerに登録されていなければ死亡(削除)しない。
	// ※復活したオブジェクトはGameObjectManagerに登録される。
	template<class T>
	T* ObjectSpawn::DeathAndRespawnObject(GameObject** reSpawn,float wait,const Vector3& pos,const Quaternion& rot,const Vector3& scale,Transform* parent = nullptr) {
		if (_respawner == nullptr) {
			// リスポーンするオブジェクトは一体なので登録されていないか判定する。

			// GameObjectを削除(死亡)。
			INSTANCE(GameObjectManager)->AddRemoveList(gameObject);
			// リスポナーに登録(復活予告)。
			_respawner = _CreateSpawner<T>(wait, pos, rot, scale, parent);
			GameObject* work = _respawner->GetSpawnObject();
			if (reSpawn) {
				*reSpawn = work;
			}
			return static_cast<T*>(work);
		}
	}

	// オブジェクトを生成予告する関数。
	// テンプレート引数：	発生させたいオブジェクトのクラス名。
	// 引数：	生成予告したオブジェクトへのポインタが入る(Null以外の値の入ったポインタを入力すると上書きされ、もともと入っていたアドレス情報はロストするため注意、不要な場合は直接Nullを入力する)。
	//			何秒後に生成するか。
	//			生成後の位置(ワールド座標)。
	//			生成後の回転(ワールド座標)。
	//			生成後の拡縮(ワールド座標)。
	//			生成後に設定する親(デフォルトはNull)。
	// 戻り値：	生成予告したオブジェクトへのポインタ(第一引数と同じ)。
	// ※生成予告したオブジェクトはGameObjectManagerに登録される。
	template<class T>
	T* ObjectSpawn::SpawnObject(GameObject** spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent = nullptr) {
		GameObject* work = _CreateSpawner<T>()->GetSpawnObject();
		if (spawn) {
			*spawn = work;
		}
		return static_cast<T*>(work);
	}

	Spawner* GetReSpawner() {
		return _respawner;
	}
private:
	// オブジェクト発生装置を生成する関数。
	// テンプレート引数：	発生させたいオブジェクトのクラス名。
	// 引数：	何秒後に発生させるか。
	//			発生後の位置(ワールド座標)。
	//			発生後の回転(ワールド座標)。
	//			発生後の拡縮(ワールド座標)。
	//			発生後に設定する親。
	// 戻り値：	生成した発生装置。
	// ※発生させたオブジェクトはGameObjectManagerに登録される。
	template<class T>
	Spawner* ObjectSpawn::_CreateSpawner(float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent) {
		Spawner* spawner = INSTANCE(GameObjectManager)->AddNew<Spawner>("Spawner", gameObject->GetPriorty());
		spawner->Create(INSTANCE(GameObjectManager)->AddNew<T>(const_cast<char*>(gameObject->GetName()), gameObject->GetPriorty()), wait, pos, rot, scale, parent);
		_spawner.push_back(spawner);	// スポーンリストに追加。
		return spawner;
	}

private:
	vector<Spawner*> _spawner;	// 発生装置リスト。
	Spawner* _respawner = nullptr;		// 復活装置はひとつ。
};

