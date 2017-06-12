#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

enum BarColor {  Red = 0, Yellow,Green,Blue};
// �o�[�̒��g�B
class CBarElement:public ImageObject{
public:
	CBarElement(char* name): ImageObject(name) {
	};
	~CBarElement() {};
	// ���������ɃQ�[�W�̐F�����߂�B
	void Create(BarColor,float,float, Transform*);
	void Update()override;
	void SetValue(float value) {
		m_TargetValue = value;
		m_WorkValue = (m_TargetValue - m_Value) / (60.0f * m_Time);
		m_Dir = m_WorkValue / fabsf(m_WorkValue);
	}
	float GetValue() {
		return m_Value;
	}
	float GetTargetValue() {
		return m_TargetValue;
	}
	//void AddValue(int value) {
	//	m_Value += value;
	//}
private:
	void BarValueUpdate();
	void BarScaling();
private:
	float m_TargetValue;	// ��{���̒l(�X�V�����ł̖ڕW�l)�B
	float m_Value;		// ��{���̒l(�X�V�����ŕϓ�������l�ŁA���ۂɉ�ʂɕ\������Ă��鐔�l)�B
	float m_MaxValue;	// ��{���̍ő�l�B
	float m_InitBarScale_X;	// �o�[�摜�̍ŏ��̃X�P�[��(x)�B
	float m_InitBarPosition_X;	// �o�[�摜�̍ŏ��̃|�W�V����(x)�B
	float m_WorkValue; // m_Value��m_TargetValue�Ɍ����ď��X�ɐi�߂Ă����Ƃ��̒l�B
	float m_Dir = 1.0f;	// �X�V����(+1��-1)�B
	static float m_Time;	// m_Value��m_TargetValue�ɓ��B����܂ł̎���(�b)�B
};

namespace {
	static const char* BarFileNameArray[] = {
		"hp.png",	// �ԃQ�[�W�B
		"hp_back.png",	// ���F�Q�[�W�B
		"hp_Green.png",	// �΃Q�[�W�B
		"hp_Blue.png",	// �Q�[�W�B
	};
}

// �o�[�B
// ���p�����[�^�[�̌��Z�ɂ͂܂���Ή��B
class CHadBar :
	public Component
{
public:
	CHadBar(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()){};
	~CHadBar();
	// �o�[�����֐��B
	// ����:	�ʒu(���[�J�����W)�B
	//			��](���[�J�����W)�B
	//			�g�k(���[���h���W)�B
	//			�o�[�̍ő�l�B
	//			�����l�B
	void Create(const vector<BarColor>&, float max, float value, Transform* tr = nullptr, Vector3 pos = Vector3(175.0f, 20.0f, 0.0f),Vector2 scale = Vector2(1.0f, 3.0f));
	void Update()override;

private:
	// �ǂ̏��Ԃłǂ̐F��\�����邩�����߂��z���n���ACBarElement�̃C���X�^���X�𐶐�����֐��B
	// ����ɒǉ������F�̃Q�[�W���猸���Ă����B
	void ActiveBarColor(const vector<BarColor>& BarColorArray, float max, float value,Transform* tr);
	// �����Q�[�W������ۂ̃C�x���g�B
	void BreakEvent();

public:
	void SetValue(float value);
	//void AddValue(int value) {
	//	m_NowBar->AddValue(value);
	//}
	// ���Q�[�W�d�˂邩��ԋp�B
	inline short GetMaxBarNum()const {
		return m_MaxBarNum;
	}
	// �����Q�[�W�ڂ���ԋp�B
	inline short GetNowBarNum()const {
		return m_NowBarNum;
	}

private:
	short m_MaxBarNum;	// ���Q�[�W���d�˂邩�B
	vector<CBarElement*> m_BarElement;	// �o�[�B
	CBarElement* m_NowBar = nullptr;	// ���ݍX�V���̃o�[�B
	short m_NowBarNum;	// ���ݍX�V���̃o�[��\���Y�����B
	float m_Varue;		// �S�o�[���킹�Ă̒l�B
	float m_MaxValue;		// �S�o�[���킹�Ă̍ő�l�B
	CBarElement* m_NowSettingBar = nullptr;	// ���ݐ�s���͒��̃o�[�B
	short m_NowSettingNum;		// ���ݐ�s���͒��̃o�[��\���Y�����B
	short _framePriorty;
private:
	// �`��p�B
	ImageObject* _BarFrame = nullptr;	// �o�[�̘g�B
};