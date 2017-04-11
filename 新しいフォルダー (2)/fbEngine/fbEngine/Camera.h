#pragma once
#include "Component.h" 

class Camera :public Component
{
public:
	Camera(GameObject* g, Transform* t) :Component(g, t, typeid(this).name())
	{
		
	}
	Camera(GameObject* g, Transform* t,const char* classname) :Component(g, t, classname)
	{

	}
	void Awake();
	void Start();
	virtual void Update();

	void ViewMatrixUpdate();
	void ProjectionMatrixUpdate();

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

protected:
	//�r���[�s��
	D3DXMATRIX _View;
	//�v���W�F�N�V�����s��
	D3DXMATRIX _Projection;

	float _ViewAngle;	//��p
	float _near;		//�ߕ���
	float _far;		//������
	float _Aspect;		//�A�X�y�N�g��
};