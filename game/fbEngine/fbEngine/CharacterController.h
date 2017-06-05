/*!
* @brief	�L�����N�^�R���g���[���[�B
*/
#pragma once

#include "_Object\_Component\Component.h"
#include "_Include\CollisionEnum.h"

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
	*			�Փ˂���肽������(���E�����A���C���[�}�X�N�Ȃ̂Ńr�b�g���Z)�B
	*			�Փ˂���肽������(�㉺�����A���C���[�}�X�N�Ȃ̂Ńr�b�g���Z)�B
	*/
	void Init(GameObject* Object, Transform* tramsform, float radius, float height, Vector3 off , int type, Collider* capsule , float gravity,int attributeXZ = -1, int attributeY = -1/*static_cast<int>(fbCollisionAttributeE::ALL)*/);
	
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

	// �S���C���[�}�X�N�I�t(���E)�B
	// ���ׂĂ̏Փ˂𖳎��B
	inline void AttributeXZ_AllOff() {
		SetAttributeXZ(0x00000000);
	}
	// �S���C���[�}�X�N�I�t(�㉺)�B
	// ���ׂĂ̏Փ˂𖳎��B
	inline void AttributeY_AllOff() {
		SetAttributeY(0x00000000);
	}

	// �S���C���[�}�X�N�I��(���E)�B
	// ���ׂẴR���W�����Ɠ����蔻����s���B
	inline void AttributeXZ_AllOn() {
		SetAttributeXZ(static_cast<int>(fbCollisionAttributeE::ALL));
	}
	// �S���C���[�}�X�N�I��(�㉺)�B
	// ���ׂẴR���W�����Ɠ����蔻����s���B
	inline void AttributeY_AllOn() {
		SetAttributeY(static_cast<int>(fbCollisionAttributeE::ALL));
	}

	// �t�B���^�[�}�X�N�ɉ��Z(���E)�B
	inline void AddAttributeXZ(short bit) {
		SetAttributeXZ(m_attributeXZ | bit);
	}
	// �t�B���^�[�}�X�N�ɉ��Z(�㉺)�B
	inline void AddAttributeY(short bit) {
		SetAttributeY(m_attributeY | bit);
	}

	// �t�B���^�[�}�X�N���猸�Z(���E)�B
	inline void SubAttributeXZ(short bit) {
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		bit = ~bit;
		SetAttributeXZ(m_attributeXZ & bit);
	}
	// �t�B���^�[�}�X�N���猸�Z(�㉺)�B
	inline void SubAttributeY(short bit) {
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		bit = ~bit;
		SetAttributeY(m_attributeY & bit);
	}

	// �Փ˂���肽��������ݒ�(���E)�B
	inline void SetAttributeXZ(short mask) {
		m_attributeXZ = mask;
	}
	// �Փ˂���肽��������ݒ�(�㉺)�B
	inline void SetAttributeY(short mask) {
		m_attributeY = mask;
	}

private:
	Vector3 				m_moveSpeed = Vector3::zero;	//�ړ����x�B 
	bool 					m_isJump = false;				//�W�����v���H
	bool					m_isOnGround = true;			//�n�ʂ̏�ɂ���H
	Collider*				m_collider = nullptr;			//�R���C�_�[�B
	float					m_radius = 0.0f;				//���a�B
	float					m_height = 0.0f;				//�����B
	unique_ptr<RigidBody>	m_rigidBody = nullptr;			//���́B
	float					m_gravity = -9.8f;				//�d�́B
	int						m_attributeXZ;					// �Փ˂���肽������(���E����)�B
	int						m_attributeY;					// �Փ˂���肽������(�㉺����)�B
};