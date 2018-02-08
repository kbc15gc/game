#include"stdafx.h"
#include "ContinentObject.h"
#include "GameObject\SplitSpace.h"
#include "NPC.h"

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
	if (strcmp(GetName(), "MaouSiro.X") == 0) {
		// 魔王城を空間分割。
		int attr = static_cast<int>(fbCollisionAttributeE::ALL) & ~(Collision_ID::PLAYER) & ~(Collision_ID::SPACE) & ~(Collision_ID::ATTACK) & ~(Collision_ID::GROUND);
		SplitSpace* split = static_cast<SplitSpace*>(INSTANCE(GameObjectManager)->FindObject("SplitSpace_MaouSiro"));
		split->Split(_Model->GetModelData(), transform, 1, 10, 1, attr);
	}

	// Transformが更新されるのでここで追加。
	unique_ptr<vector<RigidBody*>> rigidArray = GetComponents<RigidBody>();
	for(int idx = 0;idx < static_cast<int>(rigidArray->size());idx++)
	{
		(*rigidArray)[idx]->AddWorld();
		_isAddPhysicsWorld = true;
	}
}

void ContinentObject::LoadModel(const char * filename, bool coll)
{
	SkinModelData* data = new SkinModelData();
	data->CloneModelData(SkinModelManager::LoadModel(filename), _Anim);
	//data->SetInstancing(true);
	_Model->SetModelData(data);
	_Model->SetCullMode(D3DCULL::D3DCULL_CW);
	_Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);
	_Model->SetModelEffect(ModelEffectE::RECEIVE_SHADOW, true);
	_Model->SetModelEffect(ModelEffectE::RECEIVE_POINTLIGHT, true);


	if (string(filename) == "tree.X")
	{
		_Model->SetTree();
		_Model->SetFresnelParam(true, Vector4(1.0f, 1.0f, 1.0f, 3.0f));
	}
	else if (string(filename) == "fence_a.X")
	{
		_Model->SetCullMode(D3DCULL::D3DCULL_NONE);
	}


	if (!coll)
	{
		//当たり判定追加。
		RigidBody* rigid = AddComponent<RigidBody>();

		if (GetComponent<ObjectRotation>() != nullptr)
		{
			CCapsuleCollider* capsule = AddComponent<CCapsuleCollider>();
			capsule->Create(0.18f, 0.5f);
			RigidBodyInfo info;
			info.mass = 0.0f;
			info.coll = capsule;
			info.physicsType = RigidBody::PhysicsType::Kinematick;
			info.id = Collision_ID::NPC_Collision | Collision_ID::NOTHITCAMERA;
			info.rotation = transform->GetRotation();
			rigid->Create(info, false);
		}
		else
		{
			//メッシュコライダー生成。
			MeshCollider* mesh = AddComponent<MeshCollider>();
			mesh->Copy(*MeshColliderManager::CloneMeshCollider(filename));
			auto sca = transform->GetLocalScale();
			mesh->GetBody()->setLocalScaling(btVector3(sca.x, sca.y, sca.z));
			RigidBodyInfo info;
			info.mass = 0.0f;
			info.coll = mesh;
			info.physicsType = RigidBody::PhysicsType::Kinematick;
			//info.offset = _Model->GetModelData()->GetCenterPos();
			info.id = (int)(Collision_ID::BUILDING);
			info.rotation = transform->GetRotation();
			rigid->Create(info, false);
		}
	}

}