#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\ItemManager\ItemManager.h"

class SkinModel;
class Player;
class ImageObject;
class HoldEquipment;

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
	SkinModel* _Model = nullptr;

	//������ꏊ�B
	Vector3 _DropPos;

	//�h���b�v�A�C�e����Info�B
	Item::BaseInfo* _DropItemInfo = nullptr;

	//��������Ɏg���B
	Player* _Player = nullptr;

	//�{�^���̃A�C�R���B
	ImageObject* _ButtonIconImage = nullptr;

	//�����B
	float		 _Life = 0.0f;

	//�t�B�[���h�Ɍ���Ă���o�������ԁB
	float		 _TotalDeltaTime = 0.0f;
	
	//���A�C�e���𗎂Ƃ����̐��B
	short int	 _DropNum = 1;

	//���Ƃ��������i�B
	HoldEquipment* _DropEquipment = nullptr;
};