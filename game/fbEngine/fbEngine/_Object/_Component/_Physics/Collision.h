#pragma once
#include "_Object\_Component\Component.h"
#include "_Include\CollisionEnum.h"

class ModelObject;
//�����蔻��̊��N���X
class Collision:public Component
{
public:
	enum class CollisionObjectType{Rigid = 0,Ghost};
	enum class PhysicsType{Dynamic = 0,Kinematick,Static};
#ifdef _DEBUG
	static const wchar_t* TypeName[];
#endif
public:
	Collision(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	Collision(GameObject* g, Transform* t,const char* classname) :
		Component(g, t, classname)
	{
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	virtual ~Collision();
	void Awake()override;
	virtual void Update()override;

	virtual void OnDestroy()override;
	//�R���W��������
	//������ btCollisionObject* �o���b�g�t�B�W�b�N�X�̃R���W�����I�u�W�F�N�g
	//������ Collider* �����蔻��̌`��
	//��O���� const int �R���W�����ɐݒ肷��ID
	//��l���� Vector3& ��_����̈ړ���
	//��܈��� bool �������Ƀ��[���h�ɒǉ����邩�B
	void Create(btCollisionObject* collision, Collider* shape, const int& id = static_cast<int>(fbCollisionAttributeE::ALL),Vector3 offset = Vector3::zero,bool isAddWorld = true);
	btCollisionObject* GetCollisionObj() const
	{
		return _CollisionObject.get();
	}
	// �V�F�A�[�h�|�C���^�Ŏ擾�B
	// �R���W�������폜���ꂽ�Ƃ���Null�`�F�b�N�������Ƃ��ȂǂɃV�F�A�[�h�|�C���^�ŃR���W�����I�u�W�F�N�g��ۊǂ��Ă����Ƃ����B
	// ��Null�`�F�b�N�̎d���͉��L�B
	// if(static_cast<Collision*>(_CollisionObject->getUserPointer())->gameObject) : �R���W�������A�^�b�`���ꂽ�Q�[���I�u�W�F�N�g��Null�`�F�b�N�B
	// if(_CollisionObject->getUserPointer()) : �R���W������Null�`�F�b�N�B
	shared_ptr<btCollisionObject> GetCollisionObj_shared()const {
		return _CollisionObject;
	}
	//�I�t�Z�b�g������̃|�W�V�����擾
	const Vector3 GetOffsetPos()
	{
		return transform->GetPosition() + _Offset;
	}

	//Create���O�ɐݒ肵�Ƃ��āB
	// �S���C���[�}�X�N�I�t�B
	// ���ׂĂ̏Փ˂𖳎��B
	inline void FilterMask_AllOff() {
		SetFilterMask(0x00000000);
	}

	// �S���C���[�}�X�N�I���B
	// ���ׂẴR���W�����Ɠ����蔻����s���B
	inline void FilterMask_AllOn() {
		SetFilterMask(static_cast<int>(fbCollisionAttributeE::ALL));
	}

	// �t�B���^�[�}�X�N�ɉ��Z�B
	inline void AddFilterMask(short bit) {
		SetFilterMask(_FilterMask | bit);
	}

	// �t�B���^�[�}�X�N���猸�Z�B
	inline void SubFilterMask(short bit) {
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		bit = ~bit;
		SetFilterMask(_FilterMask & bit);
	}

	void SetFilter(short group, short mask);
	void SetFilterGroup(short group);
	void SetFilterMask(short mask);
	inline short GetFilterGroup()const {
		return _FilterGroup;
	}
	inline short GetFilterMask()const {
		return _FilterMask;
	}
	// ���̃R���W�����������ɐݒ肵��������ԋp�B
	inline int GetID()const {
		return _CollisionObject->getUserIndex();
	}

	// ���̂��X���[�v��Ԃɂ���B
	// ������������͂������鏈���Ȃǂ������������B
	inline void Sleep() {
		if (_MyObjectType == CollisionObjectType::Rigid) {
			static_cast<btRigidBody*>(_CollisionObject.get())->wantsSleeping();
		}
	}
	// �R���W�������A�N�e�B�u��Ԃɂ���B
	// ������������͂������鏈���Ȃǂ��L���������B
	inline void Activate() {
		_CollisionObject->activate();
	}

	inline bool GetIsActive()const {
		return _CollisionObject->getActivationState();
	}

	// �p����ɂ���ĈقȂ鏈���B
	virtual void _AddWorldSubClass() = 0;
	// ���[���h�ɓo�^�B
	void AddWorld(); 
	// �p����ɂ���ĈقȂ鏈���B
	virtual void _RemoveWorldSubClass() = 0;
	// ���[���h����폜�B
	void RemoveWorld();
	// ���I���̂ɂ���B
	inline void OnDynamic()
	{
		_physicsType = PhysicsType::Dynamic;
	}
	// �L�l�}�e�B�b�N���̂ɂ���B
	inline void OnKinematick()
	{
		_physicsType = PhysicsType::Kinematick;
	}
	// �ÓI���̂ɂ���B
	inline void OnStatic()
	{
		_physicsType = PhysicsType::Static;
	}
	inline PhysicsType GetPhysicsType()const {
		return _physicsType;
	}
	inline CollisionObjectType GetCollisionType()const {
		return _MyObjectType;
	}
	inline const Vector3& GetOffset()const {
		return _Offset;
	}

	inline const Collider& GetShape()const {
		return *_Shape;
	}

	inline bool GetIsAddWorld()const {
		return _isAddWorld;
	}

protected:
	//�R���W�����̈ʒu���]���X�V
	void _UpdateCollisionTrans();
protected:
	//�|�W�V��������̃I�t�Z�b�g��
	Vector3 _Offset;
	//�����̑�����O���[�v
	short _FilterGroup;
	//�����蔻����Ƃ�O���[�v
	short _FilterMask;
	//�R���W�����̌`��B
	Collider *_Shape;
	//�R���W�����I�u�W�F�N�g�B
	std::shared_ptr<btCollisionObject>	_CollisionObject;
	bool _isAddWorld = false;	// ���[���h�ɒǉ��������B
	CollisionObjectType _MyObjectType;	// ���̂��S�[�X�g���B
	PhysicsType _physicsType;
};