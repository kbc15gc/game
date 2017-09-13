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

	//移動関数
	void Move()override;
	void SetIsMove(bool value)
	{
		_IsMove = value;
	}
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

	// このカメラに切り替わった時に呼ばれるコールバック。
	virtual void ChangeCameraReAction() {
		//プレイヤーの更新を止める。
		_Player->SetIsStopUpdate(false);
	}



private:

	//レイの形状
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

};