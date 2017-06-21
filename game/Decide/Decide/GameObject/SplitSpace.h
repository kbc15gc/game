#pragma once

#include"fbEngine\fbstdafx.h"

class SpaceCollisionObject;

// ��Ԃ�\���o�E���f�B���O�{�b�N�X�𐶐�����N���X�B
class SplitSpace :public GameObject{
public:
	SplitSpace(const char* name):GameObject(name) {};
	~SplitSpace() {};

	void Awake()override;

	// ��ԕ����֐��B
	// �����F	���f���f�[�^(���̃��f�������ł���T�C�Y�̃{�b�N�X���`���A��������)�B
	//			Transform���(���̏������Ƃɐ��������{�b�N�X���ړ��A��]�A�g�k����B)�B
	//			������(��)�B
	//			������(�c)�B
	//			������(���s)�B
	void Split(const SkinModelData* data, const Transform& transform,int x, int y, int z);

	//// �w�肵���v�f�ԍ��̋�ԃR���W�����擾�B
	//inline const SpaceCollisionObject* GetCollisionObject(int x, int y, int z) const{
	//	if (y >= _SpaceCollisions.size() || x >= _SpaceCollisions[0].size() || z >= _SpaceCollisions[0][0].size()) {
	//		abort();	// �w�肵���v�f�ԍ����z��O�B
	//	}
	//	return _SpaceCollisions[y][x][z];
	//};

	// �Փ˂�����ԃR���W�����̏Փ˃I�u�W�F�N�g�ɒǉ��o�^�B
	// �����F	�Փ˔�����������Q�[���I�u�W�F�N�g�B
	void AddObjectHitSpace(const GameObject& object);

	// �e��ԃR���W�����ɏՓ˂��Ă���I�u�W�F�N�g��o�^����B
	void RegistrationObject();
private:
	enum Space { Right = 0, Left, Up, Down, Front, Back, RightUp, RightDown, LeftUp, LeftDown, UpFront, DownFront, UpBack, DownBack, RightUpFront, RightDownFront, LeftUpFront, LeftDownFront, RightUpBack, RightDownBack, LeftUpBack, LeftDownBack, Max };

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

	// �אڂ����ԃI�u�W�F�N�g��o�^����B
	void _AdjacentSpace();
private:
	Vector3 _unSplitSpaceSize;	// �����O�̋�ԃT�C�Y�B
	Vector3 _splitSpaceSize;	// ������̃{�b�N�X�������̋�ԃT�C�Y�B
	vector<vector<vector<SpaceCollisionObject*>>> _SpaceCollisions;	// �����������ꂼ��̋�ԃR���W�����B
	Vector3 _AdjacentIndex[Space::Max];	// �e�Y�����ɉ��Z����l�B
	int _splitX = 1;	// ������(��)�B
	int _splitY = 1;	// ������(�c)�B
	int _splitZ = 1;	// ������(���s)�B
};