#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\ItemManager\ItemManager.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include"fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\TextImage\AttentionTextOnly.h"

class SkinModel;
class Player;
class ImageObject;
class HoldItemBase;
class ParticleEffect;
class CCharacterController;

//�h���b�v�A�C�e���B
class DropItem :public GameObject
{
public:


	//�R���X�g���N�^�B
	DropItem(const char* name);

	//�f�X�g���N�^�B
	~DropItem();

	void Awake()override;

	//�h���b�v�A�C�e�����쐬�B
	//���Ƃ��A�C�e����ID�ƃA�C�e���R�[�h�A���W�A���Ƃ���(����̏ꍇ�͐ݒ肵�Ă�1�ɂȂ�)�B
	void Create(int id,int typeId, const Vector3& pos, int dropNum = 1);

	//�X�V�B
	void Update()override;

	//�O������Info��ݒ肷��p�B
	void SetInfo(Item::BaseInfo* info) {
		_DropItemInfo = info;
	}

	//�t�B�[���h��ɑ��݂ł���ő厞�Ԃ��O������ݒ肷��p�B
	void SetAppearMaxTime(float life) {
		_AppearMaxTime = life;
	}

	//���Ƃ��A�C�e���̐��B
	void SetDropNum(int num) {
		_DropNum = num;
	}

#ifdef _DEBUG
	void Debug()override;
#endif // _DEBUG

private:

	//������������̃����N���`�F�b�N���A�����N�ɓK����SE�ƃG�t�F�N�g��I���B
	void _EquipmentRankCheck_SelectSEAndEffect();

	//���̃N���X��Gameobject�ɓo�^�����I�u�W�F�N�g��S�폜�p�֐��B
	void _Release();

	//flag�����Ď擾�������A���s�̓K��������������߂�B
	void _SelectText(bool flag);

	//flag�����Ď擾���������s�̕������AttentionText�ɐݒ�B
	void _SetText(const wchar_t* string, bool flag);

	//�E�����A�C�e�����C���x���g����Add�֐��ɑ���B
	bool _AddInventory(Item::ItemCodeE code);

private:
	SkinModel*			  _Model				= nullptr;	    //���f���B
	Vector3				  _DropPos			    = Vector3::zero;//������ꏊ�B
	Item::BaseInfo*		  _DropItemInfo		    = nullptr;	    //�h���b�v�A�C�e����Info�B
	Player*				  _Player			    = nullptr;	    //��������Ɏg���B
	ImageObject*		  _ButtonIconImage	    = nullptr;	    //�{�^���̃A�C�R���B
	float				  _AppearMaxTime		= 0.0f;		    //�t�B�[���h��ɑ��݂ł���ő厞�ԁB
	float				  _TotalAppearTime	    = 0.0f;		    //�t�B�[���h�Ɍ���Ă���o�������ԁB
	short int			  _DropNum			    = 1;		    //���A�C�e���𗎂Ƃ����̐�(�f�t�H���g��1��)�B
	HoldItemBase*		  _DropEquipment		= nullptr;	    //���Ƃ��������i�B
	ParticleEffect*		  _RareDropPE		    = nullptr;	    //���A�h���b�v�A�C�e���p�̃G�t�F�N�g�B
	float				  _GetLength			= 5.0f;		    //�h���b�v�A�C�e�����E���鋗���B
	float				  _ButtonIconPosOffSet  = 120.0f;	    //�A�C�R�����o���ʒu�𒲐�����p�B
	SoundSource*		  _RareDropSE		    = nullptr;	    //���A�h���b�v�������̉��B
	SoundSource*		  _DropSE			    = nullptr;	    //�h���b�v�������̉��B
	int					  _ButtonIconPriority   = 0;			//�A�C�R���̗D��x�B
	Color				  _ModelColor		    = Color::zero;  //���f���𓧖��ɂ��Ă������߂Ɏg���B
	CCharacterController* _CCharacterController = nullptr;      //�L�����N�^�[�R���g���[���[�B															 
	float				  _Gravity				= 0.0f;			//�d�́B
	CharacterLight		  _TreasureChestLight;					//�󔠂̃��C�g�B
	Vector3				  _TextPos				= Vector3::zero;//�e�L�X�g���o���ꏊ�B
	float				  _TextFontSize			= 0.0f;			//�e�L�X�g�̃T�C�Y�B

	float _DitheRocalTime = 0.0f;	//!< �f�B�U���[�J���^�C��.
	float _DitheTime = 1.0f;		//!< �f�B�U�^�C��.

	AttentionTextOnly* _AttentionText = nullptr;

};