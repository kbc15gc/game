#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class CharacterParameter :public Component {
public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
	};
	~CharacterParameter() {
	};
public:
	// ���f����(�΁A���Ȃ�)�B
	enum class Element{None = 0};
	// ��������(�a�A�ŁA���Ȃ�)�B
	enum class Physical{None = 0};
	// �p�����[�^�[�񋓁B
	enum Param { MIN = -1,HP = 0, MAXHP, MP, MAXMP, ATK, DEF, DEX, AGI, MAX };
	//�������B
	void ParamInit(int hp,int maxhp,int mp,int maxmp, int atk, int def, int dex, int agi);
	
	void Update()override;

	// �w�肵���p�����[�^�[�ɉ��Z�B
	// �����F	�p�����[�^�[�^�C�v�B
	//			���Z�ʁB
	inline void AddParam(Param idx,const int add) {
		_OutCheck(idx);
		_Param[idx] += add;
	}
	// �w�肵���p�����[�^�[���猸�Z�B
	// �����F	�p�����[�^�[�^�C�v�B
	//			���Z�ʁB
	inline void SubParam(Param idx, const int sub) {
		_OutCheck(idx);
		_Param[idx] -= sub;
	}
	// �w�肵���p�����[�^�[�擾�B
	// �����F	�p�����[�^�[�^�C�v�B
	inline int GetParam(Param idx)const {
		_OutCheck(idx);
		return _Param[idx];
	}

	//���񂾂��ǂ����̃t���O���擾�B
	//ture�Ȃ玀��ł���B
	inline bool GetDeathFalg()
	{
		return _DeathFlag;
	}

	// ��_���[�W����(�p�����[�^�[�Ƀ_���[�W��^����)�B
	// ����:		�G����̃_���[�W�B
	//				�L�����N�^�[�̍s���Ŕ�������h�䗦(�h��s���Ȃǂɂ���ĕϓ�����l�A�f�t�H���g��1)�B
	//				�����i�̖h���(�f�t�H���g��0)�B
	int ReciveDamage(int defaultDamage, int defidx = 1, int Equipment = 0);

	// ��_���[�W�v�Z(�v�Z�݂̂Ńp�����[�^�[�ɉe���͂Ȃ�)�B
	// ����:		�G����̃_���[�W�B
	//				�L�����N�^�[�̍s���Ŕ�������h�䗦(�h��s���Ȃǂɂ���ĕϓ�����l�A�f�t�H���g��1)�B
	//				�����i�̖h���(�f�t�H���g��0)�B
	int ReceiveDamageMass(int defaultDamage, int defidx = 1,int Equipment = 0);

	//�^�_���[�W�v�Z�B
	// �����F		�L�����N�^�[�̍s���Ŕ�������U����(�U���̎�ނȂǂɂ���ĕϓ�����l�A�f�t�H���g��1)�B
	inline int GiveDamageMass(int atk = 1)
	{
		return _Param[Param::ATK] * atk;
	}
private:

	// �z��O�ɃA�N�Z�X���ĂȂ����`�F�b�N�B
	inline void _OutCheck(int num)const {
		if (num <= Param::MIN || num >= Param::MAX) 
			abort();
	}

private:
	int _Param[Param::MAX];
	bool _DeathFlag = false;//���񂾂��ǂ����̃t���O�B
};