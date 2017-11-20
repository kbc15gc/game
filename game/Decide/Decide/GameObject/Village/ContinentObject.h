#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

namespace
{
	//�I�u�W�F�N�g�̏��
	struct ObjectInfo
	{
		char filename[256];	//���f���t�@�C����
		Vector3 pos;	//�|�W�V����
		Quaternion ang;	//��]
		Vector3 sca;	//�X�P�[��
		int hitcamera;
		int coll;		//�R���W���������邩
	};

	//�����o�ϐ��̏��ݒ�
	const Support::DATARECORD ObjectInfoData[] =
	{
		{ "filename",Support::DataTypeE::STRING, offsetof(struct ObjectInfo,filename),sizeof(char) * 256 },
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::QUATERNION, offsetof(struct ObjectInfo,ang),sizeof(Quaternion) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct ObjectInfo,sca),sizeof(Vector3) },
		{ "hitcamera",Support::DataTypeE::INT, offsetof(struct ObjectInfo,hitcamera),sizeof(int) },
		{ "coll",Support::DataTypeE::INT, offsetof(struct ObjectInfo,coll),sizeof(int) },

	};

	//�R���W�����̏��
	struct CollisionInfo
	{
		Vector3 pos;	//�|�W�V����
		Quaternion ang;	//��]
		Vector3 sca;	//�X�P�[��
		int camerahit;	//�J�����Ɠ����邩�ǂ����̃t���O�B
	};

	//�����o�ϐ��̏��ݒ�
	const Support::DATARECORD CollisionInfoData[] =
	{
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct CollisionInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::QUATERNION, offsetof(struct CollisionInfo,ang),sizeof(Quaternion) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct CollisionInfo,sca),sizeof(Vector3) },
		{ "camerahit",Support::DataTypeE::INT, offsetof(struct CollisionInfo,camerahit),sizeof(int) },
	};
}

//���j�ɂ���ďo��I�u�W�F�N�g
class ContinentObject:public GameObject
{
public:
	ContinentObject(const char* name);
	void Awake()override;
	void Start()override;
	//�ǂݍ��ރ��f���̃t�@�C�����w��
	//�R���W�����������Ő������邩�B
	void LoadModel(const char* filename, bool coll);

	inline bool GetIsAddPhysicsWorld(){
		return _isAddPhysicsWorld;
	}
protected:
	//���f���`��
	SkinModel* _Model;
	//�A�j���[�V����
	Animation* _Anim;

private:
	bool _isAddPhysicsWorld = false;
};