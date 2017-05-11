#include "SkinModelManager.h"

map<UINT64, SkinModelData*> SkinModelManager::modelDataMap;

SkinModelData* SkinModelManager::LoadModel(const char* filename)
{
	int hash = Support::MakeHash(filename);
	SkinModelData* output = nullptr;
	//�o�^����ĂȂ�
	if (modelDataMap[hash] == nullptr)
	{
		//����
		SkinModelData* Original = new SkinModelData();
		//�t�@�C���p�X����
		char* filepath = new char[64];
		strcpy_s(filepath,strlen("Asset/Xfile/")+1,"Asset/Xfile/");
		strcat_s(filepath, strlen(filepath) + strlen(filename)+1, filename);
		//�ǂݎ��
		if (Original->LoadModelData(filepath))
		{
			//�o�^
			output = modelDataMap[hash] = Original;
		}
		else
		{
			//SAFE_DELETE(Original);
			//���f���Ȃ������p�̃��f���\��
			output = LoadModel("NonModel.X");
		}
	}
	else
	{
		//�}�b�v����擾
		output = modelDataMap[hash];
	}
	
	return output;
}