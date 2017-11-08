/**
* JsonDataを管理するクラスの定義.
*/
#pragma once


/**
* JsonDataを管理するクラス.
* シングルトンクラス.
*/
class JsonData : Noncopyable
{
public:

	/**
	* コンストラクタ
	*/
	JsonData()
	{
	}

	/**
	* デストラクタ.
	*/
	~JsonData()
	{
	}

	/**
	* 読み込み.
	* Asset\Data\SaveDataフォルダ内のjsonファイル.
	*
	* @param filename	ファイル名.
	*
	* @return true or false.
	*/
	bool Load(string filename);

	/**
	* 書き込み.
	*
	* @param filename	ファイル名.
	*/
	void Save(string filename);

	/**
	* データオブジェクトの取得.
	*
	* @return オブジェクト.
	*/
	picojson::object& GetDataObject()
	{
		return DataObject_;
	}

	/**
	* データオブジェクトの取得.
	*
	* @param key	キー.
	*
	* @return オブジェクト.
	*/
	picojson::object& GetDataObject(string key)
	{
		if (DataObject_[key].is<picojson::object>())
		{
			return DataObject_[key].get<picojson::object>();
		}
		FB_ASSERT(false, "データが存在していません");
	}

	/**
	* データオブジェクトの取得.
	*
	* @param key	キー.
	*
	* @return オブジェクト.
	*/
	picojson::array& GetDataArray(string key)
	{
		if (DataObject_[key].is<picojson::array>())
		{
			return DataObject_[key].get<picojson::array>();
		}
		FB_ASSERT(false, "データが存在していません");
	}

	/**
	* オブジェクトを登録する.
	*
	* @param key	キー.
	* @param obj	オブジェクト.
	*/
	void SetDataObject(string key, picojson::object& obj)
	{
		DataObject_[key] = picojson::value(obj);
	}

	/**
	* オブジェクトを登録する.
	*
	* @param key	キー.
	* @param arr	配列.
	*/
	void SetDataObject(string key, picojson::array& arr)
	{
		DataObject_[key] = picojson::value(arr);
	}

private:

	/** データオブジェクト. */
	picojson::object DataObject_;

};

