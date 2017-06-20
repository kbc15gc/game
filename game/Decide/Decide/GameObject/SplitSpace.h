#pragma once

#include"fbEngine\fbstdafx.h"

class CollisionObject;

// ��Ԃ�\���o�E���f�B���O�{�b�N�X�𐶐�����N���X�B
class SplitSpace :public GameObject{
public:
	SplitSpace(const char* name):GameObject(name) {};
	~SplitSpace() {};
	// ��ԕ����֐��B
	// �����F	���f���f�[�^(���̃��f�������ł���T�C�Y�̃{�b�N�X���`���A��������)�B
	//			Transform���(���̏������Ƃɐ��������{�b�N�X���ړ��A��]�A�g�k����B)�B
	//			������(��)�B
	//			������(�c)�B
	//			������(���s)�B
	void Split(const SkinModelData* data, const Transform& transform,int x, int y, int z);
private:
	// �ő��Ԃ�\���{�b�N�X���`����֐��B
	// �����F	���f���f�[�^(���̃��f�������ł���T�C�Y�̃{�b�N�X���`����)�B
	//			��`�����{�b�N�X�̃T�C�Y(�߂�l�̒l�Ɠ���)�B
	// �߂�l�F	��`�����{�b�N�X�̃T�C�Y�B
	const Vector3& CreateSpaceBox(const SkinModelData& data,Vector3& size);
	
	// �������ꂽ��Ԃ�\���{�b�N�X�R���W�����𐶐�����֐��B
	// �����F	�����O�̃{�b�N�X�T�C�Y�B
	//			�����O�̃{�b�N�X��Transform���(���̏������Ƃɐ��������{�b�N�X���ړ��A��]�A�g�k����B)�B
	//			������(��)�B
	//			������(�c)�B
	//			������(���s)�B
	void CreateSplitBox(const Vector3& size, const Transform& transform,int x,int y,int z);

private:
	Vector3 _unSplitSpaceSize;	// �����O�̋�ԃT�C�Y�B
	Vector3 _splitSpaceSize;	// ������̃{�b�N�X�������̋�ԃT�C�Y�B
	vector<CollisionObject*> _SpaceCollisions;	// �����������ꂼ��̋�ԃR���W�����B
};