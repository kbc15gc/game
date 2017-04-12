#include "SkinModelManager.h"
#include "SkinModelData.h"

map<UINT64, SkinModelData*> SkinModelManager::modelDataMap;

SkinModelData* SkinModelManager::LoadModel(const char* filename)
{
	int hash = Support::MakeHash(filename);
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
		Original->LoadModelData(filepath);
		//�o�^
		modelDataMap[hash] = Original;
	}
	//�l�����������Ă���
	return modelDataMap[hash];
}