#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "GameObject\Component\CharacterParameter.h"
class ImageObject;

//�o�t�A�f�o�t��\������N���X�B
class BuffDebuffICon :public Component
{
public:

	BuffDebuffICon(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~BuffDebuffICon();
public:

	//�g���A�C�R���̃^�C�v�B
	//�v���C���[����HpBar�̉��B�G�l�~�[����HpBar�̏�B
	enum class UseIconType
	{
		Player = 0,
		Enemy
	};
	//���̎�ށB
	enum class Arrow
	{
		Up,		//�o�t���B
		Down,	//�f�o�t���B
		Max
	};

	//�\������A�C�R���̏����܂Ƃ߂�p�B
	struct BuffDebuff
	{
		ImageObject*				_ArrowIconImage;			//BuffDebuffTypeIcon�ɓY������A�C�R���B
		ImageObject*				_BuffDebuffTypeIconImage;	//���̃X�e�[�^�X���オ���Ă��邩��\���A�C�R���B
		CharacterParameter::Param   _Param;						//�ǂ̃p�����[�^�[����ێ�����p�B
		bool						_isBuff;					//�o�t���f�o�t���ǂ����̃t���O�B
	};


	//�������B
	void Awake()override;

	//�X�V�B
	void Update()override;

#ifdef _DEBUG
	void Debug()override;
#endif
	//�o�t�A�C�R���𐶐��B
	//����:�o�t���|����p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void BuffIconCreate(CharacterParameter::Param param);

	//�f�o�t�A�C�R���𐶐��B
	//����:�f�o�t���|����p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void DebuffIconCreate(CharacterParameter::Param param);

	//�o�t�A�C�R���̍폜�B
	//����:���ʎ��Ԃ��؂ꂽ�p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void DeleteBuffIcon(CharacterParameter::Param param);

	//�f�o�t�A�C�R���̍폜�B
	//����:���ʎ��Ԃ��؂ꂽ�p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	void DeleteDebuffIcon(CharacterParameter::Param param);

	//���g�Ɋ|�����Ă���S�Ẵo�t�f�o�t�A�C�R�����폜�B
	void DeleteAllBuffDebuffIcon();

	//�A�C�R����`�悵�Ȃ��B
	void RenderDisable();

	//�A�C�R����`�悷��B
	void RenderEnable();

	//�g���A�C�R���^�C�v���v���C���[�ɐݒ�B
	void SelectUseIconType_Player() {
		_UseIconType = UseIconType::Player;
	}

	//�g���A�C�R���^�C�v���G�l�~�[�ݒ�B
	void SelectUseIconType_Enemy() {
		_UseIconType = UseIconType::Enemy;
	}

	//HpBar��transform��ݒ�B
	void SetHpBarTransform(Transform* trns) {
		_HpBarTransform= trns;
	}

	//�o�t�f�o�t�A�C�R���̃T�C�Y��ݒ�(�T�C�Y���w�肵�Ȃ��Ă��f�t�H���g�T�C�Y�ɂȂ�)�B
	void SetBuffDebfuuIconSize(const Vector2& size) {
		_BuffDebfuuIconSize = size;
	}

	//���̃T�C�Y��ݒ�(�T�C�Y���w�肵�Ȃ��Ă��f�t�H���g�T�C�Y�ɂȂ�)�B
	void ArrowIconSize(const Vector2& size) {
		_ArrowSize = size;
	}
private:
	//�ǉ�����p�����[�^�[��ǉ����Ă����̂����`�F�b�N�A�ǉ����\�Ȃ�true�A�ǉ��o���Ȃ��Ȃ�false�B
	//����:�p�����[�^�[(Atk,Matk,Def,MDef,Dex)�B
	bool _AddCheck(CharacterParameter::Param param,bool isBuff);

	void SetPlayerOffset() {
		_IconOffSet = 60.0f;
		_IconPosOffset = { 50.0f,120.0f, 0.0f };
		_ArrowPosOffet = { 20.0f, 12.0f, 0.0f };
	}

	void SetEnemyOffset() {
		_IconOffSet = 30.0f;
		_IconPosOffset = { -70.0f,-20.0f,0.0f };
		_ArrowPosOffet = { 5.0f,5.0f,0.0f };
	}
private:
	vector<BuffDebuff*>	_BuffDebuffList;								//�o�t�f�o�t�̃��X�g�B
	Transform*			_HpBarTransform		= nullptr;					//HpBar��Transform�Q�Ɨp�B
	Vector2				_ScreenPos			= Vector2::zero;			//HpBar�̃|�W�V��������X�N���[�����W�ɕϊ��������́B
	Vector2				_BuffDebfuuIconSize = Vector2(40.0f, 40.0f);	//�o�t�f�o�t�A�C�R���T�C�Y�B
	Vector2				_ArrowSize			= Vector2(13.0f, 20.0f);	//���̃T�C�Y�B
	UseIconType			_UseIconType		= UseIconType::Player;		//�A�C�R���^�C�v�B
	float				_IconOffSet			= 0.0f;						//�o�t�A�f�o�t���d�ˊ|������ۂɂ��炷���߂Ɏg���B																		//�G�l�~�[���̃A�C�R����HpBar����ǂꂾ����������OFFSET�B
	Vector3				_IconPosOffset		= { 0.0f,0.0f,0.0f };		//�A�C�R����HpBar����ǂꂾ����������OFFSET�B
	Vector3				_ArrowPosOffet		= { 0.0f,.0f,0.0f };		//�A�C�R����������ǂꂾ����������OFFSET�B
	int					_Priority			= 0;						//�D��x�B
};

namespace {
	//�\������X�e�[�^�X�A�C�R���B
	static char* TypeIconText[static_cast<int>(CharacterParameter::Param::MAX)] =
	{
		"Hp.pmg",			//Hp(�p�����[�^�[�Ƃ̍��ڍ��킹�p�B���̏��g��Ȃ�)
		"Mp.pmg",			//Mp(�p�����[�^�[�Ƃ̍��ڍ��킹�p�B���̏��g��Ȃ�)
		"sword.png",		//���B
		"magic.png",		//��B
		"armor.png",		//�Z�B
		"cloaks.png",		//���B
		"UI/S_Light01.png", //�N���e�B�J�����B
		"Crt.png",
		"Lv.png",
	};

	//�\��������A�C�R���B
	static char* ArrowIconText[static_cast<int>(BuffDebuffICon::Arrow::Max)] =
	{
		"BuffIArrow.png",	//�o�t���B
		"DebuffArrow.png"	//�f�o�t���B
	};
}