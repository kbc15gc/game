#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

class ParameterBar;

enum BarColor { None = -1,Red = 0, Yellow, Green, Blue,Max };

// �o�[�̒��g�B
class BarElement:public ImageObject{
public:
	BarElement(char* name): ImageObject(name) {
	};
	~BarElement() {
	};
	// ���������ɃQ�[�W�̐F�����߂�B
	void Create(BarColor,float, Transform*);
	void Start()override;
	void Update()override;
	void ImageRender()override;

	// �o�[��V�����l�ōď������B
	void Reset(float max) {
		_MaxValue = max;
		_TargetValue = max;
		_Value = max;
		transform->SetLocalPosition(Vector3(-0.9f, 0.0f, 0.0f));
		transform->SetLocalScale(Vector3(2.54f, 1.0f, 1.0f));
	}


	void SetValue(float value) {
		if (fabsf(_TargetValue - value) <= 0.0001f) {
			// �����l���Đݒ肳�ꂽ�Ɣ��f�B
			return;
		}
		_TargetValue = value;
		_WorkValue = (_TargetValue - _Value) / (60.0f * _Time);
		_Dir = _WorkValue / fabsf(_WorkValue);
	}
	inline float GetTargetValue() const{
		return _TargetValue;
	}
	inline float GetValue() const {
		return _Value;
	}
	inline float GetMaxValue()const {
		return _MaxValue;
	}

	inline void SetIsInterpolation(bool flg) {
		_isInterpolation = flg;
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
	bool _isInterpolation = false;	// �o�[�̃X�P�[�����O���Ԃ��邩�B
};

namespace {
	static const char* BarFileNameArray[] = {
		"hp.png",	// �ԃQ�[�W�B
		"hp_Yellow.png",	// ���F�Q�[�W�B
		"hp_Green.png",	// �΃Q�[�W�B
		"hp_Blue.png",	// �Q�[�W�B
	};
}

// �o�[�̓L�����N�^�[�̎��s�D��x�Ƃ͓Ɨ����čX�V�������̂ŁA���C���[�Ƃ��ăN���X�����ށB
class BarAdapter :public GameObject {
public:
	BarAdapter(char* name) : GameObject(name) {
		//constNum++;
	};
	~BarAdapter();
	// �o�[�����֐��B
	// ����:	�ǂ̏��Ԃłǂ̐F�̃Q�[�W��\�����邩�����߂��z��(��ɒǉ������F�̃Q�[�W����X�V)�B
	//			�o�[�ɐݒ肷��ő�l(HP�ő�ʂȂ�)�B
	//			�o�[�ɐݒ肷�鏉���l(HP�ʂȂ�)�B
	//			�o�[�̃X�P�[�����O�̍ۂɕ�ԏ������g�p���邩�B
	//			�o�[�̘g��`�悷�邩�B
	//			�e��Transform���(���ݒ肩null�w��Őݒ肵�Ȃ��悤�ɂł���)�B
	//			�ʒu(���[�J�����W�A���ݒ�ŉ�ʂ̍���ɕ\��)�B
	//			�g�k(���[���h���W�A���ݒ�ŉ�ʂ̍���ɕ\��)�B
	//			�o�[�̔w�i��`�悷�邩�B
	//			HUD�Ƃ��Ďg�p���邩�B
	void Create(const vector<BarColor>& colors, float max, float value, bool isInterpolation, bool isRenderFrame, Transform* tr, const Vector3& pos, const Vector2& scale,bool isBackColor, bool isHud);
	void Update();
	void ImageRender()override;

	// �쐬�ς݂̃o�[�ɒl��ݒ肵�Ȃ����֐��B
	// �����F	�ő�l�B
	//			���ݒl�B
	void Reset(float max, float value, bool isInterpolation);

	void SetActive(const bool act, const bool children = false)override
	{
		_BarFrame->SetActive(act);
		// �o�[�֘A�̃I�u�W�F�N�g��S���؂�ւ��B
		for (auto& element : _BarElement) {
			element->SetActive(act);
		}
		_BarBack->SetActive(act);
		GameObject::SetActive(act, children);
	}

private:
	// �o�[�̘g�𐶐�����֐��B
	// �����F	�ʒu(���[���h���W)�B
	//			�g�k(���[���h���W)�B
	//			HUD�Ƃ��Ďg�p���邩(�f�t�H���g��true)�B
	void _CreateBarFrame(const Vector3& pos, const Vector3& scale, bool isHud);
	// �ǂ̏��Ԃłǂ̐F��\�����邩�����߂��z���n���ACBarElement�̃C���X�^���X�𐶐�����֐��B
	// ����ɒǉ������F�̃Q�[�W���猸���Ă����B
	void _ActiveBarColor(const vector<BarColor>& BarColorArray, float max, Transform* tr);
	// �����Q�[�W������ۂ̃C�x���g�B
	void _BreakEvent();
	// �����̒l�Ŋe�o�[�ɐݒ肷��l���X�V�B
	void _UpdateValue(float value);
	// �o�[�̒l�����Z�X�V�B
	// �����F	���̒l�Ƃ̍����B
	void _UpdateSubValue(float Difference);
	// �o�[�̒l�����Z�X�V�B
	void _UpdateAddValue(float Difference);
	// HUD�Ƃ��Ďg�p���Ȃ��ꍇ�ɃX�N���[�����W�n�ɕϊ�����֐��B
	void _ToScreenPos();

public:
	inline void SubValue(float value) {
		_UpdateValue(_Value - value);
	}
	inline void AddValue(float value) {
		_UpdateValue(_Value + value);
	}
	inline void SetValue(float value) {
		_UpdateValue(value);
	}
	// ���Q�[�W�d�˂邩��ԋp�B
	inline short GetMaxBarNum()const {
		return _MaxBarNum;
	}
	// �����Q�[�W�ڂ���ԋp�B
	inline short GetNowBarNum()const {
		return _NowBarNum;
	}

	inline void SetParentComponet(ParameterBar* parent) {
		_parentComponent = parent;
	}

	inline void SetIsRender(bool flg) {
     		_isRender = flg;
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
private:
	// �`��p�B
	bool _isHud;		// HUD�Ƃ��Ďg�p���邩�B
	bool _isRenderFrame;	// �o�[�̘g��`�悷�邩�B
	unique_ptr<ImageObject> _BarFrame;	// �o�[�̘g�B
	unique_ptr<ImageObject> _BarBack;	// �o�[�̔w�i�B
private:
	ParameterBar* _parentComponent = nullptr;	// ���̃A�_�v�^�[�𐶐������e�R���|�[�l���g�B
	bool _isRender = true;
	bool _isBackColor = false;	// �o�[�̔w�i��`�悷�邩�B

	//static int destNum;
	//static int constNum;
};

// �o�[�B
class ParameterBar :
	public Component
{
	static const Vector3 CreatePos_DefaultArg;
	static const Vector2 CreateScale_DefaultArg;
public:
	ParameterBar(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
		//constNum++;
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~ParameterBar() {};

	void OnDestroy()override;

	// �o�[�����֐��B
	// ����:	�ǂ̏��Ԃłǂ̐F�̃Q�[�W��\�����邩�����߂��z��(��ɒǉ������F�̃Q�[�W����X�V)�B
	//			�o�[�ɐݒ肷��ő�l(HP�ő�ʂȂ�)�B
	//			�o�[�ɐݒ肷�鏉���l(HP�ʂȂ�)�B
	//			�o�[�̃X�P�[�����O�̍ۂɕ�ԏ������g�p���邩(�f�t�H���g��true)�B
	//			�o�[�̘g��`�悷�邩(�f�t�H���g��true)�B
	//			�e��Transform���(���ݒ肩null�w��Őݒ肵�Ȃ��悤�ɂł���)�B
	//			�X�V�D��x(�f�t�H���g��5)�B
	//			�ʒu(���[�J�����W�A���ݒ�ŉ�ʂ̍���ɕ\��)�B
	//			�g�k(���[���h���W�A���ݒ�ŉ�ʂ̍���ɕ\��)�B
	//			�o�[�̔w�i���g�p���邩(�f�t�H���g��false)�B
	//			HUD�Ƃ��Ďg�p���邩(�f�t�H���g��true)�B
	inline void Create(const vector<BarColor>& colors, float max, float value,bool isInterpolation = true, bool isRenderFrame = true, Transform* tr = nullptr, int priorty = 5, const Vector3& pos = CreatePos_DefaultArg, const Vector2& scale = CreateScale_DefaultArg, bool isBackColor = false,bool isHud = true) {
		if (_Object) {
			INSTANCE(GameObjectManager)->AddRemoveList(_Object);
		}
		_Object = INSTANCE(GameObjectManager)->AddNew<BarAdapter>("ParamterBar", priorty);
		_Object->Create(colors,max, value, isInterpolation,isRenderFrame, tr, pos, scale, isBackColor,isHud);
		_Object->SetParentComponet(this);	// �X�V�Ǘ����s�����߂ɂ��̃N���X�̃|�C���^��n���B
	}

	// �쐬�ς݂̃o�[�ɒl��ݒ肵�Ȃ����֐��B
	// �����F	�ő�l�B
	//			���ݒl�B
	//			�o�[�̃X�P�[�����O�ɕ�ԏ������g�p���邩�B
	inline void Reset(float max, float value,bool isInterpolaation) {
		_Object->Reset(max,value, isInterpolaation);
	}
public:
	inline void SubValue(float value) {
		_Object->SubValue(value);
	}
	inline void AddValue(float value) {
		_Object->AddValue(value);
	}
	inline void SetValue(float value) {
		_Object->SetValue(value);
	}
	// ���Q�[�W�d�˂邩��ԋp�B
	inline short GetMaxBarNum()const {
		return _Object->GetMaxBarNum();
	}
	// �����Q�[�W�ڂ���ԋp�B
	inline short GetNowBarNum()const {
		return _Object->GetNowBarNum();
	}
	// HadBar��Transform�����擾�B
	inline Transform* GetTransform()const {
		return _Object->transform;
	}

	inline void RenderEnable() {
		_Object->SetIsRender(true);
	}
	inline void RenderDisable() {
		_Object->SetIsRender(false);
	}

	void SetEnable(const bool flg)override
	{
		if (_Object) {
			_Object->SetActive(flg);
		}
		Component::SetEnable(flg);
	}

private:
	BarAdapter* _Object = nullptr;
	//static int destNum;
	//static int constNum;
};