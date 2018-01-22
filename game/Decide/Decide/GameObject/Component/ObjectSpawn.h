#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class SplitSpace;


// �I�u�W�F�N�g�������u�B
// �o�^����GameObject����U��A�N�e�B�u�ɂ��A�w�肵���^�C�~���O�ŃA�N�e�B�u��������B
class Spawner : public GameObject {
public:
	Spawner(char* name) : GameObject(name) {
	};
	~Spawner() {};

	// �������u�������B
	// �����F	����������I�u�W�F�N�g�B
	//			���b��ɔ������邩�B
	//			������̈ʒu(���[���h���W)�B
	//			������̉�](���[���h���W)�B
	//			������̊g�k(���[���h���W)�B
	//			������ɐݒ肷��e�B
	// �߂�l�F �����\�������I�u�W�F�N�g�B
	void Create(GameObject* spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent);

	// �X�|�i�[���X�V�B
	// �߂�l�F	�����o�^�����I�u�W�F�N�g���������ꂽ��(true�Ő����ς�)�B
	// ���������u�����݂��邩�A�������u�����݂��邩���R���|�[�l���g���Ŕ��肵�������߁A���̊֐����K�v�B
	bool MyUpdate();

	// �X�|�i�[���X�V�B
	// ���������I�u�W�F�N�g���j�������ƃR���|�[�l���g�͍폜����邽�߁A�폜��̍X�V�������Ϗ����邽�߂ɂ�������K�v�B
	void Update()override;

	inline GameObject* GetSpawnObject()const {
		return _spawnObject;
	}

	inline void SetIsRemoveComponent(bool flg) {
		_isRemoveComponent = flg;
	}

private:
	bool _ClosedUpdate();
private:
	GameObject* _spawnObject = nullptr;	// ����������I�u�W�F�N�g�B
	float _waitTime = 0.0f;	// �҂����ԁB
	float _timeCounter = 0.0f;	// �J�E���^�[�B
	bool _isRemoveComponent = false;	// ���̃N���X�̃C���X�^���X��ێ����Ă���R���|�[�l���g���폜���ꂽ���B
public:
	static vector<SplitSpace*> _splitSpace;	// ��ԕ���(���������G�l�~�[��o�^���邽�߂ɕێ�)�B
};

// ���̃R���|�[�l���g���A�^�b�`����Ǝw�肵���ʒu�Ɏw�肵���^�C�~���O�ŃQ�[���I�u�W�F�N�g�𔭐�������B
class ObjectSpawn :public Component {
public:
	ObjectSpawn(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~ObjectSpawn() {
		_respawner = nullptr;
		for (auto spawner : _spawner) {
			spawner->SetIsRemoveComponent(true);
		}
		_spawner.clear();
	};

	void Update()override;

	// �A�^�b�`����Ă���I�u�W�F�N�g�����S��������A����������֐��B
	// �e���v���[�g�����F	�������������I�u�W�F�N�g�̃N���X��(��{�I�ɃA�^�b�`���ꂽ�Q�[���I�u�W�F�N�g�̃N���X��)�B
	// �����F	���������I�u�W�F�N�g�ւ̃|�C���^������(Null�ȊO�̒l�̓������|�C���^����͂���Ə㏑������A���Ƃ��Ɠ����Ă����A�h���X���̓��X�g���邽�ߒ��ӁA�s�v�ȏꍇ�͒���Null����͂���)�B
	//			���b��ɕ������邩�B
	//			������̈ʒu(���[���h���W)�B
	//			������̉�](���[���h���W)�B
	//			������̊g�k(���[���h���W)�B
	//			������ɐݒ肷��e(�f�t�H���g��Null)�B
	// �߂�l�F	���������I�u�W�F�N�g�ւ̃|�C���^(�������Ɠ���)�B
	// ��GameObjectManager�ɓo�^����Ă��Ȃ���Ύ��S(�폜)���Ȃ��B
	// �����������I�u�W�F�N�g��GameObjectManager�ɓo�^�����B
	template<class T>
	T* ObjectSpawn::DeathAndRespawnObject(GameObject** reSpawn,float wait,const Vector3& pos,const Quaternion& rot,const Vector3& scale,Transform* parent = nullptr) {
		if (_respawner == nullptr) {
			// ���X�|�[������I�u�W�F�N�g�͈�̂Ȃ̂œo�^����Ă��Ȃ������肷��B

			// GameObject���폜(���S)�B
			INSTANCE(GameObjectManager)->AddRemoveList(gameObject);
			// ���X�|�i�[�ɓo�^(�����\��)�B
			_respawner = _CreateSpawner<T>(wait, pos, rot, scale, parent);
			GameObject* work = _respawner->GetSpawnObject();
			if (reSpawn) {
				*reSpawn = work;
			}
			return static_cast<T*>(work);
		}
	}

	// �I�u�W�F�N�g�𐶐��\������֐��B
	// �e���v���[�g�����F	�������������I�u�W�F�N�g�̃N���X���B
	// �����F	�����\�������I�u�W�F�N�g�ւ̃|�C���^������(Null�ȊO�̒l�̓������|�C���^����͂���Ə㏑������A���Ƃ��Ɠ����Ă����A�h���X���̓��X�g���邽�ߒ��ӁA�s�v�ȏꍇ�͒���Null����͂���)�B
	//			���b��ɐ������邩�B
	//			������̈ʒu(���[���h���W)�B
	//			������̉�](���[���h���W)�B
	//			������̊g�k(���[���h���W)�B
	//			������ɐݒ肷��e(�f�t�H���g��Null)�B
	// �߂�l�F	�����\�������I�u�W�F�N�g�ւ̃|�C���^(�������Ɠ���)�B
	// �������\�������I�u�W�F�N�g��GameObjectManager�ɓo�^�����B
	template<class T>
	T* ObjectSpawn::SpawnObject(GameObject** spawn, float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent = nullptr) {
		GameObject* work = _CreateSpawner<T>()->GetSpawnObject();
		if (spawn) {
			*spawn = work;
		}
		return static_cast<T*>(work);
	}

	Spawner* GetReSpawner() {
		return _respawner;
	}
private:
	// �I�u�W�F�N�g�������u�𐶐�����֐��B
	// �e���v���[�g�����F	�������������I�u�W�F�N�g�̃N���X���B
	// �����F	���b��ɔ��������邩�B
	//			������̈ʒu(���[���h���W)�B
	//			������̉�](���[���h���W)�B
	//			������̊g�k(���[���h���W)�B
	//			������ɐݒ肷��e�B
	// �߂�l�F	���������������u�B
	// �������������I�u�W�F�N�g��GameObjectManager�ɓo�^�����B
	template<class T>
	Spawner* ObjectSpawn::_CreateSpawner(float wait, const Vector3& pos, const Quaternion& rot, const Vector3& scale, Transform* parent) {
		Spawner* spawner = INSTANCE(GameObjectManager)->AddNew<Spawner>("Spawner", gameObject->GetPriorty());
		spawner->Create(INSTANCE(GameObjectManager)->AddNew<T>(const_cast<char*>(gameObject->GetName()), gameObject->GetPriorty()), wait, pos, rot, scale, parent);
		_spawner.push_back(spawner);	// �X�|�[�����X�g�ɒǉ��B
		return spawner;
	}

private:
	vector<Spawner*> _spawner;	// �������u���X�g�B
	Spawner* _respawner = nullptr;		// �������u�͂ЂƂB
};

