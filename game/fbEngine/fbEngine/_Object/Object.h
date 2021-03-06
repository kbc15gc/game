#pragma once
#include "_Object/Noncopyable.h"
//オブジェクトの基底クラス
class Object : Noncopyable
{
public:
	Object()
	{
		_Name = "Empty";
	}
	Object(const char* name)
	{
		_Name = new char[strlen(name) + 1];
		//文字列コピー
		strcpy(_Name, name);
	}
	virtual ~Object()
	{
		SAFE_DELETE(_Name);
	};

	//インスタンスが作成された時に実行される
	//純粋仮想関数ではないので絶対オーバーライドしなくてもいい
	virtual void Awake(){};

	//最初の一回のアップデートで呼ばれる。
	//初期化を行う純粋仮想関数
	virtual void Start(){};

	//更新の前に呼び出される関数。
	virtual void PreUpdate() {};

	//更新を行う純粋仮想関数
	virtual void Update(){};

	//更新の後に呼び出される関数
	//純粋仮想関数ではないので絶対オーバーライドしなくてもいい
	virtual void LateUpdate(){};

	//内包されたアップデート。
	void ConnotePreUpdate()
	{
		//最初の一回ならStartを呼び出す。
		if (_FirstUpdate)
		{
			Start();
			_FirstUpdate = false;
		}
		else
			PreUpdate();
	}

	void ConnoteUpdate()
	{
		// Startが呼ばれるまでは無視。
		if (!_FirstUpdate)
		{
			Update();
		}
	}

	void ConnoteLateUpdate()
	{
		// Startが呼ばれるまでは無視。
		if (!_FirstUpdate)
		{
			LateUpdate();
		}
	}

#ifdef _DEBUG
	// デバッグ用のコードを呼び出す。
	// オブジェクトのアクティブ非アクティブに関わらず呼び出される。
	virtual void Debug() {};
#endif

	//描画の前に呼び出されます
	virtual void PreRender(){};
	//描画を行う
	virtual void Render(){};
	//ポストエフェクト描画
	virtual void PostRender(){};
	//イメージ(2D)描画
	virtual void ImageRender(){};

	// マネージャーに登録している場合、削除リストに積まれた瞬間に呼び出される処理。
	// ※デストラクタとは別。
	// ※デストラクタよりも前に呼ばれる。
	virtual void OnDestroy() {};

	//[有効/アクティブ]になった時に呼ばれる関数。
	virtual void OnEnable() {};

	//[無効/非アクティブ]になった時に呼ばれる関数。
	virtual void OnDisable() {};

	const char* GetName() const
	{
		return _Name;
	}
protected:
	//オブジェクトの名前
	char* _Name;
private:
	//最初の一回。
	bool _FirstUpdate = true;
};