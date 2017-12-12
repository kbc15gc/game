/**
* セーブデータ管理クラスの定義.
*/
#pragma once

/**
* セーブデータ管理クラス.
*/
class SaveDataManager
{
private:

	/**
	* コンストラクタ.
	*/
	SaveDataManager()
	{
	}
	
	/**
	* デストラクタ.
	*/
	~SaveDataManager()
	{
	}

public:

	/**
	* インスタンスを取得.
	*/
	static SaveDataManager& Instance()
	{
		static SaveDataManager instance;
		return instance;
	}

private:

	/** コンティニューフラグ. */
	bool _IsContinue = false;

};
