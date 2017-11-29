#include"stdafx.h"
#include "MeshObjectChip.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\SplitSpace.h"

MeshObjectChip::MeshObjectChip(const char * name) :
	GameObject(name)
{

}

void MeshObjectChip::Awake()
{
}

void MeshObjectChip::Start() {

}

void MeshObjectChip::Update()
{
}

void MeshObjectChip::Init(const char* name, const Vector3& pos, const Vector3& sca, const Quaternion& q)
{
	
	transform->SetLocalPosition(pos);
	/*transform->SetLocalScale(sca);
	transform->SetLocalRotation(q);*/

	//�X�L�����f���쐬
	SkinModel* model = AddComponent<SkinModel>();
	//���f���f�[�^�쐬
	SkinModelData* modeldata = new SkinModelData();
	//���f���ݒ�
	modeldata->CloneModelData(SkinModelManager::LoadModel(name));
	model->SetModelData(modeldata);

	//model->SetModelEffect(ModelEffectE::FRUSTUM_CULLING, false);
	model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	//�����蔻��ǉ��B
	RigidBody* rigid = AddComponent<RigidBody>();
	MeshCollider* mesh = AddComponent<MeshCollider>();

	//���b�V���R���C�_�[�����B
	mesh->Create(model);
	RigidBodyInfo info;
	info.mass = 0.0f;
	info.coll = mesh;
	info.id = Collision_ID::BUILDING;
	rigid->Create(info, false);
}