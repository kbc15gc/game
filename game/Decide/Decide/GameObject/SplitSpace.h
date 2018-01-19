#pragma once

#include "fbEngine\fbstdafx.h"
#include "GameObject\Component\ObjectSpawn.h"
#include "SupportData.h"

class SpaceCollisionObject;

// ��Ԃ�\���o�E���f�B���O�{�b�N�X�𐶐�����N���X�B
class SplitSpace :public GameObject{
public:
	SplitSpace(const char* name):GameObject(name) {
		Spawner::_splitSpace = this;
	};
	~SplitSpace() {
		_ReleaseSpaceCollisions();
		Spawner::_splitSpace = nullptr;
	};

	void OnDestroy()override;

	void Awake()override;
	void Update()override;
#ifdef _DEBUG
	void Debug()override;
#endif
	// ��ԕ����֐��B
	// �����F	���f���f�[�^(���̃��f�������ł���T�C�Y�̃{�b�N�X���`���A��������)�B
	//			Transform���(���̏������Ƃɐ��������{�b�N�X���ړ��A��]�A�g�k����B)�B
	//			������(��)�B
	//			������(�c)�B
	//			������(���s)�B
	//			�Փ˔������肽�����������C���[�Ŏw��(�f�t�H���g�͑S����)�B
	void Split(const SkinModelData* data, Transform* transform,int x, int y, int z,int attr = _defaultAttr,const Vector3& offset = Vector3::zero);

	// �Փ˂�����ԃR���W�����̏Փ˃I�u�W�F�N�g�ɒǉ��o�^�B
	// �����F	�Փ˔�����������Q�[���I�u�W�F�N�g�B
	void AddObjectHitSpace(const GameObject& object);

	// �e��ԃR���W�����ɏՓ˂��Ă���I�u�W�F�N�g��o�^����B
	void RegistrationObject();

	// �w�肵���Y�����̋�Ԃ��擾�B
	SpaceCollisionObject* GetSpaceObject(Int3 num) {
		if (num.x < _splitX && num.y < _splitY && num.z < _splitZ && num.x >= 0 && num.y >= 0 && num.z >= 0) {
			return _SpaceCollisions[num.x][num.y][num.z];
		}
		else {
			return nullptr;
		}
	}
private:
	enum Space { Right = 0, Left, Up, Down, Front, Back, RightUp, RightDown,RightFront,RightBack, LeftUp, LeftDown,LeftFront,LeftBack, UpFront, UpBack, DownFront, DownBack, RightUpFront, RightDownFront, RightUpBack, RightDownBack, LeftUpFront, LeftDownFront,LeftUpBack, LeftDownBack, Max };

	// �ő��Ԃ�\���{�b�N�X���`����֐��B
	// �����F	���f���f�[�^(���̃��f�������ł���T�C�Y�̃{�b�N�X���`����)�B
	//			Transform���B
	//			��`�����{�b�N�X�̃T�C�Y(�߂�l�̒l�Ɠ���)�B
	// �߂�l�F	��`�����{�b�N�X�̃T�C�Y�B
	const Vector3& CreateSpaceBox(const SkinModelData& data, const Transform& transform,Vector3& size);
	
	// �������ꂽ��Ԃ�\���{�b�N�X�R���W�����𐶐�����֐��B
	// �����F	�����O�̃{�b�N�X�T�C�Y�B
	//			�����O�̃{�b�N�X��Transform���(���̏������Ƃɐ��������{�b�N�X���ړ��A��]�A�g�k����B)�B
	//			������(��)�B
	//			������(�c)�B
	//			������(���s)�B
	//			�Փ˔������肽�����������C���[�Ŏw��(�f�t�H���g�͑S����)�B
	void CreateSplitBox(const Vector3& size, Transform* transform,int x,int y,int z,int attr,const Vector3& offset);

	// �אڂ����ԃI�u�W�F�N�g��o�^����B
	void _AdjacentSpace();

	// �v���C���[�ƏՓ˂��Ă����ԃR���W���������ׂĂ̋�ԃR���W��������T������B
	SpaceCollisionObject* IsHitPlayerToAll();

	// �v���C���[�ƏՓ˂�����Ԃ����ƂɃA�N�e�B�u��Ԃ�ύX�B
	void ChangeActivateSpace(SpaceCollisionObject* Obj);

	void _ReleaseSpaceCollisions();
private:
	Vector3 _unSplitSpaceSize;	// �����O�̋�ԃT�C�Y�B
	Vector3 _splitSpaceSize;	// ������̃{�b�N�X�������̋�ԃT�C�Y�B
	vector<vector<vector<SpaceCollisionObject*>>> _SpaceCollisions;	// �����������ꂼ��̋�ԃR���W�����B
	Int3 _AdjacentIndex[Space::Max];	// �e�Y�����ɉ��Z����l�B
	int _splitX = 1;	// ������(��)�B
	int _splitY = 1;	// ������(�c)�B
	int _splitZ = 1;	// ������(���s)�B
	static const int _defaultAttr;
	SpaceCollisionObject* _nowHitSpace = nullptr;	// ���ݏՓ˂��Ă����ԃR���W�����B
};