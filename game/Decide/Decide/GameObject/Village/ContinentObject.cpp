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

	_Model->SetModelEffect(ModelEffectE::CAST_ENVIRONMENT, false);

	_Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncNone);

	_Model->SetAlpha(true, 0.9f);

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
	SkinModelData* data = new SkinModelData();
	data->CloneModelData(SkinModelManager::LoadModel(filename), _Anim);
	//data->SetInstancing(true);
	_Model->SetModelData(data);
	_Model->SetCullMode(D3DCULL::D3DCULL_CW);

	if (string(filename) == "tree.X")
	{
		_Model->SetTree();
	}

	_Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	////�����蔻��ǉ��B
	//RigidBody* rigid = AddComponent<RigidBody>();
	//MeshCollider* mesh = AddComponent<MeshCollider>();

	////���b�V���R���C�_�[�����B
	//mesh->Create(_Model);
	//RigidBodyInfo info;
	//info.mass = 0.0f;
	//info.coll = mesh;
	//info.id = Collision_ID::BUILDING;
	//info.rotation = transform->GetRotation();
	//rigid->Create(info, false);


}
