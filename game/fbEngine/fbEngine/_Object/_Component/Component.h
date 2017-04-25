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

	/*virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void PreRender() {};
	virtual void Render() {};*/

	//�R���|�[�l���g��L���ɂ���
	bool enable;
	//�������A�^�b�`���Ă���Q�[���I�u�W�F�N�g�̃A�h���X
	GameObject* gameObject;
	//gameObject�̃g�����X�t�H�[���̃A�h���X
	Transform* transform;
private:
	//���s����(���s���ɂ����ݒ�ł��Ȃ�)
	int _ExecutionOrder;
};