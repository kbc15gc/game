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

	// コリジョンの大きさを再設定する関数。
	// 引数：	再設定する大きさ(半径)。
	void Resize(float radius);


private:
#ifdef _DEBUG
	// 形状に応じたモデルデータをロード。
	void ColliderModelLoad()override;
#endif
public:
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btSphereShape*		shape;
};