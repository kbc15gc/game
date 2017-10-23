#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
class TextObject;

//���ӕ�������\������N���X�B
class AttentionTextOnly :
	public GameObject
{
public:

	//�e�L�X�g�̈ړ������B
	enum MoveType
	{
		Invalid = -1,
		Not = 0,		//�ړ����Ȃ�(�����l)�B
		Up,				//��B
		Down,			//���B
		Max
	};
	//�R���X�g���N�^�B
	AttentionTextOnly(const char* name);

	//�f�X�g���N�^�B
	~AttentionTextOnly();
	
	//�e�L�X�g���쐬�B
	//�����F������A���W,�T�C�Y�A�J���[�A�ړ����������Ȃ�Not�ȊO��I��(�����w�肵�Ȃ��ꍇ�͈ړ����Ȃ�)�B
	void CreateText(const wchar_t * string,
		const Vector3& textPos,
		float size,
		const Color& color,
		MoveType type = MoveType::Not);

	//�X�V�B
	void Update()override;

	//�e�L�X�g���X�g�̑S�폜�B
	void DeleteList();
private:
	//vector�ɐς܂ꂽ�e�e�L�X�g�̏��i�[�p�B
	struct TextInfo {
		TextObject* _Text;	//�e�L�X�g�B
		Color		_Color;	//�e�L�X�g�̃J���[�B
		Vector3		_Dir;	//�ړ���������B
	};
private:
	//�e�L�X�g���X�g�B
	vector<TextInfo*>	_TextList;

	//�e�L�X�g�̈ړ���
	float				_MoveSpeed = 0.001f;

	//Vector3(1000.0f,250.0f,0.0f),33.0f�A�C�e���̌��ʂ������e�L�X�g�̒��x�ǂ������̐ݒ�B
};

namespace {
	static Vector3 MoveTypeS[static_cast<int>(AttentionTextOnly::MoveType::Max)] = {
		{0.0f,0.0f,0.0f},		//�ړ����Ȃ��B
		{ 0.0f,-1.0f,0.0f },	//��B�@
		{ 0.0f,1.0f,0.0f },		//���B
	};
}