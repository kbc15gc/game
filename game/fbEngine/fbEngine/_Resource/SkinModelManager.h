#pragma once

class SkinModelManager
{
public:	
	//3D���f����ǂݍ���,���f���̃f�[�^�l��Ԃ�
	//�������Fstring �t�@�C���̖��O
	static SkinModelData* LoadModel(const char* filename);
private:
	static map<UINT64, SkinModelData*> modelDataMap;
};