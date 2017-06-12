#include "stdafx.h"
#include "GameObject\Component\HadBar.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_Component\_2D\Sprite.h"

// CBarElement。
float CBarElement::m_Time = 0.2f;
void CBarElement::Create(BarColor color, float max, float value,Transform* parent) {
	// 登録されているゲージの画像を受け取った値で決定。
	Sprite* sp = AddComponent<Sprite>();
	sp->SetTexture(LOADTEXTURE(const_cast<char*>(BarFileNameArray[color])));

	m_MaxValue = max;
	m_TargetValue = value;
	m_Value = value;
	// Transform設定。
	transform->SetParent(parent);
	transform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_InitBarScale_X = transform->GetScale().x;
	m_InitBarPosition_X = transform->GetLocalPosition().x;
}

void CBarElement::Update() {
	// 目標値に向けてバーの値を徐々に更新。
	BarValueUpdate();
	// 現在の値でバーの長さを更新する。
	BarScaling();
}

void CBarElement::BarValueUpdate() {
	if (m_Value >= m_TargetValue && m_Dir > 0) {
		// 更新方向が+、かつ目標値に到達した。
		m_Value = m_TargetValue;
	}
	else if (m_Value <= m_TargetValue && m_Dir < 0) {
		// 更新方向が-、かつ目標値に到達した。
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

// CHadBar。
CHadBar::~CHadBar()
{
}

void CHadBar::Create(const vector<BarColor>& BarColorArray,float max,float value,Transform* parent,Vector3 pos, Vector2 scale) {
	// バーのフレームを生成。
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

	// 指定された色の順と数に従ってバーを生成。
	ActiveBarColor(BarColorArray,max,value, _BarFrame->transform);
	// 最初のバーを決定。
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
			// 更新中のバーを変更。
			m_NowBar = m_BarElement[m_NowBarNum];
		}
	}
}

void CHadBar::SetValue(float value) {
	float work = static_cast<int>(value) % (static_cast<int>(m_MaxValue) / m_MaxBarNum);	// 一本のバーにセットする値を算出。

	bool isBreak = false;	// 1ゲージ以上削っているか。

	float Difference = m_Varue - value;	// 一瞬前の値との差分を算出。
	if(Difference >= 0.0001f){
		// 一瞬前の値と違う値が設定された。
		while (true) {
			float nowBarValue = m_NowSettingBar->GetTargetValue();
			if (nowBarValue <= Difference) {
				// 1ゲージ以上削った。
				// ブレイク時のイベント。
				this->BreakEvent();
				// とりあえず今のバーの値を0にする。
				m_NowSettingBar->SetValue(0.0f);
				m_NowSettingNum++;
				if (m_NowSettingNum < m_MaxBarNum) {
					// 次のバーがあればそちらにターゲットを変更。
					m_NowSettingBar = m_BarElement[m_NowSettingNum];
					// 設定した値を差分から引き、新しい差分を設定。
					Difference -= nowBarValue;
					if (nowBarValue < Difference) {
						// 1ゲージ以上あふれる場合は処理を続行。
						continue;
					}
					else {
						// ちょうど1ゲージ使った。
						break;
					}
				}
				else {
					// 次のバーがなければ処理を終了。
					m_NowSettingNum--;
					break;
				}
			}
			else {
				// あふれない場合はバーに入れて処理を終了。
				m_NowSettingBar->SetValue(work);
				break;
			}
		}
		// 全体としての値を更新。
		m_Varue = value;
	}
}

void CHadBar::ActiveBarColor(const vector<BarColor>& BarColorArray,float max,float value,Transform* tr) {
	m_MaxBarNum = BarColorArray.size();
	int idx = BarColorArray.size() + _framePriorty;
	for (auto BarColor : BarColorArray) {
		if (idx < 0) {
			abort();
			// 0より小さい優先度設定した。
		}
		CBarElement* bar = INSTANCE(GameObjectManager)->AddNew<CBarElement>("BarElement", idx);
		// バーを重ねる場合は値を重ねるバーの数分だけ分割する。
		bar->Create(BarColor, max / m_MaxBarNum, value / m_MaxBarNum,tr);
		bar->Update();
		m_BarElement.push_back(bar);

		idx--;
	}
}

void CHadBar::BreakEvent() {
}