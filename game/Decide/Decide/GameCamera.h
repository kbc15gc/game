#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
class HistoryBook;

//ゲームカメラ
class GameCamera :public GameObject
{
public:
	GameCamera(const char* name) :GameObject(name) {}
	~GameCamera();
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
	//通常時のカメラ挙動
	void _StandardBehavior();
	//本を見ている時の挙動
	void _HistoryBehavior();
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

	Vector3 _toPosition = Vector3::zero;

};