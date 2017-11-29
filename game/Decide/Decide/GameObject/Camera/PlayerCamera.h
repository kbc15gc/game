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

	//カメラ補助。
	void _CameraSupport();

	//レイを飛ばして、カメラの移動先を確認。
	Vector3 _ClosetRay();

	//カメラを移動させる処理。
	void _Move()override;

	
	// このカメラに切り替わった時に呼ばれるコールバック。
	virtual void ChangeCameraReAction() {
		//プレイヤーの更新を止める。
		_Player->SetIsStopUpdate(false);
	}

	//カメラリセット。
	void CameraReset();
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
	//プレイヤー向いている方向。
	const Vector3* _PForward = nullptr;

	//カメラリセットフラグ。
	bool _Reset = false;
	float _Timer = 0.0f;
	Vector3 tmp = Vector3::zero;
};