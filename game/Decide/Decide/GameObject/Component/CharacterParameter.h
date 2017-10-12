#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "fbEngine\fbstdafx.h"

class HoldWeapon;
class HoldArmor;

class CharacterParameter :public Component {
public:
	
#ifdef _DEBUG
	// �f�o�b�O�o�͗p�̔z��B
	// ���p�����[�^�[�̎�ނ�ǉ������珇�Ԓʂ�ɒǉ����āB
	static const wchar_t* ENUM_NAME[];
#endif

	// ���f����(�΁A���Ȃ�)�B
	enum class Element { None = -1 };
	// ��������(�a�A�ŁA���Ȃ�)�B
	enum class Physical { None = -1 };

	// �_���[�W���B
	struct DamageInfo {
		int value = 0;	// �_���[�W�ʁB
		bool isCritical = false;	// �N���e�B�J�����B
		bool isMagic = false;	// ���@�U�����B
		bool isThrough = false;	// �h��ђʍU�����B
		Element element;	// ���f����(���ݖ��g�p)�B
		Physical physical;	// ��������(���ݖ��g�p)�B
	};

	// �p�����[�^�[�񋓁B
	//			HP�B
	//			MP�B
	//			�U���́B
	//			���@�U���́B
	//			�h��́B
	//			���@�h��B
	//			��p�x(�N���e�B�J��������)�B
	//			�N���e�B�J���З́B
	//			���x���B
	enum Param { MIN = -1,HP = 0, MP, ATK, MAT, DEF, MDE, DEX, CRT ,LV, MAX };

private:
	// �e��p�����[�^�̏�ԊǗ��p�\���́B
	struct ParamInfo {
		// �L�����N�^�[�̊�b�p�����[�^(���̒l�����ƂɃo�t�A�f�o�t�AHP�v�Z���s��)�B
		// �����x���A�b�v�Ȃǂɂ���Ċ�b�p�����[�^���X�V�����ۂ͂������ύX����B
		int originParam = 0;
		int param = 0;	// �Q�[���ł̎��ۂ̃p�����[�^(�o�t�A�f�o�t��̒l��A���݂�HP��MP�Ȃ�)�B

		int buffPercentage = 0;		// �p�����[�^�Ɋ|�����Ă���o�t�̌��ʒl(�p�[�Z���g)�B
		float buffInterval = 0.0f;		// �o�t�̌��ʎ��ԁB
		float buffCounter = 0.0f;		// �o�t�̌o�ߎ��ԁB

		int debuffPercentage = 0;	// �p�����[�^�Ɋ|�����Ă���o�t�̌��ʒl(�p�[�Z���g)�B
		float debuffInterval = 0.0f;	// �f�o�t�̌��ʎ��ԁB
		float debuffCounter = 0.0f;		// �f�o�t�̌o�ߎ��ԁB
	};

public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~CharacterParameter() {
	};

	void Awake()override;

	void Update()override;



	// �p�����[�^���Z�b�g�B
	// �����F	�ݒ肷��e���b�p�����[�^�B
	// ���f�o�t��o�t���������Ă���ꍇ�͐V�����p�����[�^�����ƂɍČv�Z����Đݒ肳���B
	void ParamReset(int param[Param::MAX]);
	// �p�����[�^���Z�b�g�B
	// �����F	�ݒ肷��e���b�p�����[�^�B
	// ���f�o�t��o�t���������Ă���ꍇ�͐V�����p�����[�^�����ƂɍČv�Z����Đݒ肳���B
	void ParamReset(const vector<int>& param);

	// ��_���[�W����(�p�����[�^�[�Ƀ_���[�W��^����)�B
	// ����:		�G����̃_���[�W���B
	//				�h��(�f�t�H���g��null�A���햢��������null��ݒ�)�B
	//				�L�����N�^�[�̍s���Ŕ�������h�䗦(�h��s���Ȃǂɂ���ĕϓ�����l�A�f�t�H���g��1)�B
	// �߂�l:		�󂯂��_���[�W�B
	int ReciveDamage(const DamageInfo& info, HoldArmor* armor = nullptr, int defidx = 1);

	// ��_���[�W�v�Z(�v�Z�݂̂Ńp�����[�^�[�ɉe���͂Ȃ�)�B
	// ����:		�G����̃_���[�W���B
	//				�h��(�f�t�H���g��null�A���햢��������null��ݒ�)�B
	//				�L�����N�^�[�̍s���Ŕ�������h�䗦(�h��s���Ȃǂɂ���ĕϓ�����l�A�f�t�H���g��1)�B
	// �߂�l:		�󂯂�_���[�W�B
	int ReceiveDamageMass(const DamageInfo& info, HoldArmor* armor = nullptr,int defidx = 1);

	// �h��͖����̔�_���[�W�����B
	// ����:		�G����̃_���[�W�B
	int ReciveDamageThrough(int damage);

	//�^�_���[�W�v�Z�B
	// �����F	���@�U�����B
	//			�h��ђʍU�����B
	//			����(�f�t�H���g��null�A���햢��������null��ݒ�)�B	
	//			�_���[�W��(�U���̎�ނȂǂɂ��U���͂ɑ΂��銄���A���̒l��0.01f�|�����l���U���͂ɏ�Z����A�P�ʂ̓p�[�Z���g�A�f�t�H���g��100)�B
	// �߂�l:	�^����_���[�W�̏��B
	unique_ptr<DamageInfo> GiveDamageMass(bool isMagic,bool isThrough, HoldWeapon* weapon = nullptr, int percentage = 100);

	// HP�񕜊֐��B
	// �����F	�񕜗ʁB
	// �߂�l�F	�񕜂ł������B
	bool HeelHP(int value);
	// MP�񕜊֐��B
	// �����F	�񕜗ʁB
	// �߂�l�F	�񕜂ł������B
	bool HeelMP(int value);

	// �o�t�֐��B
	// �����F	�o�t���|�������p�����[�^�B
	//			���ʒl(�p�[�Z���g)�B
	//			���ʎ���(�b)�B
	// ��HP��MP��CRT��LV�͖����B
	void Buff(Param idx, int percentage,float time);

	// �o�t�����֐��B
	// �����F	�����������p�����[�^�B
	// ��HP��MP��CRT��LV�͖����B
	void BuffClear(Param idx);

	// �S�o�t�����֐��B
	// ��HP��MP��CRT��LV�͖����B
	void BuffClearAll();

	// �f�o�t�֐��B
	// �����F	�f�o�t���|�������p�����[�^�B
	//			���ʒl(�p�[�Z���g)�B
	//			���ʎ���(�b)�B
	// ��HP��MP��CRT��LV�͖����B
	void Debuff(Param idx, int percentage, float time);

	// �f�o�t�����֐��B
	// �����F	�����������p�����[�^�B
	// ��HP��MP��CRT��LV�͖����B
	void DebuffClear(Param idx);

	// �S�f�o�t�����֐��B
	// ��HP��MP��CRT��LV�͖����B
	void DebuffClearAll();


	// �w�肵���p�����[�^�[(�o�t�l���܂�)�擾�B
	// �����F	�p�����[�^�[�^�C�v�B
	inline int GetParam(Param idx)const {
		_OutCheck(idx);
		return _Info[idx].param;
	}
	// �w�肵���p�����[�^�[(�o�t�l������)�擾�B
	// �����F	�p�����[�^�[�^�C�v�B
	inline int GetPigmentParam(Param idx)const {
		if (idx == Param::HP || idx == Param::MP) {
			// HP��MP�͔�Ή��B
			abort();
		}
		_OutCheck(idx);
		return _Info[idx].originParam;
	}

	// �w�肵���p�����[�^�[�̌��݂̃o�t�l�擾�B
	// �����F	�p�����[�^�[�^�C�v�B
	inline int GetBuffParam(Param idx)const {
		if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
			// �o�t�ƃf�o�t�ɑΉ����Ă��Ȃ����͔̂�Ή��B
			abort();
		}
		return static_cast<int>(static_cast<float>(_Info[idx].buffPercentage) * 0.01f * _Info[idx].originParam);
	}
	// �w�肵���p�����[�^�[�̌��݂̃f�o�t�l�擾�B
	// �����F	�p�����[�^�[�^�C�v�B
	inline int GetDebuffParam(Param idx)const {
		if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
			// �o�t�ƃf�o�t�ɑΉ����Ă��Ȃ����͔̂�Ή��B
			abort();
		}
		return static_cast<int>(static_cast<float>(_Info[idx].debuffPercentage) * 0.01f * _Info[idx].originParam);
	}

	inline int GetMaxHP()const {
		return _Info[Param::HP].originParam;
	}

	inline int GetMaxMP() const{
		return _Info[Param::MP].originParam;
	}

	//���񂾂��ǂ����̃t���O���擾�B
	//ture�Ȃ玀��ł���B
	inline bool GetDeathFlg()
	{
		return _DeathFlag;
	}



private:

	// �z��O�ɃA�N�Z�X���ĂȂ����`�F�b�N�B
	inline void _OutCheck(short num)const {
		if (num <= Param::MIN || num >= Param::MAX) 
			abort();
	}

	// ���݂̃o�t�l�A�f�o�t�l�Ńp�����[�^���X�V�B
	void _UpdateParam(Param idx);

	// �e��p�����[�^�̏�Ԃ��X�V�B
	void _UpdateInfo();
private:
	// �L�����N�^�[�̏�ԊǗ��p�\���́B
	// �����̃N���X���ł̂ݎg�p�B
	vector<ParamInfo> _Info;

	bool _DeathFlag = false;//���񂾂��ǂ����̃t���O�B
};
