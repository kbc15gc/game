/*!
* @brief	�L�����N�^�R���g���[���[�B
*/

#pragma once

#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "RigidBody.h"
#include "Component.h"
#include "GameObject.h"


/*!
* @brief	�L�����N�^�R���g���[���[�B
*/
class CCharacterController : public Component {
public:
	CCharacterController(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
		m_collider = g->AddComponent<CCapsuleCollider>();
	}
	~CCharacterController()
	{

	}
	/*!
	* @brief	�������B
	*/
	void Init(float radius, float height, RigidBody* rigid, CCapsuleCollider* capsule);
	/*!
	* @brief	���s�B
	*/
	void Execute();
	/*!
	* @brief	���W���擾�B
	*/
	const Vector3& GetPosition() const
	{
		return transform->localPosition;
	}
	/*!
	* @brief	���W��ݒ�B
	*/
	void SetPosition(const Vector3& pos)
	{
		transform->localPosition = pos;
	}
	/*!
	* @brief	�ړ����x��ݒ�B
	*/
	void SetMoveSpeed(const Vector3& speed)
	{
		m_moveSpeed = speed;
	}
	/*!
	* @brief	�ړ����x���擾�B
	*/
	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	/*!
	* @brief	�W�����v������B
	*/
	void Jump()
	{
		m_isJump = true;
		m_isOnGround = false;
	}
	/*!
	* @brief	�W�����v��������
	*/
	bool IsJump() const
	{
		return m_isJump;
	}
	/*!
	* @brief	�n�ʏ�ɂ��邩����B
	*/
	bool IsOnGround() const
	{
		return m_isOnGround;
	}
	/*!
	* @brief	�R���C�_�[���擾�B
	*/
	CCapsuleCollider* GetCollider()
	{
		return m_collider;
	}
	/*!
	* @brief	�d�͂��擾�B
	*/
	void SetGravity(float gravity)
	{
		m_gravity = gravity;
	}
	/*!
	* @brief	���̂��擾�B
	*/
	RigidBody* GetRigidBody()
	{
		return m_rigidBody;
	}
	/*!
	* @brief	���̂𕨗��G���W������폜�B�B
	*/
	void RemoveRigidBoby();
private:
	Vector3 			m_moveSpeed = Vector3::zero;	//�ړ����x�B 
	bool 				m_isJump = false;				//�W�����v���H
	bool				m_isOnGround = true;			//�n�ʂ̏�ɂ���H
	CCapsuleCollider*	m_collider;						//�R���C�_�[�B
	float				m_radius = 0.0f;
	float				m_height = 0.0f;
	RigidBody*			m_rigidBody;					//���́B
	float				m_gravity = -9.8f;				//�d�́B
};