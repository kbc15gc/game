#pragma once
#include "GameCamera.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Player\Player.h"
class HistoryBook;


class PlayerCamera : public GameCamera
{
public:
	//コンストラクタ。
	PlayerCamera(const char* name) :
		GameCamera(name),
		_Spring(2.0f),
		_Damping(1.0f)
	{
	}
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

	//カメラをY軸に回転(横)。
	void _RotateHorizon(float roty);

	//カメラをX軸に回転(縦)。
	void _RotateVertical(float rotx);

	//レイを飛ばして、カメラの移動先を確認。
	Vector3 _ClosetRay();

	//プレイヤーの方向を向く。
	void _LookAtPlayer();

	//カメラを移動させる処理。
	void _Move()override;

	//バネの様に追跡。
	void _SpringChaseMove(float time);

	// このカメラに切り替わった時に呼ばれるコールバック。
	virtual void ChangeCameraReAction() {
		//プレイヤーの更新を止める。
		_Player->SetIsStopUpdate(false);
	}
private:
	//当たり判定をとるためのレイの形状
	SphereCollider* _Sphere;
	//プレイヤーへ向かうベクトル
	D3DXVECTOR3 _ToPlayerDir;
	//カメラへのベクトル。
	D3DXVECTOR3 _ToCamera;
	//距離
	float _Dist;
	//当たり判定の半径
	float _Radius;

	//歴史書オブジェクト。
	HistoryBook* _HistoryBook = nullptr;

	/** 移動可能フラグ. */
	bool _IsMove = true;

	//前フレームのポジション。
	Vector3 _PrevPosition = Vector3::zero;

	//カメラがオブジェクトと衝突しているか？
	bool _IsHitObject;

	//カメラが最終的に到達する目標座標。
	Vector3 _DestinationPos;
	//カメラ移動の加速度。
	Vector3 _Velocity;
	//ダンピング定数。
	float _Damping;
	//バネ定数。
	float _Spring;
};