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
#include "HFSM\EnemySpeakState.h"
#include "HFSM\EnemyChaceState.h"
#include "HFSM\LastBossThroneState.h"
#include "HFSM\LastBossMagicianState.h"
#include "HFSM\LastBossHistoryTamperingState.h"
#include "HFSM\LastBossDownState.h"
#include "HFSM\EnemyBackStepState.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\Component\BuffDebuffICon.h"


EnemyCharacter::NearEnemyInfo EnemyCharacter::nearEnemyInfo = NearEnemyInfo(FLT_MAX,nullptr);

EnemyCharacter::EnemyCharacter(const char* name) :GameObject(name)
{

}

EnemyCharacter::~EnemyCharacter()
{

}


void EnemyCharacter::Awake() {
	_Type = vector<vector<int>>(static_cast<int>(Item::ItemCodeE::Max), vector<int>(LoadEnemyInfo::dropMax, -1));

	// このクラスで使用するコンポーネントを追加。
	// ※下記の関数を継承先のクラスで上書きしている場合はそちらが呼ばれる。
	_BuildMyComponents();

	// 位置情報初期化。
	transform->SetPosition(Vector3(0.0f,0.0f,0.0f));
	
	// 継承先により変わる処理。
	_AwakeSubClass();
}

void EnemyCharacter::Start() {
	// 使用するステートを列挙。
	_BuildState();
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
	//_MyComponent.CharacterController->AddRigidBody();	// ワールドに登録した瞬間にバウンディングボックスが生成されるため、初期情報設定のためここで登録。
	
	//プレイヤー。
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

}

void EnemyCharacter::Update() {

	if (_MyComponent.Parameter->GetDeathFlg())
	{
		if (_NowStateIdx != State::Death) {
			_ChangeState(State::Death);
			static_cast<EnemyDeathState*>(_NowState)->SetWaitTime(2.0f);
		}
	}
	else {
		// 死んでない。

		float work = Vector3(_Player->transform->GetPosition() - transform->GetPosition()).Length();
		if (work < nearEnemyInfo.length) {
			// プレイヤーとの距離が最短距離だった。
			nearEnemyInfo.length = work;
			nearEnemyInfo.object = this;
		}
	}

	_BarRenderUpdate();

	// 継承先により変わる処理。
	_UpdateSubClass();

	// エネミーのエフェクト更新。
	EffectUpdate();

	if (_NowState) {
		// 現在のステートを更新。
		if (_NowState->Update()) {
			// ステート処理終了。

			EnemyCharacter::State work = _NowStateIdx;	// 終了したステートを保存。
			// とりあえずステートを切り替えて必ず終了処理を呼ぶ。
			_ChangeState(EnemyCharacter::State::None);

			// ステートが終了したことを通知。
			_EndNowStateCallback(work);
		}
	}
	else {
		// 何もしないステートに入っているので常にステート終了コールバックを呼ぶ。
		_EndNowStateCallback(State::None);
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

	// 最短エネミーの情報をリセット。
	nearEnemyInfo = NearEnemyInfo(FLT_MAX, nullptr);
}


bool EnemyCharacter::IsOutsideDiscovery() {
	Vector3 work = _InitPos - transform->GetPosition();
	work.y = 0.0f;
	float NowRange = work.Length();
	if (NowRange > _discoveryRange) {
		// 追跡範囲外に出た。
		return true;
	}
	return false;
}

bool EnemyCharacter::IsOutsideWandering(const Vector3& Add) {
	Vector3 work = _InitPos - (transform->GetPosition() + Add);
	work.y = 0.0f;
	float NowRange = work.Length();
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
		_Player->transform->GetPosition(),
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
	_MyComponent.CharacterController->Init(_collisionInfo.offset, _collisionInfo.id, _MyComponent.Collider, _Gravity,true);
	
	// キャラクターコントローラーにパラメーターを設定。
	_ConfigCharacterController();

	_MyComponent.CharacterController->SetGravity(_Gravity);

	// キャラクターコントローラ押し出しコンポーネント作成。
	_CreateExtrudeCollision();

	int Attribute = Collision_ID::NOT_ID;
	if (_MyComponent.ExtrudeCollisions.size() > 0) {
		_MyComponent.CharacterExtrude->Init(_MyComponent.ExtrudeCollisions, Attribute);
	}
	else {
		// キャラクターコントローラ押し出し用の剛体を設定してない。

		abort();
	}

	_ConfigCharacterExtrude();
}

void EnemyCharacter::_BuildModelData() {
	//スキンモデルデータ作成。
	SkinModelData* modeldata = new SkinModelData();
	// モデルデータ読み込み。
	modeldata->CloneModelData(SkinModelManager::LoadModel(_FileName), _MyComponent.Animation);
	//モデルコンポーネントにモデルデータを設定。
	_MyComponent.Model->SetModelData(modeldata);
	//_MyComponent.Model->SetModelEffect(ModelEffectE::SPECULAR, false);

	_MyComponent.Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);
	_MyComponent.Model->SetModelEffect(ModelEffectE::DITHERING, true);

	_MyComponent.AnimationEventPlayer->Init(_MyComponent.Animation->GetNumAnimationSet());
}

void EnemyCharacter::_BuildState() {
	// 徘徊ステートを追加。
	_MyState.push_back(unique_ptr<EnemyState>(new EnemyWanderingState(this)));
	// 発見ステートを追加。
	_MyState.push_back(unique_ptr<EnemyDiscoveryState>(new EnemyDiscoveryState(this)));
	// 追跡ステートを追加。
	_MyState.push_back(unique_ptr<EnemyChaceState>(new EnemyChaceState(this)));
	// バックステップステートを追加。
	_MyState.push_back(unique_ptr<EnemyBackStepState>(new EnemyBackStepState(this)));
	// 威嚇ステートを追加。
	_MyState.push_back(unique_ptr<EnemyThreatState>(new EnemyThreatState(this)));
	// 会話ステートを追加。
	_MyState.push_back(unique_ptr<EnemySpeakState>(new EnemySpeakState(this)));
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

	_BuildStateSubClass();
}

void EnemyCharacter::_BuildAnimation() {

	// テーブル初期化。
	for (int idx = 0; idx < static_cast<int>(AnimationType::Max); idx++) {
		_AnimationNo[idx] = -1;
	}

	// アニメーションの再生時間テーブルを作成。
	vector<double> Datas = vector<double>(_MyComponent.Animation->GetNumAnimationSet(), -1.0);

	// 継承先でアニメーションの終了時間とエネミー共通のアニメーション列挙子への関連付けを行う。
	_BuildAnimationSubClass(Datas);

	// アニメーションの終了時間をコンポーネントに通知。
	for (int idx = 0; idx < static_cast<int>(Datas.size()); idx++) {
		_MyComponent.Animation->SetAnimationEndTime(static_cast<UINT>(idx), Datas[idx]);
	}
};

void EnemyCharacter::Drop() {
	_DropSubClass();
	for (int idx = 0; idx < static_cast<int>(Item::ItemCodeE::Max); idx++)
	{
		for (int i = 0; i < LoadEnemyInfo::dropMax; i++)
		{
			//落とすアイテムかをチェック。
			if (_Type[idx][i] != -1)
			{
				DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
				//落とすアイテムのidとコードを指定。
				item->Create(_Type[idx][i], idx, transform->GetPosition(), 2);
			}
		}
	}
	_Player->TakeDrop(GetDropEXP(), GetDropMoney());
}

/**
* アイテム効果.
*/
bool EnemyCharacter::ItemEffect(Item::ItemInfo * info)
{
	//戻り値.
	bool returnValue = false;

	returnValue = BuffAndDebuff(info->effectValue,info->time);

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

//	//戻り値.
//	bool returnValue = false;
//
//	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
//		int value = info->effectValue[idx];
//		if (value > 0) {
//			// バフ。
//			if (_MyComponent.ParticleEffect) {
//				_MyComponent.ParticleEffect->BuffEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// エフェクトコンポーネントないよ。
//				abort();
//			}
//#endif //  _DEBUG
//
//			_MyComponent.Parameter->Buff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(value), info->time);
//			_MyComponent.BuffDebuffICon->SelectUseIconType_Enemy();
//			_MyComponent.BuffDebuffICon->BuffIconCreate(static_cast<CharacterParameter::Param>(idx));
//			returnValue = true;
//		}
//		else if (value < 0) {
//			// デバフ(デメリット)。
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
//			_MyComponent.BuffDebuffICon->SelectUseIconType_Enemy();
//			_MyComponent.BuffDebuffICon->DebuffIconCreate(static_cast<CharacterParameter::Param>(idx));
//			returnValue = true;
//		}
//	}
	return returnValue;
}

bool EnemyCharacter::BuffAndDebuff(int effectValue[CharacterParameter::Param::MAX], float time) {
	bool ret = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
		int value = effectValue[idx];
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

			_MyComponent.Parameter->Buff(static_cast<CharacterParameter::Param>(idx), value, time);
			_MyComponent.BuffDebuffICon->SelectUseIconType_Enemy();
			_MyComponent.BuffDebuffICon->BuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			EnemyPlaySound(EnemyCharacter::SoundIndex::StatusUp);

			ret = true;
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
			_MyComponent.Parameter->Debuff(static_cast<CharacterParameter::Param>(idx), abs(value), time);
			_MyComponent.BuffDebuffICon->SelectUseIconType_Enemy();
			_MyComponent.BuffDebuffICon->DebuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			EnemyPlaySound(EnemyCharacter::SoundIndex::StatusDown);

			ret = true;
		}
	}
	return ret;
}

void EnemyCharacter::_ChangeState(State next) {
	if (next != EnemyCharacter::State::None && static_cast<int>(next) >= static_cast<int>(_MyState.size())) {
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

	if (next == State::None) {
		// 次のステートが指定されていない。
		_NowState = nullptr;
	}
	else {
		// 新しいステートに切り替え。
		_NowState = _MyState[static_cast<int>(next)].get();
		_NowState->Entry();
	}

	// ステート切り替え時のコールバック呼び出し。
	_ChangeStateCallback(_NowStateIdx, next);

	// 現在のステートの添え字を保存。
	_NowStateIdx = next;
}

void EnemyCharacter::_BuildSoundTable() {
	// 汎用効果音登録。
	//_SoundData = vector<unique_ptr<SoundData>>(static_cast<int>(SoundIndex::Max));

	// 攻撃音登録。
	_ConfigSoundData(EnemyCharacter::SoundIndex::Damage, "Damage_01.wav", 1.0f);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Buoon, "Buoonn.wav",1.0f);
	_ConfigSoundData(EnemyCharacter::SoundIndex::AttackGolem, "EnemyGolemAttack01.wav", 1.0f);

	// 死んだ時の声登録。
	_ConfigSoundData(EnemyCharacter::SoundIndex::Death, "EnemyGolemDie.wav",1.0f);

	// ダメージを受けた時の声登録。
	_ConfigSoundData(EnemyCharacter::SoundIndex::DamageGolem, "EnemyGolemDamage.wav", 1.0f);

	_ConfigSoundData(EnemyCharacter::SoundIndex::StatusUp, "Player/statusup.wav", 1.0f);
	_ConfigSoundData(EnemyCharacter::SoundIndex::StatusDown, "Player/statusdown.wav", 1.0f);

}

void EnemyCharacter::_ConfigSoundData(SoundIndex idx, char* filePath, bool is3D, bool isLoop) {
	if (idx >= SoundIndex::Max) {
		// 継承先独自の効果音。

		_SoundData.resize(static_cast<int>(idx) + 1);
	}
	_SoundData[static_cast<int>(idx)].reset(_CreateSoundData(filePath, is3D, isLoop));
}

EnemyCharacter::SoundData* EnemyCharacter::_CreateSoundData(char* filePath, float volume, bool isLoop, bool is3D) {
	EnemyCharacter::SoundData* data = new EnemyCharacter::SoundData;
	strcpy(data->Path, filePath);
	data->volume = volume;
	data->Is3D = is3D;
	data->IsLoop = isLoop;
	data->Source = INSTANCE(GameObjectManager)->AddNew<SoundSource>(filePath, 0);
	string work = "Asset/Sound/";
	work = work + filePath;
	data->Source->Init(work.c_str(), is3D);
	return data;
}

void EnemyCharacter::HitAttackCollisionEnter(AttackCollision* hitCollision) {
	if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Player)
	{
		if (_MyComponent.Parameter->GetParam(CharacterParameter::HP) > 0)
		{
			GiveDamage(*hitCollision->GetDamageInfo(),hitCollision->GetReactionType());
		}
	}
}

void EnemyCharacter::GiveDamage(const CharacterParameter::DamageInfo& info, AttackCollision::ReactionType reaction) {
	int _damage;
	if ((_NowState && _NowState->IsPossibleDamage()) || _NowState == nullptr) {
		// ダメージを与えられるステートだった。

		_damage = _MyComponent.Parameter->ReciveDamage(info);

		// ダメージ値をもとにパラメーター更新。
		_MyComponent.HPBar->SubValue(static_cast<float>(_damage));

		//受けたダメージ量を表示。
		Color c;
		if (_damage == 0)
		{
			c = Color::white * 0.3f;
		}
		else
		{
			c = Color::red;
		}
		AttackValue2D* attackvalue = INSTANCE(GameObjectManager)->AddNew<AttackValue2D>("AttackValue2D", 5);
		attackvalue->Init(transform, _damage, info.isCritical,1.5f, Vector3(0.0f, 1.0f, 0.0f), c);

		if (_isDamageMotion) {
			// ダメージ時にモーションを再生するか。

			if (reaction != AttackCollision::ReactionType::NotAction) {
				// 無反応の攻撃ではなかった。

				// エネミーはひるみモーションしかないのでとりあえずひるませる。

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
	}
	else {
		_damage = 0;
	}
}

float EnemyCharacter::GetNowSelectAttackRange()const {
	if (_nowAttack) {
		return _nowAttack->GetAttackRange();
	}
	return 0.0f;
}


/**
* エフェクト用更新.
*/
void EnemyCharacter::EffectUpdate() {
	bool isBuffEffect = false;
	bool isDeBuffEffect = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::Param::DEX; idx++) {

		if (_MyComponent.Parameter->GetBuffParam_Percentage((CharacterParameter::Param)idx) > 0)
		{
			isBuffEffect = true;
		}
		else
		{
			_MyComponent.BuffDebuffICon->DeleteBuffIcon((CharacterParameter::Param)idx);
		}
		if (_MyComponent.Parameter->GetDebuffParam_Percentage((CharacterParameter::Param)idx) > 0)
		{
			isDeBuffEffect = true;
		}
		else
		{
			_MyComponent.BuffDebuffICon->DeleteDebuffIcon((CharacterParameter::Param)idx);
		}
	}
	_MyComponent.ParticleEffect->SetBuffEffectFlag(isBuffEffect);
	_MyComponent.ParticleEffect->SetDebuffEffectFlag(isDeBuffEffect);
}



