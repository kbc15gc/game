#pragma once
#include "_Object\Noncopyable.h"


namespace fbEngine
{
	namespace VPad
	{
		//���z�{�^����`�B
		enum ButtonE {
			ButtonUp,		//��B
			ButtonDown,		//���B
			ButtonLeft,		//���B
			ButtonRight,	//�E�B
			ButtonA,		//A�{�^���B
			ButtonB,		//B�{�^���B
			ButtonY,		//X�{�^���B
			ButtonX,		//Y�{�^���B
			ButtonSelect,	//�Z���N�g�{�^���B
			ButtonStart,	//�X�^�[�g�{�^���B
			ButtonRB1,		//RB1�{�^���B
			ButtonRB2,		//RB2�{�^���B
			ButtonRB3,		//RB3�{�^���B
			ButtonLB1,		//LB1�{�^���B
			ButtonLB2,		//LB2�{�^���B
			ButtonLB3,		//LB3�{�^���B
			ButtonNum,		//�{�^���̐��B
		};
	}
}

//���z�p�b�h�N���X�B
class VirtualPad :Noncopyable
{
public:
	VirtualPad();
	//
	void Initialize(KeyBoard* key, XInput* xbox);
	//
	bool IsPush(fbEngine::VPad::ButtonE button);
	bool IsPress(fbEngine::VPad::ButtonE button);
	Vector2 GetAnalog(AnalogE analog);
	bool KeyRepeat(fbEngine::VPad::ButtonE button, float interval);
private:
	XInput* _Pad;
	KeyBoard* _Key;

	int _Trigger[fbEngine::VPad::ButtonE::ButtonNum];	//�g���K�[���͂̃t���O�B
	int _Press[fbEngine::VPad::ButtonE::ButtonNum];		//�v���X���͂̃t���O�B
};