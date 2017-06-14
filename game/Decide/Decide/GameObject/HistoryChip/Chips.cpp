#include"stdafx.h"
#include "Chips.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\Player\Player.h"
#include "GameObject\Village\HistoryManager.h"

#include "GameObject\Village\HistoryMenuSelect.h"


namespace
{
	//各コインのファイルネーム
	char* filename[] = {
		"",//0番目はないため空の情報を
		"FireChip.X",
		"TetuChip.X",
		"AburaChip.X"
	};

	//各コインのポジション
	Vector3 pos[] =
	{
		Vector3(0,0,0),
		Vector3(344, 69, -1255),
		Vector3(385, 69, -1275),
		Vector3(356, 69, -1245),
	};

	//あたり判定の距離。
	const float atari = 1.2f;
	//回転のスピード
	const float rotation_speed = 2.0f;
}

Chips::Chips(const char * name) :
	GameObject(name)
{

}

void Chips::Awake()
{
	//チップを取得したSEを初期化
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/coin.wav");
}

void Chips::Start()
{
	//プレイヤーを検索
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	//ヒストリーメニューセレクト検索
	_HistoryMenuSelect = (HistoryMenuSelect*)INSTANCE(GameObjectManager)->FindObject("HistoryMenuSelect");
}

void Chips::Update()
{
	static Vector3 rotation = Vector3::zero;
	rotation.y += rotation_speed;
	transform->SetLocalAngle(rotation);

	//プレイヤーとの距離
	float toLenght = (transform->GetLocalPosition() - _Player->transform->GetLocalPosition()).Length();
	//一定の距離内だとオブジェクト削除
	if (toLenght <= atari)
	{
		//チップ取得SE
		_SE->Play(false);

		//取得したチップをメニューにセット
		INSTANCE(HistoryManager)->SetHistoryChip(0, 0, (int)_ChipID);
		_HistoryMenuSelect->ChipSelect(_ChipID);
		//取得したらもういらないので削除
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}

void Chips::SetChipID(const ChipID & id)
{ 
	//外部からセットしたIDを設定。
	_ChipID = id;

	//設定されたIDのモデルをロード。
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel(filename[(int)_ChipID]));
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);

	//設定されたIDのモデルの位置と大きさを設定。
	transform->SetLocalPosition(pos[(int)_ChipID]);
	transform->SetLocalScale(Vector3::one);
}
