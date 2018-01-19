#pragma once
#include "fbEngine\_Object\_GameObject\CollisionObject.h"
#include "SupportData.h"

class Player;
class SplitSpace;

class SpaceCollisionObject :public CollisionObject {
public:
	SpaceCollisionObject(char* name) :CollisionObject(name) {};
	~SpaceCollisionObject(){
		_HitCollisions.clear();
	};

	// 空間コリジョン生成関数。
	// 引数：	位置(ローカル座標)。
	//			回転(ローカル座標)。
	//			コリジョンサイズ。
	//			コリジョン属性。
	//			親のTransform情報。
	//			衝突判定したいコリジョン属性。
	//			空間配列での添え字。
	void Create(const Vector3& pos, const Quaternion& rot, const Vector3& size, int id, Transform* parent, int attr,const Int3& myNumber);
	void Start()override;

	// 空間の状態を更新する。
	void UpdateActiveSpace();

	// この空間コリジョンがプレイヤーと衝突しているか判定する
	bool isHitPlayer();

	// 周囲の空間がプレイヤーと衝突しているか判定する。
	SpaceCollisionObject* IsHitPlayerAdjacent();

	// 衝突しているオブジェクトをすべて検出。 
	void RegistrationObject();

	// 衝突していれば衝突オブジェクトに追加登録。
	// 引数：	衝突判定をしたいゲームオブジェクト。
	void AddObjectHitSpace(GameObject& object);

	// この空間と隣接空間をアクティブ化。
	inline void EhableThisAndAdjacent() {
		EnableObjects();
		EnableObjectsAdjacent();
	}

	// この空間とこの空間に隣接する空間の中で、渡された空間オブジェクトに隣接していない空間を非アクティブ化。
	// 引数：	新しい中心空間(Nullを渡すとすべて非アクティブ化する)。
	// ※のみ探索される。
	void DisableNotAdjacent(const SpaceCollisionObject* Obj);

	// この空間に属するオブジェクトを非アクティブ化。
	inline void DisableObjects() {
		if (_isActive) {
			// アクティブ化されている。
			_SetActives(false);
		}
	}

	// この空間に属するオブジェクトをアクティブ化。
	inline void EnableObjects() {
		if (!_isActive) {
			// まだアクティブ化されてない。
			_SetActives(true);
		}
	}


	void SetSplitSpace(SplitSpace* split) {
		_splitSpace = split;
	}
	
	// 空間オブジェクトに属するオブジェクト群をアクティブ化しているかのフラグを設定する関数。
	// ※基本的に使用しない。
	void SetIsActive(bool flg) {
		_isActive = flg;
	}
private:


	// 隣接する空間オブジェクトに属するオブジェクトをすべてアクティブ化。
	void EnableObjectsAdjacent();

	// この空間に属するオブジェクトにアクティブフラグ設定。
	void _SetActives(bool flg);

	// 渡された番号の空間が隣接しているか。
	inline bool IsAdjacent(const Int3& num) {
		return ((abs(static_cast<int>(_myNumber.x) - static_cast<int>(num.x)) <= 1) && (abs(static_cast<int>(_myNumber.y) - static_cast<int>(num.y)) <= 1) && (abs(static_cast<int>(_myNumber.z) - static_cast<int>(num.z)) <= 1));
	}

public:
	// プレイヤーと衝突しているか取得。
	inline bool GetisHitPlayer()const {
		return _isHitPlayer;
	}

	// 隣接する空間オブジェクトを追加。
	inline void AddAdjacentSpaceObject(SpaceCollisionObject* obj) {
		_adjacentSpaceObjects.push_back(obj);
	}

	// 配列での添え字を取得。
	inline const Int3& GetMyNumber()const {
		return _myNumber;
	}
private:
	vector<shared_ptr<btCollisionObject>> _HitCollisions;	// 衝突しているコリジョン。

	vector<SpaceCollisionObject*> _adjacentSpaceObjects;		// 隣接する空間オブジェクト。
	GameObject* _player = nullptr;
	bool _isHitPlayer;	// このコリジョンオブジェクトがプレイヤーと衝突しているか。
	int _attribute;	// この空間に登録するオブジェクトのコリジョン属性。
	bool _isActive = true;	// この空間に属するオブジェクトがアクティブ化しているか。
private:
	SplitSpace* _splitSpace = nullptr;	// この空間オブジェクトを持つ空間分割クラスのポインタ。
	Int3 _myNumber;	// 自分が分割された空間のどこに位置するか(処理を軽くするため)。	
};