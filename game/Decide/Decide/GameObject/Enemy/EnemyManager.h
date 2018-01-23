#pragma once

#include "fbEngine\fbstdafx.h"
#include "GameObject\Component\CharacterParameter.h"
#include "GameObject\Enemy\EnemyCharacter.h"

class SplitSpace;

namespace LoadEnemyInfo{

	const int dropMax = 5;
	const int ColorNum = 4;

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
		int item[dropMax];
		int armor[dropMax];
		int weapon[dropMax];
		float probability[dropMax * 3];	//�h���b�v�m��*3���B
		int colorflag;
		Vector4 color;
		float viewAngle;
		float viewRange;
		float wanderingRange;
		float discoveryRange;
		float respawnTime;
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
		{ "param",	Support::DataTypeE::INT_ARRAY, offsetof(struct EnemyInfo,param),	sizeof(EnemyInfo::param) },
		{ "exp",	Support::DataTypeE::INT, offsetof(struct EnemyInfo,exp),	sizeof(int) },
		{ "money",	Support::DataTypeE::INT, offsetof(struct EnemyInfo,money),	sizeof(int) },
		{ "position",	Support::DataTypeE::VECTOR3, offsetof(struct EnemyInfo,position),	sizeof(Vector3) },
		{ "rotation",	Support::DataTypeE::QUATERNION, offsetof(struct EnemyInfo,rotation),	sizeof(Quaternion) },
		{ "scale",	Support::DataTypeE::VECTOR3, offsetof(struct EnemyInfo,scale),	sizeof(Vector3) },
		{ "item",	Support::DataTypeE::INT_ARRAY, offsetof(struct EnemyInfo,item),	sizeof(EnemyInfo::item) },
		{ "armor",	Support::DataTypeE::INT_ARRAY, offsetof(struct EnemyInfo,armor),	sizeof(EnemyInfo::armor) },
		{ "weapon",	Support::DataTypeE::INT_ARRAY, offsetof(struct EnemyInfo,weapon),	sizeof(EnemyInfo::weapon) },
		{ "probability",	Support::DataTypeE::FLOAT_ARRAY, offsetof(struct EnemyInfo,probability),	sizeof(EnemyInfo::probability) },
		{ "colorflag",Support::DataTypeE::INT, offsetof(struct EnemyInfo,colorflag),	sizeof(EnemyInfo::colorflag) },
		{ "color",	Support::DataTypeE::VECTOR4, offsetof(struct EnemyInfo,color),	sizeof(Vector4) },
		{ "viewAngle",	Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,viewAngle),	sizeof(EnemyInfo::viewAngle) },
		{ "viewRange",	Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,viewRange),	sizeof(EnemyInfo::viewRange) },
		{ "wanderingRange",	Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,wanderingRange),	sizeof(EnemyInfo::wanderingRange) },
		{ "discoveryRange",Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,discoveryRange),	sizeof(EnemyInfo::discoveryRange) },
		{ "respawnTime",	Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,respawnTime),	sizeof(EnemyInfo::respawnTime) },

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
		unique_ptr<LoadEnemyInfo::EnemyInfo> InfoData;	// CSV�t�@�C������ǂݍ��񂾐ݒ�f�[�^�B
		Spawner* Respawner = nullptr;	// �G�l�~�[�𕜊������邽�߂̃X�|�i�[�B
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
	
	// �e�[�u���ɓo�^����Ă���������ƂɃG�l�~�[���쐬�B
	// �����F	�ǂ̏ꏊ�̃G�l�~�[���쐬���邩�B
	//			�G�l�~�[���̔z��B
	void CreateEnemys(LocationCodeE location,vector<unique_ptr<LoadEnemyInfo::EnemyInfo>>& infos);

	// �G�l�~�[���S�֐��B
	// ���X�|�i�[�R���|�[�l���g������Ύ����Ń��X�|�[������B
	void DeathEnemy(EnemyCharacter* object);


private:
	vector<vector<unique_ptr<ManagingData>>> _enemys;	// �G�l�~�[�B
private:
	static EnemyManager* _instance;
};

