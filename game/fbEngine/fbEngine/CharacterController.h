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
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	}
	~CCharacterController()
	{

	}

	/*!
	* @brief	�������B
	* param		���f���̒��S�ƃR���W�����̒��S�̍����B
	*			�R���W�����̑����B
	*			�R���W�����`��B
	*			�d�́B
	*			�Փ˂����ۉ����߂���鑮��(���E�����A�ݒ肵�������̃R���W�����ɃL�����N�^�[�������߂����A���C���[�}�X�N�Ȃ̂Ńr�b�g���Z)�B
	*			�Փ˂����ۉ����߂���鑮��(�㉺�����A�ݒ肵�������̃R���W�����ɃL�����N�^�[�������߂����A���C���[�}�X�N�Ȃ̂Ńr�b�g���Z)�B
	*			�����ɕ������[���h�ɃR���W������o�^����(false�ɂ����ꍇ�͌�œo�^�֐����Ă΂Ȃ��Ɠo�^����Ȃ�)�B
	*/
	void Init(Vector3 off , int type, Collider* coll , float gravity,int attributeXZ = -1, int attributeY = -1/*static_cast<int>(fbCollisionAttributeE::ALL)*/,bool isAddWorld = true);
	
	/*!
	* @brief	�L�����N�^�[�R���g���[���ɐݒ肳��Ă���ړ��ʕ��ړ��B
	* �߂�l�F�@�Փˉ���������̎��ۂ̈ړ���(�f���^�^�C�����������ĂȂ��ړ���)�B
	*/
	const Vector3& Execute();

	/*!
	* @brief	�ړ��ʂ�ݒ�(�f���^�^�C�����������ĂȂ��ړ���)�B
	*/
	inline void SetMoveSpeed(const Vector3& speed) {
		m_moveSpeed = speed;
	}
	/*!
	* @brief	�ړ��ʂ��擾(�f���^�^�C�����������ĂȂ��ړ���)�B
	*/
	inline const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	// �O�I�v���Ŕ��������ړ��ʂ����Z�B
	// �����F	1�b�P�ʂł̈ړ���(�f���^�^�C�����������ĂȂ��ړ���)�B
	inline void AddOutsideSpeed(const Vector3& add){
		_outsideSpeed = _outsideSpeed + add;
	}
	// �Փˉ������܂߂����ۂ̈ړ��ʂ��擾�B
	// �߂�l�F	�Փˉ���������̎��ۂ̈ړ���(�f���^�^�C�����������ĂȂ��ړ���)�B
	// ��Excute�֐����Ă΂��O�ɌĂ΂ꂽ�ꍇ(0,0,0)���O���Excute�̌��ʂ��ԋp�����B
	inline const Vector3& GetmoveSpeedExcute()const {
		return _moveSpeedExcute;
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
		return m_rigidBody;
	}

	/*!
	* @brief	���̂𕨗��G���W���ɓo�^�B�B
	*/
	inline void AddRigidBody() {
		m_rigidBody->AddWorld();
	}
	/*!
	* @brief	���̂𕨗��G���W������폜�B�B
	*/
	inline void RemoveRigidBoby() {
		m_rigidBody->RemoveWorld();
	}

	// �S���C���[�}�X�N�I�t(���E)�B
	// ���ׂĂ̏Փ˂𖳎�(�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void AttributeXZ_AllOff() {
		SetAttributeXZ(0x00000000);
	}
	// �S���C���[�}�X�N�I�t(�㉺)�B
	// ���ׂĂ̏Փ˂𖳎�(�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void AttributeY_AllOff() {
		SetAttributeY(0x00000000);
	}

	// �S���C���[�}�X�N�I��(���E)�B
	// ���ׂẴR���W�����Ɠ����蔻����s��(�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void AttributeXZ_AllOn() {
		SetAttributeXZ(static_cast<int>(fbCollisionAttributeE::ALL));
	}
	// �S���C���[�}�X�N�I��(�㉺)�B
	// ���ׂẴR���W�����Ɠ����蔻����s��(�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void AttributeY_AllOn() {
		SetAttributeY(static_cast<int>(fbCollisionAttributeE::ALL));
	}

	// �t�B���^�[�}�X�N�ɉ��Z(���E�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void AddAttributeXZ(short bit) {
		SetAttributeXZ(m_attributeXZ | bit);
	}
	// �t�B���^�[�}�X�N�ɉ��Z(�㉺�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void AddAttributeY(short bit) {
		SetAttributeY(m_attributeY | bit);
	}

	// �t�B���^�[�}�X�N���猸�Z(���E�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void SubAttributeXZ(short bit) {
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		bit = ~bit;
		SetAttributeXZ(m_attributeXZ & bit);
	}
	// �t�B���^�[�}�X�N���猸�Z(�㉺�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void SubAttributeY(short bit) {
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		bit = ~bit;
		SetAttributeY(m_attributeY & bit);
	}

	// �Փ˂���肽��������ݒ�(���E�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void SetAttributeXZ(short mask) {
		m_attributeXZ = mask;
	}
	// �Փ˂���肽��������ݒ�(�㉺�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void SetAttributeY(short mask) {
		m_attributeY = mask;
	}

private:
	// �v�f�ʂɈړ��ʂ�I�����鏈���B
	float _MoveSpeedSelection(float mySpeed, float outSpeed);

private:
	Vector3 				m_moveSpeed = Vector3::zero;	//�L�����N�^�[�������I�Ɉړ�����ʁB
	Vector3					_outsideSpeed = Vector3::zero;	//�L�����N�^�[���O�I�v���ňړ�����ʁB 
	Vector3					_moveSpeedExcute = Vector3::zero;				// �Փˉ����I����̎��ۂɈړ������ʁB
	bool 					m_isJump = false;				//�W�����v���H
	bool					m_isOnGround = true;			//�n�ʂ̏�ɂ���H
	Collider*				m_collider = nullptr;			//�R���C�_�[�B
	Vector3					_halfSize;						// �R���C�_�[�̃T�C�Y(���ۂ̃T�C�Y�̔���)�B
	RigidBody*				m_rigidBody = nullptr;			//���́B
	float					m_gravity = -9.8f;				//�d�́B
	int						m_attributeXZ;					// �Փ˂����ۉ����߂���鑮��(���E�����A�ݒ肵�������̃R���W�����ɃL�����N�^�[�������߂����)�B
	int						m_attributeY;					// �Փ˂����ۉ����߂���鑮��(�㉺�����A�ݒ肵�������̃R���W�����ɃL�����N�^�[�������߂����)�B
};