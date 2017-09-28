#pragma once
#include "fbEngine\_Object\Object.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

//�o�t�A�f�o�t��\������N���X�B
class BuffDebuffICon :public GameObject
{
public:
	//�o�t�f�o�t���ǂ̃X�e�[�^�X�ɉe�����Ă��邩�𔻒f����̂Ɏg���B
	enum class Param
	{
		Atk = 0,	//�����U���́B
		MAtk,		//���@�U���́B
		Def,		//�����h��́B
		MDef,		//���@�h��́B
		Dex			//�N���e�B�J�����B
	};

	//�R���X�g���N�^�B
	BuffDebuffICon(char* name) :
		GameObject(name)
	{
	}

	//�f�X�g���N�^�B
	~BuffDebuffICon()
	{
	}

	//�������B
	void Awake()override;

	//�o�t�f�o�t�������ăA�C�R���𐶐��B
	void Buff(Param );

	//�ǂ̃o�t�f�o�t�������ăA�C�R���𐶐��B
	void Debuff(Param);
private:
	ImageObject* _ArrowIconImage = nullptr;				//BuffDebuffTypeIcon�ɓY������A�C�R���B
	ImageObject* _BuffDebuffTypeIconImage = nullptr;	//���̃X�e�[�^�X���オ���Ă��邩��\���A�C�R���B
};