#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

//フリーカメラクラスの定義。
class FreeCamera : public GameObject
{
public:
	//コンストラクタ。
	FreeCamera(const char* name) :
		GameObject(name)
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
		//ふかんカメラにする前のカメラをゲット。
		_GameCamera = INSTANCE(GameObjectManager)->mainCamera;

		//ふかんカメラに切り替え。
		INSTANCE(GameObjectManager)->mainCamera = _FreeCamera;

		_FreeCameraFlag = true;
	}
private:
	//カメラコンポーネント
	Camera* _FreeCamera;

	//ゲームカメラ。
	Camera* _GameCamera = nullptr;

	bool _FreeCameraFlag = false;

	Vector3 _toPosition = Vector3::zero;
};

