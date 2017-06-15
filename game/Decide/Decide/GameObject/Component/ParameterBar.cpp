#include "stdafx.h"
#include "GameObject\Component\ParameterBar.h"

// CBarElement。
float BarElement::_Time = 0.2f;
void BarElement::Create(BarColor color, float max,Transform* parent) {
	Start(); // オブジェクトマネージャーに登録していないため、自前で呼ぶ。

	// 登録されているゲージの画像を受け取った値で決定。
	SetTexture(LOADTEXTURE(const_cast<char*>(BarFileNameArray[color])));

	_MaxValue = max;
	_TargetValue = max;
	_Value = max;

	// Transform設定。
	transform->SetParent(parent);
	transform->SetLocalPosition(Vector3(-0.9f, 0.0f, 0.0f));
	transform->SetLocalScale(Vector3(2.54f, 1.0f, 1.0f));
	_InitBarScale_X = transform->GetLocalScale().x;
	_InitBarPosition_X = transform->GetLocalPosition().x;
}

void BarElement::Start() {
	// コンポーネント群を初期化(GameObjectManagerに登録していないので、自前で呼び出す)。
	GetComponentManager().Start();
	// 継承元の更新処理を呼び出す。
	ImageObject::Start();
}

void BarElement::Update() {
	// 目標値に向けてバーの値を徐々に更新。
	_BarValueUpdate();
	// 現在の値でバーの長さを更新する。
	_BarScaling();

	// コンポーネント群を更新(GameObjectManagerに登録していないので、自前で呼び出す)。
	GetComponentManager().Update();
	// 継承元の更新処理を呼び出す。
	ImageObject::Update();
}

void BarElement::ImageRender() {
	// コンポーネント群を描画(GameObjectManagerに登録していないので、自前で呼び出す)。
	GetComponentManager().ImageRender();

	ImageObject::ImageRender();
}

void BarElement::_BarValueUpdate() {
	if (_Value >= _TargetValue && _Dir > 0) {
		// 更新方向が+、かつ目標値に到達した。
		_Value = _TargetValue;
	}
	else if (_Value <= _TargetValue && _Dir < 0) {
		// 更新方向が-、かつ目標値に到達した。
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

	// サイズを下げた分ずらす。
	float offset = (_InitBarScale_X - scale.x) * 0.5f;
	Vector3 pos = transform->GetLocalPosition();
	pos.x = _InitBarPosition_X - (offset * GetTexture()->Size.x);	// テクスチャサイズをスケーリングした値を掛けてからずらす。
	transform->SetLocalPosition(pos);
}

// ParameterBar。
const Vector3 ParameterBar::CreatePos_DefaultArg = Vector3(175.0f, 21.9f, 0.0f);
const Vector2 ParameterBar::CreateScale_DefaultArg = Vector2(1.0f, 1.0f);

ParameterBar::~ParameterBar()
{
}

void ParameterBar::Create(const vector<BarColor>& BarColorArray,float max,float value,Transform* parent,const Vector3& pos,const Vector2& scale, bool isHud) {
	// 作業用Transform情報を保存。
	// 親子関係を組んだ場合は親が移動したりすると自動更新される。
	if (parent) {
		_Transform->SetParent(parent);
	}
	_Transform->SetLocalPosition(pos);
	_Transform->SetScale(Vector3(scale.x, scale.y, 1.0f));

	// バーのフレームを生成。
	// ※親子関係を作成すると勝手に更新されるため、ここでは親子関係のない絶対座標を渡す。
	CreateBarFrame(_Transform->GetPosition(), _Transform->GetScale(),isHud);

	// 指定された色の順と数に従ってバーを生成。
	_ActiveBarColor(BarColorArray,max,value, _BarFrame->transform);
	// 最初のバーを決定。
	_NowBarNum = _NowSettingNum = 0;
	_NowBar = _NowSettingBar = _BarElement[_NowBarNum].get();

	// 各値の初期値設定。
	_MaxValue = max;
	_Value = max;

	_UpdateValue(value);	// 初期値を各バーに分配して設定。
}

void ParameterBar::Update() {
	// オブジェクトマネージャーに登録していないため、自前で呼ぶ。
	{
		_BarFrame->GetComponentManager().Update();
		// Transform情報は毎フレーム更新されるため、毎フレームスクリーン座標にして上書きする。
		_ToScreenPos();

		_NowBar->Update();
	}
	if (_NowBar->GetValue() <= 0.0f) {
		_NowBarNum++;
		if (_NowBarNum < 0) {
			_NowBarNum = 0;
		}
		if (_NowBarNum < _MaxBarNum) {
			// 更新中のバーを変更。
			_NowBar = _BarElement[_NowBarNum].get();
		}
	}
}

void ParameterBar::ImageRender() {
	// 背面のものから描画していく。
	_BarFrame->GetComponentManager().ImageRender();// オブジェクトマネージャーに登録していないため、自前で呼ぶ。
	for (int idx = _BarElement.size() - 1; idx >= 0;idx--) {
		_BarElement[idx]->ImageRender(); // オブジェクトマネージャーに登録していないため、自前で呼ぶ。
	}
}

void ParameterBar::_UpdateValue(float value) {
	float work = static_cast<int>(value) % (static_cast<int>(_MaxValue) / _MaxBarNum);	// 一本のバーにセットする値を算出。※割った余りを算出したいのでintにキャストしている。

	float Difference = _Value - value;	// 一瞬前の値との差分を算出。
	if(Difference >= 0.0001f){
		// 一瞬前の値と違う値が設定された。
		while (true) {
			float nowBarValue = _NowSettingBar->GetTargetValue();	// 現在のゲージが持つ値を取得。
			if (nowBarValue <= Difference) {
				// 1ゲージ以上削った。
				// ブレイク時のイベント。
				this->_BreakEvent();
				// とりあえず今のバーの値を0にする。
				_NowSettingBar->SetValue(0.0f);
				_NowSettingNum++;
				if (_NowSettingNum < _MaxBarNum) {
					// 次のバーがあればそちらにターゲットを変更。
					_NowSettingBar = _BarElement[_NowSettingNum].get();
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
					_NowSettingNum--;
					break;
				}
			}
			else {
				// あふれない場合はバーに入れて処理を終了。
				_NowSettingBar->SetValue(work);
				break;
			}
		}
		// 全体としての値を更新。
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
		bar->Awake(); // オブジェクトマネージャーに登録していないため、自前で呼ぶ。

		// バーを重ねる場合は値を重ねるバーの数分だけ分割する。
		bar->Create(BarColor, max / _MaxBarNum,tr);
		_BarElement.push_back(unique_ptr<BarElement>(bar));
	}
}

void ParameterBar::_BreakEvent() {
}

void ParameterBar::_ToScreenPos() {
	if (!_isHud) {
		// HUDとして使用せず、キャラクターの座標系を用いて周辺に表示する。

		// スクリーン座標系に変換して再設定。
		_BarFrame->transform->SetPosition(Vector3(INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Transform->GetPosition()), 0.0f));
	}
}
