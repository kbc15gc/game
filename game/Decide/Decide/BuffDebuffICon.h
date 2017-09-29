#pragma once
#include "fbEngine\_Object\Object.h"

class ImageObject;

//�o�t�A�f�o�t��\������N���X�B
class BuffDebuffICon :public GameObject
{
public:
	//�\������A�C�R���̏����܂Ƃ߂�p�B
	struct BuffDebuff
	{
		ImageObject* _ArrowIconImage;			//BuffDebuffTypeIcon�ɓY������A�C�R���B
		ImageObject* _BuffDebuffTypeIconImage;	//���̃X�e�[�^�X���オ���Ă��邩��\���A�C�R���B
	};

	//�o�t�f�o�t���ǂ̃X�e�[�^�X�ɉe�����Ă��邩�𔻒f����̂Ɏg���B
	enum class Param
	{
		HP,			//HP(�L�����N�^�[�p�����[�^�[�Ƃ̍��ڍ��킹�p�B���̏��g��Ȃ�)�B
		MP,			//MP(�L�����N�^�[�p�����[�^�[�Ƃ̍��ڍ��킹�p�B���̏��g��Ȃ�)�B
		Atk,		//�����U���́B
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

	//�X�V�B
	void Update()override;

	//�o�t�A�C�R���𐶐��B
	//����:�o�t���|����p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void BuffIconCreate(Param param);

	//�f�o�t�������ăA�C�R���𐶐��B
	//����:�f�o�t���|����p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void DebuffIconCreate(Param param);
private:
	vector<BuffDebuff*>	_PlayerBuffDebuffList;					//�v���C���[�Ɋ|�����Ă���o�t�f�o�t�̃��X�g�B
	Transform*			_PlayerHpBarTransform = nullptr;		//�v���C���[��HpBar��Transform�Q�Ɨp�B

};

//�\������X�e�[�^�X�A�C�R���B
static char* TypeIconText[static_cast<int>(BuffDebuffICon::Param::Max)] =
{	
	"Hp.pmg",			//Hp(�p�����[�^�[�Ƃ̍��ڍ��킹�p�B���̏��g��Ȃ�)
	"Mp.pmg",			//Mp(�p�����[�^�[�Ƃ̍��ڍ��킹�p�B���̏��g��Ȃ�)
	"sword.png",		//���B
	"magic.png",		//��B
	"armor.png",		//�Z�B
	"cloaks.png",		//���B
	"UI/S_Light01.png"	//�N���e�B�J�����B
};
