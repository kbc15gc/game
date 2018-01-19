#pragma once

#include "fbEngine\fbstdafx.h"
#include "GameObject\Component\ObjectSpawn.h"
#include "SupportData.h"

class SpaceCollisionObject;

// 空間を表すバウンディングボックスを生成するクラス。
class SplitSpace :public GameObject{
public:
	SplitSpace(const char* name):GameObject(name) {
		Spawner::_splitSpace = this;
	};
	~SplitSpace() {
		_ReleaseSpaceCollisions();
		Spawner::_splitSpace = nullptr;
	};

	void OnDestroy()override;

	void Awake()override;
	void Update()override;
#ifdef _DEBUG
	void Debug()override;
#endif
	// 空間分割関数。
	// 引数：	モデルデータ(このモデルを内包できるサイズのボックスを定義し、分割する)。
	//			Transform情報(この情報をもとに生成したボックスを移動、回転、拡縮する。)。
	//			分割数(横)。
	//			分割数(縦)。
	//			分割数(奥行)。
	//			衝突判定を取りたい属性をレイヤーで指定(デフォルトは全属性)。
	void Split(const SkinModelData* data, Transform* transform,int x, int y, int z,int attr = _defaultAttr,const Vector3& offset = Vector3::zero);

	// 衝突した空間コリジョンの衝突オブジェクトに追加登録。
	// 引数：	衝突判定をしたいゲームオブジェクト。
	void AddObjectHitSpace(const GameObject& object);

	// 各空間コリジョンに衝突しているオブジェクトを登録する。
	void RegistrationObject();

	// 指定した添え字の空間を取得。
	SpaceCollisionObject* GetSpaceObject(Int3 num) {
		if (num.x < _splitX && num.y < _splitY && num.z < _splitZ && num.x >= 0 && num.y >= 0 && num.z >= 0) {
			return _SpaceCollisions[num.x][num.y][num.z];
		}
		else {
			return nullptr;
		}
	}
private:
	enum Space { Right = 0, Left, Up, Down, Front, Back, RightUp, RightDown,RightFront,RightBack, LeftUp, LeftDown,LeftFront,LeftBack, UpFront, UpBack, DownFront, DownBack, RightUpFront, RightDownFront, RightUpBack, RightDownBack, LeftUpFront, LeftDownFront,LeftUpBack, LeftDownBack, Max };

	// 最大空間を表すボックスを定義する関数。
	// 引数：	モデルデータ(このモデルを内包できるサイズのボックスを定義する)。
	//			Transform情報。
	//			定義したボックスのサイズ(戻り値の値と同じ)。
	// 戻り値：	定義したボックスのサイズ。
	const Vector3& CreateSpaceBox(const SkinModelData& data, const Transform& transform,Vector3& size);
	
	// 分割された空間を表すボックスコリジョンを生成する関数。
	// 引数：	分割前のボックスサイズ。
	//			分割前のボックスのTransform情報(この情報をもとに生成したボックスを移動、回転、拡縮する。)。
	//			分割数(横)。
	//			分割数(縦)。
	//			分割数(奥行)。
	//			衝突判定を取りたい属性をレイヤーで指定(デフォルトは全属性)。
	void CreateSplitBox(const Vector3& size, Transform* transform,int x,int y,int z,int attr,const Vector3& offset);

	// 隣接する空間オブジェクトを登録する。
	void _AdjacentSpace();

	// プレイヤーと衝突している空間コリジョンをすべての空間コリジョンから探索する。
	SpaceCollisionObject* IsHitPlayerToAll();

	// プレイヤーと衝突した空間をもとにアクティブ空間を変更。
	void ChangeActivateSpace(SpaceCollisionObject* Obj);

	void _ReleaseSpaceCollisions();
private:
	Vector3 _unSplitSpaceSize;	// 分割前の空間サイズ。
	Vector3 _splitSpaceSize;	// 分割後のボックス一つ当たりの空間サイズ。
	vector<vector<vector<SpaceCollisionObject*>>> _SpaceCollisions;	// 分割したそれぞれの空間コリジョン。
	Int3 _AdjacentIndex[Space::Max];	// 各添え字に加算する値。
	int _splitX = 1;	// 分割数(横)。
	int _splitY = 1;	// 分割数(縦)。
	int _splitZ = 1;	// 分割数(奥行)。
	static const int _defaultAttr;
	SpaceCollisionObject* _nowHitSpace = nullptr;	// 現在衝突している空間コリジョン。
};