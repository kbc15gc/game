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
		GameCamera(name)
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
	//通常時のカメラ挙動
	void _StandardBehavior();

	//カメラをY軸に回転(横)。
	void _RotateHorizon(float roty);

	//カメラをX軸に回転(縦)。
	void _RotateVertical(float rotx);

	//プレイヤーの方向を向く。
	void _LookAtPlayer();

	//カメラを移動させる処理。
	void _Move()override;

	//レイを飛ばして、カメラの移動先を確認。
	Vector3 _ClosetRay();

	//バネ移動を計算。
	Vector3 _CalcSpringDamp(Vector3 curr, Vector3 trgpos, Vector3 prevtrg, float delta, float spring, float damp, float springlen);

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
};