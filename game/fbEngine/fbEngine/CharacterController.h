/*!
* @brief	キャラクタコントローラー。
*/

#pragma once

#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "RigidBody.h"
#include "Component.h"
#include "GameObject.h"


/*!
* @brief	キャラクタコントローラー。
*/
class CCharacterController : public Component {
public:
	CCharacterController(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
		//m_collider = g->AddComponent<CCapsuleCollider>();
	}
	~CCharacterController()
	{

	}
	/*!
	* @brief	初期化。
	* param		ゲームオブジェクト
	*			トランスフォーム
	*			半径。
	*			高さ。
	*			モデルの中心とコリジョンの中心の差分
	*			コリジョンの属性。
	*			コリジョン形状。
	*			重力
	*/
	void Init(GameObject* Object,Transform* tramsform,float radius, float height, Vector3 off , Collision_ID, Collider* capsule , float gravity);
	/*!
	* @brief	実行。
	*/
	void Execute();
	/*!
	* @brief	移動速度を設定。
	*/
	void SetMoveSpeed(const Vector3& speed)
	{
		m_moveSpeed = speed;
	}
	/*!
	* @brief	移動速度を取得。
	*/
	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	/*!
	* @brief	ジャンプさせる。
	*/
	void Jump()
	{
		m_isJump = true;
		m_isOnGround = false;
	}
	/*!
	* @brief	ジャンプ中か判定
	*/
	bool IsJump() const
	{
		return m_isJump;
	}
	/*!
	* @brief	地面上にいるか判定。
	*/
	bool IsOnGround() const
	{
		return m_isOnGround;
	}
	/*!
	* @brief	コライダーを取得。
	*/
	Collider* GetCollider() const
	{
		return m_collider;
	}
	/*!
	* @brief	重力を取得。
	*/
	void SetGravity(float gravity)
	{
		m_gravity = gravity;
	}
	/*!
	* @brief	剛体を取得。
	*/
	RigidBody* GetRigidBody()
	{
		return m_rigidBody.get();
	}
	/*!
	* @brief	剛体を物理エンジンから削除。。
	*/
	void RemoveRigidBoby();
private:
	Vector3 				m_moveSpeed = Vector3::zero;	//移動速度。 
	bool 					m_isJump = false;				//ジャンプ中？
	bool					m_isOnGround = true;			//地面の上にいる？
	Collider*				m_collider = nullptr;			//コライダー。
	float					m_radius = 0.0f;				//半径。
	float					m_height = 0.0f;				//高さ。
	unique_ptr<RigidBody>	m_rigidBody = nullptr;			//剛体。
	float					m_gravity = -9.8f;				//重力。
};