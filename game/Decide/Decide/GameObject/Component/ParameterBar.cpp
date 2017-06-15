#include "stdafx.h"
#include "GameObject\Component\ParameterBar.h"

// CBarElement�B
float BarElement::_Time = 0.2f;
void BarElement::Create(BarColor color, float max,Transform* parent) {
	Start(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB

	// �o�^����Ă���Q�[�W�̉摜���󂯎�����l�Ō���B
	SetTexture(LOADTEXTURE(const_cast<char*>(BarFileNameArray[color])));

	_MaxValue = max;
	_TargetValue = max;
	_Value = max;

	// Transform�ݒ�B
	transform->SetParent(parent);
	transform->SetLocalPosition(Vector3(-0.9f, 0.0f, 0.0f));
	transform->SetLocalScale(Vector3(2.54f, 1.0f, 1.0f));
	_InitBarScale_X = transform->GetLocalScale().x;
	_InitBarPosition_X = transform->GetLocalPosition().x;
}

void BarElement::Start() {
	// �R���|�[�l���g�Q��������(GameObjectManager�ɓo�^���Ă��Ȃ��̂ŁA���O�ŌĂяo��)�B
	GetComponentManager().Start();
	// �p�����̍X�V�������Ăяo���B
	ImageObject::Start();
}

void BarElement::Update() {
	// �ڕW�l�Ɍ����ăo�[�̒l�����X�ɍX�V�B
	_BarValueUpdate();
	// ���݂̒l�Ńo�[�̒������X�V����B
	_BarScaling();

	// �R���|�[�l���g�Q���X�V(GameObjectManager�ɓo�^���Ă��Ȃ��̂ŁA���O�ŌĂяo��)�B
	GetComponentManager().Update();
	// �p�����̍X�V�������Ăяo���B
	ImageObject::Update();
}

void BarElement::ImageRender() {
	// �R���|�[�l���g�Q��`��(GameObjectManager�ɓo�^���Ă��Ȃ��̂ŁA���O�ŌĂяo��)�B
	GetComponentManager().ImageRender();

	ImageObject::ImageRender();
}

void BarElement::_BarValueUpdate() {
	if (_Value >= _TargetValue && _Dir > 0) {
		// �X�V������+�A���ڕW�l�ɓ��B�����B
		_Value = _TargetValue;
	}
	else if (_Value <= _TargetValue && _Dir < 0) {
		// �X�V������-�A���ڕW�l�ɓ��B�����B
		_Value = _TargetValue;
	}
	else {
		_Value += _WorkValue;
	}
}

void BarElement::_BarScaling() {
	float weight = _Value / _MaxValue;
	if (fabsf(weight) <= 0.001f) {
		weight = 0.0f;
	}
	Vector3 scale = transform->GetLocalScale();
	scale.x = _InitBarScale_X * weight;
	transform->SetLocalScale(scale);

	// �T�C�Y�������������炷�B
	float offset = (_InitBarScale_X - scale.x) * 0.5f;
	Vector3 pos = transform->GetLocalPosition();
	pos.x = _InitBarPosition_X - (offset * GetTexture()->Size.x);	// �e�N�X�`���T�C�Y���X�P�[�����O�����l���|���Ă��炸�炷�B
	transform->SetLocalPosition(pos);
}

// ParameterBar�B
const Vector3 ParameterBar::CreatePos_DefaultArg = Vector3(175.0f, 21.9f, 0.0f);
const Vector2 ParameterBar::CreateScale_DefaultArg = Vector2(1.0f, 1.0f);

ParameterBar::~ParameterBar()
{
}

void ParameterBar::Create(const vector<BarColor>& BarColorArray,float max,float value,Transform* parent,const Vector3& pos,const Vector2& scale, bool isHud) {
	// ��ƗpTransform����ۑ��B
	// �e�q�֌W��g�񂾏ꍇ�͐e���ړ������肷��Ǝ����X�V�����B
	if (parent) {
		_Transform->SetParent(parent);
	}
	_Transform->SetLocalPosition(pos);
	_Transform->SetScale(Vector3(scale.x, scale.y, 1.0f));

	// �o�[�̃t���[���𐶐��B
	// ���e�q�֌W���쐬����Ə���ɍX�V����邽�߁A�����ł͐e�q�֌W�̂Ȃ���΍��W��n���B
	CreateBarFrame(_Transform->GetPosition(), _Transform->GetScale(),isHud);

	// �w�肳�ꂽ�F�̏��Ɛ��ɏ]���ăo�[�𐶐��B
	_ActiveBarColor(BarColorArray,max,value, _BarFrame->transform);
	// �ŏ��̃o�[������B
	_NowBarNum = _NowSettingNum = 0;
	_NowBar = _NowSettingBar = _BarElement[_NowBarNum].get();

	// �e�l�̏����l�ݒ�B
	_MaxValue = max;
	_Value = max;

	_UpdateValue(value);	// �����l���e�o�[�ɕ��z���Đݒ�B
}

void ParameterBar::Update() {
	// �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
	{
		_BarFrame->GetComponentManager().Update();
		// Transform���͖��t���[���X�V����邽�߁A���t���[���X�N���[�����W�ɂ��ď㏑������B
		_ToScreenPos();

		_NowBar->Update();
	}
	if (_NowBar->GetValue() <= 0.0f) {
		_NowBarNum++;
		if (_NowBarNum < 0) {
			_NowBarNum = 0;
		}
		if (_NowBarNum < _MaxBarNum) {
			// �X�V���̃o�[��ύX�B
			_NowBar = _BarElement[_NowBarNum].get();
		}
	}
}

void ParameterBar::ImageRender() {
	// �w�ʂ̂��̂���`�悵�Ă����B
	_BarFrame->GetComponentManager().ImageRender();// �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
	for (int idx = _BarElement.size() - 1; idx >= 0;idx--) {
		_BarElement[idx]->ImageRender(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
	}
}

void ParameterBar::_UpdateValue(float value) {
	float work = static_cast<int>(value) % (static_cast<int>(_MaxValue) / _MaxBarNum);	// ��{�̃o�[�ɃZ�b�g����l���Z�o�B���������]����Z�o�������̂�int�ɃL���X�g���Ă���B

	float Difference = _Value - value;	// ��u�O�̒l�Ƃ̍������Z�o�B
	if(Difference >= 0.0001f){
		// ��u�O�̒l�ƈႤ�l���ݒ肳�ꂽ�B
		while (true) {
			float nowBarValue = _NowSettingBar->GetTargetValue();	// ���݂̃Q�[�W�����l���擾�B
			if (nowBarValue <= Difference) {
				// 1�Q�[�W�ȏ������B
				// �u���C�N���̃C�x���g�B
				this->_BreakEvent();
				// �Ƃ肠�������̃o�[�̒l��0�ɂ���B
				_NowSettingBar->SetValue(0.0f);
				_NowSettingNum++;
				if (_NowSettingNum < _MaxBarNum) {
					// ���̃o�[������΂�����Ƀ^�[�Q�b�g��ύX�B
					_NowSettingBar = _BarElement[_NowSettingNum].get();
					if (nowBarValue < Difference) {
						// 1�Q�[�W�ȏ゠�ӂ��B

						// �ݒ肵���l��������������A�V����������ݒ�B
						Difference -= nowBarValue;
						// �����𑱍s�B
						continue;
					}
					else {
						// ���傤��1�Q�[�W�g�����B
						break;
					}
				}
				else {
					// ���̃o�[���Ȃ���Ώ������I���B
					_NowSettingNum--;
					break;
				}
			}
			else {
				// ���ӂ�Ȃ��ꍇ�̓o�[�ɓ���ď������I���B
				_NowSettingBar->SetValue(work);
				break;
			}
		}
		// �S�̂Ƃ��Ă̒l���X�V�B
		_Value = value;
	}
}

void ParameterBar::CreateBarFrame(const Vector3& pos,const Vector3& scale,bool isHud){
	_BarFrame.reset(new ImageObject("BarFrame"));
	_BarFrame->Awake();
	_BarFrame->Start();
	_BarFrame->GetComponentManager().Start();

	_BarFrame->transform->SetPosition(pos);
	_BarFrame->transform->SetScale(scale);

	_isHud = isHud;
	_ToScreenPos();

	_BarFrame->transform->UpdateTransform();
	_BarFrame->SetTexture(LOADTEXTURE("hpsp_bar.png"));
}

void ParameterBar::_ActiveBarColor(const vector<BarColor>& BarColorArray,float max,float value,Transform* tr) {
	_MaxBarNum = BarColorArray.size();
	for (auto BarColor : BarColorArray) {
		BarElement* bar = new BarElement("BarElement")/*INSTANCE(GameObjectManager)->AddNew<CBarElement>("BarElement", idx)*/;
		bar->Awake(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB

		// �o�[���d�˂�ꍇ�͒l���d�˂�o�[�̐���������������B
		bar->Create(BarColor, max / _MaxBarNum,tr);
		_BarElement.push_back(unique_ptr<BarElement>(bar));
	}
}

void ParameterBar::_BreakEvent() {
}

void ParameterBar::_ToScreenPos() {
	if (!_isHud) {
		// HUD�Ƃ��Ďg�p�����A�L�����N�^�[�̍��W�n��p���Ď��ӂɕ\������B

		// �X�N���[�����W�n�ɕϊ����čĐݒ�B
		_BarFrame->transform->SetPosition(Vector3(INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Transform->GetPosition()), 0.0f));
	}
}
