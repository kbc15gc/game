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
	enum Param{HP = 0,MP,ATK,DEF,DEX,AGI,MAX};
	/*������
	*/
	void ParamInit(int hp, int mp, int atk, int def, int dex, int agi);
	
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
private:
	int _Param[Param::MAX];
	//int _HP;	//�q�b�g�|�C���g
	//int _MP;	//�}�W�b�N�|�C���g
	//int _ATK;	//�U����
	//int _DEF;	//�h���
	//int _DEX;	//������
	//int _AGI;	//����
};