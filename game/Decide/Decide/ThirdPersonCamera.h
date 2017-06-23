#pragma once
#include "GameCamera.h"
//ふかんカメラクラスの定義。
class ThirdPersonCamera :	public GameCamera
{
public:
	//コンストラクタ。
	ThirdPersonCamera(const char* name) :
		GameCamera(name)
	{
	}
	~ThirdPersonCamera();

	//コンストラクタ後の初期化。
	void Awake()override;

	//初期化。
	void Start()override;

	//更新。
	void UpdateSubClass()override;
private:
	//移動関数。
	void _Move();

	//このカメラをメインカメラに切り替える。
	void SetMainCamera()
	{
	}
private:
	Camera* _ThirdPersonCamera = nullptr;
	bool _ThirdPersonCameraFlag = false;
};

