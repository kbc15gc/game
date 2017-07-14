#pragma once
#include "fbEngine\_Object\_GameObject\TextObject.h"

class Camera;

class AttackValue2D : public GameObject
{
public:
	AttackValue2D(const char * name) :GameObject(name)
	{
	}
	~AttackValue2D() {
	}

	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	//ダメージ量を出す２Dの初期化
	// 座標。
	// ダメージ量。
	// 表示する時間。
	// ずらす量。
	void Init(Vector3 pos,int damagevalue, float lifetime, Vector3 offset = Vector3::zero);
private:
	//攻撃量のテキスト
	TextObject* _AttackText;
	//攻撃量の最大生存時間。
	float _LifeTime;
	//攻撃量の現在の時間。
	float _Time;
	//攻撃量テキストが作成されているか。
	bool _CreatAttackValue;
	//ポジション。
	Vector3 _Pos;
	//カメラ
	Camera* camera;
};