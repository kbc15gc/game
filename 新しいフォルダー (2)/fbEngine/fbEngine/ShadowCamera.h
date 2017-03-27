#pragma once
#include "Camera.h" 

class ShadowCamera :public Camera
{
public:
	ShadowCamera(GameObject* g, Transform* t) :Camera(g, t, typeid(this).name())
	{
		
	}
	void Update()override;
	//平行投影
	//くっそ長い関数名
	void ParallelProjectionMatrixUpdate()
	{
		float w = _near * tan(D3DXToRadian(viewAngle) / 2) * 2;
		float h = w * aspect;
		//影は平行投影で写したいなり
		//平行投影でしないなら影は伸びる
		D3DXMatrixOrthoLH(
			&projection,
			w,
			h,
			_near,
			_far);
	};
};