#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

	//�I�u�W�F�N�g�̏ڍ�
struct MeshCollisionInfo {
	const char* modelName;
	Vector3 pos;
	Quaternion angle;
	Vector3 scale;
};

class MeshObjectChipManager : public GameObject
{
public:
	MeshObjectChipManager(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
private:
	
};
