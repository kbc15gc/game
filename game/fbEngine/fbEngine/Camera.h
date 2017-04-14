#pragma once
#include "Component.h" 

class Camera :public Component
{
public:
	Camera(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
		
	}
	Camera(GameObject* g, Transform* t,const char* classname) :
		Component(g, t, classname)
	{

	}
	~Camera();
	void Awake();
	void Start();
	virtual void Update();

	void ViewMatrixUpdate();
	void ProjectionMatrixUpdate();

	//�󂯎�������[���h�|�W�V�������X�N���[�����W�ɕϊ�
	Vector2 WorldToScreen(Vector3 wpos);
	//�󂯎�����X�N���[�����W�����[���h�|�W�V�����ɕϊ�(z��0�ɂȂ�B)
	Vector3 ScreenToWorld(Vector2 spos);

	const D3DXMATRIX& GetViewMat(){ return _View; }
	const D3DXMATRIX& GetProjectionMat(){ return _Projection; }

	//��p�ݒ�
	void SetViewAngle(const float& theta)
	{
		_ViewAngle = theta;
	}
	//�ߕ��ʐݒ�
	void SetNear(const float& n)
	{
		_near = n;
	}
	//�����ʐݒ�
	void SetFar(const float& f)
	{
		_far = f;
	}
	//�A�X�y�N�g�䗦�ݒ�
	void SetAspect(const float& asp)
	{
		_Aspect = asp;
	}
	//�����_�ݒ�
	void SetViewPoint(const Vector3& pos)
	{
		//�A�h���X�m�ۂ���Ă��Ȃ�
		if (_ViewPoint == nullptr)
		{
			//�m�ۂ����B
			_ViewPoint = new Vector3();
		}
		*_ViewPoint = pos;
	}
	//�����_�̃|�C���^��n���B
	void SetViewPoint(Vector3* point)
	{
		if (_ViewPoint != nullptr)
		{
			//�O�̃A�h���X�폜
			SAFE_DELETE(_ViewPoint);
		}
		_ViewPoint = point;
	}
	Vector3 GetViewPoint()
	{
		if (_ViewPoint == nullptr)
		{
			_ViewPoint = new Vector3();
		}
		return *_ViewPoint;
	}
protected:
	//�r���[�s��
	D3DXMATRIX _View;
	//�v���W�F�N�V�����s��
	D3DXMATRIX _Projection;

	//�����_
	Vector3* _ViewPoint;
	float _ViewAngle;	//��p
	float _near;		//�ߕ���
	float _far;		//������
	float _Aspect;		//�A�X�y�N�g��
};