#pragma once
#include "_Object\Object.h"
#include "_Object\_Component\ComponentManager.h"

class Component;

//�Q�[���I�u�W�F�N�g�̊�b�N���X
class GameObject : public Object
{
public:
	GameObject();
	GameObject(const char* name);
	virtual ~GameObject();

	/*virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void PreRender() {};
	virtual void Render() {};*/

	//�R���|�[�l���g�ǉ�
	template<class T>
	T* AddComponent()
	{
		return _Components.AddComponent<T>(this,transform);
	}
	//�R���|�[�l���g�ǉ�
	void AddComponent(Component* c)
	{
		return _Components.AddComponent(c);
	}
	//���O����R���|�[�l���g�擾
	Component* GetComponent(const char* name)
	{
		return _Components.GetComponent(name);
	}
	//�R���|�[�l���g�擾
	template <class T>
	T* GetComponent()
	{
		return _Components.GetComponent<T>();
	}

	//�Q�[���I�u�W�F�N�g�}�l�[�W���ł����g��Ȃ��B(�ǂ��ɂ����ׂ�)
	const ComponentManager& GetComponentManager()
	{
		return _Components;
	}

	//�I�u�W�F�N�g�̃A�N�e�B�u�t���O��ݒ肷��@�Z�b�^�[
	virtual void SetActive(const bool& act)
	{
		_Active = act;
	}

	//�A�N�e�B�u���ǂ����擾�@�Q�b�^�[
	const bool& GetActive()
	{
		return _Active;
	}
	//�j���t���O���Z�b�g����B
	//��������true�ɂ���Ǝq�ɂ܂ŃZ�b�g����B
	void SetDiscard(const bool& b,const bool& child = false)
	{
		_Discard = b;
		if(child)
		{
			for each (Transform* t in transform->GetChildren())
			{
				//�S�Ă̎q�ɐe�Ɠ����l��ݒ�
				t->gameObject->SetDiscard(_Discard);
			}
		}

		
	}

	const bool& GetDiscard()
	{
		return _Discard;
	}
public:
	//�g�����X�t�H�[��(�ȒP�ɃA�N�Z�X�����������B)
	Transform* transform;	
protected:
	//�R���|�[�l���g����
	ComponentManager _Components;
	//�A�N�e�B�u�łȂ��I�u�W�F�N�g�͕`����A�b�v�f�[�g������Ȃ�
	bool _Active;
	//�I�u�W�F�N�g��j������
	bool _Discard;
};