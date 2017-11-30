#include "stdafx.h"
#include "GameObject\Component\ParameterBar.h"

// CBarElement�B
float BarElement::_Time = 0.1f;
void BarElement::Create(BarColor color, float max,Transform* parent) {
	Start(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB

	// �o�^����Ă���Q�[�W�̉摜���󂯎�����l�Ō���B
	SetTexture(LOADTEXTURE(const_cast<char*>(BarFileNameArray[color])));

	_MaxValue = max;
	_TargetValue = max;
	_Value = max;
	_isInterpolation = false;

	// Transform�ݒ�B
	transform->SetParent(parent);
	transform->SetLocalPosition(Vector3(-0.9f, 0.0f, 0.0f));
	transform->SetLocalScale(Vector3(2.54f, 1.0f, 1.0f));
	_InitBarScale_X = transform->GetLocalScale().x;
	_InitBarPosition_X = transform->GetLocalPosition().x;
}

void BarElement::Start() {
	// �R���|�[�l���g�Q��������(GameObjectManager�ɓo�^���Ă��Ȃ��̂ŁA���O�ŌĂяo��)�B
	//GetComponentManager().Start();
	// �p�����̍X�V�������Ăяo���B
	ImageObject::Start();
}

void BarElement::Update() {
	// �ڕW�l�Ɍ����ăo�[�̒l�����X�ɍX�V�B
	_BarValueUpdate();
	// ���݂̒l�Ńo�[�̒������X�V����B
	_BarScaling();

	// �R���|�[�l���g�Q���X�V(GameObjectManager�ɓo�^���Ă��Ȃ��̂ŁA���O�ŌĂяo��)�B
	GetComponentManager().ConnoteUpdate();
	// �p�����̍X�V�������Ăяo���B
	ImageObject::Update();
}

void BarElement::ImageRender() {
	// �R���|�[�l���g�Q��`��(GameObjectManager�ɓo�^���Ă��Ȃ��̂ŁA���O�ŌĂяo��)�B
	GetComponentManager().ImageRender();

	ImageObject::ImageRender();
}

void BarElement::_BarValueUpdate() {
	if (_isInterpolation) {
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
	else {
		_Value = _TargetValue;
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

// BarAdapter�B
BarAdapter::~BarAdapter() {
	_BarFrame->GetComponentManager().OnDestroy();// �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
	_BarBack->GetComponentManager().OnDestroy();
	for (int idx = _BarElement.size() - 1; idx >= 0; idx--) {
		_BarElement[idx]->OnDestroy(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
	}
};

void BarAdapter::Create(const vector<BarColor>& BarColorArray, float max, float value,  bool isInterpolation, bool isRenderFrame, Transform* parent, const Vector3& pos, const Vector2& scale, bool isBackColor, bool isHud) {
	// ��ƗpTransform����ۑ��B
	// �e�q�֌W��g�񂾏ꍇ�͐e���ړ������肷��Ǝ����X�V�����B
	if (parent) {
		transform->SetParent(parent);
	}
	transform->SetLocalPosition(pos);
	transform->SetScale(Vector3(scale.x, scale.y, 1.0f));

	// �o�[�̘g��`�悷�邩�B
	_isRenderFrame = isRenderFrame;

	// �o�[�̃t���[���𐶐��B
	// ���e�q�֌W���쐬����Ə���ɍX�V����邽�߁A�����ł͐e�q�֌W�̂Ȃ���΍��W��n���B
	_CreateBarFrame(transform->GetPosition(), transform->GetScale(), isHud);


	_isBackColor = isBackColor;

	// �o�[�̔w�i�����B
	_BarBack.reset(new ImageObject("BarBack"));
	_BarBack->Awake();
	_BarBack->Start();
	// Transform�ݒ�B
	_BarBack->transform->SetParent(_BarFrame->transform);
	_BarBack->transform->SetLocalPosition(Vector3(-0.9f, 0.0f, 0.0f));
	_BarBack->transform->SetLocalScale(Vector3(2.54f, 1.0f, 1.0f));

	_BarBack->GetComponentManager().ConnoteUpdate();
	_BarBack->SetTexture(LOADTEXTURE("hp_back.png"));


	// �w�肳�ꂽ�F�̏��Ɛ��ɏ]���ăo�[�𐶐��B
	_ActiveBarColor(BarColorArray, max, _BarFrame->transform);

	this->SetIsRender(true);

	// �e�l�̏����l�ݒ�B
	_MaxValue = max;
	_Value = max;

	// �ŏ��̃o�[������B
	_NowBarNum = _NowSettingNum = 0;
	_NowBar = _NowSettingBar = _BarElement[_NowBarNum].get();

	_UpdateValue(value);	// �����l���e�o�[�ɕ��z���Đݒ�B
	
	// �ݒ肵���l�ł��ׂẴo�[���X�V�B
	for (int idx = 0; idx < _BarElement.size(); idx++) {
		_BarElement[idx]->Update();	
		_BarElement[idx]->SetIsInterpolation(isInterpolation);	// ��ԃt���O���I���ɂ���B
	}
}

void BarAdapter::Reset(float max, float value,bool isInterpolation) {
	for (auto& element : _BarElement) {
		// �S�Ẵo�[��V�����ő�l�ŏ������B
		element->Reset(max / _MaxBarNum);
	}

	// �ŏ��̃o�[������B
	_NowBarNum = _NowSettingNum = 0;
	_NowBar = _NowSettingBar = _BarElement[_NowBarNum].get();

	// �e�l�̏����l�ݒ�B
	_MaxValue = max;
	_Value = max;

	_UpdateValue(value);	// �����l���e�o�[�ɕ��z���Đݒ�B

	// �ݒ肵���l�ł��ׂẴo�[���X�V�B
	for (int idx = 0; idx < _BarElement.size(); idx++) {
		_BarElement[idx]->Update();
		_BarElement[idx]->SetIsInterpolation(isInterpolation);	// ��ԃt���O���I���ɂ���B
	}
}

void BarAdapter::Update() {
	if (_parentComponent) {
		if (_parentComponent->gameObject) {
			if (!_parentComponent->gameObject->GetActive()) {
				// ���̃o�[�����I�u�W�F�N�g����A�N�e�B�u�ɂȂ��Ă���΂��̃A�_�v�^�[����A�N�e�B�u�ɂ���B
				this->SetActive(false);
			}
		}
	}

	// �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
	{
		_BarFrame->GetComponentManager().ConnoteUpdate();
		_BarBack->GetComponentManager().ConnoteUpdate();
		// Transform���͖��t���[���X�V����邽�߁A���t���[���X�N���[�����W�ɂ��ď㏑������B
		_ToScreenPos();

		_NowBar->Update();
	}
	//while (true) {
		if (_NowBar->GetValue() <= 0.0f) {
			_NowBarNum++;
			if (_NowBarNum < _MaxBarNum) {
				// �X�V���̃o�[��ύX�B
				_NowBar = _BarElement[_NowBarNum].get();
			}
			else {
				// ����ȏ�o�[�������̂ŏ������I���B

				_NowBarNum--;
				return;
				//break;
			}
		}
		else if (_NowBar->GetValue() >= _NowBar->GetMaxValue()) {
			_NowBarNum--;
			if (_NowBarNum >= 0) {
				// �X�V���̃o�[��ύX�B
				_NowBar = _BarElement[_NowBarNum].get();
			}
			else {
				// ����ȏ�o�[�������̂ŏ������I���B

				_NowBarNum++;
				return;
				//break;
			}
		}
		else {
			// ���݂̃o�[���X�V���B

			//break;
		}
	//}
}

void BarAdapter::ImageRender() {
	if (_isRender) {
		// �w�ʂ̂��̂���`�悵�Ă����B
		if (_isRenderFrame) {
			// �o�[�̘g��`�悷��B
			_BarFrame->GetComponentManager().ImageRender();// �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
		}
		if (_isBackColor) {
			_BarBack->GetComponentManager().ImageRender();
		}
		for (int idx = _BarElement.size() - 1; idx >= 0; idx--) {
			_BarElement[idx]->ImageRender(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
		}
	}
}

void BarAdapter::_UpdateValue(float value) {
	float Difference = _Value - value;	// ��u�O�̒l�Ƃ̍������Z�o�B
	if (fabsf(Difference) >= 0.0001f) {
		// ��u�O�̒l�ƈႤ�l���ݒ肳�ꂽ�B
		if (Difference > 0.0f) {
			// ���Z�����B
			_UpdateSubValue(Difference);
		}
		else {
			// ���Z�����B
			_UpdateAddValue(Difference);
		}

		// �S�̂Ƃ��Ă̒l���X�V�B
		if (value < 0.0f) {
			_Value = 0.0f;
		}
		else if (value >= _MaxValue) {
			_Value = _MaxValue;
		}
		else {
			_Value = value;
		}
	}
}

void BarAdapter::_UpdateSubValue(float Difference) {
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
			_NowSettingBar->SetValue(nowBarValue - Difference);
			break;
		}
	}
}

void BarAdapter::_UpdateAddValue(float Difference) {
	Difference *= -1.0f;
	while (true) {
		float nowBarBusy = _NowSettingBar->GetMaxValue() - _NowSettingBar->GetTargetValue();	// ���݂̃Q�[�W�ɓ���邱�Ƃ̂ł���l���擾�B
		if (Difference > nowBarBusy) {
			// 1�Q�[�W�ɓ��肫��Ȃ��B
			// �Ƃ肠�������̃o�[�̒l���ő�l�ɂ���B
			_NowSettingBar->SetValue(_NowSettingBar->GetMaxValue());
			_NowSettingNum--;
			if (_NowSettingNum >= 0) {
				// ���̃o�[������΂�����Ƀ^�[�Q�b�g��ύX�B
				_NowSettingBar = _BarElement[_NowSettingNum].get();
				// �ݒ肵���l��������������A�V����������ݒ�B
				Difference -= nowBarBusy;
				// �����𑱍s�B
				continue;
			}
			else {
				// ���̃o�[���Ȃ���Ώ������I���B
				_NowSettingNum++;
				break;
			}
		}
		else {
			// ���ӂ�Ȃ��ꍇ�̓o�[�ɓ���ď������I���B
			if (fabsf(nowBarBusy - Difference) < 0.0001f) {
				// �W���X�g�ő�l���ݒ肳�ꂽ�B
				_NowSettingBar->SetValue(_NowSettingBar->GetMaxValue());
			}
			else {
				// ���̃o�[�ɓ���ė]�����l��ݒ�B
				_NowSettingBar->SetValue(_NowSettingBar->GetTargetValue() + Difference);
			}

			break;
		}
	}
}

void BarAdapter::_CreateBarFrame(const Vector3& pos, const Vector3& scale, bool isHud) {
	_BarFrame.reset(new ImageObject("BarFrame"));
	_BarFrame->Awake();
	_BarFrame->Start();
	_BarFrame->GetComponentManager().ConnoteUpdate();

	_BarFrame->transform->SetPosition(pos);
	_BarFrame->transform->SetScale(scale);

	_isHud = isHud;
	_ToScreenPos();

	_BarFrame->transform->UpdateTransform();
	_BarFrame->SetTexture(LOADTEXTURE("hpsp_bar.png"));
}

void BarAdapter::_ActiveBarColor(const vector<BarColor>& BarColorArray, float max, Transform* tr) {
	_MaxBarNum = BarColorArray.size();
	for (auto BarColor : BarColorArray) {
		BarElement* bar = new BarElement("BarElement")/*INSTANCE(GameObjectManager)->AddNew<CBarElement>("BarElement", idx)*/;
		bar->Awake(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB

		// �o�[���d�˂�ꍇ�͒l���d�˂�o�[�̐���������������B
		bar->Create(BarColor, max / _MaxBarNum,tr);
		_BarElement.push_back(unique_ptr<BarElement>(bar));
	}
}

void BarAdapter::_BreakEvent() {
}

void BarAdapter::_ToScreenPos() {
	if (!_isHud) {
		// HUD�Ƃ��Ďg�p�����A�L�����N�^�[�̍��W�n��p���Ď��ӂɕ\������B
		
		// �X�N���[�����W�n�ɕϊ����čĐݒ�B
		_BarFrame->transform->SetPosition(Vector3(INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(transform->GetPosition()), 0.0f));
	}
}

// ParameterBar�B
const Vector3 ParameterBar::CreatePos_DefaultArg = Vector3(175.0f, 21.9f, 0.0f);
const Vector2 ParameterBar::CreateScale_DefaultArg = Vector2(1.0f, 1.0f);

ParameterBar::~ParameterBar()
{
	INSTANCE(GameObjectManager)->AddRemoveList(_Object);
}

void ParameterBar::Update() {
	//�o�[�̍X�V�������Ă΂�Ă���̂ŃA�_�v�^�[���A�N�e�B�u�ɂ���B
	_Object->SetActive(true);
}