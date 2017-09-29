#include"stdafx.h"
#include "ContinentObject.h"

ContinentObject::ContinentObject(const char * name):
	GameObject(name)
{
}

void ContinentObject::Awake()
{
	_Model = AddComponent<SkinModel>();
	_Anim = AddComponent<Animation>();

	_Model->SetModelEffect(ModelEffectE::CAST_ENVIRONMENT,false);

	_Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncNone);

}

void ContinentObject::Start() {

	// Transform���X�V�����̂ł����Œǉ��B
	unique_ptr<vector<RigidBody*>> rigidArray = GetComponents<RigidBody>();
	for (auto rigid : *rigidArray.get()) {
		rigid->AddWorld();
	}
}

void ContinentObject::LoadModel(const char * filename)
{
	SkinModelData* data= new SkinModelData();
	data->CloneModelData(SkinModelManager::LoadModel(filename), _Anim);
	_Model->SetModelData(data);
	_Model->SetCullMode(D3DCULL::D3DCULL_CW);

	//�����蔻��ǉ��B
	//RigidBody* rigid = AddComponent<RigidBody>();
	//MeshCollider* mesh = AddComponent<MeshCollider>();

	//���b�V���R���C�_�[�����B
	//mesh->Create(_Model);
	//rigid->Create(0, mesh, Collision_ID::BUILDING,Vector3::zero,Vector3::zero,false);
}
