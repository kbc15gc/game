#pragma once
#include "GameCamera.h"
#include "GameObject\Player\Player.h"

#if _DEBUG
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
	// このカメラに切り替わった時に呼ばれるコールバック。
	virtual void ChangeCameraReAction() {
		//プレイヤーの更新を止める。
		_Player->SetIsStopUpdate(true);
	}

	//フリーカメラの回転。
	void FreeCameraRotation();

	//フリーカメラの上下移動。
	void FreeCameraMoveUpDown();

	//プレイヤーカメラの位置に戻す。
	void Return();
private:

	float _R_STICK_Y = 0.0f;					//入力された右スティックの上下成分を保持。
	float _R_STICK_X = 0.0f;					//入力された右スティックの左右成分を保持。

	const float _CameraRotationSpeed = 10.0f;	//カメラの回転スピード。

	const float _DashSpeed = 70.0f;				//ダッシュ時のカメラのスピード。

	const float _NormalSpeed = 10.0f;			//通常時のカメラのスピード。
};

#endif // _DEBUG