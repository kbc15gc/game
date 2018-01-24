#pragma once
#include "_Object/Noncopyable.h"
#include "Component.h"

//コンポーネントの管理
class ComponentManager :Noncopyable
{
public:
	~ComponentManager();

	void PreUpdate()const;
	void Update()const;
	void LateUpdate()const;
#ifdef _DEBUG
	void Debug()const;
#endif
	void PreRender()const;
	void Render()const;
	void PostRender()const;
	void ImageRender()const;
	void OnDestroy()const;	// アタッチしているGameObjectが削除リストに積まれたら呼ばれる処理。

	//コンポーネント型のみを追加したい・・・。
	template<class T>
	T* AddComponent(GameObject* g,Transform* t)
	{
		T* component = new T(g, t);
		_Components.push_back(component);
		//sortする
		std::stable_sort(_Components.begin(), _Components.end());
		component->Awake();
		return component;
	}

	//不要？？？
	void AddComponent(Component* c)
	{
		_Components.push_back(c);
	}

	Component* GetComponent(const char* name);

	template <class T>
	T* GetComponent();

	template <class T>
	unique_ptr<vector<T*>> GetComponents();

#ifdef _DEBUG
	const vector<Component*>& GetComponentAll() const{
		return _Components;
	}
#endif

	template <class T>
	void RemoveComponent()
	{
		char* name = (char*)typeid(T*).name();

		vector<Component*>::iterator it = _Components.begin();
		while (it != _Components.end())
		{
			if (strcmp((*it)->GetName(), name) == 0)
			{
				//メモリ解放。
				SAFE_DELETE(*it);
				_Components.erase(it);
				return;
			}
			it++;
		}
	}
	//全てのコンポーネントの解放
	void Release();
private:
	//コンポーネントたち
	vector<Component*> _Components;
};

template<class T>
inline T * ComponentManager::GetComponent()
{
	//テンプレート型の内部の型情報取得
	const type_info& Ttype = typeid(T);

	//foreachは内部で値をコピーしているので、ポインタを返すなら使えない。
	vector<Component*>::const_iterator it = _Components.cbegin();
	while (it != _Components.end())
	{
		//コンポーネント型の内部の型情報取得
		const type_info& type = typeid(*(*it));
		//型情報比較
		if (Ttype == type)
		{
			return (T*)*it;
		}
		it++;
	}

	return nullptr;
}

template<class T>
inline unique_ptr<vector<T*>> ComponentManager::GetComponents()
{
	//テンプレート型の内部の型情報取得
	const type_info& Ttype = typeid(T);
	unique_ptr<vector<T*>> Tmp(new vector<T*>());

	//foreachは内部で値をコピーしているので、ポインタを返すなら使えない。
	vector<Component*>::const_iterator it = _Components.cbegin();
	while (it != _Components.end())
	{
		//コンポーネント型の内部の型情報取得
		const type_info& type = typeid(*(*it));
		//型情報比較
		if (Ttype == type)
		{
			Tmp->push_back((T*)*it);
		}
		it++;
	}

	return move(Tmp);

	////配列。
	//int size = Tmp.size();
	//if (size > 0)
	//{
	//	T** Array = new T*[size];
	//	for (size_t i = 0; i < Tmp.size(); i++)
	//	{
	//		Array[i] = Tmp[i];
	//	}

	//	return Array;
	//}
	return nullptr;
}
