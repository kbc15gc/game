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
		if (radius < 0.0f || height < 0.0f) {
			// コライダーサイズに0より小さい値が設定されてるよ。
			abort();
		}
		shape = new btCapsuleShape(radius, height);
		btVector3 work = shape->getImplicitShapeDimensions();
		_halfSize = Vector3(work.x(), work.y() + work.x(), work.z());
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