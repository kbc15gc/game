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
	*			�����o����������(���C���[�}�X�N�Ȃ̂Ńr�b�g���Z)�B
	*/
	void Init(const vector<RigidBody*>& collisions,int attribute = -1);

	/*!
	* @brief	�����o�����s�B
	* param		1�b�P�ʂł̈ړ���(�f���^�^�C�����������ĂȂ��ړ���)�B
	* ���L�����N�^�[�R���g���[�����A�^�b�`����Ă��Ȃ��I�u�W�F�N�g�͉����o���Ȃ��B
	*/
	void Extrude(const Vector3& speed);

	// �����o�����������I�[���I�t�B
	// ���ׂẴR���W�����������o���Ȃ��B
	inline void Attribute_AllOff() {
		SetAttribute(0x00000000);
	}

	// �����o�����������I�[���I���B
	// ���ׂẴR���W�����������o���B
	inline void Attribute_AllOn() {
		SetAttribute(static_cast<int>(fbCollisionAttributeE::ALL));
	}

	// �����o�����������ɉ��Z�B
	inline void AddAttribute(int bit) {
		SetAttribute(_attribute | bit);
	}

	// �����o�������������猸�Z�B
	inline void SubAttribute(int bit) {
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		bit = ~bit;
		SetAttribute(_attribute & bit);
	}

	// �����o������������ݒ�B
	inline void SetAttribute(int mask) {
		_attribute = mask;
	}

private:
	vector<Vector3>	_halfSize;					// �R���C�_�[�̃T�C�Y(���ۂ̃T�C�Y�̔���)�B
	vector<RigidBody*> _collisions;	//���́B
	int	_attribute;					// �����o�����������B
};