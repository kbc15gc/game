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
#include "HFSM\EnemyDeathState.h"
#include "HFSM\EnemyDamageReactionState.h"
#include "HFSM\EnemyThreatState.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\Component\BuffDebuffICon.h"

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

	// 位置情報初期化。
	transform->SetPosition(Vector3(0.0f,0.0f,0.0f));
	
	// 使用するステートを列挙。
	_BuildState();

	// 継承先により変わる処理。
	_AwakeSubClass();

}

void EnemyCharacter::Start() {
	// 剛体生成。
	_BuildCollision();
	// モデル生成。
	_BuildModelData();
	// アニメーションテーブル作成。
	_BuildAnimation();
	// アニメーションイベント設定。
	_ConfigAnimationEvent();
	// サウンドテーブル作成。
	_BuildSoundTable();

	_MoveSpeed = Vector3::zero;	// 初期化。
	
	// 継承先により変わる処理。
	_StartSubClass();

	// 初期位置設定。
	_InitPos = transform->GetPosition();

	// 継承先で初期位置が設定された可能性があるため更新。
	_MyComponent.CharacterController->Execute();
	_MyComponent.CharacterController->AddRigidBody();	// ワールドに登録した瞬間にバウンディングボックスが生成されるため、初期情報設定のためここで登録。
	
	int Attribute = Collision_ID::PLAYER;
	_MyComponent.CharacterExtrude->Init(_MyComponent.CharacterController->GetRigidBody(), Attribute);

	//プレイヤー。
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

void EnemyCharacter::Update() {

	if (_MyComponent.Parameter->GetDeathFlg())
	{
		if (_NowStateIdx != State::Death) {
			_ChangeState(State::Death);
			static_cast<EnemyDeathState*>(_NowState)->SetWaitTime(1.0f);
		}
	}

	_BarRenderUpdate();

	// 継承先により変わる処理。
	_UpdateSubClass();

	// エネミーのエフェクト更新。
	EffectUpdate();

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
	// 移動した結果、指定した属性のものが衝突していれば押し出す。
	_MyComponent.CharacterExtrude->Extrude(_MyComponent.CharacterController->GetmoveSpeedExcute());
}

void EnemyCharacter::LateUpdate() {
	// 継承先により変わる処理。
	_LateUpdateSubClass();

	_MoveSpeed = Vector3::zero;	// 使い終わったので初期化。
}


bool EnemyCharacter::IsOutsideDiscovery() {
	float NowRange = Vector3(_InitPos - transform->GetPosition()).Length();
	if (NowRange > _discoveryRange) {
		// 追跡範囲外に出た。
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

		// 威嚇ステートに移行。
		_ChangeState(State::Threat);
	}
}

void EnemyCharacter::ConfigDamageReaction(bool isMotion, unsigned short probability) {
	_isDamageMotion = isMotion;
	if (isMotion) {
		if (probability <= 1) {
			_isDamageMotionRandom = false;
		}
		else {
			_isDamageMotionRandom = true;
			_damageMotionProbability = probability;
		}
	}
}


void EnemyCharacter::_BarRenderUpdate() {
	if (_MyComponent.HPBar) {
		float distance = 60.0f;
		if (Vector3(_Player->transform->GetPosition() - transform->GetPosition()).Length() <= distance) {
			_MyComponent.HPBar->RenderEnable();
			_MyComponent.BuffDebuffICon->RenderEnable();
		}
		else {
			_MyComponent.HPBar->RenderDisable();
			_MyComponent.BuffDebuffICon->RenderDisable();

		}
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
	// スポナーコンポーネント追加。
	_MyComponent.Spawner = AddComponent<ObjectSpawn>();
	// アニメーションイベントコンポーネント追加。
	_MyComponent.AnimationEventPlayer = AddComponent<AnimationEventPlayer>();

	// パーティクルエフェクトコンポーネント追加。
	_MyComponent.ParticleEffect = AddComponent<ParticleEffect>();

	// バフデバフアイコンコンポーネント追加。
	_MyComponent.BuffDebuffICon = AddComponent<BuffDebuffICon>();
	// 剛体押し出しコンポーネント追加。
	_MyComponent.CharacterExtrude = AddComponent<CharacterExtrude>();
}

void EnemyCharacter::_BuildCollision() {
	// コリジョンのパラメータを決定。
	_ConfigCollision();

	if (_collisionInfo.radius <= 0.0f || _collisionInfo.height <= 0.0f) {
		// 継承先でサイズ設定してる？。
		abort();
	}
	if (_MyComponent.Collider == nullptr) {
		// 継承先でコライダーコンポーネント設定してる？。
		abort();
	}

	// キャラクターコントローラー作成。
	// ※コライダーコンポーネントは継承先で追加。
	_MyComponent.CharacterController->Init(_collisionInfo.offset, Collision_ID::ENEMY, _MyComponent.Collider, _Gravity,false);
	
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
	// 威嚇ステートを追加。
	_MyState.push_back(unique_ptr<EnemyThreatState>(new EnemyThreatState(this)));
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
	// ダメージ反応ステートを追加。
	_MyState.push_back(unique_ptr<EnemyDamageReactionState>(new EnemyDamageReactionState(this)));
	// 死亡ステートを追加。
	_MyState.push_back(unique_ptr<EnemyDeathState>(new EnemyDeathState(this)));
}

/**
* アイテム効果.
*/
bool EnemyCharacter::ItemEffect(Item::ItemInfo * info)
{
//	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
//		if (_MyComponent.Parameter)
//		{
//			if (_MyComponent.ParticleEffect) {
//				_MyComponent.ParticleEffect->DeBuffEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// エフェクトコンポーネントないよ。
//				abort();
//			}
//#endif //  _DEBUG
//			_MyComponent.Parameter->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
//			_MyComponent.BuffDebuffICon->DebuffIconCreate(static_cast<BuffDebuffICon::Param>(idx));
//		}
//	}
//
//	return true;

//戻り値.
	bool returnValue = false;

	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
		int value = info->effectValue[idx];
		if (value > 0) {
			// バフ。
			if (_MyComponent.ParticleEffect) {
				_MyComponent.ParticleEffect->BuffEffect();
			}
#ifdef  _DEBUG
			else {
				// エフェクトコンポーネントないよ。
				abort();
			}
#endif //  _DEBUG

			_MyComponent.Parameter->Buff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(value), info->time);
			_MyComponent.BuffDebuffICon->BuffIconCreate(static_cast<BuffDebuffICon::Param>(idx));
			returnValue = true;
		}
		else if (value < 0) {
			// デバフ(デメリット)。
			if (_MyComponent.ParticleEffect) {
				_MyComponent.ParticleEffect->DeBuffEffect();
			}
#ifdef  _DEBUG
			else {
				// エフェクトコンポーネントないよ。
				abort();
			}
#endif //  _DEBUG
			_MyComponent.Parameter->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
			_MyComponent.BuffDebuffICon->SelectUseIconType_Enemy();
			_MyComponent.BuffDebuffICon->DebuffIconCreate(static_cast<BuffDebuffICon::Param>(idx));
			returnValue = true;
		}
	}
	return returnValue;
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

	void EnemyCharacter::_ConfigSoundData(SoundIndex idx, char* filePath, bool is3D, bool isLoop) {
	strcpy(_SoundData[static_cast<int>(idx)].Path, filePath);
	_SoundData[static_cast<int>(idx)].Is3D = is3D;
	_SoundData[static_cast<int>(idx)].IsLoop = isLoop;
	_SoundData[static_cast<int>(idx)].Source = INSTANCE(GameObjectManager)->AddNew<SoundSource>(filePath, 0);
	string work = "Asset/Sound/";
	work = work + filePath;
	_SoundData[static_cast<int>(idx)].Source->Init(work.c_str(), is3D);
}

void EnemyCharacter::HitAttackCollisionEnter(AttackCollision* hitCollision) {
	if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Player)
	{
		if (_MyComponent.Parameter->GetParam(CharacterParameter::HP) > 0)
		{
			GiveDamage(hitCollision->GetDamage(),hitCollision->GetIsMagic());
		}
	}
}

void EnemyCharacter::GiveDamage(int damage,bool isMagic) {
	int _damage;
	if (_NowState->IsPossibleDamage()) {
		// ダメージを与えられるステートだった。

		_damage = _MyComponent.Parameter->ReciveDamage(damage, isMagic);

		// ダメージ値をもとにパラメーター更新。
		_MyComponent.HPBar->SubValue(_damage);

		//受けたダメージ量を表示。
		AttackValue2D* attackvalue = INSTANCE(GameObjectManager)->AddNew<AttackValue2D>("AttackValue2D", 5);
		attackvalue->Init(_damage, 1.5f, Vector3(0.0f, 1.0f, 0.0f), Color::blue);
		attackvalue->transform->SetParent(transform);

		if (_isDamageMotion) {
			// のけぞるか。

			if (_isDamageMotionRandom) {
				// のけぞりモーションの再生をランダムにする。

				if (rand() % _damageMotionProbability == 0) {
					// のけぞり。

					// ダメージ反応ステートに移行可能。
					_ChangeState(State::Damage);
				}
			}
			else {
				// 確定でのけぞりモーションを再生する。

				// ダメージ反応ステートに移行可能。
				_ChangeState(State::Damage);
			}
		}
	}
	else {
		_damage = 0;
	}
}

/**
* エフェクト用更新.
*/
void EnemyCharacter::EffectUpdate() {
	bool isBuffEffect = false;
	bool isDeBuffEffect = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::Param::DEX; idx++) {

		if (_MyComponent.Parameter->GetBuffParam((CharacterParameter::Param)idx) > 0.0f)
		{
			isBuffEffect = true;
		}
		else
		{
			_MyComponent.BuffDebuffICon->DeleteBuffIcon(static_cast<BuffDebuffICon::Param>(idx));
		}
		if (_MyComponent.Parameter->GetDebuffParam((CharacterParameter::Param)idx) > 0.0f)
		{
			isDeBuffEffect = true;
		}
		else
		{
			_MyComponent.BuffDebuffICon->DeleteDebuffIcon(static_cast<BuffDebuffICon::Param>(idx));
		}
	}
	_MyComponent.ParticleEffect->SetBuffEffectFlag(isBuffEffect);
	_MyComponent.ParticleEffect->SetDebuffEffectFlag(isDeBuffEffect);
}



// EnemyAttack。

void EnemyAttack::Init(int animType, float interpolate, int animLoopNum) {
	_animType = animType;
	_interpolate = interpolate;
	_animLoopNum = animLoopNum;
}



// EnemySingleAttack。

bool EnemySingleAttack::Update(){
	if (!_isPlaying) {
		// 攻撃モーション一度終了。
		return true;
	}
	return false;
}

