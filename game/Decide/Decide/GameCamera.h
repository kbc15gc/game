#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;

//ゲームカメラ
class GameCamera :public GameObject
{
public:
	GameCamera(const char* name) :GameObject(name) {}
	void Awake()override;
	void Start()override;
	void Update()override;
	//カメラ横回転
	void RotTransversal(float roty);
	//カメラが縦に回転。
	void RotLongitudinal(float rotx);
private:
	//移動関数
	void _Move();
private:
	//レイの形状
	SphereCollider* _Sphere;
	//カメラコンポーネント
	Camera* _Camera;
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
};