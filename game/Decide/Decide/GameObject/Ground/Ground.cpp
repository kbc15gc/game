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
	//�}�e���A���擾
	Material* material = modeldata->FindMaterial("ground4_Diffuse.tga");
	//�X�v���b�g�}�b�v�Z�b�g
	material->SetTexture(Material::TextureHandleE::SplatMap, TextureManager::LoadBaseTexture("Xfile/groundSplatmap.png"));		
	material->SetTexture(Material::TextureHandleE::TerrainTex0, TextureManager::LoadBaseTexture("Xfile/Grass.tga"));			//��	��
	material->SetTexture(Material::TextureHandleE::TerrainTex1, TextureManager::LoadBaseTexture("Xfile/ground1_Diffuse.tga"));	//��	�n��
	material->SetTexture(Material::TextureHandleE::TerrainTex2, TextureManager::LoadBaseTexture("Xfile/Sand_Albedo.tga"));		//��	��
	model->SetModelData(modeldata);
	model->terain = true;
	//model->SetModelEffect(ModelEffectE::SPECULAR, false);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);
	//model->SetModelEffect(ModelEffectE::RECEIVE_SHADOW, false);
	model->SetModelEffect(ModelEffectE::CAST_ENVIRONMENT,true);
	//�J�����O���������Ȃ��悤�ɂ���B
	model->SetModelEffect(ModelEffectE::FRUSTUM_CULLING, false);

	model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	RigidBody* rigid = AddComponent<RigidBody>();
	MeshCollider* mesh = AddComponent<MeshCollider>();

	mesh->Create(model);
	rigid->Create(0, mesh, Collision_ID::GROUND);
	transform->SetLocalPosition(Vector3::zero);
	transform->SetLocalScale(Vector3::one);

}

void Ground::Start() {
#ifdef OFF_SPLITSPACE
#else
	// ��ԕ����R���W���������B
	int attr = static_cast<int>(fbCollisionAttributeE::ALL) & ~(Collision_ID::PLAYER) & ~(Collision_ID::SPACE) & ~(Collision_ID::ATTACK) & ~(Collision_ID::GROUND);
	static_cast<SplitSpace*>(INSTANCE(GameObjectManager)->FindObject("SplitSpace"))->Split(GetComponent<SkinModel>()->GetModelData(), transform, 5, 1, 5, attr, Vector3(0.0f, 100.0f, 0.0f));
#endif
}

void Ground::Update()
{

}