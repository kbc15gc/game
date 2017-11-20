#include"stdafx.h"
#include "Chip.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\Player\Player.h"
#include "HistoryManager.h"

namespace
{
	//各コインのポジション
	Vector3 pos[] =
	{
		Vector3(-481.0f, 66.0f, 266.0f),		//火
		Vector3(-453.0f, 58.0f, -295.0f),		//木
		Vector3(-1060.0f, 69.0f, -1950.0f),				//石
		Vector3(-125.0f,54.2f,443.0f),//!< 狩りのチップ.
		Vector3(-351.0f,65.0f,649.0f),//!< 農業のチップ
		Vector3(275.0f,54.0f,621.0f),//!< 銅のチップ.
		Vector3(249.36f,65.0f,121.88f),//!< 鉄のチップ.
		Vector3(410.43f,90.0f,214.52),//!< 石油のチップ.
		//!< 薬のチップ.
		//!< チップの数.
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

void Chip::Render()
{
	TEXTURE* texture = LOADTEXTURE((char*)ChipFileName[(int)_ChipID].c_str());
	_Material->SetTexture(Material::TextureHandleE::DiffuseMap, texture->pTexture);
}

/**
* チップIDを設定.
*/
void Chip::SetChipID(ChipID chipID)
{ 
	//外部からセットしたIDを設定。
	_ChipID = chipID;

	//設定されたIDのモデルをロード。
	_Model = AddComponent<SkinModel>();
	SkinModelData* modelData = new SkinModelData();
	modelData->CloneModelData(SkinModelManager::LoadModel("Page.x"));
	_Material = modelData->FindMaterial("HuntingPage.png");
	_Model->SetModelData(modelData);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);
	//model->SetModelEffect(ModelEffectE::SPECULAR, true);
	//model->SetAllBlend(Color::white * 13);
	//設定されたIDのモデルの位置と大きさを設定。
	transform->SetLocalPosition(pos[(int)_ChipID]);
	transform->SetLocalScale(Vector3::one);

}

void Chip::SetDropChipID(ChipID chipID,const Vector3& pos)

{
	//外部からセットしたIDを設定。
	_ChipID = chipID;

	//設定されたIDのモデルをロード。
	_Model = AddComponent<SkinModel>();
	SkinModelData* modelData = new SkinModelData();
	modelData->CloneModelData(SkinModelManager::LoadModel("Page.x"));
	_Material = modelData->FindMaterial("HuntingPage.png");
	_Model->SetModelData(modelData);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);
	//model->SetModelEffect(ModelEffectE::SPECULAR, true);
	//model->SetAllBlend(Color::white * 13);
	//設定されたIDのモデルの位置と大きさを設定。
	transform->SetLocalPosition(pos);
	transform->SetLocalScale(Vector3::one);
}
