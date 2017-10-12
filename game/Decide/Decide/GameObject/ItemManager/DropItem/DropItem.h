#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\ItemManager\ItemManager.h"

class SkinModel;
class Player;
class ImageObject;
class HoldEquipment;
class ParticleEffect;

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
	//���Ƃ��A�C�e����Info�A���W�A���Ƃ���(����̏ꍇ�͐ݒ肵�Ă�1�ɂȂ�)�B
	void Create(Item::BaseInfo* info, const Vector3& pos, int dropNum = 1);

	//�X�V�B
	void Update()override;

	//�O������Info��ݒ肷��p�B
	void SetInfo(Item::BaseInfo* info) {
		_DropItemInfo = info;
	}

	//�������O������ݒ肷��p�B
	void SetLife(float life) {
		_Life = life;
	}

	//���Ƃ��A�C�e���̐��B
	void SetDropNum(int num) {
		_DropNum = num;
	}

#ifdef _DEBUG
	void Debug();
#endif // _DEBUG


private:
	SkinModel*			_Model			 = nullptr;
	Vector3				_DropPos		 = Vector3::zero;//������ꏊ�B
	Item::BaseInfo*		_DropItemInfo	 = nullptr;		 //�h���b�v�A�C�e����Info�B
	Player*				_Player			 = nullptr;		 //��������Ɏg���B
	ImageObject*		_ButtonIconImage = nullptr;		 //�{�^���̃A�C�R���B
	float				_Life			 = 0.0f;		 //�����B
	float				_TotalDeltaTime  = 0.0f;		 //�t�B�[���h�Ɍ���Ă���o�������ԁB
	short int			_DropNum		 = 1;			 //���A�C�e���𗎂Ƃ����̐�(�f�t�H���g��1��)�B
	HoldEquipment*		_DropEquipment	 = nullptr;		 //���Ƃ��������i�B
	ParticleEffect*		_RareDropPE		 = nullptr;		 //���A�h���b�v�A�C�e���p�̃G�t�F�N�g�B
	ParticleEffect*		_NormalDropPE	 = nullptr;		 //���A�ȊO�̃h���b�v�A�C�e���p�G�t�F�N�g�B
};