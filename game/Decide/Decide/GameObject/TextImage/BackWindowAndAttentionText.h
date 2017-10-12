#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Player\Player.h"
class ImageObject;
class TextObject;

//�w�i�摜�ƒ��ӕ���\������N���X�B
class BackWindowAndAttentionText :
	public GameObject
{
public:
	//�R���X�g���N�^�B
	BackWindowAndAttentionText(const char* name);

	//�f�X�g���N�^�B
	~BackWindowAndAttentionText();

	void Awake()override;

	//�e�L�X�g�Ɣw�i�摜�̍쐬�B
	//�\�����镶����A�w�i�摜�̍��W�A�w�i�摜�̃T�C�Y�A�e�L�X�g�̃��[�J�����W�A�e�L�X�g�̃T�C�Y�A�e�L�X�g�̃J���[�B
	void Create(const wchar_t * string,
		const Vector3& windowPos,
		const Vector2& windowSize,
		const Vector3& textPos,
		float textSize,
		const Color& color);

	//�X�V�B
	void Update()override;

#ifdef _DEBUG
	void Debug()override;
#endif


	//�w�i�摜�̈ʒu�ݒ�B
	void SetWindowPos(const Vector3& pos) {
		_AttentionBackWindowPos = pos;
	}

	//�w�i�摜�̃T�C�Y�ݒ�B
	void SetWindowSize(const Vector2& size) {
		_AttentionBackWindowSize = size;
	}

	//�e�L�X�g�̃��[�J�����W�ݒ�B
	void SetTextPos(const Vector3& pos) {
		_AttentionTextPos = pos;
	}

	//�e�L�X�g�̕����T�C�Y�ݒ�B
	void SetTextFontSize(float size) {
		_AttentionTextFontSize = size;
	}

	//�e�L�X�g�̕����B
	void SetText(const wchar_t * string) {
		_AttentionText->SetText(string);
	}

	//�A�N�e�B�u�t���O��true�ɕύX�B
	void SetActiveTrue() {
		_AttentionBackWindowImage->SetActive(true, true);
		//static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->PlayerStopEnable();
	}

	//�A�N�e�B�u�t���O��false�ɕύX�B
	void SetActiveFalse() {
		_AttentionBackWindowImage->SetActive(false, true);
		//static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->PlayerStopDisable();
	}
private:
	ImageObject*	_AttentionBackWindowImage	= nullptr;			//�w�i�摜�B
	Vector3			_AttentionBackWindowPos		= Vector3::zero;	//�G���[�e�L�X�g�̔w�i�E�C���h�E�̈ʒu�B
	Vector2			_AttentionBackWindowSize	= Vector2::zero;	//�G���[�e�L�X�g�̔w�i�E�C���h�E�̃T�C�Y�B
	TextObject*		_AttentionText				= nullptr;			//�e�L�X�g(���ӕ�)�B
	Vector3			_AttentionTextPos			= Vector3::zero;	//�G���[�e�L�X�g�̃��[�J�����W�B
	float			_AttentionTextFontSize		= 0.0f;				//�G���[�e�L�X�g�̃t�H���g�T�C�Y�B
};