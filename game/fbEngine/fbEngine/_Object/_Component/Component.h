#pragma once
#include "_Object\Object.h"

class GameObject;
class Transform;

class Component : public Object
{
public:
	Component(GameObject* g, Transform* t,const char* classname,int order = 1):Object(classname)
	{
		enable = true;
		gameObject = g;
		transform = t;
		_ExecutionOrder = order;
	}
	virtual ~Component() {};

	bool operator<(const Component& right)
	{
		return (this->_ExecutionOrder > right._ExecutionOrder);
	}	

#ifdef _DEBUG
	inline wchar_t* GetDebugName() {
		return name;
	}
#endif

	/*virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void PreRender() {};
	virtual void Render() {};*/

	//�L���t���O�̐ݒ�B
	//[in] �t���O�B
	virtual void SetEnable(const bool flg)
	{
		//�؂�ւ�������ɌĂяo���B�B
		if ((enable != flg) && flg)
			OnEnable();
		else
			OnDisable();

		enable = flg;
	}

	bool GetEnable()
	{
		return enable;
	}

	//�������A�^�b�`���Ă���Q�[���I�u�W�F�N�g�̃A�h���X
	GameObject* gameObject;
	//gameObject�̃g�����X�t�H�[���̃A�h���X
	Transform* transform;
private:
	//�R���|�[�l���g�̗L���t���O�B
	bool enable;
	//���s����(���s���ɂ����ݒ�ł��Ȃ�)
	int _ExecutionOrder;

#ifdef _DEBUG
protected:
	wchar_t name[FILENAME_MAX];
#endif
};