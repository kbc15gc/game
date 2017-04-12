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
		//m_collider = g->AddComponent<CCapsuleCollider>();
	}
	~CCharacterController()
	{

	}
	/*!
	* @brief	�������B
	* param		�Q�[���I�u�W�F�N�g
	*			�g�����X�t�H�[��
	*			���a�B
	*			�����B
	*			���f���̒��S�ƃR���W�����̒��S�̍���
	*			�R���W�����̑����B
	*			�R���W�����`��B
	*			�d��
	*/
	void Init(GameObject* Object,Transform* tramsform,float radius, float height, Vector3 off , Collision_ID, Collider* capsule , float gravity);
	/*!
	* @brief	���s�B
	*/
	void Execute();
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
	Collider* GetCollider() const
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
		return m_rigidBody.get();
	}
	/*!
	* @brief	���̂𕨗��G���W������폜�B�B
	*/
	void RemoveRigidBoby();
private:
	Vector3 				m_moveSpeed = Vector3::zero;	//�ړ����x�B 
	bool 					m_isJump = false;				//�W�����v���H
	bool					m_isOnGround = true;			//�n�ʂ̏�ɂ���H
	Collider*				m_collider = nullptr;			//�R���C�_�[�B
	float					m_radius = 0.0f;				//���a�B
	float					m_height = 0.0f;				//�����B
	unique_ptr<RigidBody>	m_rigidBody = nullptr;			//���́B
	float					m_gravity = -9.8f;				//�d�́B
};