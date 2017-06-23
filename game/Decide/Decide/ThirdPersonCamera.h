#pragma once
#include "GameCamera.h"
#include "GameObject\Player\Player.h"

//ふかんカメラクラスの定義。
class ThirdPersonCamera :	public GameCamera
{
public:

	//ふかんカメラの高さ。
	enum class Camera_Height
	{
		Invalid = -1,
		Low = 0,
		Middle,
		Height
	};
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

	void Move()override;
private:
	
	//enmuの加算。
	Camera_Height& Add (Camera_Height& height)
	{
		//今の高さより一段階上に上げる。
		height = static_cast<Camera_Height>(static_cast<int>(height) + 1);

		//計算された値が一番上の高さ以上なら一番上の高さに修正。
		if (height >= Camera_Height::Height)
		{
			height = Camera_Height::Height;
			return height;
		}

		//計算された高さを返す。
		return height;
	}

	//enmuの減算。
	Camera_Height& Subtract(Camera_Height& height)
	{
		//今の高さより一段階下に下げる。
		height = static_cast<Camera_Height>(static_cast<int>(height) - 1);

		//計算された値が一番下の高さ以下なら一番下の高さに修正。
		if (height == Camera_Height::Invalid)
		{
			height = Camera_Height::Low;
			return height;
		}

		//計算された高さを返す。
		return height;
	}

	//カメラの高さを変更。
	void ChangeHeight();

	// このカメラに切り替わった時に呼ばれるコールバック。
	virtual void ChangeCameraReAction() {
		transform->SetPosition(_PlayerPos->x, 0.0f, _PlayerPos->z);
		//プレイヤーの更新を止める。
		_Player->SetIsStopUpdate(true);
	}

private:
	
	Camera_Height _NowHeight = Camera_Height::Low;//今の高さ。

};

