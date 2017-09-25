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

	//受け取ったワールドポジションをスクリーン座標に変換
	Vector2 WorldToScreen(Vector3 wpos);
	//受け取ったスクリーン座標をワールドポジションに変換(zは0になる。)
	Vector3 ScreenToWorld(Vector2 spos);

	//クリックしたコリジョンを取得
	//[in] レイの長さ。
	//[in] 当たり判定をとりたい属性。
	Collision* GetClickCollision(float rayLength, int attribute);

	const D3DXMATRIX& GetViewMat(){ return _View; }
	const D3DXMATRIX& GetProjectionMat(){ return _Projection; }

	//画角設定
	void SetViewAngle(const float& theta)
	{
		_ViewAngle = theta;
	}

	/**
	* 画角を取得.
	*/
	float GetViewAngle() const
	{
		return _ViewAngle;
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
	//注視点設定
	void SetViewPoint(const Vector3& pos)
	{
		//アドレス確保されていない
		if (_ViewPoint == nullptr)
		{
			//確保するよ。
			_ViewPoint = new Vector3();
		}
		*_ViewPoint = pos;
	}
	//注視点のポインタを渡す。
	void SetViewPoint(Vector3* point)
	{
		if (_ViewPoint != nullptr)
		{
			//前のアドレス削除
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

	/**
	* 注視点を取得.
	*/
	const Vector3& GetTarget() const
	{
		return _Target;
	}

	/**
	* 注視点を設定.
	*/
	void SetTarget(const Vector3& tar)
	{
		_Target = tar;
	}

	/**
	* 視点を取得.
	*/
	const Vector3& GetPosition() const
	{
		return transform->GetPosition();
	}

protected:

	/** 注視点. */
	Vector3 _Target = Vector3::zero;

	//ビュー行列
	D3DXMATRIX _View;
	//プロジェクション行列
	D3DXMATRIX _Projection;

	//注視点
	Vector3* _ViewPoint;
	float _ViewAngle;	//画角
	float _near;		//近平面
	float _far;		//遠平面
	float _Aspect;		//アスペクト比
};