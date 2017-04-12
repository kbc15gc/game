#pragma once
#include "Component.h"

//�R���|�[�l���g�̊Ǘ�
class ComponentManager :Noncopyable
{
public:
	~ComponentManager();
	void Start()const;
	void Update()const;
	void LateUpdate()const;
	void PreRender()const;
	void Render()const;
	void PostRender()const;
	void ImageRender()const;

	//�R���|�[�l���g�^�݂̂�ǉ��������E�E�E�B
	template<class T>
	T* AddComponent(GameObject* g,Transform* t)
	{
		T* component = new T(g, t);
		_Components.push_back(component);
		component->Awake();
		return component;
	}

	//�s�v�H�H�H
	void AddComponent(Component* c)
	{
		_Components.push_back(c);
	}

	Component* GetComponent(const char* name);

	template <class T>
	T* GetComponent();

	template <class T>
	void RemoveComponent()
	{
		char* name = (char*)typeid(T).name();

		vector<Component*>::iterator it = _Components.begin();
		while (it != _Components.end())
		{
			if (strcmp((*it)->GetName(), name) == 0)
			{
				SAFE_DELETE(*it);
				_Components.erase(it);
				return;
			}
			it++;
		}
	}
	//�S�ẴR���|�[�l���g�̉��
	void Release();
private:
	//�R���|�[�l���g����
	vector<Component*> _Components;
};

template<class T>
inline T * ComponentManager::GetComponent()
{
	//�e���v���[�g�^�̓����̌^���擾
	const type_info& Ttype = typeid(T);

	//foreach�͓����Œl���R�s�[���Ă���̂ŁA�|�C���^��Ԃ��Ȃ�g���Ȃ��B
	vector<Component*>::const_iterator it = _Components.cbegin();
	while (it != _Components.end())
	{
		//�R���|�[�l���g�^�̓����̌^���擾
		const type_info& type = typeid(*(*it));
		//�^����r
		if (Ttype == type)
		{
			return (T*)*it;
		}
		it++;
	}

	return nullptr;
}