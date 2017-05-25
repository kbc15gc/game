#pragma once
#include "_Object\_Component\_Physics\Collider.h"

//球状のコライダー
class SphereCollider : public Collider
{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	SphereCollider(GameObject* g, Transform* t);
	/*!
	 * @brief	デストラクタ。
	 */
	~SphereCollider();
	/*!
	 * @brief	球体コライダーを作成。
	 */
	void Create( const float radius );

private:
//	 形状に応じたモデルデータをロード。
	void ColliderModelLoad()override;

public:
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btSphereShape*		shape;
};