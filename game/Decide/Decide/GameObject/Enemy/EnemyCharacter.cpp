#include "EnemyCharacter.h"
#include "HFSM\EnemyState.h"
#include "fbEngine\SkinModel.h"
#include "fbEngine\SkinModelData.h"
#include "fbEngine\Animation.h"
#include "fbEngine\CharacterController.h"

EnemyCharacter::EnemyCharacter(const char* name) :GameObject(name)
{
}

EnemyCharacter::~EnemyCharacter()
{

}


void EnemyCharacter::Awake() {
	// このクラスで使用するコンポーネントを追加。
	// ※下記の関数を継承先のクラスで上書きしている場合はそちらが呼ばれる。
	_BuildMyComponents();
	// 使用するステートを列挙。
	_BuildState();
	// とりあえず何もしないステートに移行。
	// ※ステートを使用する場合は継承先のStart関数でステートを切り替える。
	_ChangeState(State::NotAction);
}

void EnemyCharacter::Start() {
	// 剛体生成。
	_BuildCollision();
	// モデル生成。
	_BuildModelData();
}

void EnemyCharacter::Update() {
	// 現在のステートを更新。
	_NowState->Update();
}


void EnemyCharacter::_BuildMyComponents() {
	// モデル情報を追加。
	_MyComponent.Model = AddComponent<SkinModel>();
	// アニメーションを追加。
	_MyComponent.Animation = AddComponent<Animation>();
	// キャラクターコントローラを追加。
	_MyComponent.CharacterController = AddComponent<CCharacterController>();
}

void EnemyCharacter::_BuildCollision() {
	// コリジョンのパラメータを決定。
	_ConfigCollision();

	if (_Radius <= 0.0f || _Height <= 0.0f) {
		// 継承先でサイズ設定してる？。
		abort();
	}
	if (_MyComponent.Collider == nullptr) {
		// 継承先でコライダーコンポーネント設定してる？。
		abort();
	}

	float gravity = -30.0f;

	// キャラクターコントローラー作成。
	// ※コライダーコンポーネントは継承先で追加。
	_MyComponent.CharacterController->Init(this, transform, _Radius, _Height, Vector3::zero, Collision_ID::ENEMY, _MyComponent.Collider, gravity);
}

void EnemyCharacter::_BuildModelData() {
	//スキンモデルデータ作成。
	SkinModelData* modeldata = new SkinModelData();
	// モデルデータ読み込み。
	modeldata->CloneModelData(SkinModelManager::LoadModel(_FileName), _MyComponent.Animation);
	//モデルコンポーネントにモデルデータを設定。
	_MyComponent.Model->SetModelData(modeldata);
	//_MyComponent.Model->SetModelEffect(ModelEffectE::SPECULAR, false);
}

void EnemyCharacter::_BuildState() {
	// 何もしないステートを追加。
	_MyState.push_back(unique_ptr<EnemyState>(new EnemyState(this)));
}

void EnemyCharacter::_ChangeState(State next) {
	if (static_cast<int>(next) >= _MyState.size() || static_cast<int>(next) < 0) {
		// 渡された数字が配列の容量を超えている。
		abort();
	}

	if (_NowState) {
		// 現在のステートがnullでない。
		// 現在のステートを後片付け。
		_NowState->Exit(next);
	}

	// 新しいステートに切り替え。
	_NowState = _MyState[static_cast<int>(next)].get();
	_NowState->Entry();

	// 現在のステートの添え字を保存。
	_NowStateIdx = next;
}