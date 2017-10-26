#pragma once

#include "fbEngine\fbstdafx.h"
#include "GameObject\Component\CharacterParameter.h"
#include "GameObject\Enemy\EnemyCharacter.h"

class SplitSpace;

namespace {

	// CSV����ǂݍ��ރG�l�~�[�̃f�[�^�`���B
	struct EnemyInfo : Noncopyable {
		int type;		// ��ʁB
		//char fileName[FILENAME_MAX];	// ���f���f�[�^�̃t�@�C�����B
		int param[CharacterParameter::Param::MAX];	// �e��p�����[�^�B
		int exp;	// ���Ƃ��o���l�B
		int money;	// ���Ƃ����z�B
		Vector3 position;	// �ʒu�B
		Quaternion rotation;	// ��]�B
		Vector3 scale;	// �g�k�B
		int item[5];
		int armor[5];
		int weapon[5];
	};

	//EnemyInfo�\���̂̍\���t�H�[�}�b�g(�����o�ϐ�)�B
	// ��1�̃��R�[�h�͈ȉ��̗v�f�ō\�������B
	// �v�f�F	�ϐ����B
	//			�f�[�^�^�B
	//			�f�[�^�̐擪���猩���e�v�f�̈ʒu(�o�C�g)�B
	//			�v�f����̃T�C�Y(�o�C�g)�B
	const Support::DATARECORD EnemyInfoDecl[] =
	{
		{ "type",Support::DataTypeE::INT, offsetof(struct EnemyInfo,type),	sizeof(EnemyCharacter::EnemyType) },
		{ "param",	Support::DataTypeE::INTARRAY, offsetof(struct EnemyInfo,param),	sizeof(EnemyInfo::param) },
		{ "exp",	Support::DataTypeE::INT, offsetof(struct EnemyInfo,exp),	sizeof(int) },
		{ "money",	Support::DataTypeE::INT, offsetof(struct EnemyInfo,money),	sizeof(int) },
		{ "position",	Support::DataTypeE::VECTOR3, offsetof(struct EnemyInfo,position),	sizeof(Vector3) },
		{ "rotation",	Support::DataTypeE::QUATERNION, offsetof(struct EnemyInfo,rotation),	sizeof(Quaternion) },
		{ "scale",	Support::DataTypeE::VECTOR3, offsetof(struct EnemyInfo,scale),	sizeof(Vector3) },
		{ "item",	Support::DataTypeE::INTARRAY, offsetof(struct EnemyInfo,item),	sizeof(EnemyInfo::item) },
		{ "armor",	Support::DataTypeE::INTARRAY, offsetof(struct EnemyInfo,armor),	sizeof(EnemyInfo::armor) },
		{ "weapon",	Support::DataTypeE::INTARRAY, offsetof(struct EnemyInfo,weapon),	sizeof(EnemyInfo::weapon) },
	};
}


class EnemyManager {
private:
	// �I�u�W�F�N�g�Ǘ��p�\���́B
	struct ManagingData {
		//ManagingData(EnemyCharacter* obj,  unique_ptr<EnemyInfo> info) {
		//	Object = obj;
		//	InfoData = move(info);	// ���j�[�N�|�C���^�̏��L�������n�B
		//}
		EnemyCharacter* Object = nullptr;	// InfoData�����Ƃɐ������ꂽ�I�u�W�F�N�g�f�[�^�B
		unique_ptr<EnemyInfo> InfoData;	// CSV�t�@�C������ǂݍ��񂾐ݒ�f�[�^�B
	};
private:
	EnemyManager();
	~EnemyManager();
public:
	
	// �C���X�^���X�̎擾.
	static EnemyManager* Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new EnemyManager();
		}
		return _instance;
	}

	// ������.
	void Start();

	// �G�l�~�[�̈ʒu�f�[�^�Ȃǂ��O���t�@�C������ǂݍ���ŕۑ��B
	void LoadEnemyOrigin();
	
	// �ǂݍ��񂾃G�l�~�[��GameObjectManager�ɒǉ��B
	void CreateEnemy();

	// �G�l�~�[���S�֐��B
	// ���X�|�i�[�R���|�[�l���g������Ύ����Ń��X�|�[������B
	void DeathEnemy(EnemyCharacter* object);


private:
	vector<ManagingData*> _enemys;
private:
	static EnemyManager* _instance;
};

