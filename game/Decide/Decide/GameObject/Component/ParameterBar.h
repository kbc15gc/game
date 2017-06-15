#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

enum BarColor {  Red = 0, Yellow,Green,Blue};
// �o�[�̒��g�B
class BarElement:public ImageObject{
public:
	BarElement(char* name): ImageObject(name) {
	};
	~BarElement() {};
	// ���������ɃQ�[�W�̐F�����߂�B
	void Create(BarColor,float, Transform*);
	void Start()override;
	void Update()override;
	void ImageRender()override;
	void SetValue(float value) {
		_TargetValue = value;
		_WorkValue = (_TargetValue - _Value) / (60.0f * _Time);
		_Dir = _WorkValue / fabsf(_WorkValue);
	}
	float GetValue() {
		return _Value;
	}
	float GetTargetValue() {
		return _TargetValue;
	}
private:
	void _BarValueUpdate();
	void _BarScaling();
private:
	float _TargetValue;	// ��{���̒l(�X�V�����ł̖ڕW�l)�B
	float _Value;		// ��{���̒l(�X�V�����ŕϓ�������l�ŁA���ۂɉ�ʂɕ\������Ă��鐔�l)�B
	float _MaxValue;	// ��{���̍ő�l�B
	float _InitBarScale_X;	// �o�[�摜�̍ŏ��̃X�P�[��(x)�B
	float _InitBarPosition_X;	// �o�[�摜�̍ŏ��̃|�W�V����(x)�B
	float _WorkValue; // m_Value��m_TargetValue�Ɍ����ď��X�ɐi�߂Ă����Ƃ��̒l�B
	float _Dir = 1.0f;	// �X�V����(+1��-1)�B
	static float _Time;	// m_Value��m_TargetValue�ɓ��B����܂ł̎���(�b)�B
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
class ParameterBar :
	public Component
{
	static const Vector3 CreatePos_DefaultArg;
	static const Vector2 CreateScale_DefaultArg;
public:
	ParameterBar(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
		// ���̃R���|�[�l���g�����Q�[���I�u�W�F�N�g�ɊǗ����Ϗ�����B
		// ���I�u�W�F�N�g�w���゠�܂��낵���͂Ȃ��Ǝv���B
		if (g) {
			_Transform = g->AddComponent<Transform>();
		}
	};
	~ParameterBar();
	// �o�[�����֐��B
	// ����:	�ǂ̏��Ԃłǂ̐F�̃Q�[�W��\�����邩�����߂��z��(��ɒǉ������F�̃Q�[�W����X�V)�B
	//			�o�[�ɐݒ肷��ő�l(HP�ő�ʂȂ�)�B
	//			�o�[�ɐݒ肷�鏉���l(HP�ʂȂ�)�B
	//			�o�[�̘g��`�悷�邩�B
	//			�e��Transform���(���ݒ肩null�w��Őݒ肵�Ȃ��悤�ɂł���)�B
	//			�ʒu(���[�J�����W�A���ݒ�ŉ�ʂ̍���ɕ\��)�B
	//			�g�k(���[���h���W�A���ݒ�ŉ�ʂ̍���ɕ\��)�B
	//			HUD�Ƃ��Ďg�p���邩(�f�t�H���g��true)�B
	void Create(const vector<BarColor>&, float max, float value, bool isRenderFrame = true, Transform* tr = nullptr,const Vector3& pos = CreatePos_DefaultArg,const Vector2& scale = CreateScale_DefaultArg, bool isHud = true);
	void Update()override;
	void ImageRender()override;

private:
	// �o�[�̘g�𐶐�����֐��B
	// �����F	�ʒu(���[���h���W)�B
	//			�g�k(���[���h���W)�B
	//			HUD�Ƃ��Ďg�p���邩(�f�t�H���g��true)�B
	void CreateBarFrame(const Vector3& pos, const Vector3& scale, bool isHud);
	// �ǂ̏��Ԃłǂ̐F��\�����邩�����߂��z���n���ACBarElement�̃C���X�^���X�𐶐�����֐��B
	// ����ɒǉ������F�̃Q�[�W���猸���Ă����B
	void _ActiveBarColor(const vector<BarColor>& BarColorArray, float max, float value,Transform* tr);
	// �����Q�[�W������ۂ̃C�x���g�B
	void _BreakEvent();
	// �����̒l�Ńo�[�̃T�C�Y���X�V�B
	void _UpdateValue(float value);
	// HUD�Ƃ��Ďg�p���Ȃ��ꍇ�ɃX�N���[�����W�n�ɕϊ�����֐��B
	void _ToScreenPos();
public:
	inline void SubValue(float value) {
		_UpdateValue(_Value - value);
	}
	void AddValue(float value) {
		_UpdateValue(_Value + value);
	}
	// ���Q�[�W�d�˂邩��ԋp�B
	inline short GetMaxBarNum()const {
		return _MaxBarNum;
	}
	// �����Q�[�W�ڂ���ԋp�B
	inline short GetNowBarNum()const {
		return _NowBarNum;
	}
	// HadBar��Transform�����擾�B
	inline Transform* GetTransform()const {
		return _Transform;
	}
private:
	short _MaxBarNum;	// ���Q�[�W���d�˂邩�B
	vector<unique_ptr<BarElement>> _BarElement;	// �o�[�B
	BarElement* _NowBar = nullptr;	// ���ݍX�V���̃o�[�B
	short _NowBarNum;	// ���ݍX�V���̃o�[��\���Y�����B
	float _Value;		// �S�o�[���킹�Ă̒l�B
	float _MaxValue;		// �S�o�[���킹�Ă̍ő�l�B
	BarElement* _NowSettingBar = nullptr;	// ���ݐ�s���͒��̃o�[�B
	short _NowSettingNum;		// ���ݐ�s���͒��̃o�[��\���Y�����B
	short _framePriorty;
	Transform* _Transform = nullptr;	// Bar��Transform���(�O����)�B
private:
	// �`��p�B
	bool _isHud;		// HUD�Ƃ��Ďg�p���邩�B
	bool _isRenderFrame;	// �o�[�̘g��`�悷�邩�B
	unique_ptr<ImageObject> _BarFrame;	// �o�[�̘g�B
};