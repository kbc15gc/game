#pragma once
#include "GameObject\Camera\GameCamera.h"

namespace
{
	static const int MAX_ARRAY_NUM = 20;
	//�C�x���g�J�������N������̂ɕK�v�ȏ��B
	struct EventCameraInfo
	{
		Vector3 pos[MAX_ARRAY_NUM];			//���W�B
		Quaternion rot[MAX_ARRAY_NUM];	//��]�B
		float times[MAX_ARRAY_NUM];			//���ԁB
		int size;							//�z��̗v�f���B
	};

	const Support::DATARECORD EventCameraData[] =
	{
		{ "pos",Support::DataTypeE::VECTOR3_ARRAY ,offsetof(struct EventCameraInfo,pos),sizeof(Vector3) * MAX_ARRAY_NUM },
		{ "rot",Support::DataTypeE::QUATERNION_ARRAY ,offsetof(struct EventCameraInfo,rot),sizeof(Quaternion) * MAX_ARRAY_NUM },
		{ "times",Support::DataTypeE::FLOAT_ARRAY ,offsetof(struct EventCameraInfo,times),sizeof(float) * MAX_ARRAY_NUM },
		{ "size",Support::DataTypeE::INT ,offsetof(struct EventCameraInfo,size),sizeof(int) },
	};
}

//�C�x���g�Ŏg���J�����B
class EventCamera :public GameCamera
{
public:
	EventCamera(const char* name) :GameCamera(name) {};
	~EventCamera() {};

	//�R���X�g���N�^��̏������B
	void Awake()override;
	//�������B
	void Start()override;
	//�X�V�B
	void UpdateSubClass()override;

	//���s�B
	void Excute(int id);
	//�C�x���g�J�����I���B
	void EndEvent();
private:
	virtual void _Move()override;
private:
	//���s���t���O�B
	bool _Runtime = false;
	//���݁A�Y�����̉��Ԗڂ��H
	int _Index = 0;
	//�^�C�}�[�B
	float _Timer = 0.0f;
	//�C�x���g�J�����̏��B
	EventCameraInfo _Info;
};