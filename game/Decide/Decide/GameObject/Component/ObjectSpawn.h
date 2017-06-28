#pragma once
#include "fbEngine\_Object\_Component\Component.h"
//#include "fbEngine\_Object\_GameObject\GameObjectManager.h"
//#include "fbEngine\_Object\_GameObject\GameObject.h"

// �o�^����GameObject����U��A�N�e�B�u�ɂ��A�w�肵���^�C�~���O�ŃA�N�e�B�u��������B
class Spawner : public GameObject {
	Spawner(char* name) : GameObject(name) {};
	~Spawner() {};

	void Update()override;

	// �����\���B
	// �����F	����������I�u�W�F�N�g�B
	//			���b��ɕ������邩�B
	//			������̈ʒu(���[���h���W)�B
	//			������̉�](���[���h���W)�B
	//			������̊g�k(���[���h���W)�B
	//			������ɐݒ肷��e(�f�t�H���g��Null)�B
	void SpawnStandby(GameObject* spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent);
private:
	GameObject* _spawnObject = nullptr;	// ����������I�u�W�F�N�g�B
	float _waitTime = 0.0f;	// �҂����ԁB
	float _timeCounter = 0.0f;	// �J�E���^�[�B
};

// ���̃R���|�[�l���g���A�^�b�`����Ǝw�肵���ʒu�Ɏw�肵���^�C�~���O�ŃQ�[���I�u�W�F�N�g�𔭐�������B
class ObjectSpawn :public Component {
public:
	ObjectSpawn(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~ObjectSpawn() {};
	// �A�^�b�`����Ă���I�u�W�F�N�g�����S��������A����������֐��B
	// �����F	���b��ɕ������邩�B
	//			������̈ʒu(���[���h���W)�B
	//			������̉�](���[���h���W)�B
	//			������̊g�k(���[���h���W)�B
	//			������ɐݒ肷��e(�f�t�H���g��Null)�B
	// ���I�u�W�F�N�g�}�l�[�W���[�ɓo�^����Ă�����̂̂ݗL���B
	template<class T>
	void ObjectSpawn::DeathObject(float wait,const Vector3& pos,const Quaternion& rot,const Vector3& scale,Transform* parent = nullptr) {
		if (_spawner == nullptr) {
			INSTANCE(GameObjectManager)->AddRemoveList(gameObject);
			_spawner = INSTANCE(GameObjectManager)->AddNew<Spawner>("Spawner", gameObject->_Pri);
			_spawner->SpawnStandby(INSTANCE(GameObjectManager)->AddNew<T*>(gameObject->_Name, gameObject->_Pri), wait,pos,rot,scale, parent);
		}
	}
private:
	Spawner* _spawner = nullptr;	// �o�^�����I�u�W�F�N�g�𔭐�������I�u�W�F�N�g�B
};

