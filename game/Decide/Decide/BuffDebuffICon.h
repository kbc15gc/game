#pragma once
#include "fbEngine\_Object\Object.h"

class ImageObject;

//�o�t�A�f�o�t��\������N���X�B
class BuffDebuffICon :public GameObject
{
public:
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

	//�\������A�C�R���̏����܂Ƃ߂�p�B
	struct BuffDebuff
	{
		ImageObject* _ArrowIconImage;			//BuffDebuffTypeIcon�ɓY������A�C�R���B
		ImageObject* _BuffDebuffTypeIconImage;	//���̃X�e�[�^�X���オ���Ă��邩��\���A�C�R���B
		Param        _Param;					//�ǂ̃p�����[�^�[����ێ�����p�B
		bool		 _isBuff;					//�o�t���f�o�t���ǂ����̃t���O�B
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

	//�f�o�t�A�C�R���𐶐��B
	//����:�f�o�t���|����p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void DebuffIconCreate(Param param);


	//�o�t�A�C�R���̍폜�B
	//����:���ʎ��Ԃ��؂ꂽ�p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void DeleteBuffIcon(Param param);

	//�f�o�t�A�C�R���̍폜�B
	//����:���ʎ��Ԃ��؂ꂽ�p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void DeleteDebuffIcon(Param param);

	//�A�C�R����`�悵�Ȃ��B
	void RenderDisable();

	//�A�C�R����`�悷��B
	void RenderEnable();
private:
	//�ǉ�����p�����[�^�[��ǉ����Ă����̂����`�F�b�N�A�ǉ����\�Ȃ�true�A�ǉ��o���Ȃ��Ȃ�false�B
	//����:�p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	bool _AddCheck(Param param);
private:
	vector<BuffDebuff*>	_PlayerBuffDebuffList;					//�v���C���[�Ɋ|�����Ă���o�t�f�o�t�̃��X�g�B
	Transform*			_PlayerHpBarTransform = nullptr;		//�v���C���[��HpBar��Transform�Q�Ɨp�B
};

namespace {
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
}