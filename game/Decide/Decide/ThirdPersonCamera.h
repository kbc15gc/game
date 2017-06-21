#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

//ふかんカメラクラスの定義。
class ThirdPersonCamera :	public GameObject
{
public:
	//コンストラクタ。
	ThirdPersonCamera(const char* name) :
		GameObject(name)
	{
	}
	~ThirdPersonCamera();

	//コンストラクタ後の初期化。
	void Awake()override;

	//初期化。
	void Start()override;

	//更新。
	void Update()override;
private:
	//移動関数。
	void _Move();

	//このカメラをメインカメラに切り替える。
	void SetMainCamera()
	{
		//ふかんカメラにする前のカメラをゲット。
		_GameCamera = INSTANCE(GameObjectManager)->mainCamera;

		//ふかんカメラに切り替え。
		INSTANCE(GameObjectManager)->mainCamera = _ThirdPersonCamera;

		_ThirdPersonCameraFlag = true;
	}
private:
	//ふかんカメラコンポーネント。
	Camera* _ThirdPersonCamera = nullptr;

	//ゲームカメラ。
	Camera* _GameCamera = nullptr;

	bool _ThirdPersonCameraFlag = false;
};

