#include "stdafx.h"
#include "GameObject\Component\HadBar.h"

// CBarElement。
float CBarElement::m_Time = 0.2f;
void CBarElement::Create(BarColor color, float max,Transform* parent) {
	Start(); // オブジェクトマネージャーに登録していないため、自前で呼ぶ。

	// 登録されているゲージの画像を受け取った値で決定。
	SetTexture(LOADTEXTURE(const_cast<char*>(BarFileNameArray[color])));

	m_MaxValue = max;
	m_TargetValue = max;
	m_Value = max;

	// Transform設定。
	transform->SetParent(parent);
	transform->SetLocalPosition(Vector3(-0.9f, 0.0f, 0.0f));
	transform->SetLocalScale(Vector3(2.54f, 1.0f, 0.0f));
	m_InitBarScale_X = transform->GetLocalScale().x;
	m_InitBarPosition_X = transform->GetLocalPosition().x;
}

void CBarElement::Start() {
	// コンポーネント群を初期化(GameObjectManagerに登録していないので、自前で呼び出す)。
	GetComponentManager().Start();
	// 継承元の更新処理を呼び出す。
	ImageObject::Start();
}

void CBarElement::Update() {
	// 目標値に向けてバーの値を徐々に更新。
	BarValueUpdate();
	// 現在の値でバーの長さを更新する。
	BarScaling();

	// コンポーネント群を更新(GameObjectManagerに登録していないので、自前で呼び出す)。
	GetComponentManager().Update();
	// 継承元の更新処理を呼び出す。
	ImageObject::Update();
}

void CBarElement::ImageRender() {
	// コンポーネント群を描画(GameObjectManagerに登録していないので、自前で呼び出す)。
	GetComponentManager().ImageRender();

	ImageObject::ImageRender();
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
	float weight = m_Value / m_MaxValue;
	if (fabsf(weight) <= 0.001f) {
		weight = 0.0f;
	}
	Vector3 scale = transform->GetLocalScale();
	scale.x = m_InitBarScale_X * weight;
	transform->SetLocalScale(scale);

	// サイズを下げた分ずらす。
	float offset = (m_InitBarScale_X - scale.x) * 0.5f;
	Vector3 pos = transform->GetLocalPosition();
	pos.x = m_InitBarPosition_X - (offset * GetTexture()->Size.x);	// テクスチャサイズをスケーリングした値を掛けてからずらす。
	transform->SetLocalPosition(pos);
}

// CHadBar。
CHadBar::~CHadBar()
{
}

const Vector3 CHadBar::CreatePos_DefaultArg = Vector3(175.0f, 1125.0f, 0.0f);
void CHadBar::Create(const vector<BarColor>& BarColorArray,float max,float value,Transform* parent,Vector3 pos, Vector2 scale) {
	// バーのフレームを生成。
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

	// 指定された色の順と数に従ってバーを生成。
	ActiveBarColor(BarColorArray,max,value, _BarFrame->transform);
	// 最初のバーを決定。
	m_NowBarNum = m_NowSettingNum = 0;
	m_NowBar = m_NowSettingBar = m_BarElement[m_NowBarNum].get();

	m_MaxValue = max;

	m_Value = max;

	UpdateValue(value);	// 初期値を各バーに分配して設定。
}

void CHadBar::Update() {
	// オブジェクトマネージャーに登録していないため、自前で呼ぶ。
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
			// 更新中のバーを変更。
			m_NowBar = m_BarElement[m_NowBarNum].get();
		}
	}
}

void CHadBar::ImageRender() {
	// 背面のものから描画していく。
	_BarFrame->GetComponentManager().ImageRender();// オブジェクトマネージャーに登録していないため、自前で呼ぶ。
	for (int idx = m_BarElement.size() - 1; idx >= 0;idx--) {
		m_BarElement[idx]->ImageRender(); // オブジェクトマネージャーに登録していないため、自前で呼ぶ。
	}
}

void CHadBar::UpdateValue(float value) {
	float work = static_cast<int>(value) % (static_cast<int>(m_MaxValue) / m_MaxBarNum);	// 一本のバーにセットする値を算出。※割った余りを算出したいのでintにキャストしている。

	float Difference = m_Value - value;	// 一瞬前の値との差分を算出。
	if(Difference >= 0.0001f){
		// 一瞬前の値と違う値が設定された。
		while (true) {
			float nowBarValue = m_NowSettingBar->GetTargetValue();	// 現在のゲージが持つ値を取得。
			if (nowBarValue <= Difference) {
				// 1ゲージ以上削った。
				// ブレイク時のイベント。
				this->BreakEvent();
				// とりあえず今のバーの値を0にする。
				m_NowSettingBar->SetValue(0.0f);
				m_NowSettingNum++;
				if (m_NowSettingNum < m_MaxBarNum) {
					// 次のバーがあればそちらにターゲットを変更。
					m_NowSettingBar = m_BarElement[m_NowSettingNum].get();
					if (nowBarValue < Difference) {
						// 1ゲージ以上あふれる。

						// 設定した値を差分から引き、新しい差分を設定。
						Difference -= nowBarValue;
						// 処理を続行。
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
		m_Value = value;
	}
}

void CHadBar::ActiveBarColor(const vector<BarColor>& BarColorArray,float max,float value,Transform* tr) {
	m_MaxBarNum = BarColorArray.size();
	for (auto BarColor : BarColorArray) {
		CBarElement* bar = new CBarElement("BarElement")/*INSTANCE(GameObjectManager)->AddNew<CBarElement>("BarElement", idx)*/;
		bar->Awake(); // オブジェクトマネージャーに登録していないため、自前で呼ぶ。

		// バーを重ねる場合は値を重ねるバーの数分だけ分割する。
		bar->Create(BarColor, max / m_MaxBarNum,tr);
		m_BarElement.push_back(unique_ptr<CBarElement>(bar));
	}
}

void CHadBar::BreakEvent() {
}