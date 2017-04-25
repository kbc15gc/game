#pragma once
#include "GameObject.h"

//3DModel��\������Q�[���I�u�W�F�N�g
class ModelObject :public GameObject
{
public:
	ModelObject(char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
	void LoadModel(char* path);
	SkinModel* GetSkinModel()
	{
		return _Model;
	}
private:
	SkinModel* _Model;
	Animation* _Anim;
};
