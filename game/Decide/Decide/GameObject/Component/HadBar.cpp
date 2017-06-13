#include "stdafx.h"
#include "GameObject\Component\HadBar.h"

// CBarElement�B
float CBarElement::m_Time = 0.2f;
void CBarElement::Create(BarColor color, float max,Transform* parent) {
	Start(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB

	// �o�^����Ă���Q�[�W�̉摜���󂯎�����l�Ō���B
	SetTexture(LOADTEXTURE(const_cast<char*>(BarFileNameArray[color])));

	m_MaxValue = max;
	m_TargetValue = max;
	m_Value = max;

	// Transform�ݒ�B
	transform->SetParent(parent);
	transform->SetLocalPosition(Vector3(-0.9f, 0.0f, 0.0f));
	transform->SetLocalScale(Vector3(2.54f, 1.0f, 0.0f));
	m_InitBarScale_X = transform->GetLocalScale().x;
	m_InitBarPosition_X = transform->GetLocalPosition().x;
}

void CBarElement::Start() {
	// �R���|�[�l���g�Q��������(GameObjectManager�ɓo�^���Ă��Ȃ��̂ŁA���O�ŌĂяo��)�B
	GetComponentManager().Start();
	// �p�����̍X�V�������Ăяo���B
	ImageObject::Start();
}

void CBarElement::Update() {
	// �ڕW�l�Ɍ����ăo�[�̒l�����X�ɍX�V�B
	BarValueUpdate();
	// ���݂̒l�Ńo�[�̒������X�V����B
	BarScaling();

	// �R���|�[�l���g�Q���X�V(GameObjectManager�ɓo�^���Ă��Ȃ��̂ŁA���O�ŌĂяo��)�B
	GetComponentManager().Update();
	// �p�����̍X�V�������Ăяo���B
	ImageObject::Update();
}

void CBarElement::ImageRender() {
	// �R���|�[�l���g�Q��`��(GameObjectManager�ɓo�^���Ă��Ȃ��̂ŁA���O�ŌĂяo��)�B
	GetComponentManager().ImageRender();

	ImageObject::ImageRender();
}

void CBarElement::BarValueUpdate() {
	if (m_Value >= m_TargetValue && m_Dir > 0) {
		// �X�V������+�A���ڕW�l�ɓ��B�����B
		m_Value = m_TargetValue;
	}
	else if (m_Value <= m_TargetValue && m_Dir < 0) {
		// �X�V������-�A���ڕW�l�ɓ��B�����B
		m_Value = m_TargetValue;
	}
	else {
		m_Value += m_WorkValue;
	}
}

void CBarElement::BarScaling() {
	float weight = m_Value / m_MaxValue;
	if (fabsf(weight) <= 0.001f) {
		weight = 0.0f;
	}
	Vector3 scale = transform->GetLocalScale();
	scale.x = m_InitBarScale_X * weight;
	transform->SetLocalScale(scale);

	// �T�C�Y�������������炷�B
	float offset = (m_InitBarScale_X - scale.x) * 0.5f;
	Vector3 pos = transform->GetLocalPosition();
	pos.x = m_InitBarPosition_X - (offset * GetTexture()->Size.x);	// �e�N�X�`���T�C�Y���X�P�[�����O�����l���|���Ă��炸�炷�B
	transform->SetLocalPosition(pos);
}

// CHadBar�B
CHadBar::~CHadBar()
{
}

const Vector3 CHadBar::CreatePos_DefaultArg = Vector3(175.0f, 1125.0f, 0.0f);
void CHadBar::Create(const vector<BarColor>& BarColorArray,float max,float value,Transform* parent,Vector3 pos, Vector2 scale) {
	// �o�[�̃t���[���𐶐��B
	{
		_BarFrame.reset(new ImageObject("BarFrame"));
		_BarFrame->Awake();
		_BarFrame->Start();
		_BarFrame->GetComponentManager().Start();
		if (parent) {
			_BarFrame->transform->SetParent(parent);
		}
		_BarFrame->transform->SetLocalPosition(pos);
		_BarFrame->transform->SetScale(Vector3(scale.x, scale.y, 0.0f));
		_BarFrame->transform->UpdateTransform();
		_BarFrame->SetTexture(LOADTEXTURE("hpsp_bar.png"));
	}

	// �w�肳�ꂽ�F�̏��Ɛ��ɏ]���ăo�[�𐶐��B
	ActiveBarColor(BarColorArray,max,value, _BarFrame->transform);
	// �ŏ��̃o�[������B
	m_NowBarNum = m_NowSettingNum = 0;
	m_NowBar = m_NowSettingBar = m_BarElement[m_NowBarNum].get();

	m_MaxValue = max;

	m_Value = max;

	UpdateValue(value);	// �����l���e�o�[�ɕ��z���Đݒ�B
}

void CHadBar::Update() {
	// �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
	{
		_BarFrame->GetComponentManager().Update();
		m_NowBar->Update();
	}
	if (m_NowBar->GetValue() <= 0.0f) {
		m_NowBarNum++;
		if (m_NowBarNum < 0) {
			m_NowBarNum = 0;
		}
		if (m_NowBarNum < m_MaxBarNum) {
			// �X�V���̃o�[��ύX�B
			m_NowBar = m_BarElement[m_NowBarNum].get();
		}
	}
}

void CHadBar::ImageRender() {
	// �w�ʂ̂��̂���`�悵�Ă����B
	_BarFrame->GetComponentManager().ImageRender();// �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
	for (int idx = m_BarElement.size() - 1; idx >= 0;idx--) {
		m_BarElement[idx]->ImageRender(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB
	}
}

void CHadBar::UpdateValue(float value) {
	float work = static_cast<int>(value) % (static_cast<int>(m_MaxValue) / m_MaxBarNum);	// ��{�̃o�[�ɃZ�b�g����l���Z�o�B���������]����Z�o�������̂�int�ɃL���X�g���Ă���B

	float Difference = m_Value - value;	// ��u�O�̒l�Ƃ̍������Z�o�B
	if(Difference >= 0.0001f){
		// ��u�O�̒l�ƈႤ�l���ݒ肳�ꂽ�B
		while (true) {
			float nowBarValue = m_NowSettingBar->GetTargetValue();	// ���݂̃Q�[�W�����l���擾�B
			if (nowBarValue <= Difference) {
				// 1�Q�[�W�ȏ������B
				// �u���C�N���̃C�x���g�B
				this->BreakEvent();
				// �Ƃ肠�������̃o�[�̒l��0�ɂ���B
				m_NowSettingBar->SetValue(0.0f);
				m_NowSettingNum++;
				if (m_NowSettingNum < m_MaxBarNum) {
					// ���̃o�[������΂�����Ƀ^�[�Q�b�g��ύX�B
					m_NowSettingBar = m_BarElement[m_NowSettingNum].get();
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
					m_NowSettingNum--;
					break;
				}
			}
			else {
				// ���ӂ�Ȃ��ꍇ�̓o�[�ɓ���ď������I���B
				m_NowSettingBar->SetValue(work);
				break;
			}
		}
		// �S�̂Ƃ��Ă̒l���X�V�B
		m_Value = value;
	}
}

void CHadBar::ActiveBarColor(const vector<BarColor>& BarColorArray,float max,float value,Transform* tr) {
	m_MaxBarNum = BarColorArray.size();
	for (auto BarColor : BarColorArray) {
		CBarElement* bar = new CBarElement("BarElement")/*INSTANCE(GameObjectManager)->AddNew<CBarElement>("BarElement", idx)*/;
		bar->Awake(); // �I�u�W�F�N�g�}�l�[�W���[�ɓo�^���Ă��Ȃ����߁A���O�ŌĂԁB

		// �o�[���d�˂�ꍇ�͒l���d�˂�o�[�̐���������������B
		bar->Create(BarColor, max / m_MaxBarNum,tr);
		m_BarElement.push_back(unique_ptr<CBarElement>(bar));
	}
}

void CHadBar::BreakEvent() {
}