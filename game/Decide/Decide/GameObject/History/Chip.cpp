#include"stdafx.h"
#include "Chip.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\Player\Player.h"
#include "HistoryManager.h"

namespace
{
	//各コインのファイルネーム
	char* filename[] = {
		"FireChip.X",
		"TetuChip.X",
		"AburaChip.X"
	};

	//各コインのポジション
	Vector3 pos[] =
	{
		Vector3(75, 69, -1387),
		Vector3(65, 69, -1387),
		Vector3(70, 69, -1387),
	};

	//あたり判定の距離。
	const float atari = 1.5f;
	//回転のスピード
	const float rotation_speed = 2.0f;

}

/**
* コンストラクタ後の初期化.
*/
void Chip::Awake()
{
	//チップを取得したSEを初期化
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/coin.wav");
	//プレイヤーを検索
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

/**
* 初期化.
*/
void Chip::Start()
{
	
}

/**
* 更新.
*/
void Chip::Update()
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

		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(_ChipID);

		//取得したらもういらないので削除
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}

/**
* チップIDを設定.
*/
void Chip::SetChipID(ChipID chipID)
{ 
	//外部からセットしたIDを設定。
	_ChipID = chipID;

	//設定されたIDのモデルをロード。
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel(filename[(int)_ChipID]));
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);
	model->SetModelEffect(ModelEffectE::SPECULAR, true);
	model->SetAllBlend(Color::white * 13);
	//設定されたIDのモデルの位置と大きさを設定。
	transform->SetLocalPosition(pos[(int)_ChipID]);
	transform->SetLocalScale(Vector3::one);
}

void Chip::SetDropChipID(ChipID chipID,const Vector3& pos)
{
	//外部からセットしたIDを設定。
	_ChipID = chipID;

	//設定されたIDのモデルをロード。
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel(filename[(int)_ChipID]));
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);
	model->SetModelEffect(ModelEffectE::SPECULAR, true);
	model->SetAllBlend(Color::white * 13);
	//設定されたIDのモデルの位置と大きさを設定。
	transform->SetLocalPosition(pos);
	transform->SetLocalScale(Vector3::one);
}
