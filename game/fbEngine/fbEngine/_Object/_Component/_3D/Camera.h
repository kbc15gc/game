#pragma once
#include "_Object\_Component\Component.h"

class Collision;

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

	//�N���b�N�����R���W�������擾
	//[in] ���C�̒����B
	//[in] �����蔻����Ƃ肽�������B
	Collision* GetClickCollision(float rayLength, int attribute);

	const D3DXMATRIX& GetViewMat()const{ return _View; }
	const D3DXMATRIX& GetProjectionMat()const{ return _Projection; }

	//��p�ݒ�
	void SetViewAngle(const float& theta)
	{
		_ViewAngle = theta;
	}

	/**
	* ��p���擾.
	*/
	float GetViewAngle() const
	{
		return _ViewAngle;
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
	/**
	* �����_���擾.
	*/
	const Vector3& GetTarget()
	{
		return _Target;
	}

	/**
	* �����_��ݒ�.
	*/
	void SetTarget(const Vector3& tar)
	{
		_Target = tar;
	}

	/**
	* ���_���擾.
	*/
	const Vector3& GetPosition() const
	{
		return transform->GetPosition();
	}

	void SetUseTarget(bool f)
	{
		_useTarget = f;
	}

protected:

	/** �����_. */
	Vector3 _Target = Vector3::front;
	bool _useTarget = true;

	//�r���[�s��
	D3DXMATRIX _View;
	//�v���W�F�N�V�����s��
	D3DXMATRIX _Projection;

	float _ViewAngle;	//��p
	float _near;		//�ߕ���
	float _far;		//������
	float _Aspect;		//�A�X�y�N�g��
};