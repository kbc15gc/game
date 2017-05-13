#include"stdafx.h"
#include "FireChip.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\Player\Player.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\Village\HistoryInfo.h"

FireChip::FireChip(const char * name) :
	GameObject(name)
{

}

void FireChip::Awake()
{
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("FireChip.X"));
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	transform->SetLocalPosition(Vector3(30.0f, 6.0f, 0.0f));
	transform->SetLocalScale(Vector3::one);
}

void FireChip::Start()
{
	//プレイヤーを検索
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

void FireChip::Update()
{
	static Vector3 angle(0.0f, 0.0f, 0.0f);
	angle.y += 2.0f;
	transform->SetLocalAngle(angle);

	//プレイヤーとの距離
	float toLenght = fabs(transform->GetLocalPosition().Length() - _Player->transform->GetLocalPosition().Length());
	//一定の距離内だとオブジェクト削除
	if (toLenght <= 0.2f)
	{
		INSTANCE(HistoryManager)->SetHistoryChip(0, 0, (int)ChipID::FIRE);
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
	

}