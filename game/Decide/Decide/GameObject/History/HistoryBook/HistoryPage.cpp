/**
* 歴史書のページクラスの実装.
*/
#include"stdafx.h"
#include"HistoryPage.h"

/**
* コンストラクタ後の初期化.
*/
void HistoryPage::Awake()
{
	_Model = AddComponent<SkinModel>();

	SkinModelData* modelData = new SkinModelData();
	modelData->CloneModelData(SkinModelManager::LoadModel("Page.x"));

	_Model->SetModelData(modelData);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	//transform->SetLocalScale(Vector3(0.4f, 0.4f, 0.4f));

}

/**
* 初期化.
*/
void HistoryPage::Start()
{
}

/**
* 更新.
*/
void HistoryPage::Update()
{
}
