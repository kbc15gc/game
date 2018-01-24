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

	//有効フラグの設定。
	//[in] フラグ。
	virtual void SetEnable(const bool flg)
	{
		//切り替わった時に呼び出す。。
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

	//自分がアタッチしているゲームオブジェクトのアドレス
	GameObject* gameObject;
	//gameObjectのトランスフォームのアドレス
	Transform* transform;
private:
	//コンポーネントの有効フラグ。
	bool enable;
	//実行順番(実行時にしか設定できない)
	int _ExecutionOrder;

#ifdef _DEBUG
protected:
	wchar_t name[FILENAME_MAX];
#endif
};