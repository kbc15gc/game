#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Player\Player.h"

class Player;

//カメラの基底クラス。
class GameCamera :public GameObject
{
public:
	//コンストラクタ。
	GameCamera(const char* name) :
		GameObject(name)
	{
	}

	~GameCamera();

	//コンストラクタ後の初期化。
	void Awake()override;

	//初期化。
	void Start()override;

	//更新。
	void Update()override;

	// 継承先の更新処理。
	virtual void UpdateSubClass() = 0;

	//次のカメラを指定。
	void SetNextCamera(GameCamera* next);

	void ChangeRequest();

	//このカメラをメインカメラとして使用する。
	void ActiveCamera()
	{
		Camera* cam = INSTANCE(GameObjectManager)->mainCamera;	//GameObjectManagerに登録している場合、取り外して更新を停止する。
		if (cam) {
			static_cast<GameCamera*>(cam->gameObject)->UnActivateFlg();
		}
		INSTANCE(GameObjectManager)->mainCamera = this->_Camera;
		_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
		_Player->SetIsStopUpdate(false);
		_isActivate = true;
	}
	
	void LateUpdate()override {
		if (_isActivate) {
			ActivateFlg();
			_isActivate = false;
		}
	}
	// 継承先の更新処理。
	virtual void Move() = 0;

protected:
	void ActivateFlg() {
		_isActive = true;
	}
	void UnActivateFlg() {
		_isActive = false;
		_isActivate = false;
	}

	//カメラの移動速度を設定。
	void SetCameraSpeed(float speed)
	{
		_MoveSpeed = speed;
	}

protected:
	//カメラコンポーネント。
	Camera* _Camera = nullptr;

	//次のカメラ。
	GameCamera* _NextCamera = nullptr;

	//このカメラが更新処理を行うか。
	bool _isActive = true;	

	//このカメラをアクティブ化するか。
	bool _isActivate = false;

	//プレイヤーオブジェクト
	Player* _Player = nullptr;

	//プレイヤーのポジションへの参照
	const Vector3* _PlayerPos = &Vector3::zero;

	//カメラの移動速度。
	float _MoveSpeed = 3.0f;

};