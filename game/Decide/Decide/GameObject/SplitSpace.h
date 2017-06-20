#pragma once

#include"fbEngine\fbstdafx.h"

class CollisionObject;

// 空間を表すバウンディングボックスを生成するクラス。
class SplitSpace :public GameObject{
public:
	SplitSpace(const char* name):GameObject(name) {};
	~SplitSpace() {};
	// 空間分割関数。
	// 引数：	モデルデータ(このモデルを内包できるサイズのボックスを定義し、分割する)。
	//			Transform情報(この情報をもとに生成したボックスを移動、回転、拡縮する。)。
	//			分割数(横)。
	//			分割数(縦)。
	//			分割数(奥行)。
	void Split(const SkinModelData* data, const Transform& transform,int x, int y, int z);
private:
	// 最大空間を表すボックスを定義する関数。
	// 引数：	モデルデータ(このモデルを内包できるサイズのボックスを定義する)。
	//			定義したボックスのサイズ(戻り値の値と同じ)。
	// 戻り値：	定義したボックスのサイズ。
	const Vector3& CreateSpaceBox(const SkinModelData& data,Vector3& size);
	
	// 分割された空間を表すボックスコリジョンを生成する関数。
	// 引数：	分割前のボックスサイズ。
	//			分割前のボックスのTransform情報(この情報をもとに生成したボックスを移動、回転、拡縮する。)。
	//			分割数(横)。
	//			分割数(縦)。
	//			分割数(奥行)。
	void CreateSplitBox(const Vector3& size, const Transform& transform,int x,int y,int z);

private:
	Vector3 _unSplitSpaceSize;	// 分割前の空間サイズ。
	Vector3 _splitSpaceSize;	// 分割後のボックス一つ当たりの空間サイズ。
	vector<CollisionObject*> _SpaceCollisions;	// 分割したそれぞれの空間コリジョン。
};