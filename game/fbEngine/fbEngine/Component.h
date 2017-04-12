#pragma once
#include "Object.h"

//�O����`
class GameObject;
class Transform;

//�R���|�[�l���g�̊��N���X

class Component : public Object
{
public:
	Component(GameObject* g, Transform* t,const char* classname):Object(classname)
	{
		enable = true;
		gameObject = g;
		transform = t;
	}
	virtual ~Component() {};
	

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
};