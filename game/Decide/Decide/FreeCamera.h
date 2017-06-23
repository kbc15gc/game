#pragma once
#include "GameCamera.h"

//フリーカメラクラスの定義。
class FreeCamera : public GameCamera
{
public:
	//コンストラクタ。
	FreeCamera(const char* name) :
		GameCamera(name)
	{
	}

	//デストラクタ。
	~FreeCamera();

	//コンストラクタ後の初期化。
	void Awake()override;

	//初期化。
	void Start()override;

	//更新。
	void UpdateSubClass()override;

	void Move()override;
private:
	bool _FreeCameraFlag = false;

	Vector3 _toPosition = Vector3::zero;
};

