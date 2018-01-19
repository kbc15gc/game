#pragma once
#include "fbEngine\_Object\_GameObject\CollisionObject.h"
#include "SupportData.h"

class Player;
class SplitSpace;

class SpaceCollisionObject :public CollisionObject {
public:
	SpaceCollisionObject(char* name) :CollisionObject(name) {};
	~SpaceCollisionObject(){
		_HitCollisions.clear();
	};

	// ��ԃR���W���������֐��B
	// �����F	�ʒu(���[�J�����W)�B
	//			��](���[�J�����W)�B
	//			�R���W�����T�C�Y�B
	//			�R���W���������B
	//			�e��Transform���B
	//			�Փ˔��肵�����R���W���������B
	//			��Ԕz��ł̓Y�����B
	void Create(const Vector3& pos, const Quaternion& rot, const Vector3& size, int id, Transform* parent, int attr,const Int3& myNumber);
	void Start()override;

	// ��Ԃ̏�Ԃ��X�V����B
	void UpdateActiveSpace();

	// ���̋�ԃR���W�������v���C���[�ƏՓ˂��Ă��邩���肷��
	bool isHitPlayer();

	// ���͂̋�Ԃ��v���C���[�ƏՓ˂��Ă��邩���肷��B
	SpaceCollisionObject* IsHitPlayerAdjacent();

	// �Փ˂��Ă���I�u�W�F�N�g�����ׂČ��o�B 
	void RegistrationObject();

	// �Փ˂��Ă���ΏՓ˃I�u�W�F�N�g�ɒǉ��o�^�B
	// �����F	�Փ˔�����������Q�[���I�u�W�F�N�g�B
	void AddObjectHitSpace(GameObject& object);

	// ���̋�ԂƗאڋ�Ԃ��A�N�e�B�u���B
	inline void EhableThisAndAdjacent() {
		EnableObjects();
		EnableObjectsAdjacent();
	}

	// ���̋�ԂƂ��̋�Ԃɗאڂ����Ԃ̒��ŁA�n���ꂽ��ԃI�u�W�F�N�g�ɗאڂ��Ă��Ȃ���Ԃ��A�N�e�B�u���B
	// �����F	�V�������S���(Null��n���Ƃ��ׂĔ�A�N�e�B�u������)�B
	// ���̂ݒT�������B
	void DisableNotAdjacent(const SpaceCollisionObject* Obj);

	// ���̋�Ԃɑ�����I�u�W�F�N�g���A�N�e�B�u���B
	inline void DisableObjects() {
		if (_isActive) {
			// �A�N�e�B�u������Ă���B
			_SetActives(false);
		}
	}

	// ���̋�Ԃɑ�����I�u�W�F�N�g���A�N�e�B�u���B
	inline void EnableObjects() {
		if (!_isActive) {
			// �܂��A�N�e�B�u������ĂȂ��B
			_SetActives(true);
		}
	}


	void SetSplitSpace(SplitSpace* split) {
		_splitSpace = split;
	}
	
	// ��ԃI�u�W�F�N�g�ɑ�����I�u�W�F�N�g�Q���A�N�e�B�u�����Ă��邩�̃t���O��ݒ肷��֐��B
	// ����{�I�Ɏg�p���Ȃ��B
	void SetIsActive(bool flg) {
		_isActive = flg;
	}
private:


	// �אڂ����ԃI�u�W�F�N�g�ɑ�����I�u�W�F�N�g�����ׂăA�N�e�B�u���B
	void EnableObjectsAdjacent();

	// ���̋�Ԃɑ�����I�u�W�F�N�g�ɃA�N�e�B�u�t���O�ݒ�B
	void _SetActives(bool flg);

	// �n���ꂽ�ԍ��̋�Ԃ��אڂ��Ă��邩�B
	inline bool IsAdjacent(const Int3& num) {
		return ((abs(static_cast<int>(_myNumber.x) - static_cast<int>(num.x)) <= 1) && (abs(static_cast<int>(_myNumber.y) - static_cast<int>(num.y)) <= 1) && (abs(static_cast<int>(_myNumber.z) - static_cast<int>(num.z)) <= 1));
	}

public:
	// �v���C���[�ƏՓ˂��Ă��邩�擾�B
	inline bool GetisHitPlayer()const {
		return _isHitPlayer;
	}

	// �אڂ����ԃI�u�W�F�N�g��ǉ��B
	inline void AddAdjacentSpaceObject(SpaceCollisionObject* obj) {
		_adjacentSpaceObjects.push_back(obj);
	}

	// �z��ł̓Y�������擾�B
	inline const Int3& GetMyNumber()const {
		return _myNumber;
	}
private:
	vector<shared_ptr<btCollisionObject>> _HitCollisions;	// �Փ˂��Ă���R���W�����B

	vector<SpaceCollisionObject*> _adjacentSpaceObjects;		// �אڂ����ԃI�u�W�F�N�g�B
	GameObject* _player = nullptr;
	bool _isHitPlayer;	// ���̃R���W�����I�u�W�F�N�g���v���C���[�ƏՓ˂��Ă��邩�B
	int _attribute;	// ���̋�Ԃɓo�^����I�u�W�F�N�g�̃R���W���������B
	bool _isActive = true;	// ���̋�Ԃɑ�����I�u�W�F�N�g���A�N�e�B�u�����Ă��邩�B
private:
	SplitSpace* _splitSpace = nullptr;	// ���̋�ԃI�u�W�F�N�g������ԕ����N���X�̃|�C���^�B
	Int3 _myNumber;	// �������������ꂽ��Ԃ̂ǂ��Ɉʒu���邩(�������y�����邽��)�B	
};