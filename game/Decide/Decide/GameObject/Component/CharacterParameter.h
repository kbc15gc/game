#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class CharacterParameter :public Component {
public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~CharacterParameter() {};
public:
	// ���f����(�΁A���Ȃ�)�B
	enum class Element{None = 0};
	// ��������(�a�A�ŁA���Ȃ�)�B
	enum class Physical{None = 0};
	// �p�����[�^�[�񋓁B
	enum Param { HP = 0, MAXHP, MP, MAXMP, ATK, DEF, DEX, AGI, MAX };
	//�������B
	void ParamInit(int hp,int maxhp,int mp,int maxmp, int atk, int def, int dex, int agi);
	
	// �w�肵���p�����[�^�[�ɉ��Z�B
	// �����F	�p�����[�^�[�^�C�v�B
	//			���Z�ʁB
	inline void AddParam(Param idx,const int add) {
		_Param[idx] += add;
	}
	// �w�肵���p�����[�^�[���猸�Z�B
	// �����F	�p�����[�^�[�^�C�v�B
	//			���Z�ʁB
	inline void SubParam(Param idx, const int sub) {
		_Param[idx] -= sub;
	}
	// �w�肵���p�����[�^�[�擾�B
	// �����F	�p�����[�^�[�^�C�v�B
	inline int GetParam(Param idx)const {
		return _Param[idx];
	}

	//�_���[�W�̌v�Z�B
	//������:�U���n�̃p�����[�^�[ ������:�h��n�̃p�����[�^�[�B
	int DamageMass(int atk, int defidx);
	//���񂾂��ǂ����̃t���O���擾�B
	//ture�Ȃ玀��ł���B
	inline bool GetDeathFalg()
	{
		return _DeathFlag;
	}

	//�^����_���[�W�̌v�Z�B
	inline int AttackDamageMass(int atk = 1)
	{
		return _Param[Param::ATK] * atk;
	}
private:
	int _Param[Param::MAX];
	bool _DeathFlag = false;//���񂾂��ǂ����̃t���O�B

};