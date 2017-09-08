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
	void Create(float radius, float height);

	// コリジョンの大きさを再設定する関数。
	// 引数：	半径。
	//			高さ。
	void Resize(float radius, float height);

private:
#ifdef _DEBUG
	// 形状に応じたモデルデータをロード。
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
	btCapsuleShape*		shape;
};