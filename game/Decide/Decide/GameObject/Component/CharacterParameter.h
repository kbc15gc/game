#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "fbEngine\fbstdafx.h"

class CharacterParameter :public Component {
public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~CharacterParameter() {
	};

	void Awake()override;
public:

#ifdef _DEBUG
	// �f�o�b�O�o�͗p�̔z��B
	// ���p�����[�^�[�̎�ނ�ǉ������珇�Ԓʂ�ɒǉ����āB
	static const wchar_t* ENUM_NAME[];
#endif

	// ���f����(�΁A���Ȃ�)�B
	enum class Element{None = 0};
	// ��������(�a�A�ŁA���Ȃ�)�B
	enum class Physical{None = 0};
	// �p�����[�^�[�񋓁B
	//			HP�B
	//			HP�ő�l�B
	//			MP�B
	//			MP�ő�l�B
	//			�U���́B
	//			�h��́B
	//			�����́B
	//			�q���́B
	//			���x���B
	//			�o���l�B
	//			���Ƃ��o���l�B
	enum Param { MIN = -1,HP = 0, MAXHP, MP, MAXMP, ATK, DEF, DEX, AGI, LV, EXP, DROPEXP, MAX };
	//�������B
	// �����F	HP�B
	//			HP�ő�l�B
	//			MP�B
	//			MP�ő�l�B
	//			�U���́B
	//			�h��́B
	//			�����́B
	//			�q���́B
	//			���x���B
	//			�o���l�B
	//			���Ƃ��o���l�B
	void ParamInit(int hp,int maxhp,int mp,int maxmp, int atk, int def, int dex, int agi, int lv, int exp, int dropexp);
	//�������B
	void ParamInit(int param[Param::MAX]);

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

	// �S�p�����[�^�[�擾�B
	inline const vector<int>& GetParams() {
		return _Param;
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
	inline int GiveDamageMass(float atk = 1)
	{
		return _Param[Param::ATK] * atk;
	}
	// ���x���A�b�v�B
	// �����F		���x���A�b�v�ɕK�v�Ȍo���l�̒l�B
	void LevelUP(int lvupexp, int hp, int mp, int atk, int def, int dex, int agi);
private:

	// �z��O�ɃA�N�Z�X���ĂȂ����`�F�b�N�B
	inline void _OutCheck(int num)const {
		if (num <= Param::MIN || num >= Param::MAX) 
			abort();
	}

private:
	vector<int> _Param;
	bool _DeathFlag = false;//���񂾂��ǂ����̃t���O�B
};
