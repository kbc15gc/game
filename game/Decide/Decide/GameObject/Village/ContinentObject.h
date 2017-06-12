#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

namespace
{
	//�I�u�W�F�N�g�̏��
	struct ObjectInfo
	{
		char filename[256];	//���f���t�@�C����
		Vector3 pos;	//�|�W�V����
		Vector3 ang;	//��]
		Vector3 sca;	//�X�P�[��
	};

	//�����o�ϐ��̏��ݒ�
	const Support::DATARECORD ObjectInfoData[4] =
	{
		{ "filename",Support::DataTypeE::STRING, offsetof(struct ObjectInfo,filename),sizeof(char)*256 },
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,ang),sizeof(Vector3) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,sca),sizeof(Vector3) },
	};
}

//���j�ɂ���ďo��I�u�W�F�N�g
class ContinentObject:public GameObject
{
public:
	ContinentObject(const char* name);
	void Awake()override;
	//�ǂݍ��ރ��f���̃t�@�C�����w��
	void LoadModel(const char* filename);
protected:
	SkinModel* _Model;
	Animation* _Anim;
};