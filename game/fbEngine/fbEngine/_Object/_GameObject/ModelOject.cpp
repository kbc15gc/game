#include"fbstdafx.h"
#include "ModelOject.h"

void ModelObject::Awake()
{
	_Model = AddComponent<SkinModel>();
	_Anim = AddComponent<Animation>();
}

void ModelObject::Update()
{
}

void ModelObject::LoadModel(char * path)
{
	SkinModelData* modeldata = nullptr;
	modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel(path), _Anim);
	_Model->SetModelData(modeldata);
}
