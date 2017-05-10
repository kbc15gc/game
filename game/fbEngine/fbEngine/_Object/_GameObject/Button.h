#pragma once
#include "GameObject.h"
#include "_Object\_Component\_2D\Sprite.h"
#include "_Object\_Component\_2D\SquareCollision.h"


namespace fbButton
{
	//�{�^���̃^�C�v
	enum class ButtonTypeE : unsigned int
	{
		PRESS,		//������Ă���Ԃ̂�ON�ɂȂ�
		TRIGGER		//�g���K�[
	};
}

namespace
{
	//�{�^���̏��
	enum class ButtonStateE : unsigned int
	{
		NORMAL,		//�ʏ�
		ON,			//��ɃJ�[�\��������
		PRESS,		//������Ă���
		DISABLED,	//����
		NUM
	};
}

class Button:public GameObject
{
public:
	Button(char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;

	//�ꊇ�ݒ�
	void Initialize(TEXTURE* on, TEXTURE* off, const Vector2& size, const fbButton::ButtonTypeE& type = fbButton::ButtonTypeE::PRESS);

	//�{�^���̃e�N�X�`���ݒ�(on,off�Ń{�^�����ς��)
	void SetTexture(TEXTURE* on,TEXTURE* off=nullptr);
	//�R���W�����̃T�C�Y�ݒ�
	void SetSize(const Vector2& size);
	//�{�^���̃^�C�v�ݒ�
	void SetButtonType(const fbButton::ButtonTypeE& type);
	//�F�ύX
	void SetButtonColor(const ButtonStateE state, const Color& color);
	//�I���I�t���O������ݒ�(�g���K�[����Ȃ��ƈӖ��Ȃ����ǁB)
	void SetOnOff(const bool& f);
	const bool& GetIsOn()
	{
		return _IsOn;
	}
	Sprite* GetSprite()
	{
		return _Sprite;
	}
private:
	//�X�e�[�^�X�X�V�`�F�b�N,�߂�l�͕ύX����Ă��ǂ����H
	bool _CheckState();
	//�X�e�[�g�ω��ɔ������ύX
	void _ChangeState();
private:
	Sprite* _Sprite;			//�X�v���C�g
	SquareCollision* _Square;	//�l�p�`�̂����蔻��

	fbButton::ButtonTypeE _Type;	//�{�^���̃^�C�v
	ButtonStateE _State;	//�X�e�[�g
	bool _IsOn;	//ON��OFF���H

	//ON,OFF�̎��̃e�N�X�`��
	TEXTURE *_Tex[2];
	//�����l�͔����炾�񂾂񍕂��Ȃ��Ă���
	//�e�X�e�[�^�X�̎��̐F
	Color _Color[(unsigned int)ButtonStateE::NUM] = { Color::white,Color(0.8f,0.8f,0.8f),Color(0.5f,0.5f,0.5f),Color(0.3f,0.3f,0.3f) };
};