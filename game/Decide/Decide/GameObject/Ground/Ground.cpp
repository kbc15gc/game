#include"stdafx.h"
#include "Ground.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\SplitSpace.h"

Ground::Ground(const char * name) :
GameObject(name)
{

}

void Ground::Awake()
{
	_Model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("Ground.X"));
	//マテリアル取得
	Material* material = modeldata->FindMaterial("Ground.png");
	//スプラットマップセット
	material->SetTexture(Material::TextureHandleE::SplatMap, TextureManager::LoadBaseTexture("Xfile/GroundMap_uv.png"));		
	material->SetTexture(Material::TextureHandleE::TerrainTex0, TextureManager::LoadBaseTexture("Xfile/IWA.png"));				//赤	
	material->SetTexture(Material::TextureHandleE::TerrainTex1, TextureManager::LoadBaseTexture("Xfile/Ground.png"));			//緑	
	material->SetTexture(Material::TextureHandleE::TerrainTex2, TextureManager::LoadBaseTexture("Xfile/YUKI.jpg"));			//青	
	_Model->SetModelData(modeldata);
	_Model->terain = true;
	//_Model->SetModelEffect(ModelEffectE::SPECULAR, false);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);
	//_Model->SetModelEffect(ModelEffectE::RECEIVE_SHADOW, false);
	_Model->SetModelEffect(ModelEffectE::CAST_ENVIRONMENT,true);
	//カリング処理をしないようにする。
	_Model->SetModelEffect(ModelEffectE::FRUSTUM_CULLING, false);
	_Model->SetModelEffect(ModelEffectE::RECEIVE_POINTLIGHT, true);


	_Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	_Model->SetFogParam(FogFunc::FogFuncDist, 350.0f, 600.0f, Vector4(0, 0, 0, 1), true);

	RigidBody* rigid = AddComponent<RigidBody>();
	MeshCollider* mesh = AddComponent<MeshCollider>();

	mesh->Copy(*MeshColliderManager::CloneMeshCollider("Ground.X"));
	rigid->Create(0, mesh, Collision_ID::GROUND);
	transform->SetLocalPosition(Vector3::zero);
	transform->SetLocalScale(Vector3::one);

}
//#define OFF_SPLITSPACE
void Ground::Start() {
#ifdef OFF_SPLITSPACE
#else
	// 空間分割コリジョン生成。
	int attr = static_cast<int>(fbCollisionAttributeE::ALL) & ~(Collision_ID::PLAYER) & ~(Collision_ID::SPACE) & ~(Collision_ID::ATTACK) & ~(Collision_ID::GROUND);
	//static_cast<SplitSpace*>(INSTANCE(GameObjectManager)->FindObject("SplitSpace"))->Split(GetComponent<SkinModel>()->GetModelData(), transform, 7, 1, 7, attr, Vector3(0.0f, 100.0f, 0.0f));
	//static_cast<SplitSpace*>(INSTANCE(GameObjectManager)->FindObject("SplitSpace"))->Split(GetComponent<SkinModel>()->GetModelData(), transform, 5, 1, 5, attr, Vector3(0.0f, 100.0f, 0.0f));
	static_cast<SplitSpace*>(INSTANCE(GameObjectManager)->FindObject("SplitSpace"))->Split(GetComponent<SkinModel>()->GetModelData(), transform, 7, 7, 7, attr, Vector3(0.0f, 100.0f, 0.0f));
	//static_cast<SplitSpace*>(INSTANCE(GameObjectManager)->FindObject("SplitSpace"))->Split(GetComponent<SkinModel>()->GetModelData(), transform, 7, 20, 7, attr, Vector3(0.0f, 100.0f, 0.0f));
#endif
}

void Ground::Update()
{

}