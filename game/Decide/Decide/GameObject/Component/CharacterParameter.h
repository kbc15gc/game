#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "fbEngine\fbstdafx.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon\HoldWeapon.h"

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
	//			���@�U���́B
	//			�h��́B
	//			���@�h��B
	//			��p�x(�N���e�B�J��������)�B
	//			���x���B
	//			�o���l�B
	//			���Ƃ��o���l�B
	//			�������B
	enum Param { MIN = -1,HP = 0, MAXHP, MP, MAXMP, ATK, MAT, DEF, MDE, DEX, LV, EXP, DROPEXP, MONEY, MAX };
	//�������B
	// �����F	HP�B
	//			HP�ő�l�B
	//			MP�B
	//			MP�ő�l�B
	//			�U���́B
	//			���@�U���́B
	//			�h��́B
	//			���@�h��B
	//			��p�x(�N���e�B�J��������)�B
	//			���x���B
	//			�o���l�B
	//			���Ƃ��o���l�B
	//			�������B
	void ParamInit(int hp,int maxhp,int mp,int maxmp, int atk, int mat, int def, int mde, int dex, int lv, int exp, int dropexp,int money);
	//�������B
	void ParamInit(int param[Param::MAX]);
	void ParamInit(const vector<int>& param);

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

	int* GetParamPt(Param param)
	{
		return &_Param[param];
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
	// �����F	���@�U�����B
	//			����(�f�t�H���g��null�A���햢��������null��ݒ�)�B	
	//			�L�����N�^�[�̍s���Ŕ�������U����(�U���̎�ނȂǂɂ���ĕϓ�����l�A�f�t�H���g��1)�B
	inline int GiveDamageMass(bool isMagic,HoldWeapon* weapon = nullptr,int atk = 1)
	{
		int damage = 0;
		int weaponDamage = 0;
		int critMax = 100;	// �N���e�B�J�����ő�B
		int crit = _Param[Param::DEX];// �N���e�B�J�����B

		if (isMagic) {
			damage = _Param[Param::MAT];
			if (weapon) {
				//weaponDamage = weapon->GetInfo();
				crit += static_cast<Item::WeaponInfo*>(weapon->GetInfo()->Dex);
			}
		}
		else {
			damage = _Param[Param::ATK];
			if (weapon) {
				weaponDamage = weapon->GetAttack();
			}
		}

		damage += weaponDamage;
		if()
		damage * 

		// �N���e�B�J���B
		damage += static_cast<Item::WeaponInfo*>(weapon->GetInfo())->ATK + (static_cast<Item::WeaponInfo*>(weapon->GetInfo())->ATK * weapon->GetAtkRnd() * 0.1f);
		
		return damage * atk;
	}
	// ���x���A�b�v�B
	// �����F		���x���A�b�v�ɕK�v�Ȍo���l�̒l�B
	void LevelUP(int lvupexp, int hp, int mp, int atk, int mat,int def,int mde, int dex);
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
