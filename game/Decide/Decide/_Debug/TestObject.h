/**
* テストオブジェクトクラスの定義.
*/
#pragma once

/**
* テストオブジェクトクラス.
*/
class TestObject : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	TestObject(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~TestObject()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

private:

};