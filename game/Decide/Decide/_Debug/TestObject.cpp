/**
* テストオブジェクトクラスの実装.
*/
#include"stdafx.h"
#include"TestObject.h"

/**
* 初期化.
*/
void TestObject::Awake()
{
	SkinModel* model = AddComponent<SkinModel>();

	//スキンモデル作成
	SkinModelData* modeldata = new SkinModelData();
	//モデルデータ作成
	modeldata->CloneModelData(SkinModelManager::LoadModel("maru.X"));
	//モデル設定
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::SPECULAR, true);

	transform->SetLocalPosition(Vector3(-202.0f, 60.0f, -156.0f));
}
