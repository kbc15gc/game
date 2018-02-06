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
		_DestinationPos = _GetPlayerPos() + (_Player->transform->GetForward() * _Dist);
		transform->SetPosition(_DestinationPos);
		_Camera->Update();

		_ToCameraDir = (_Player->transform->GetForward());
		height = _ToCameraDir.y*_Dist;
	}
	//プレイヤーの方向を向く。
	void LookAtTarget()
	{
		auto next = _GetPlayerPos();

		_Tracking();
		transform->SetPosition(_DestinationPos);

		_Camera->SetTarget(next);
		transform->LockAt(next);
	}

private:
	Vector3 _GetPlayerPos();
	//通常時のカメラ挙動
	void _StandardBehavior();

	//プレイヤーの方向を向く。
	void _LookAtTarget();

	//カメラをY軸に回転(横)。
	void _RotateHorizon(float roty);

	//カメラをX軸に回転(縦)。
	void _RotateVertical(float rotx);

	//カメラ距離の伸縮。
	void _UpdateDist();

	//レイを飛ばして、カメラの移動先を確認。
	Vector3 _ClosetRay(Vector3 from, Vector3 to);

	//カメラを移動させる処理。
	void _Move()override;

	/**
	* 追尾カメラ.
	*/
	void _Tracking();
	
	// このカメラに切り替わった時に呼ばれるコールバック。
	virtual void ChangeCameraReAction() {
		//プレイヤーの更新を止める。
		_Player->SetIsStopUpdate(false);
	}

	//カメラリセット。
	void CameraReset();

	//バネの様に追跡。
	Vector3 _SpringChaseMove(const Vector3& now, const Vector3& target, float spring, float damping, float time, float speed);

	//カメラ移動の加速度。
	Vector3 _Velocity;


	 float sp = 30.0f;
	 float dp = 10.0f;
	//バネの伸び具合。
	 float spring = 30.0f;
	//バネの縮まる強さ。
	 float damping = 5.0f;

	 float speed = 5.0f;
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

	//カメラが最終的に到達する目標座標。
	Vector3 _DestinationPos = Vector3::zero;

	//ターゲットからカメラへの向きベクトル。
	Vector3 _ToCameraDir = Vector3::zero;
	float height;
	float rl = 5.0f;
	//プレイヤー向いている方向。
	const Vector3* _PForward = nullptr;

	//カメラリセットフラグ。
	bool _Reset = false;
	float _Timer = 0.0f;
	Vector3 tmp = Vector3::zero;


	Vector3 _PurposeTarget = Vector3::zero;
};