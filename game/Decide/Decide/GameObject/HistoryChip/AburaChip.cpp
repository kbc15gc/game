#include"stdafx.h"
#include "AburaChip.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\Player\Player.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\HistoryMenuSelect.h"


AburaChip::AburaChip(const char * name) :
	GameObject(name)
{

}

void AburaChip::Awake()
{
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("AburaChip.X"));
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);

	transform->SetLocalPosition(Vector3(10.0f, 6.0f, 0.0f));
	transform->SetLocalScale(Vector3::one);

	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE",0);
	_SE->Init("Asset/Sound/coin.wav");
}

void AburaChip::Start()
{
	//プレイヤーを検索
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	//ヒストリーメニューセレクト検索
	_HistoryMenuSelect = (HistoryMenuSelect*)INSTANCE(GameObjectManager)->FindObject("HistoryMenuSelect");
}

void AburaChip::Update()
{
	static Vector3 angle(0.0f, 0.0f, 0.0f);
	angle.y += 2.0f;
	transform->SetLocalAngle(angle);

	//プレイヤーとの距離
	float toLenght = (transform->GetLocalPosition() - _Player->transform->GetLocalPosition()).Length();
	//一定の距離内だとオブジェクト削除
	if (toLenght <= 1.2f)
	{
		_SE->Play(false);

		INSTANCE(HistoryManager)->SetHistoryChip(0, 0, (int)ChipID::FIRE);
		_HistoryMenuSelect->AburaSelect();
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}