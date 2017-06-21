#pragma once
#include "fbEngine\_Object\_GameObject\CollisionObject.h"

class Player;

class SpaceCollisionObject :public CollisionObject {
public:
	SpaceCollisionObject(char* name) :CollisionObject(name) {};
	~SpaceCollisionObject(){};
	void Create(const Vector3& pos, const Quaternion& rot, const Vector3& scale, int id);
	void Start()override;
	void Update()override;

	// �Փ˂��Ă���I�u�W�F�N�g�����ׂČ��o�B 
	inline void RegistrationObject() {
		_HitCollisions = GetCollision()->GetPairCollisions();
	}

	// �Փ˂��Ă���ΏՓ˃I�u�W�F�N�g�ɒǉ��o�^�B
	// �����F	�Փ˔�����������Q�[���I�u�W�F�N�g�B
	void AddObjectHitSpace(GameObject& object);

	// ���̋�Ԃɑ�����I�u�W�F�N�g���A�N�e�B�u���B
	inline void EnableObjects() {
		_SetActives(true);
	}

	// ���̋�Ԃɑ�����I�u�W�F�N�g���A�N�e�B�u���B
	inline void DisableObjects() {
		_SetActives(false);
	}

private:

	// ���̋�Ԃɑ�����I�u�W�F�N�g�ɃA�N�e�B�u�t���O�ݒ�B
	void _SetActives(bool flg);

	// �אڂ����ԃI�u�W�F�N�g�ɑ�����I�u�W�F�N�g�����ׂăA�N�e�B�u���B
	void _EnableObjectsAdjacent();

	// �����Ɏw�肵���I�u�W�F�N�g����A�^�b�`���ꂽ�R���W�����R���|�[�l���g���擾�B
	Collision* _GetAttachCollision(GameObject& object);
public:
	// �v���C���[�ƏՓ˂��Ă��邩�擾�B
	inline bool GetisHitPlayer()const {
		return _isHitPlayer;
	}

	// �אڂ����ԃI�u�W�F�N�g��ǉ��B
	inline void AddAdjacentSpaceObject(SpaceCollisionObject* obj) {
		_adjacentSpaceObjects.push_back(obj);
	}
private:
	btAlignedObjectArray<btCollisionObject*> _HitCollisions;	// �Փ˂��Ă���R���W�����B
	vector<SpaceCollisionObject*> _adjacentSpaceObjects;		// �אڂ����ԃI�u�W�F�N�g�B
	GameObject* _player = nullptr;
	bool _isHitPlayer;	// ���̃R���W�����I�u�W�F�N�g���v���C���[�ƏՓ˂��Ă��邩�B
};