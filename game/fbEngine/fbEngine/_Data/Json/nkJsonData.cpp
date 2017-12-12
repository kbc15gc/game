/**
* JsonDataを管理するクラスの実装.
*/
#include"fbstdafx.h"
#include"nkJsonData.h"

/**
* 読み込み.
*
* @param filename	ファイル名.
*
* @return true or false.
*/
bool JsonData::Load(string filename)
{
	ifstream file_in;
	file_in.open("C:\\KBCGame\\ReHistory\\SaveData\\" + filename + ".json");

	if (file_in.fail())
	{
		return false;
	}

	picojson::value DataValue;

	file_in >> DataValue;

	DataObject_ = DataValue.get<picojson::object>();

	return true;
}

/**
* 書き込み.
*
* @param filename	ファイル名.
*/
void JsonData::Save(string filename)
{
	picojson::value DataValue(DataObject_);

	ofstream file_out;
	file_out.open("C:\\KBCGame\\ReHistory\\SaveData\\" + filename + ".json");

	file_out << DataValue.serialize(true);
}

	