#include"stdafx.h"
#include "EnemyCharacter.h"
#include "HFSM\EnemyState.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWanderingState.h"
#include "HFSM\EnemyWaitState.h"
#include "HFSM\EnemyDiscoveryState.h"
#include "HFSM\EnemyAttackState.h"
#include "HFSM\EnemyStartAttackState.h"
#include "HFSM\EnemyFallState.h"
#include "AttackCollision.h"


EnemyCharacter::EnemyCharacter(const char* name) :GameObject(name)
{
}

EnemyCharacter::~EnemyCharacter()
{

}


void EnemyCharacter::Awake() {
	// 継承先により変わる処理。
	_AwakeSubClass();

	// このクラスで使用するコンポーネントを追加。
	// ※下記の関数を継承先のクラスで上書きしている場合はそちらが呼ばれる。
	_BuildMyComponents();
	// 使用するステートを列挙。
	_BuildState();
	// 剛体生成。
	_BuildCollision();
	// モデル生成。
	_BuildModelData();
	// アニメーションテーブル作成。
	_BuildAnimation();
}

void EnemyCharacter::Start() {
	_MoveSpeed = Vector3::zero;	// 初期化。
	
	// 位置情報設定。
	_InitPos = Vector3(560, 69, -1000);
	transform->SetLocalPosition(_InitPos);

	// 継承先により変わる処理。
	_StartSubClass();
}

void EnemyCharacter::Update() {

	if (_MyComponent.Parameter->HP <= 0)
	{
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}

	// 継承先により変わる処理。
	_UpdateSubClass();

	if (_NowState == nullptr) {
		// ステートを継承先で指定した？。
		abort();
	}
	// 現在のステートを更新。
	if (_NowState->Update()) {
		// ステート処理終了。

		// ステートが終了したことを通知。
		_EndNowStateCallback(_NowStateIdx);
	}

	_MyComponent.CharacterController->SetMoveSpeed(_MoveSpeed);
	// キャラクターコントローラで実際にキャラクターを制御。
	_MyComponent.CharacterController->Execute();
}

void EnemyCharacter::LateUpdate() {
	// 継承先により変わる処理。
	_LateUpdateSubClass();

	_MoveSpeed = Vector3::zero;	// 使い終わったので初期化。
}


void EnemyCharacter::CreateAttackCollision(const int eventFrame, const Vector3& pos, const Vector3& angle, const Vector3& size) {
	//現在のフレーム取得
	const int nowFrame = _MyComponent.Animation->NowFrame();
	//フレームが10の時あたり判定作成
	if (nowFrame == eventFrame)
	{
		//攻撃コリジョン作成
		unsigned int priorty = 1;
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack_enemy", priorty);
		Quaternion rot = Quaternion::Identity;
		attack->Create(_MyComponent.Parameter->AttackDamageMass(1), pos, rot, size, AttackCollision::CollisionMaster::Enemy, -1.0f);
	}
}

bool EnemyCharacter::IsOutsideWandering() {
	float NowRange = Vector3(_InitPos - transform->GetPosition()).Length();
	if (NowRange > _WanderingRange) {
		// 徘徊範囲外に出た。
		return true;
	}
	return false;
}

bool EnemyCharacter::IsOutsideWandering(const Vector3& Add) {
	float NowRange = Vector3(_InitPos - (transform->GetPosition() + Add)).Length();
	if (NowRange > _WanderingRange) {
		// 徘徊範囲外に出た。
		return true;
	}
	return false;
}


void EnemyCharacter::SearchView() {
	// 視野角判定。
	if (_SearchView.IsDiscovery(
		transform->GetPosition(),
		INSTANCE(GameObjectManager)->FindObject("Player")->transform->GetPosition(),
		transform->GetForward(),
		_ViewAngle,
		_ViewRange))
	{
		// 視線に入っている。

		// 発見ステートに移行。
		_ChangeState(State::Discovery);
	}
}


void EnemyCharacter::_BuildMyComponents() {
	// モデル情報を追加。
	_MyComponent.Model = AddComponent<SkinModel>();
	// アニメーションを追加。
	_MyComponent.Animation = AddComponent<Animation>();
	// キャラクターコントローラを追加。
	_MyComponent.CharacterController = AddComponent<CCharacterController>();
	// 回転クラスを追加。
	_MyComponent.RotationAction = AddComponent<ObjectRotation>();
	// バーコンポーネントを追加。
	_MyComponent.HPBar = AddComponent<ParameterBar>();
	//パラメーターのコンポーネント追加。
	_MyComponent.Parameter = AddComponent<CharacterParameter>();
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

	// キャラクターコントローラー作成。
	// ※コライダーコンポーネントは継承先で追加。
	_MyComponent.CharacterController->Init(this, transform, _Radius, _Height, Vector3::zero, Collision_ID::ENEMY, _MyComponent.Collider, _Gravity);
	
	// キャラクターコントローラーにパラメーターを設定。
	_ConfigCharacterController();

	_MyComponent.CharacterController->SetGravity(_Gravity);
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
	// 徘徊ステートを追加。
	_MyState.push_back(unique_ptr<EnemyState>(new EnemyWanderingState(this)));
	// 発見ステートを追加。
	_MyState.push_back(unique_ptr<EnemyDiscoveryState>(new EnemyDiscoveryState(this)));
	// 攻撃開始ステートを追加。
	_MyState.push_back(unique_ptr<EnemyStartAttackState>(new EnemyStartAttackState(this)));
	// 攻撃ステートを追加。
	_MyState.push_back(unique_ptr<EnemyAttackState>(new EnemyAttackState(this)));
	// 待機ステートを追加。
	_MyState.push_back(unique_ptr<EnemyWaitState>(new EnemyWaitState(this)));
	// 直進ステートを追加。
	_MyState.push_back(unique_ptr<EnemyTranslationState>(new EnemyTranslationState(this)));
	// 落下ステートを追加。
	_MyState.push_back(unique_ptr<EnemyFallState>(new EnemyFallState(this)));
}

void EnemyCharacter::_ChangeState(State next) {
	if (static_cast<int>(next) >= _MyState.size() || static_cast<int>(next) < 0) {
		// 渡された数字が配列の容量を超えている。
		abort();
	}

	if (_NowState) {
		// 現在のステートがnullでない。
		if (_NowState->GetIsEnd() || _NowState->IsPossibleChangeState(next)) {
			// 現在のステートから次のステートへの移行が許可されている、または現在のステートが終了している。
			// 現在のステートを後片付け。
			_NowState->Exit(next);
		}
		else {
			// 現在のステートから次のステートへの移行が許可されていない。
			return;
		}
	}

	// 新しいステートに切り替え。
	_NowState = _MyState[static_cast<int>(next)].get();
	_NowState->Entry();

	// 現在のステートの添え字を保存。
	_NowStateIdx = next;
}