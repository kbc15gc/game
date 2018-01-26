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
		Vector3(-118.829f,54.5f,443.0f),//!< 銅のチップ.
		Vector3(-322.0f,68.8f,621.87f),//!< 農業のチップ
		Vector3(275.0f,54.0f,621.0f),//!< 狩りのチップ.
		Vector3(232.431f,66.0f,-43.731f),//!< 鉄のチップ.
		Vector3(410.5f,92.9f,217.27f),//!< 石油のチップ.
		Vector3(484.71f, 72.0f, 90.86f),//!< 薬のチップ.
		//!< チップの数.
	};

	//あたり判定の距離。
	const float atari = 1.5f;
	//回転のスピード
	const float rotation_speed = 1.0f;
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
	_GetTimer += Time::DeltaTime();
	//一定の距離内だとオブジェクト削除
	if (toLenght <= atari && _GetTime <= _GetTimer && !INSTANCE(HistoryManager)->IsSetChip(_ChipID))
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
	//外部からセットしたIDを設定
	_ChipID = chipID;

	//設定されたIDのモデルをロード。
	_Model = AddComponent<SkinModel>();
	SkinModelData* modelData = new SkinModelData();
	modelData->CloneModelData(SkinModelManager::LoadModel("Page2.x"));
	_Material = modelData->FindMaterial("HuntingPage.png");
	_Model->SetModelData(modelData);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);

	_Model->SetFresnelParam(true, Vector4(5.0f, 5.0f, 5.0f, 3.0f));
	_Model->SetIsLuminance(true);
	
	//model->SetModelEffect(ModelEffectE::SPECULAR, true);
	//model->SetAllBlend(Color::white * 13);
	//設定されたIDのモデルの位置と大きさを設定。
	transform->SetLocalPosition(pos[(int)_ChipID]);
	transform->SetLocalScale(Vector3::one * 5.0f);

	_FitGround();
}

void Chip::SetDropChipID(ChipID chipID,const Vector3& pos)
{
	//外部からセットしたIDを設定。
	_ChipID = chipID;

	//設定されたIDのモデルをロード。
	_Model = AddComponent<SkinModel>();
	SkinModelData* modelData = new SkinModelData();
	modelData->CloneModelData(SkinModelManager::LoadModel("Page2.x"));
	_Material = modelData->FindMaterial("HuntingPage.png");
	_Model->SetModelData(modelData);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);

	_Model->SetFresnelParam(true, Vector4(5.0f, 5.0f, 5.0f, 3.0f));
	_Model->SetIsLuminance(true);

	//model->SetModelEffect(ModelEffectE::SPECULAR, true);
	//model->SetAllBlend(Color::white * 13);
	//設定されたIDのモデルの位置と大きさを設定。
	transform->SetLocalPosition(pos);
	transform->SetLocalScale(Vector3::one * 5.0f);

	_FitGround();
}

/**
* 足元を地面に合わせる.
*/
void Chip::_FitGround()
{
	//レイを作成する.
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	//開始位置と足元の差分.
	float startOffset = 2;

	Vector3 pos = transform->GetPosition();
	Quaternion rot = transform->GetRotation();
	//開始地点を設定.
	start.setOrigin(btVector3(pos.x, pos.y + startOffset, pos.z));

	//終了地点を設定.
	//2メートル下を見る.
	end.setOrigin(start.getOrigin() - btVector3(0, startOffset + 2, 0));

	BoxCollider bc(this, transform);
	bc.Create(Vector3(1, 1, 1));
	RigidBodyInfo info;
	info.mass = 0;
	info.coll = &bc;
	info.id = Collision_ID::DROPITEM;
	RigidBody rb(this, transform);
	rb.Create(info, false);

	fbPhysicsCallback::SweepResultGround callback;
	callback.me = this;
	callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
	callback._attribute = Collision_ID::GROUND | Collision_ID::BUILDING;

	INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)bc.GetBody(), start, end, callback);

	if (callback.isHit)
	{
		pos = callback.hitPos;
		pos.y += (_Model->GetModelData()->GetAABBSize().y) * transform->GetScale().y;
		transform->SetLocalPosition(pos);
	}
}
