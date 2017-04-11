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

	//画角設定
	void SetViewAngle(const float& theta)
	{
		_ViewAngle = theta;
	}
	//近平面設定
	void SetNear(const float& n)
	{
		_near = n;
	}
	//遠平面設定
	void SetFar(const float& f)
	{
		_far = f;
	}
	//アスペクト比率設定
	void SetAspect(const float& asp)
	{
		_Aspect = asp;
	}

protected:
	//ビュー行列
	D3DXMATRIX _View;
	//プロジェクション行列
	D3DXMATRIX _Projection;

	float _ViewAngle;	//画角
	float _near;		//近平面
	float _far;		//遠平面
	float _Aspect;		//アスペクト比
};