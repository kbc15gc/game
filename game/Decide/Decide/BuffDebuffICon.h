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
		Dex,		//�N���e�B�J�����B
		Max
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

	//�o�t�A�C�R���𐶐��B
	//����:�o�t���|����p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void BuffIconCreate(Param param);

	//�f�o�t�������ăA�C�R���𐶐��B
	//����:�f�o�t���|����p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void DebuffIconCreate(Param param);
private:
	ImageObject* _ArrowIconImage = nullptr;				//BuffDebuffTypeIcon�ɓY������A�C�R���B
	ImageObject* _BuffDebuffTypeIconImage = nullptr;	//���̃X�e�[�^�X���オ���Ă��邩��\���A�C�R���B
};

//�X�e�[�^�X�A�C�R���B
static const char* TypeIconText[static_cast<int>(BuffDebuffICon::Param::Max)] =
{
	"sword.png",
	"magic.png",
	"armor.png",
	"cloaks.png",
	"UI/S_Light01.png"
};
