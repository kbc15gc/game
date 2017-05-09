#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\MessageManager.h"
class TextBox : public GameObject
{
public:
	TextBox(const char* name);
	~TextBox();
	void Awake()override;
	void Update()override;

	//���b�Z�[�W�{�b�N�X���J���B
	void OpenMessage(const int& id);
	//���b�Z�[�W�{�b�N�X�����B
	void CloseMessage();
	//���̃��b�Z�[�W�ցB
	void NextMessage();
	void SetTextSpeed(const float speed)
	{
		_TextSpeed = speed;
	}
private:
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
	//�e�L�X�g�̏��
	MessageInfo* _Message;
	//�\�����镶�����B
	float _CharNum;
	//�e�L�X�g��\������X�s�[�h
	float _TextSpeed;
	//���b�Z�[�W�{�b�N�X���J���Ă���B
	bool _Open;
	//�A�j���[�V�������ł���
	bool _PlayingAnim;
	//�X�P�[��
	float _Scale;
};