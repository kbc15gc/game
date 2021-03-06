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
	//平行投影
	void ParallelProjectionMatrixUpdate()
	{
		//float w = 3000.0f;// _near * tan(D3DXToRadian(_ViewAngle) / 2) * 2;
		//float h = 3000.0f;// w * _Aspect;
		//平行投影をすると影が伸びなくなる
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
	//平行投影をする場合は画角が関係なくなる。なのでこいつで写す範囲を指定する。
	Vector2 _ViewVolume;
};