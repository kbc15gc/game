#include"stdafx.h"
#include "Chip.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\Player\Player.h"
#include "HistoryManager.h"

namespace
{
	//�e�R�C���̃|�W�V����
	Vector3 pos[] =
	{
		Vector3(-481.0f, 66.0f, 266.0f),		//��
		Vector3(-453.0f, 58.0f, -295.0f),		//��
		Vector3(-1060.0f, 69.0f, -1950.0f),				//��
		Vector3(-118.829f,54.5f,443.0f),//!< ���̃`�b�v.
		Vector3(-322.0f,68.8f,621.87f),//!< �_�Ƃ̃`�b�v
		Vector3(275.0f,54.0f,621.0f),//!< ���̃`�b�v.
		Vector3(232.431f,66.0f,-43.731f),//!< �S�̃`�b�v.
		Vector3(410.5f,92.9f,217.27f),//!< �Ζ��̃`�b�v.
		Vector3(484.71f, 72.0f, 90.86f),//!< ��̃`�b�v.
		//!< �`�b�v�̐�.
	};

	//�����蔻��̋����B
	const float atari = 1.5f;
	//��]�̃X�s�[�h
	const float rotation_speed = 1.0f;
}

/**
* �R���X�g���N�^��̏�����.
*/
void Chip::Awake()
{
	//�`�b�v���擾����SE��������
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/coin.wav");
	//�v���C���[������
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

/**
* ������.
*/
void Chip::Start()
{
	
}

/**
* �X�V.
*/
void Chip::Update()
{
	static Vector3 rotation = Vector3::zero;
	rotation.y += rotation_speed;
	transform->SetLocalAngle(rotation);

	//�v���C���[�Ƃ̋���
	float toLenght = (transform->GetLocalPosition() - _Player->transform->GetLocalPosition()).Length();
	_GetTimer += Time::DeltaTime();
	//���̋��������ƃI�u�W�F�N�g�폜
	if (toLenght <= atari && _GetTime <= _GetTimer && !INSTANCE(HistoryManager)->IsSetChip(_ChipID))
	{
		//�`�b�v�擾SE
		_SE->Play(false);

		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(_ChipID);

		//�擾�������������Ȃ��̂ō폜
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}

void Chip::Render()
{
	TEXTURE* texture = LOADTEXTURE((char*)ChipFileName[(int)_ChipID].c_str());
	_Material->SetTexture(Material::TextureHandleE::DiffuseMap, texture->pTexture);
}

/**
* �`�b�vID��ݒ�.
*/
void Chip::SetChipID(ChipID chipID)
{ 
	//�O������Z�b�g����ID��ݒ�
	_ChipID = chipID;

	//�ݒ肳�ꂽID�̃��f�������[�h�B
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
	//�ݒ肳�ꂽID�̃��f���̈ʒu�Ƒ傫����ݒ�B
	transform->SetLocalPosition(pos[(int)_ChipID]);
	transform->SetLocalScale(Vector3::one * 5.0f);

	_FitGround();
}

void Chip::SetDropChipID(ChipID chipID,const Vector3& pos)
{
	//�O������Z�b�g����ID��ݒ�B
	_ChipID = chipID;

	//�ݒ肳�ꂽID�̃��f�������[�h�B
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
	//�ݒ肳�ꂽID�̃��f���̈ʒu�Ƒ傫����ݒ�B
	transform->SetLocalPosition(pos);
	transform->SetLocalScale(Vector3::one * 5.0f);

	_FitGround();
}

/**
* ������n�ʂɍ��킹��.
*/
void Chip::_FitGround()
{
	//���C���쐬����.
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	//�J�n�ʒu�Ƒ����̍���.
	float startOffset = 2;

	Vector3 pos = transform->GetPosition();
	Quaternion rot = transform->GetRotation();
	//�J�n�n�_��ݒ�.
	start.setOrigin(btVector3(pos.x, pos.y + startOffset, pos.z));

	//�I���n�_��ݒ�.
	//2���[�g����������.
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
