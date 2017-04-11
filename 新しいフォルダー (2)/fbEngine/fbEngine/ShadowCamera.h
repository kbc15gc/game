#pragma once
#include "Camera.h" 

class ShadowCamera :public Camera
{
public:
	ShadowCamera(GameObject* g, Transform* t) :Camera(g, t, typeid(this).name())
	{
		
	}
	void Update()override;
	//���s���e
	//�����������֐���
	void ParallelProjectionMatrixUpdate()
	{
		float w = _near * tan(D3DXToRadian(_ViewAngle) / 2) * 2;
		float h = w * _Aspect;
		//�e�͕��s���e�Ŏʂ������Ȃ�
		//���s���e�ł��Ȃ��Ȃ�e�͐L�т�
		D3DXMatrixOrthoLH(
			&_Projection,
			w,
			h,
			_near,
			_far);
	};
};