#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_Component\_2D\Text.h"

#ifdef _DEBUG
class TextObject;

// �f�o�b�O�p�e�L�X�g�o�͗p�R���|�[�l���g�B
class OutputData : public Component {
public:
	OutputData(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
	};
	~OutputData() {
	};

	void Start()override;
	void Update()override;

private:
	// �\���p�I�u�W�F�N�g�����֐�
	// �����F	�ǉ������e�L�X�g�I�u�W�F�N�g�ւ̃|�C���^�B
	//			�e�L�X�g�I�u�W�F�N�g�ɂ��閼�O�B
	//			���[�J���̈ʒu(_lastChildren������΂�������_�ɔz�u�����A�Ȃ���Ζ���)�B
	//			�ݒ肷��e�L�X�g�̏����l(�f�t�H���g�͋�)�B
	//			�e�L�X�g�̃T�C�Y�B
	//			�e�L�X�g�̐F(�f�t�H���g��)�B
	//			����(�f�t�H���g������)�B
	TextObject* _CreateTextObject(TextObject** obj, const char* name, const Vector3& localPos = Vector3(0.0f,20.0f,0.0f), const char* initText = "", const float textSize = 40.0f , Color textColor = Color::white, fbText::TextFormatE format = fbText::TextFormatE::LEFT);

	// �w�肵����������w�肵��������ŋ��ފ֐��B
	// �����F	���ޕ�����(��)�B
	//			����łق���������B
	//			���ޕ�����(�E)�B			
	wchar_t* _TuckText(wchar_t* left, wchar_t* text, wchar_t* right);
	void OutputTextParam();
	void OutputTextTrans();

private:
	// �f�o�b�O�p�o�́B
	TextObject* _textName = nullptr;	// ���O�B
	TextObject* _textParam = nullptr;	// �p�����[�^�[�B
	TextObject* _textTransform = nullptr;	// Transform���B
	TextObject* _lastChildren = nullptr;	// �Ō�ɒǉ������e�L�X�g�I�u�W�F�N�g�B
	Vector3 _rootPos = Vector3(750.0f, 130.0f, 0.0f);	// �ŏ��ɕ\������e�L�X�g�̈ʒu�B
};
#endif
