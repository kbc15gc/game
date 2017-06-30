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
		Low,
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
			//高さが中から高になったのでカメラの位置を保存。
			_PrevCameraPos = transform->GetPosition();
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

		//高さが中になったので保存していた位置を適用。
		if (height==Camera_Height::Middle)
		{
			//切り替わる前の中のカメラ位置を設定。
			transform->SetPosition(_PrevCameraPos);
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

	//プレイヤーカメラの位置に戻す。
	void Return();
private:
	
	Camera_Height _NowHeight = Camera_Height::Low;		//今の高さ。
	const float _LowCameraSpeed = 9.0f;					//低い高さでの移動スピード。
	const float _MiddleCameraSpeed = 25.0f;				//中の高さでの移動スピード。
	const float _MiddleCameraDashSpeed = 100.0f;		//中の高さでのダッシュスピード。
	Vector3 _PrevCameraPos = Vector3::zero;				//切り替わる前のカメラの位置を保持。
	Vector3 _HeightPos = Vector3(0.0f, 5000.0f, 0.0f);	//高の高さ。
};

