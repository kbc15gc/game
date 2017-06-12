#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

namespace
{
	//オブジェクトの情報
	struct ObjectInfo
	{
		char filename[256];	//モデルファイル名
		Vector3 pos;	//ポジション
		Vector3 ang;	//回転
		Vector3 sca;	//スケール
	};

	//メンバ変数の情報設定
	const Support::DATARECORD ObjectInfoData[4] =
	{
		{ "filename",Support::DataTypeE::STRING, offsetof(struct ObjectInfo,filename),sizeof(char)*256 },
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,ang),sizeof(Vector3) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,sca),sizeof(Vector3) },
	};
}

//歴史によって出るオブジェクト
class ContinentObject:public GameObject
{
public:
	ContinentObject(const char* name);
	void Awake()override;
	//読み込むモデルのファイル名指定
	void LoadModel(const char* filename);
protected:
	SkinModel* _Model;
	Animation* _Anim;
};