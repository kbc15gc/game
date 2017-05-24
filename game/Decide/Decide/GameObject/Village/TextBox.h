#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\MessageManager.h"

namespace
{
	enum TextBoxStateE: unsigned int
	{
		CLOSE = BIT(0),					//����
		OPEN = BIT(1),					//�J����
		ANIMATION = BIT(2),				//�A�j���[�V������
		TITLE = CLOSE | ANIMATION,		//�^�C�g��
		OPENING = BIT(3) | ANIMATION,	//�J���Ă���
		CLOSING = BIT(4) | ANIMATION,	//���Ă���
	};
}

class TextBox : public GameObject
{
public:
	TextBox(const char* name);
	~TextBox();
	void Awake()override;
	void Update()override;

	//�g�p���郁�b�Z�[�W�ݒ�B
	void SetMessageID(const int& id);
	//�b��
	void Speak();
	//�^�C�g����\�����邩�ǂ����H
	void Title(bool show);
	
	//���b�Z�[�W�{�b�N�X�����B
	void CloseMessage();
	
	//�e�L�X�g�\���X�s�[�h�ݒ�B
	void SetTextSpeed(const float speed)
	{
		_TextSpeed = speed;
	}
private:
	//���b�Z�[�W�{�b�N�X���J���B
	void _OpenMessage();
	//���̃��b�Z�[�W�ցB
	void _NextMessage();
	//�e�L�X�g���R���|�[�l���g�ɓK�p
	void _SetText(const char* text);
	//�w�肵��ID�̃��b�Z�[�W��ݒ�
	void _SetMessage(const int& id);
	//�|�b�v�A�b�v�A�j���[�V�����Ƃ�����
	void _Animation();
	//�\�������镶��������
	void _IncreaseChar();
private:
	//�e�L�X�g�{�b�N�X�̔w�i���ƃJ�[�\����
	ImageObject* _BoxImage[2];
	//�e�L�X�g
	TextObject* _Text;
	//�ŏ��̃e�L�X�gID
	int _TextID;
	//�e�L�X�g�̏��
	MessageInfo* _Message;

	//�\�����镶�����B
	float _CharNum;
	//�e�L�X�g��\������X�s�[�h
	float _TextSpeed;
	//���b�Z�[�W�{�b�N�X���J���Ă���B
	TextBoxStateE _State;
	//�X�P�[��
	float _AnimeTime;
};