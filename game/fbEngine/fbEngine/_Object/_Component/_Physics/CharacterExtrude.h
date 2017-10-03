/*!
* @brief	�L�����N�^�R���g���[���[�B
*/
#pragma once

#include "_Object\_Component\Component.h"
#include "_Include\CollisionEnum.h"

/*!
* @brief	���������ʏՓ˂����I�u�W�F�N�g�������o���N���X�B
*/
class CharacterExtrude : public Component {
public:
	CharacterExtrude(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	}
	~CharacterExtrude()
	{

	}

	/*!
	* @brief	�������B
	* param		�R���W�����B
	*			�����o����������(���E�����A�����߂��ꂽ���ꍇ�ɐݒ�A���C���[�}�X�N�Ȃ̂Ńr�b�g���Z)�B
	*			�����o����������(�㉺�����A�����߂��ꂽ���ꍇ�ɐݒ�A���C���[�}�X�N�Ȃ̂Ńr�b�g���Z)�B
	*/
	void Init(RigidBody* collision,int attributeXZ = -1, int attributeY = -1);

	/*!
	* @brief	�����o�����s�B
	* param		�ړ��ʁB
	* ���L�����N�^�[�R���g���[�����A�^�b�`����Ă��Ȃ��I�u�W�F�N�g�͉����o���Ȃ��B
	*/
	void Extrude(const Vector3& speed);

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
		SetAttributeXZ(_attributeXZ | bit);
	}
	// �t�B���^�[�}�X�N�ɉ��Z(�㉺�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void AddAttributeY(short bit) {
		SetAttributeY(_attributeY | bit);
	}

	// �t�B���^�[�}�X�N���猸�Z(���E�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void SubAttributeXZ(short bit) {
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		bit = ~bit;
		SetAttributeXZ(_attributeXZ & bit);
	}
	// �t�B���^�[�}�X�N���猸�Z(�㉺�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void SubAttributeY(short bit) {
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		bit = ~bit;
		SetAttributeY(_attributeY & bit);
	}

	// �Փ˂���肽��������ݒ�(���E�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void SetAttributeXZ(short mask) {
		_attributeXZ = mask;
	}
	// �Փ˂���肽��������ݒ�(�㉺�A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	inline void SetAttributeY(short mask) {
		_attributeY = mask;
	}

private:
	Vector3	_halfSize;					// �R���C�_�[�̃T�C�Y(���ۂ̃T�C�Y�̔���)�B
	RigidBody* _collision = nullptr;	//���́B
	int	_attributeXZ;					// �Փ˂���肽������(���E�����A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
	int	_attributeY;					// �Փ˂���肽������(�㉺�����A�Փˉ����̏ȗ��݂̂Ń��[���h�Ŕ���͎���)�B
};