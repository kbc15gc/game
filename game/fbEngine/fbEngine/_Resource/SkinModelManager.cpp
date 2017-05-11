#include "SkinModelManager.h"

map<UINT64, SkinModelData*> SkinModelManager::modelDataMap;

SkinModelData* SkinModelManager::LoadModel(const char* filename)
{
	int hash = Support::MakeHash(filename);
	SkinModelData* output = nullptr;
	//登録されてない
	if (modelDataMap[hash] == nullptr)
	{
		//生成
		SkinModelData* Original = new SkinModelData();
		//ファイルパス生成
		char* filepath = new char[64];
		strcpy_s(filepath,strlen("Asset/Xfile/")+1,"Asset/Xfile/");
		strcat_s(filepath, strlen(filepath) + strlen(filename)+1, filename);
		//読み取り
		if (Original->LoadModelData(filepath))
		{
			//登録
			output = modelDataMap[hash] = Original;
		}
		else
		{
			//SAFE_DELETE(Original);
			//モデルなかった用のモデル表示
			output = LoadModel("NonModel.X");
		}
	}
	else
	{
		//マップから取得
		output = modelDataMap[hash];
	}
	
	return output;
}