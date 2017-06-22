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
	void Update()override;

private:
	//移動関数
	void _Move();

	//このカメラをメインカメラに切り替える。
	void SetMainCamera()
	{
		
	}
private:
	//カメラコンポーネント
	Camera* _FreeCamera;

	bool _FreeCameraFlag = false;

	Vector3 _toPosition = Vector3::zero;
};

