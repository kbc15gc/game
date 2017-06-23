#pragma once
#include "GameCamera.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
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

	/**
	* 補間レートを取得.
	*/
	float GetLerpRate() const
	{
		return _LerpRate;
	}

	//移動関数
	void Move()override;
private:

	/**
	* カメラ横回転.
	*
	* @param roty	回転角度.
	*/
	void _RotTransversal(float roty);

	/**
	* カメラが縦に回転.
	*
	* @param roty	回転角度.
	*/
	void _RotLongitudinal(float rotx);

	//通常時のカメラ挙動
	void _StandardBehavior();
	//本を見ている時の挙動
	void _HistoryBehavior();

private:

	//レイの形状
	SphereCollider* _Sphere;
	//プレイヤーオブジェクト
	//Player*		_Player;
	//プレイヤーのポジションへの参照
	//const Vector3* _PlayerPos;
	//プレイヤーへ向かうベクトル
	D3DXVECTOR3 _ToPlayerDir;
	//距離
	float _Dist;
	//当たり判定の半径
	float _Radius;

	//歴史書オブジェクト。
	HistoryBook* _HistoryBook = nullptr;
	//歴史書のポジションへの参照。
	Vector3 _HistoryBookPos;
	//線形補間されたカメラの位置。
	Vector3 _LerpCameraPos;
	//線形補間されたカメラの注視点の位置。
	Vector3 _LerpCameraLookAtPos;
	//線形補間に使う補間比率。
	float _LerpRate;
	//歴史書を見る前のカメラの位置を格納する用。
	Vector3 _PrevGameCameraPos;

	//切り替わった瞬間のカメラの視点.
	Vector3 _CameraPos = Vector3::zero;
};