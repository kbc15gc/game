#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class MeshObjectChip : public GameObject
{
public:
	MeshObjectChip(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;

	//���b�V���I�u�W�F�N�g�}�l�[�W���[�ŌĂ�ŏ������B
	//filename
	//���W
	//�g��
	//��]
	void Init(const char* name, const Vector3& pos, const Vector3& sca, const Quaternion& q);
};