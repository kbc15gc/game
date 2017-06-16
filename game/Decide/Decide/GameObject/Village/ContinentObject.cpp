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

}

void ContinentObject::LoadModel(const char * filename)
{
	SkinModelData* data= new SkinModelData();
	data->CloneModelData(SkinModelManager::LoadModel(filename), _Anim);
	_Model->SetModelData(data);
}
