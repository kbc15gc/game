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
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("Ground.X"));
	modeldata->CalcWidthAndHeight();
	//マテリアル取得
	Material* material = modeldata->FindMaterial("ground4_Diffuse.tga");
	//スプラットマップセット
	material->SetTexture(Material::TextureHandleE::SplatMap, TextureManager::LoadBaseTexture("Xfile/groundSplatmap.png"));		
	material->SetTexture(Material::TextureHandleE::TerrainTex0, TextureManager::LoadBaseTexture("Xfile/Grass.tga"));			//赤	草
	material->SetTexture(Material::TextureHandleE::TerrainTex1, TextureManager::LoadBaseTexture("Xfile/ground1_Diffuse.tga"));	//緑	地面
	material->SetTexture(Material::TextureHandleE::TerrainTex2, TextureManager::LoadBaseTexture("Xfile/Sand_Albedo.tga"));		//青	砂
	model->SetModelData(modeldata);
	model->terain = true;
	//model->SetModelEffect(ModelEffectE::SPECULAR, false);
	//model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	model->SetModelEffect(ModelEffectE::CAST_ENVIRONMENT,true);

	model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncNone);

	RigidBody* rigid = AddComponent<RigidBody>();
	MeshCollider* mesh = AddComponent<MeshCollider>();

	mesh->Create(model);
	rigid->Create(0, mesh, Collision_ID::GROUND);

	transform->SetLocalPosition(Vector3::zero);
	transform->SetLocalScale(Vector3::one);

}

void Ground::Start() {
	// とりあえずテスト。
	int attr = static_cast<int>(fbCollisionAttributeE::ALL) & ~(Collision_ID::PLAYER) & ~(Collision_ID::SPACE) & ~(Collision_ID::ATTACK) & ~(Collision_ID::GROUND);
	// 空間分割コリジョン生成。
	INSTANCE(GameObjectManager)->AddNew<SplitSpace>("SplitSpace", 10)->Split(GetComponent<SkinModel>()->GetModelData(), transform, 5, 1, 1, attr);
}

void Ground::Update()
{

}