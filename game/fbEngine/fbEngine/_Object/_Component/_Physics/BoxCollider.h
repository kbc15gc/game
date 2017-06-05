#pragma once
#include "_Object\_Component\_Physics\Collider.h"

//箱型のコライダー
class BoxCollider : public Collider
{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	BoxCollider(GameObject* g, Transform* t);
	/*!
	 * @brief	デストラクタ。
	 */
	~BoxCollider();
	/*!
	 * @brief	ボックスコライダーを作成。
	 */
	void Create( const Vector3& size );

private:
	// 形状に応じたモデルデータをロード。
	void ColliderModelLoad()override;

public:
	btCollisionShape* GetBody() override
	{
		return _Shape;
	}
private:
	btBoxShape*		_Shape;
};