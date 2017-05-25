#pragma once
#include "_Object\_Component\_Physics\Collider.h"

//カプセル状のコライダー
class CCapsuleCollider : public Collider
{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	CCapsuleCollider(GameObject* g, Transform* t);
	/*!
	 * @brief	デストラクタ。
	 */
	~CCapsuleCollider();
	/*!
	* @brief	作成。
	*/
	void Create(float radius, float height)
	{
		shape = new btCapsuleShape(radius, height);
	}

private:
	// 形状に応じたモデルデータをロード。
	void ColliderModelLoad()override {
		// カプセルはまだないので生成しない。
	}


public:
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btCapsuleShape*		shape;
};