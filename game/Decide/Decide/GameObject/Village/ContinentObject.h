#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

namespace
{
	//オブジェクトの情報
	struct ObjectInfo
	{
		char filename[256];	//モデルファイル名
		Vector3 pos;	//ポジション
		Quaternion ang;	//回転
		Vector3 sca;	//スケール
		int hitcamera;
		int coll;		//コリジョンがあるか
	};

	//メンバ変数の情報設定
	const Support::DATARECORD ObjectInfoData[] =
	{
		{ "filename",Support::DataTypeE::STRING, offsetof(struct ObjectInfo,filename),sizeof(char) * 256 },
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::QUATERNION, offsetof(struct ObjectInfo,ang),sizeof(Quaternion) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,sca),sizeof(Vector3) },
		{ "hitcamera",Support::DataTypeE::INT, offsetof(struct ObjectInfo,hitcamera),sizeof(int) },
		{ "coll",Support::DataTypeE::INT, offsetof(struct ObjectInfo,coll),sizeof(int) },

	};

	//コリジョンの情報
	struct CollisionInfo
	{
		Vector3 pos;	//ポジション
		Quaternion ang;	//回転
		Vector3 sca;	//スケール
		int camerahit;	//カメラと当たるかどうかのフラグ。
	};

	//メンバ変数の情報設定
	const Support::DATARECORD CollisionInfoData[] =
	{
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct CollisionInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::QUATERNION, offsetof(struct CollisionInfo,ang),sizeof(Quaternion) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct CollisionInfo,sca),sizeof(Vector3) },
		{ "camerahit",Support::DataTypeE::INT, offsetof(struct CollisionInfo,camerahit),sizeof(int) },
	};
}

//歴史によって出るオブジェクト
class ContinentObject:public GameObject
{
public:
	ContinentObject(const char* name);
	void Awake()override;
	void Start()override;
	//読み込むモデルのファイル名指定
	//コリジョンを自動で生成するか。
	void LoadModel(const char* filename, bool coll);

	inline bool GetIsAddPhysicsWorld(){
		return _isAddPhysicsWorld;
	}
protected:
	//モデル描画
	SkinModel* _Model;
	//アニメーション
	Animation* _Anim;

private:
	bool _isAddPhysicsWorld = false;
};