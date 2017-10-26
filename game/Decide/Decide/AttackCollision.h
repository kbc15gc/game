#pragma once
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "GameObject\Component\CharacterParameter.h"

class AttackCollision : public GameObject
{
public:
	enum class CollisionMaster{Player = 0,Enemy, Other};	// �N���R���W�����𔭐����������B

	struct HitObjectInfo {
		GameObject* object = nullptr;		// �Փ˂����I�u�W�F�N�g�B
		shared_ptr<btCollisionObject> coll;
		bool isCallStay = false;
	};
public:
	AttackCollision(const char * name) :GameObject(name)
	{

	};
	~AttackCollision() {
		_hitInfos.clear();
	}

	void Awake()override;
	void Update()override;
	void LateUpdate()override;

	// �U������p�̃R���W���������B
	// �����F	�R���W��������҂ɂ��_���[�W���B
	//			�ʒu(���[�J�����W)�B
	//			��](���[�J�����W�A�N�H�[�^�j�I��)�B
	//			�T�C�Y�B
	//			�N���R���W�����𐶐��������B
	//			�R���W��������(0.0f��菬�����l�Ŗ���)�B
	//			�R���W���������҂�����(���̊֐����Ă΂�Ă��牽�b��ɃR���W�����������邩)�B
	//			�e�ɂ�����Transform���(�������Ȃǂ̏�ŃR���W���������������ꍇ�Ɏg�p)�B
	void Create(unique_ptr<CharacterParameter::DamageInfo> info, const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master = CollisionMaster::Other, float lifeTime = -1.0f,float waitTime = 0.0f, Transform* Parent = nullptr);

	inline void SetParent(Transform* Parent) {
		transform->SetParent(Parent);
	}
	inline void RemoveParent() {
		transform->SetParent(nullptr);
	}
	//�N�������������R���W���������Q�b�g�B
	inline CollisionMaster GetMaster()  const
	{
		return _master;
	}

	CharacterParameter::DamageInfo* GetDamageInfo()
	{
		return _DamageInfo.get();
	}

	inline GostCollision* GetGostCollision()const {
		return _Gost;
	}
private:	
	// �U���R���W���������֐��B
	void CreateCollision();
	// �Փˌ��o�B
	void DetectionCollision();
	// �Փ˂����u�ԌĂԃR�[���o�b�N�����B
	void _CallBackEnter(btCollisionObject* coll);
	// �Փ˂��Ă���ԌĂё�����R�[���o�b�N�����B
	void _CallBackStay(GameObject* coll);
	// �Փ˂����u�ԌĂԃR�[���o�b�N�����B
	void _CallBackExit(GameObject* coll);
	// �R���W�����I�u�W�F�N�g����Q�[���I�u�W�F�N�g���擾����B
	GameObject* _CollisionObjectToGameObject(btCollisionObject* coll) {
		if (!coll) {
			return nullptr;
		}
		Collision* Coll = static_cast<Collision*>(coll->getUserPointer());
		if (Coll) {
			return Coll->gameObject;
		}
		else {
			// �R���W�����R���|�[�l���g���Ȃ��B
			return nullptr;
		}
	}

private:
	Collider* _Colider = nullptr;	// �R���W�����`��B
	GostCollision* _Gost = nullptr;	// �S�[�X�g�B
	float _time;				//��Ɨp�J�E���^�[�B
	float _waitTime = 0.0f;		// �R���W���������҂����ԁB
	float _lifeTime = -1.0f;		// �R���W��������(0.0f��菬�����l�Ŗ���)�B
	CollisionMaster _master;	// �N�������������R���W�������B
	vector<unique_ptr<HitObjectInfo>> _hitInfos;	// �������Ă���I�u�W�F�N�g���B
	bool _isCreateCollision = false;	// �R���W�����𐶐��������B
	unique_ptr<CharacterParameter::DamageInfo> _DamageInfo;
};