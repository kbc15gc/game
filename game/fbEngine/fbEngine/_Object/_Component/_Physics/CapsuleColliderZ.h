#pragma once
#include "_Object\_Component\_Physics\Collider.h"

//カプセル状のコライダーZ
class CCapsuleColliderZ : public Collider
{
public:
	/*!
	* @brief	コンストラクタ。
	*/
	CCapsuleColliderZ(GameObject* g, Transform* t);
	/*!
	* @brief	デストラクタ。
	*/
	~CCapsuleColliderZ();
	/*!
	* @brief	作成。
	*/
	void Create(float radius, float height);

	// コリジョンの大きさを再設定する関数。
	// 引数：	半径。
	//			高さ。
	void Resize(float radius, float height);

private:
	// 形状に応じたモデルデータをロード。
#ifdef _DEBUG
	void ColliderModelLoad()override {
		// カプセルはまだないので生成しない。
	}
#endif

public:
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btCapsuleShapeZ*		shape;
};