#pragma once
#include "GameObject\Camera\GameCamera.h"

namespace
{
	static const int MAX_ARRAY_NUM = 21;
	//�C�x���g�J�������N������̂ɕK�v�ȏ��B
	struct EventCameraInfo
	{
		Vector3 pos[MAX_ARRAY_NUM];			//���W�B
		Quaternion rot[MAX_ARRAY_NUM];		//��]�B
		float time[MAX_ARRAY_NUM];			//���ԁB
		float fade[MAX_ARRAY_NUM];			//�t�F�[�h���邩�ǂ����H
		int size;							//�z��̗v�f���B
	};

	const Support::DATARECORD EventCameraData[] =
	{
		{ "pos",Support::DataTypeE::VECTOR3_ARRAY ,offsetof(struct EventCameraInfo,pos),sizeof(EventCameraInfo::pos) },
		{ "rot",Support::DataTypeE::QUATERNION_ARRAY ,offsetof(struct EventCameraInfo,rot),sizeof(EventCameraInfo::rot) },
		{ "time",Support::DataTypeE::FLOAT_ARRAY ,offsetof(struct EventCameraInfo,time),sizeof(EventCameraInfo::time) },
		{ "fade",Support::DataTypeE::FLOAT_ARRAY ,offsetof(struct EventCameraInfo,fade),sizeof(EventCameraInfo::fade) },
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

	//�O�̃J�����B
	Transform* _BeforeCamera;
	//���݂̏󋵁B
	Vector3 _NowPos;
	Quaternion _NowRot;
};