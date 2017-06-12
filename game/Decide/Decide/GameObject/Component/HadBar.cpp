#include "stdafx.h"
#include "GameObject\Component\HadBar.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_Component\_2D\Sprite.h"

// CBarElement�B
float CBarElement::m_Time = 0.2f;
void CBarElement::Create(BarColor color, float max, float value,Transform* parent) {
	// �o�^����Ă���Q�[�W�̉摜���󂯎�����l�Ō���B
	Sprite* sp = AddComponent<Sprite>();
	sp->SetTexture(LOADTEXTURE(const_cast<char*>(BarFileNameArray[color])));

	m_MaxValue = max;
	m_TargetValue = value;
	m_Value = value;
	// Transform�ݒ�B
	transform->SetParent(parent);
	transform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_InitBarScale_X = transform->GetScale().x;
	m_InitBarPosition_X = transform->GetLocalPosition().x;
}

void CBarElement::Update() {
	// �ڕW�l�Ɍ����ăo�[�̒l�����X�ɍX�V�B
	BarValueUpdate();
	// ���݂̒l�Ńo�[�̒������X�V����B
	BarScaling();
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
	//float weight = static_cast<float>(m_Value) / m_MaxValue;
	//if (fabsf(weight) <= 0.001f) {
	//	weight = 0.0f;
	//}
	//Vector3 scale = transform->GetScale();
	//scale.x = m_InitBarScale_X * weight;
	//transform->SetScale(scale);

	//float offset = m_InitBarScale_X - scale.x;
	//Vector3 pos = transform->GetLocalPosition();
	//pos.x = m_InitBarPosition_X - (offset / 2);
	//transform->SetLocalPosition(pos);
}

// CHadBar�B
CHadBar::~CHadBar()
{
}

void CHadBar::Create(const vector<BarColor>& BarColorArray,float max,float value,Transform* parent,Vector3 pos, Vector2 scale) {
	// �o�[�̃t���[���𐶐��B
	{
		_framePriorty = 1;
		_BarFrame = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BarFrame", _framePriorty);
		if (parent) {
			_BarFrame->transform->SetParent(parent);
		}
		_BarFrame->transform->SetLocalPosition(pos);
		_BarFrame->transform->SetScale(Vector3(scale.x, scale.y, 0.0f));
		_BarFrame->transform->SetRotation(Quaternion::Identity);
		_BarFrame->transform->UpdateTransform();
		Sprite* sp = _BarFrame->AddComponent<Sprite>();
		sp->SetTexture(LOADTEXTURE("hpsp_bar.png"));
	}

	// �w�肳�ꂽ�F�̏��Ɛ��ɏ]���ăo�[�𐶐��B
	ActiveBarColor(BarColorArray,max,value, _BarFrame->transform);
	// �ŏ��̃o�[������B
	m_NowBarNum = m_NowSettingNum = 0;
	m_NowBar = m_NowSettingBar = m_BarElement[m_NowBarNum];

	m_MaxValue = max;
	m_Varue = value;
}

void CHadBar::Update() {
	m_NowBar->Update();
	if (m_NowBar->GetValue() <= 0.0f) {
		m_NowBarNum++;
		if (m_NowBarNum < 0) {
			m_NowBarNum = 0;
		}
		if (m_NowBarNum < m_MaxBarNum) {
			// �X�V���̃o�[��ύX�B
			m_NowBar = m_BarElement[m_NowBarNum];
		}
	}
}

void CHadBar::SetValue(float value) {
	float work = static_cast<int>(value) % (static_cast<int>(m_MaxValue) / m_MaxBarNum);	// ��{�̃o�[�ɃZ�b�g����l���Z�o�B

	bool isBreak = false;	// 1�Q�[�W�ȏ����Ă��邩�B

	float Difference = m_Varue - value;	// ��u�O�̒l�Ƃ̍������Z�o�B
	if(Difference >= 0.0001f){
		// ��u�O�̒l�ƈႤ�l���ݒ肳�ꂽ�B
		while (true) {
			float nowBarValue = m_NowSettingBar->GetTargetValue();
			if (nowBarValue <= Difference) {
				// 1�Q�[�W�ȏ������B
				// �u���C�N���̃C�x���g�B
				this->BreakEvent();
				// �Ƃ肠�������̃o�[�̒l��0�ɂ���B
				m_NowSettingBar->SetValue(0.0f);
				m_NowSettingNum++;
				if (m_NowSettingNum < m_MaxBarNum) {
					// ���̃o�[������΂�����Ƀ^�[�Q�b�g��ύX�B
					m_NowSettingBar = m_BarElement[m_NowSettingNum];
					// �ݒ肵���l��������������A�V����������ݒ�B
					Difference -= nowBarValue;
					if (nowBarValue < Difference) {
						// 1�Q�[�W�ȏ゠�ӂ��ꍇ�͏����𑱍s�B
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
		m_Varue = value;
	}
}

void CHadBar::ActiveBarColor(const vector<BarColor>& BarColorArray,float max,float value,Transform* tr) {
	m_MaxBarNum = BarColorArray.size();
	int idx = BarColorArray.size() + _framePriorty;
	for (auto BarColor : BarColorArray) {
		if (idx < 0) {
			abort();
			// 0��菬�����D��x�ݒ肵���B
		}
		CBarElement* bar = INSTANCE(GameObjectManager)->AddNew<CBarElement>("BarElement", idx);
		// �o�[���d�˂�ꍇ�͒l���d�˂�o�[�̐���������������B
		bar->Create(BarColor, max / m_MaxBarNum, value / m_MaxBarNum,tr);
		bar->Update();
		m_BarElement.push_back(bar);

		idx--;
	}
}

void CHadBar::BreakEvent() {
}