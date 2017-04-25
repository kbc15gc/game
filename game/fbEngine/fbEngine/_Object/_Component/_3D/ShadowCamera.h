#pragma once
#include "Camera.h" 

class ShadowCamera :public Camera
{
public:
	ShadowCamera(GameObject* g, Transform* t) :
		Camera(g, t, typeid(this).name()),
		_ViewVolume(Vector2(500,500))
	{
		
	}
	void Update()override;
	//���s���e
	void ParallelProjectionMatrixUpdate()
	{
		float w = 3000.0f;// _near * tan(D3DXToRadian(_ViewAngle) / 2) * 2;
		float h = 3000.0f;// w * _Aspect;
		//���s���e������Ɖe���L�тȂ��Ȃ�
		D3DXMatrixOrthoLH(
			&_Projection,
			_ViewVolume.x,
			_ViewVolume.y,
			_near,
			_far);
	};
	void SetViewVolume(Vector2 v)
	{
		_ViewVolume = v;
	}
protected:
	//���s���e������ꍇ�͉�p���֌W�Ȃ��Ȃ�B�Ȃ̂ł����Ŏʂ��͈͂��w�肷��B
	Vector2 _ViewVolume;
};