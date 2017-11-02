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
	//transform->SetLocalScale(sca);
	//transform->SetLocalRotation(q);

	//�X�L�����f���쐬
	SkinModel* model = AddComponent<SkinModel>();
	//���f���f�[�^�쐬
	SkinModelData* modeldata = new SkinModelData();
	//���f���ݒ�
	modeldata->CloneModelData(SkinModelManager::LoadModel(name));
	model->SetModelData(modeldata);

	model->SetModelEffect(ModelEffectE::FRUSTUM_CULLING, false);
	model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	RigidBody* rigid = AddComponent<RigidBody>();
	MeshCollider* mesh = AddComponent<MeshCollider>();

	mesh->Create(model);
	rigid->Create(0, mesh, Collision_ID::GROUND);
}