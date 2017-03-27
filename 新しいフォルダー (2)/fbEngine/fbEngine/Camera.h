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

	D3DXMATRIX View(){ return view; }
	D3DXMATRIX Projection(){ return projection; }

	//��p�ݒ�
	void ViewAngle(float theta)
	{
		viewAngle = theta;
	}
	//�ߕ��ʐݒ�
	void Near(float n)
	{
		_near = n;
	}
	//�����ʐݒ�
	void Far(float f)
	{
		_far = f;
	}
	//�A�X�y�N�g�䗦�ݒ�
	void Aspect(float asp)
	{
		aspect = asp;
	}

protected:
	//�r���[�s��
	D3DXMATRIX view;
	//�v���W�F�N�V�����s��
	D3DXMATRIX projection;

	float viewAngle;	//��p
	float _near;		//�ߕ���
	float _far;		//������
	float aspect;		//�A�X�y�N�g��
};