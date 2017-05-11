#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
class HistoryBook;

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
	enum class State
	{
		LookAtHistoryBook= 0,		//歴史書を見ている状態。
		LookAtHistoryBookStart,		//歴史書を見始めたので補間を開始。
		LookAtPlayer,				//プレイヤーを見ている状態。
		LookAtPlayerStart,			//歴史書を見終わりプレイヤーを見るようにする補間の開始。
		DoingLerpMath,				//補間を計算中。
	};
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
	HistoryBook* _HistoryBook;
	//プレイヤーのポジションへの参照。
	const Vector3* _HistoryBookPos;
	//線形補間されたポジション。
	const Vector3* _LerpPos;
	//線形補間に使う補間比率。
	float _LerpRate;
	//歴史書を見る前のカメラの位置を格納する用。
	const Vector3* _PrevGameCameraPos;
	//補間の状態。
	State _LerpState;
};