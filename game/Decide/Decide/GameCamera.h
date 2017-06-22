/**
* ゲームカメラクラスの定義.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
/**
* カメラの抽象クラス。
*/
class GameCamera :public GameObject
{
public:
	//コンストラクタ。
	GameCamera(const char* name) :
		GameObject(name)
	{
	}

	//カメラをセット。
	void SetCamera(Camera* camera)
	{
		_Camera = camera;
	}

	//カメラをゲット。
	Camera* GetCamera()
	{
		return _Camera;
	}
protected:
	//カメラコンポーネント。
	Camera* _Camera = nullptr;
};