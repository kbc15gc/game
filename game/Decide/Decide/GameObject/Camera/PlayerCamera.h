#pragma once
#include "GameCamera.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Player\Player.h"
class HistoryBook;


class PlayerCamera : public GameCamera
{
public:
	//コンストラクタ。
	PlayerCamera(const char* name);
	~PlayerCamera();

	/**
	* コンストラクタ後の初期化.
	*/
	void Awake()override;

	/**
	* 初期化.
	*/
	void Start()override;

	/**
	* 更新.
	*/
	void UpdateSubClass()override;

	void SetIsMove(bool value)
	{
		_IsMove = value;
	}

	/**
	* カメラの座標などの初期化.
	*/
	inline void Init()
	{
		_Camera->SetTarget(_GetPlayerPos());
		_FuturePos = _GetPlayerPos() + (_Player->transform->GetForward() * _Dist);
		transform->SetPosition(_FuturePos);
		_Camera->Update();

		_ToCameraDir = (_Player->transform->GetForward());
		_Height = _ToCameraDir.y * _Dist;
	}
	//プレイヤーの方向を向く。
	void LookAtTarget();

private:
	//プレイヤーの座標を取得。
	Vector3 _GetPlayerPos();

	//カメラ距離の伸縮。
	void _UpdateDist();

	//通常時のカメラ挙動
	void _StandardBehavior();

	//カメラ補助。
	void _AutoSupport();

	//カメラをY軸に回転(横)。
	void _RotateHorizon(float roty);

	//カメラをX軸に回転(縦)。
	void _RotateVertical(float rotx);

	//レイを飛ばして、カメラの移動先を確認。
	Vector3 _ClosetRay(Vector3 from, Vector3 to);

	//バネの様に追跡。
	Vector3 _SpringChaseMove(const Vector3& now, const Vector3& target, float spring, float damping,
		Vector3 &velocity, float time, float speed);

	//プレイヤーの方向を向く。
	void _LookAtTarget();

	//カメラを移動させる処理。
	void _Move()override {};

	// このカメラに切り替わった時に呼ばれるコールバック。
	virtual void ChangeCameraReAction() {
		//プレイヤーの更新を止める。
		_Player->SetIsStopUpdate(false);
	}
private:
	//歴史書オブジェクト。
	HistoryBook* _HistoryBook = nullptr;
	/** 移動可能フラグ. */
	bool _IsMove = true;

	//当たり判定をとるためのレイの形状
	SphereCollider* _Sphere = nullptr;
	//距離
	float _Dist = 0.0f;
	//当たり判定の半径
	float _Radius = 0.0f;

	//最終的に到達する座標。
	Vector3 _FuturePos = Vector3::zero, _FutureTarget = Vector3::zero;

	//ターゲットからカメラへの向きベクトル。
	Vector3 _ToCameraDir = Vector3::zero;
	//カメラの高さ。
	float _Height = 0.0f;
	//カメラ移動の加速度。
	Vector3 _MoveV, _LookV;

	//リセットフラグ。
	bool _Reset;
	Vector3 _ResetDir;


	//バネの伸び具合。
	float _SpringM = 20.0f, _SpringL = 30.0f;
	//バネの縮まる強さ。
	float _DampingM = 3.0f, _DampingL = 4.0f;
	//バネ移動の速度。
	float _SMoveSpeedM = 5.0f, _SMoveSpeedL = 5.0f;
	//カメラの回転速度
	float CAMERA_ROTSPEED = 3.5f;
};