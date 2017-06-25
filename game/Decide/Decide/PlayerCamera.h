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

	//移動関数
	void _Move();
	//通常時のカメラ挙動
	void _StandardBehavior();

private:

	//レイの形状
	SphereCollider* _Sphere;
	//プレイヤーオブジェクト
	Player*		_Player;
	//プレイヤーのポジションへの参照
	const Vector3* _PlayerPos;
	//プレイヤーへ向かうベクトル
	D3DXVECTOR3 _ToPlayerDir;
	//距離
	float _Dist;
	//当たり判定の半径
	float _Radius;

	//歴史書オブジェクト。
	HistoryBook* _HistoryBook = nullptr;

};