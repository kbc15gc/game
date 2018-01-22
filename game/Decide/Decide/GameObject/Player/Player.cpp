#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\SplitSpace.h"
#include "GameObject\TextImage\AttackValue2D.h"
#include "..\History\HistoryManager.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\Component\BuffDebuffICon.h"
#include "GameObject\ItemManager\DropItem\DropItem.h"
#include "GameObject\Enemy\EnemyCharacter.h"

namespace
{
	float SlowAnimationSpeed = 0.7f;
	float NormalAnimationSpeed = 1.0f;
	float AttackAnimationSpeed = 1.3f;
	float Oboreru = 1.0f;
}

Player::Player(const char * name) :
	GameObject(name),
	//キャラクターコントローラーNULL
	_CharacterController(NULL),
	//重力設定
	_Gravity(-0.98f),
	//現在のステート
	_CurrentState(NULL),
	//走るステート
	_RunState(this),
	//アイドルステート
	_IdolState(this),
	//攻撃ステート
	_AttackState(this),
	//ダメージを受けるステート
	_ImpactState(this),
	//死亡ステート
	_DeathState(this),
	//ストップステート
	_StopState(this),
	//デバッグか
	_Debug(false),
	//ジャンプしているか
	_NoJump(false),
	//話しているか
	_IsSpeak(false)
{
	//経験値テーブルをロード
	_LoadEXPTable();
}

Player::~Player()
{
	/*char text[256];
	sprintf(text, "~Player address %x\n", *this);
	OutputDebugString(text);*/
	//for (auto& village : _NPC)
	//{
	//	for (auto& npc : village)
	//	{
	//		npc = nullptr;
	//	}
	//}
	//_NPC.clear();
}

void Player::Awake()
{
	
	_NearNPCLen = FLT_MAX;

	//モデル
	_Model = AddComponent<SkinModel>();
	//アニメーション
	_Anim = AddComponent<Animation>();

	//カプセルコライダー
	CCapsuleCollider* coll = AddComponent<CCapsuleCollider>();
	//キャラクターコントローラー
	_CharacterController = AddComponent<CCharacterController>();
	
	//キャラクターパラメーター
	_PlayerParam = AddComponent<CharacterParameter>();
	//回転コンポーネント
	_Rotation = AddComponent<ObjectRotation>();
	// HPバー。
	_HPBar = AddComponent<ParameterBar>();
	// MPバー。
	//_MPBar = AddComponent<ParameterBar>();
	//高さ設定
	_Height = 1.3f;
	//半径設定
	_Radius = 0.6f;
	//カプセルコライダー作成
	coll->Create(_Radius, _Height);

	//スキンモデル作成
	SkinModelData* modeldata = new SkinModelData();
	//モデルデータ作成
	modeldata->CloneModelData(SkinModelManager::LoadModel("Player.X"), _Anim);
	//モデル設定
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, true);
	_Model->SetModelEffect(ModelEffectE::RECEIVE_SHADOW, true);
	_Model->SetModelEffect(ModelEffectE::LIMLIGHT, true);
	_Model->SetModelEffect(ModelEffectE::FRUSTUM_CULLING, false);
	_Model->SetModelEffect(ModelEffectE::DITHERING, true);
	//_Model->SetAllBlend(Color::white * 13);
	//_Model->SetLuminanceColor(Color(1.0f, 0.0f, 0.0f, 1.0f));

	_Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	//@todo for MOVIE
	//_Model->SetEnable(false);

	//アニメーションイベント追加
	_AnimationEventPlayer = AddComponent<AnimationEventPlayer>();
	_AnimationEventPlayer->Init((int)AnimationNo::AnimationNum);
	AnimationEventControl();

	//キャラクターコントローラー初期化
	_CharacterController->Init(Vector3(0.0f,_Height * 0.5f + _Radius,0.0f), Collision_ID::PLAYER, coll, _Gravity);

	// 以下衝突を取りたい属性(横方向)を指定。
	_CharacterController->AttributeXZ_AllOff();	// 全衝突無視。
	_CharacterController->AddAttributeXZ(Collision_ID::GROUND);		// 地面コリジョンを追加。
	_CharacterController->AddAttributeXZ(Collision_ID::ENEMY);		// 敵のコリジョン追加。
	_CharacterController->AddAttributeXZ(Collision_ID::BOSS);		// 敵のコリジョン追加。
	_CharacterController->AddAttributeXZ(Collision_ID::BUILDING);	// 建物のコリジョン追加。
	
	// 以下衝突を取りたい属性(縦方向)を指定。
	_CharacterController->AttributeY_AllOn();	// 全衝突。
	_CharacterController->SubAttributeY(Collision_ID::ENEMY);	// エネミーを削除。
	_CharacterController->SubAttributeY(Collision_ID::BOSS);	// エネミーを削除。
	_CharacterController->SubAttributeY(Collision_ID::DRARIAN);	// エネミーを削除。
	_CharacterController->SubAttributeY(Collision_ID::ATTACK);	//攻撃コリジョン削除。
	_CharacterController->SubAttributeY(Collision_ID::DROPITEM);//ドロップアイテムコリジョンを削除。
	_CharacterController->SubAttributeY(Collision_ID::ITEMRANGE);//アイテムコリジョンを削除。
	_CharacterController->SubAttributeY(Collision_ID::SPACE);//空間コリジョンを削除。

	//キャラクターコントローラーの重力設定
	_CharacterController->SetGravity(_Gravity);

	//プレイヤーのパラメーター初期化。
	int lv = 0;


	//int lv = 0;

	// テスト。
	//int lv = 30;

	if (IS_CONTINUE)
	{
		JsonData PlayerData;
		if (PlayerData.Load("Player"))
		{
			picojson::object player = PlayerData.GetDataObject("Player");
			lv = player["Level"].get<double>() - 1;
			_nowEXP = player["EXP"].get<double>();
		}
	}

//#ifdef _DEBUG
//#define Village1
//#define Village2
//#define Village3

//#ifdef Village1
//	int lv = 1;
//#elif defined(Village2)
//	int lv = 12;
//#elif defined(Village3)
//	int lv = 22;
//#endif
//#else
//	int lv = 1;
//#endif
	_PlayerParam->ParamReset(_ParamTable[lv]);

	if (!IS_CONTINUE)
	{
		SaveLevel();
	}

	// HPのバーを表示。
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Green);
		_HPBar->Create(Colors, static_cast<float>(_PlayerParam->GetMaxHP()), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)),true, true, NULL);
	}
	// MPのバーを表示。
	{
		//vector<BarColor> Colors;
		//Colors.push_back(BarColor::Blue); //175.0f, 21.9f, 0.0f
		//_MPBar->Create(Colors, static_cast<float>(_PlayerParam->GetMaxMP()), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)), true, true, _HPBar->GetTransform(), Vector3(0.0f, 40.0f, 0.0f), Vector2(1.0f, 1.0f));
	}
	//ダメージSE初期化
	_DamageSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DamageSound", 0);
	_DamageSound->Init("Asset/Sound/Damage_01.wav");
	//レベルアップ音初期化
	_LevelUpSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("LevelUpSound", 0);
	_LevelUpSound->Init("Asset/Sound/Player/lvup.wav");
	_LevelUpSound->SetVolume(0.3f);
	//死亡ボイス初期化
	_DeathSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DeathSound", 0);
	_DeathSound->Init("Asset/Sound/Player/death.wav");
	_DeathSound->SetVolume(0.3f);
	//回復SE初期化
	_HeelSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("HeelSound", 0);
	_HeelSound->Init("Asset/Sound/Player/heal02.wav");
	_HeelSound->SetVolume(2.0f);
	//ステータスアップサウンド初期化
	_StatusUpSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StatusUpSound", 0);
	_StatusUpSound->Init("Asset/Sound/Player/statusup.wav");
	_StatusUpSound->SetVolume(1.0f);
	//ステータスダウンサウンド初期化
	_StatusDownSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StatusDownSound", 0);
	_StatusDownSound->Init("Asset/Sound/Player/statusdown.wav");
	_StatusDownSound->SetVolume(1.0f);
	//攻撃サウンド初期化
	_AttackSoound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_AttackSoound->Init("Asset/Sound/Player/PlayerAttack_00.wav");
	//攻撃ボイス初期化
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack1)] = INSTANCE(GameObjectManager)->AddNew<SoundSource>("Attack1", 0);
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack2)] = INSTANCE(GameObjectManager)->AddNew<SoundSource>("Attack2", 0);
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack3)] = INSTANCE(GameObjectManager)->AddNew<SoundSource>("Attack3", 0);
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack1)]->Init("Asset/Sound/Player/attack1.wav");
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack2)]->Init("Asset/Sound/Player/attack2.wav");
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack3)]->Init("Asset/Sound/Player/attack3.wav");
#ifdef _DEBUG
	_outputData = AddComponent<OutputData>();
#endif
	//プレイヤーの装備
	_Equipment = new PlayerEquipment;

	//パーティクルエフェクト。
	_ParticleEffect = AddComponent<ParticleEffect>();

	//バフデバフアイコン。
	_BuffDebuffICon = AddComponent<BuffDebuffICon>();
	_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());

	_Model->SetCharacterLight(&_CharaLight);

	_CharaLight.SetDiffuseLightDirection(0, Vector3(1.0f, 0.0f, 0.0f));
	_CharaLight.SetDiffuseLightDirection(1, Vector3(0.0f, 0.0f, 0.0f));
	_CharaLight.SetDiffuseLightDirection(2, Vector3(0.0f, 0.0f, 0.0f));
	_CharaLight.SetDiffuseLightDirection(3, Vector3(0.0f, 0.0f, 0.0f));
	
	_CharaLight.SetDiffuseLightColor(0, Vector4(0.5f, 0.5f, 0.5f, 30.0f));
	_CharaLight.SetDiffuseLightColor(1, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_CharaLight.SetDiffuseLightColor(2, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_CharaLight.SetDiffuseLightColor(3, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	
	_CharaLight.SetAmbientLight(Vector4(0.1f, 0.1f, 0.1f, 1.0f));

	if (IS_CONTINUE)
	{
		JsonData PlayerData;
		if (PlayerData.Load("Player_Pos"))
		{
			picojson::object player = PlayerData.GetDataObject("Player");
			_RespawnPos.x = player["RespawnPos_X"].get<double>();
			_RespawnPos.y = player["RespawnPos_Y"].get<double>();
			_RespawnPos.z = player["RespawnPos_Z"].get<double>();
		}
	}
	else
	{
		SetRespawnPos(Vector3(-202.0f, 58.0f, -156.0f));
	}

	//ポジション
	transform->SetLocalPosition(_RespawnPos);

}

void Player::Start()
{
	//モデルにカメラ設定
	_Model->SetCamera(INSTANCE(GameObjectManager)->mainCamera);
	//モデルにライト設定
	_Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//アニメーションの終了時間設定
	_AnimationEndTime[(int)AnimationNo::AnimationIdol] = -1.0f;			//アイドル
	_AnimationEndTime[(int)AnimationNo::AnimationWalk] = -1.0f;			//歩く
	_AnimationEndTime[(int)AnimationNo::AnimationRun] = 0.68f;			//走る
	_AnimationEndTime[(int)AnimationNo::AnimationJump] = -1.0f;			//ジャンプ
	_AnimationEndTime[(int)AnimationNo::AnimationAttack01] = -1.0f;		//攻撃1
	_AnimationEndTime[(int)AnimationNo::AnimationAttack02] = -1.0f;		//攻撃2
	_AnimationEndTime[(int)AnimationNo::AnimationAttack03] = -1.0f;		//攻撃3
	_AnimationEndTime[(int)AnimationNo::AnimationAttack04] = -1.0f;		//攻撃4
	_AnimationEndTime[(int)AnimationNo::AnimationAttack05] = -1.0f;		//攻撃5
	_AnimationEndTime[(int)AnimationNo::AnimationImpact] = 0.6f;		//ダメージ	
	_AnimationEndTime[(int)AnimationNo::AnimationDeath] = -1.0f;		//死亡
	//各エンドタイムを設定
	for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	}
	//初期アニメーション
	PlayAnimation(AnimationNo::AnimationWalk, 0.2f);
	//初期ステート設定
	ChangeState(State::Idol);

	//if (IS_CONTINUE)
	//{
	//	JsonData PlayerData;
	//	if (PlayerData.Load("Player_Pos"))
	//	{
	//		picojson::object player = PlayerData.GetDataObject("Player");
	//		_RespawnPos.x = player["RespawnPos_X"].get<double>();
	//		_RespawnPos.y = player["RespawnPos_Y"].get<double>();
	//		_RespawnPos.z = player["RespawnPos_Z"].get<double>();
	//	}
	//}
	//else
	//{
	//	SetRespawnPos(Vector3(-202.0f, 58.0f, -156.0f));
	//}

	////ポジション
	//transform->SetLocalPosition(_RespawnPos);
	//移動速度初期化
	_MoveSpeed = Vector3::zero;
	//攻撃アニメーションステートの初期化
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;

	//レベルアップイメージ初期化
	_LevelUpImage = INSTANCE(GameObjectManager)->AddNew<LevelUpImage>("LevelUP", 9);

	//ゲーム開始時にインベントリから装備している武具を探し装備し直す。
	Re_SetEquipment();

	//// 初期位置に移動。
	//_CharacterController->Execute();
}

void Player::Update()
{
	//カレントステートがNULLでない && ストップステートじゃない場合更新
	if (_CurrentState != nullptr && _State != State::Stop)
	{
		//ステートアップデート
		_CurrentState->Update();
	}

	if (_HPBar != nullptr)
	{
		//HPバーの更新
		_HPBar->Update();
	}

	//if (_MPBar != nullptr)
	//{
	//	//MPバーの更新
	//	_MPBar->Update();
	//}

	if (_PlayerParam)
	{	
		//レベルアップするか。
		if (_EXPTable.size() > 0 &&
			_nowEXP >= _EXPTable[_PlayerParam->GetParam(CharacterParameter::LV) - 1]
			&& _PlayerParam->GetParam(CharacterParameter::LV) < 100)
		{
			_LevelUP();
		}
		//ダメージを受ける処理。
		_Damage();
		//エフェクト
		EffectUpdate();

		//@todo for debug
		//@todo for releasedebug
#if defined(_DEBUG) || defined(RELEASEDEBUG)
		_DebugPlayer();
#endif // _DEBUG // RELEASEDEBUG
	}
	
	//アニメーションコントロール
	AnimationControl();
	// ※トランスフォームを更新すると内部でオイラー角からクォータニオンを作成する処理が呼ばれる。
	// ※オイラー角を使用せず直接クォータニオンを触る場合はこの処理を呼ぶとオイラー角の値で作成されたクォータニオンで上書きされる。
	// ※都合が悪いのでとりあえずコメントアウト。
		////トランスフォーム更新
		//transform->UpdateTransform();

	//NPCと話す
	Speak();

	//char test[256];
	//sprintf(test, "pos = %f,%f,%f\n", transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z);
	//OutputDebugString(test);
}

void Player::ChangeState(State nextstate)
{
	if (_CurrentState != NULL) {
		//現在のステートを抜けるときの処理
		_CurrentState->Leave();
	}
	switch (nextstate)
	{
		
	case State::Idol:
		//待機状態
		_CurrentState = &_IdolState;
		break;
	case State::Run:
		//走る状態
		_CurrentState = &_RunState;
		break;
	case State::Attack:
		//攻撃状態
		_CurrentState = &_AttackState;
		break;
	case State::Impact:
		//ダメージを受けた状態
		_CurrentState = &_ImpactState;
		break;
	case State::Death:					
		//死亡状態
		_CurrentState = &_DeathState;
		break;
	case State::Stop:
		//ストップ状態
		_CurrentState = &_StopState;
		break;
	default:
		//デフォルト
		break;
	}

	//次のステートに変更
	_State = nextstate;
	//各ステートの入りに呼ばれる処理
	_CurrentState->Enter();
}

void Player::PlayAnimation(AnimationNo animno, float interpolatetime , int loopnum)
{
	//現在のアニメーションと違うアニメーション　&& アニメーションナンバーが無効でない
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationNo::AnimationInvalid)
	{
		_Anim->PlayAnimation((int)animno, interpolatetime , loopnum);
	}
}

void Player::AnimationControl()
{
	if (!_CharacterController)
	{
		return;
	}
	//アニメーションスピードは基本１
	_Anim->SetAnimeSpeed(NormalAnimationSpeed);
	//死亡アニメーション
	if (_State == State::Death)
	{
		_Anim->PopAnimationQueue();
		PlayAnimation(AnimationNo::AnimationDeath, 0.0f, 0);
		return;
	}
	//ジャンプアニメーション
	//ストップじゃないならジャンプする。
	if (_CharacterController->IsJump() && _State != State::Stop && !_NoJump)
	{
		PlayAnimation(AnimationNo::AnimationJump, 0.1f, 0);
	}
	else
	{
		//走るアニメーション
		if (_State == State::Run)
		{
			PlayAnimation(AnimationNo::AnimationRun, 0.1f);
		}
		//アイドルアニメーション
		else if (_State == State::Idol)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.1f);
		}
		//プレイヤーストップならアイドルアニメーション
		else if (_State == State::Stop)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
		}
		//ダメージを受けたアニメーション
		else if (_State == State::Impact)
		{
			_Anim->PopAnimationQueue();
			PlayAnimation(AnimationNo::AnimationImpact, 0.0f,0);
		}
		//アタックアニメーション
		else if (_State == State::Attack)
		{
			//攻撃の時はスピードを変更。
			_Anim->SetAnimeSpeed(AttackAnimationSpeed);
			if (_NextAttackAnimNo == AnimationNo::AnimationAttackStart)
			{
				//攻撃開始
				PlayAnimation(_NextAttackAnimNo, 0.0f, 0);
				_NowAttackAnimNo = _NextAttackAnimNo;
				_NextAttackAnimNo = AnimationNo::AnimationInvalid;
			}
			else if (_NextAttackAnimNo != AnimationNo::AnimationInvalid)
			{
				//連撃
				//Animation::PlayAnimInfo* info = new Animation::PlayAnimInfo((UINT)_NextAttackAnimNo, 0.1f, 0.7f, 1);
				//_Anim->AddAnimationQueue(info);
				//アニメーションキューに追加。
				_Anim->AddAnimationQueue(new Animation::PlayAnimInfo((UINT)_NextAttackAnimNo, 0.0f, 0.7f, 0));
				_NowAttackAnimNo = _NextAttackAnimNo;
				_NextAttackAnimNo = AnimationNo::AnimationInvalid;
			}
		}
	}
}

//攻撃を受けたとき。
void Player:: HitAttackCollisionEnter(AttackCollision* hitCollision) 
{
	if (_PlayerParam == nullptr) {
		return;
	}
	if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Enemy && _PlayerParam->GetParam(CharacterParameter::HP) > 0)
	{
#ifdef _DEBUG
		if (_Equipment == nullptr)
		{
			// 装備用の構造体がNull。
			abort();
		}
#endif
		//ダメージを受けた状態に変更
		if (_State != State::Death && _State != State::Stop && _State != State::Impact && hitCollision->GetReactionType() == AttackCollision::ReactionType::Leans)
		{
			ChangeState(State::Impact);
		}

		// ダメージを与える処理
		int damage = _PlayerParam->ReciveDamage(*hitCollision->GetDamageInfo(), _Equipment->armor);
		_HPBar->SubValue(static_cast<float>(damage));
		_DamageSound->Play(false);//ダメージを受けたときのSE
		AttackValue2D* attackvalue = INSTANCE(GameObjectManager)->AddNew<AttackValue2D>("AttackValue2D", 5);
		Color c;
		if (damage == 0)
		{
			//灰色に
			c = Color::white * 0.3f;
		}
		else
		{
			c = Color::blue;
		}
		//ダメージ量を表示する。
		attackvalue->Init(transform, damage, hitCollision->GetDamageInfo()->isCritical, 1.5f, Vector3(0.0f, _Height, 0.0f),c);
		attackvalue->transform->SetParent(transform);
	}
}

//解放。
void Player::Releace()
{
	_CharacterController = nullptr;
	_DamageSound = nullptr;
	_Rotation = nullptr;
	_PlayerParam = nullptr;
	_CurrentState = nullptr;
	_HPBar = nullptr;
	//_MPBar = nullptr;
	for (auto& p : _AttackBoiceSound)
	{
		p = nullptr;
	}
	_DeathSound = nullptr;
	//_AnimationEventPlayer = nullptr;
}

/**
* アイテムが使用された.
*/
bool Player::ItemEffect(Item::ItemInfo* info)
{
	//戻り値.
	bool returnValue = false;

	//HP回復処理.
	if (_PlayerParam->HeelHP(info->effectValue[CharacterParameter::Param::HP]))
	{
		if (_ParticleEffect)
		{
			_ParticleEffect->HeelHpEffect();
		}
		if (_HPBar) {
			_HPBar->SetValue(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::Param::HP)));
		}

		_HeelSound->Play(false);
		
		returnValue = true;
	}
	if (_PlayerParam->HeelMP(info->effectValue[CharacterParameter::Param::MP]))
	{
		if (_ParticleEffect)
		{
			_ParticleEffect->HeelMpEffect();
		}
		/*if (_MPBar) {
			_MPBar->SetValue(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::Param::MP)));
		}*/
		
		_HeelSound->Play(false);
		
		returnValue = true;
	}

	returnValue = BuffAndDebuff(info->effectValue,info->time);
//	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
//		int value = info->effectValue[idx];
//		if (value > 0) {
//			// バフ。
//			if (_ParticleEffect) {
//				_ParticleEffect->BuffEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// エフェクトコンポーネントないよ。
//				abort();
//			}
//#endif //  _DEBUG
//
//			_PlayerParam->Buff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(value), info->time);
//			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
//			_BuffDebuffICon->BuffIconCreate(static_cast<CharacterParameter::Param>(idx));
//
//			_StatusUpSound->Play(false);
//
//			returnValue = true;
//		}
//		else if (value < 0) {
//			// デバフ(デメリット)。
//			if (_ParticleEffect) {
//				_ParticleEffect->DeBuffEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// エフェクトコンポーネントないよ。
//				abort();
//			}
//#endif //  _DEBUG
//			_PlayerParam->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
//			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
//			_BuffDebuffICon->DebuffIconCreate(static_cast<CharacterParameter::Param>(idx));
//
//			_StatusDownSound->Play(false);
//
//			returnValue = true;
//		}
//	}
	return returnValue;
}

bool Player::BuffAndDebuff(int effectValue[CharacterParameter::Param::MAX], float time) {
	bool ret = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
		int value = effectValue[idx];
		if (value > 0) {
			// バフ。
			if (_ParticleEffect) {
				_ParticleEffect->BuffEffect();
			}
#ifdef  _DEBUG
			else {
				// エフェクトコンポーネントないよ。
				abort();
			}
#endif //  _DEBUG

			_PlayerParam->Buff(static_cast<CharacterParameter::Param>(idx), value, time);
			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
			_BuffDebuffICon->BuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			_StatusUpSound->Play(false);

			ret = true;
		}
		else if (value < 0) {
			// デバフ(デメリット)。
			if (_ParticleEffect) {
				_ParticleEffect->DeBuffEffect();
			}
#ifdef  _DEBUG
			else {
				// エフェクトコンポーネントないよ。
				abort();
			}
#endif //  _DEBUG
			_PlayerParam->Debuff(static_cast<CharacterParameter::Param>(idx),abs(value), time);
			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
			_BuffDebuffICon->DebuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			_StatusDownSound->Play(false);

			ret = true;
		}
	}
	return ret;
}

/**
* エフェクト用更新.
*/
void Player::EffectUpdate()
{
	bool isBuffEffect = false;
	bool isDeBuffEffect = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::Param::DEX; idx++) {

		if (_PlayerParam->GetBuffParam_Percentage((CharacterParameter::Param)idx) > 0)
		{
			isBuffEffect = true;
		}
		else
		{
			_BuffDebuffICon->DeleteBuffIcon((CharacterParameter::Param)idx);
		}

		if (_PlayerParam->GetDebuffParam_Percentage((CharacterParameter::Param)idx) > 0)
		{
			isDeBuffEffect = true;
		}
		else
		{
			_BuffDebuffICon->DeleteDebuffIcon((CharacterParameter::Param)idx);
		}
	}

	_ParticleEffect->SetBuffEffectFlag(isBuffEffect);
	_ParticleEffect->SetDebuffEffectFlag(isDeBuffEffect);
}

//攻撃を受けたとき
void Player::_Damage()
{
	//死亡ステート以外の時。
	//ライフが0になると死亡する。

	if (_PlayerParam->GetDeathFlg() == true && _State != State::Death)
	{
		_DeathSound->Play(false);
		ChangeState(State::Death);
		return;
	}

	//海に入るとダメージを食らう。
	//海の中の場合。
	//HPが0以上なら。
	//デバッグ時でない。
	//1秒間隔で。

	if (transform->GetLocalPosition().y < 48.5f && _PlayerParam->GetParam(CharacterParameter::HP) > 0 && _Debug == false)
	{
		static float time = 0.0f;
		time += Time::DeltaTime();
		//if (fmod(time, 1.0f) >= 1.0f)
		if(time >= 0.5f)
		{
			//最大HPの1割ずつ減る。
			_HPBar->SubValue(static_cast<float>((_PlayerParam->ReciveDamageThrough(static_cast<int>(_PlayerParam->GetMaxHP() * 0.1f)))));
			time = 0.0f;
		}
	}

}

//経験値テーブルをロード。
void Player::_LoadEXPTable()
{
	std::vector<std::unique_ptr<ExperiencePointTableInfo>> exptbaleinfo;
	Support::LoadCSVData<ExperiencePointTableInfo>("Asset/Data/PlayerParameter.csv", ExperiencePointTableInfoData, ARRAYSIZE(ExperiencePointTableInfoData), exptbaleinfo);
	
	_EXPTable.clear();

	for (int i = 0; i < MAXLV; i++)
	{
		_EXPTable.push_back(exptbaleinfo[i]->ExperiencePoint);

		for (int idx = 0; idx < CharacterParameter::Param::MAX; idx++) {
			_ParamTable[i][idx] = exptbaleinfo[i]->param[idx];
		}
	}
}

void Player::_LevelUP()
{
	// 現在の経験値リセット。
	_nowEXP = _nowEXP - _EXPTable[_PlayerParam->GetParam(CharacterParameter::Param::LV) - 1];	// レベルアップ時に溢れた分を現在の経験値に設定。

	// 次のレベルのパラメータを設定。
	_PlayerParam->ParamReset(_ParamTable[_PlayerParam->GetParam(CharacterParameter::Param::LV)]);

	//HPが上がったのでHPバーのHP設定しなおす。
	_HPBar->Reset(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)),true);
	//MPが上がったのでMPバーのMP設定しなおす。
	//_MPBar->Reset(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)),true);
	//レベルアップ時のイメージ表示。
	_LevelUpImage->Init();
	//レベルアップ時の音再生。
	_LevelUpSound->Play(false);
	//レベルアップエフェクト
	/*_ParticleEffect->LevelUpEffect();
	_ParticleEffect->SetLevelUPEffectFlag(true);*/

	SaveLevel();
}

void Player::Speak()
{
	//ショップイベントフラグ取得。
	bool eventflag = INSTANCE(EventManager)->IsEvent();
	////NPCを取得
	//vector<vector<NPC*>> npc;
	//npc = INSTANCE(HistoryManager)->GetNPCList();
	////NPC
	//float len = FLT_MAX;
	//
	////一番近い村人を探す。
	//for (auto village : npc)
	//{
	//	//サイズが0ならコンティニュー
	//	if (village.size() == 0)
	//	{
	//		continue;
	//	}
	//	//NPC
	//	for (auto npc : village)
	//	{
	//		if (npc == nullptr || !npc->GetActive())
	//		{
	//			continue;
	//		}
	//		//NPCからプレイヤーのベクトル
	//		Vector3 dir = npc->transform->GetPosition() - transform->GetPosition();
	//		//現在一番近いモノより近いので。
	//		if (len > dir.Length())
	//		{
	//			//他に近い人がいるので、前の人とはばいばい。
	//			if (nearnpc)
	//			{
	//				nearnpc->SetIsSpeak(false);
	//			}
	//			len = dir.Length();
	//			nearnpc = npc;
	//		}
	//	}
	//}
	//近くのnpcがいる場合
	if (_NearNPC)
	{
		_NearNPCLen = (_NearNPC->transform->GetPosition() - transform->GetPosition()).Length();
		//会話可能
		if (_NearNPCLen <= 3.0f && !eventflag)
		{
			//地面についていれば話しかけれる
			//ショップイベントでないとき。
			if (_CharacterController->IsOnGround())
			{
				//会話のためHPバーなどを消す。
				_HPBar->RenderDisable();
				//_MPBar->RenderDisable();
				//話すフラグセット
				_NearNPC->SetIsSpeak(true);
				//プレイヤー話すフラグ設定
				//ジャンプしなくなる
				_NoJump = true;
			}
		}
		else
		{
			//話すNPCがいないので
			_NearNPC->SetIsSpeak(false);
			//話し終わると
			if (_NoJump)
			{
				if (_HPBar/* && _MPBar*/)
				{
					_HPBar->RenderEnable();
					//_MPBar->RenderEnable();
					_NoJump = false;
				}
			}
		}
	}
	//NPCなし
	else
	{
		_NearNPCLen = FLT_MAX;
	}

	//村
	//for (auto village : npc)
	//{
	//	//サイズが0ならコンティニュー
	//	if (village.size() == 0)
	//	{
	//		continue;
	//	}
	//	//NPC
	//	for (auto npc : village)
	//	{
	//		if (npc == nullptr)
	//		{
	//			continue;
	//		}
	//		//NPCからプレイヤーのベクトル
	//		Vector3 dir = npc->transform->GetPosition() - transform->GetPosition();
	//		float len = dir.Length();
	//		//範囲内かどうか
	//		//ショップイベント中でないとき
	//		if (npc->GetRadius() >= len && !eventflag)
	//		{
	//			if (_CharacterController == nullptr)
	//			{
	//				return;
	//			}
	//			//地面についていれば話しかけれる
	//			//ショップイベントでないとき。
	//			if (_CharacterController->IsOnGround())
	//			{
	//				//会話のためHPバーなどを消す。
	//				_HPBar->RenderDisable();
	//				//_MPBar->RenderDisable();
	//				//話すフラグセット
	//				npc->SetIsSpeak(true);
	//				//プレイヤー話すフラグ設定
	//				//ジャンプしなくなる
	//				_NoJump = true;
	//				//これ以上処理は続けない
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			//話すNPCがいないので
	//			npc->SetIsSpeak(false);
	//			//話し終わると
	//			if (_NoJump)
	//			{
	//				if (_HPBar/* && _MPBar*/)
	//				{
	//					_HPBar->RenderEnable();
	//					//_MPBar->RenderEnable();
	//					_NoJump = false;
	//				}
	//			}
	//		}
	//	}
	//	//話す状態ならもう回さない。
	//	if (_NoJump)
	//	{
	//		break;
	//	}
	//}
}

#if defined(_DEBUG) || defined(RELEASEDEBUG)
void Player::_DebugPlayer()
{
	//お金増える
	if (KeyBoardInput->isPush(DIK_O))
	{
		INSTANCE(Inventory)->AddPlayerMoney(10000);
	}
	//経験値増える
	
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_1))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Fire);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_2))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Tree);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_3))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Stone);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_4))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Copper);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_5))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Agriculture);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_6))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Hunt);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_7))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Iron);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_8))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Oil);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_9))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Medicine);
	}



	
	int level = _PlayerParam->GetParam(CharacterParameter::LV) - 1;
	//経験値を増やす。
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_1))
	{
		if (level < 99)
		{
			_nowEXP = 0;
			_DebugLevel(++level);
		}
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_2))
	{
		if (level > 0)
		{
			_nowEXP = 0;
			_DebugLevel(--level);
		}
	}
	//レベルを上げる。
	if (level < 95)
	{
		if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_3))
		{
			_nowEXP = 0;
			level += 5;
			_DebugLevel(level);
		}
	}
	if (level >= 6)
	{
		//レベル下げる。
		if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_4))
		{
			_nowEXP = 0;
			level -= 5;
			_DebugLevel(level);
		}
	}

	//ドロップアイテムを出す。
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_5)) {
		DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
		item->Create(0,0, transform->GetPosition(), 2);
	}

	//ドロップアイテムを出す。
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_6)) {
		DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
		item->Create(0,1, transform->GetPosition(), 2);
	}

	//ドロップアイテムを出す。
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_7)) {
		DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
		item->Create(0,2, transform->GetPosition(), 2);
	}

	//プレイヤー死亡
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_D))
	{
		_HPBar->SubValue(static_cast<float>((_PlayerParam->ReciveDamageThrough(_PlayerParam->GetParam(CharacterParameter::HP)))));
	}

	//移動
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_U)) {
		transform->SetLocalPosition(-387.3f, 58.307f, -75.8f);
		_nowEXP = 0;
		_DebugLevel(5);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_I)) {
		transform->SetLocalPosition(-108.1f, 55.524f, 533.9f);
		_nowEXP = 0;
		_DebugLevel(14);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_O)) {
		transform->SetLocalPosition(218.88f, 67.0f, -0.92f);
		_nowEXP = 0;
		_DebugLevel(39);
	}

	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_C)) {
		// 魔王城周辺適正。
		transform->SetLocalPosition(-157.8f, 121.8f, 79.48f);
		_nowEXP = 0;
		_DebugLevel(42);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_V)) {
		// 魔王城1層適正。
		transform->SetLocalPosition(-142.4f, 121.8f, 175.9f);
		_nowEXP = 0;
		_DebugLevel(44);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_B)) {
		// 魔王城2層適正。
		transform->SetLocalPosition(-164.7f, 147.2f, 128.6f);
		_nowEXP = 0;
		_DebugLevel(46);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_N)) {
		// 魔王城3層適正。
		transform->SetLocalPosition(-140.9f, 169.3f, 246.8f);
		_nowEXP = 0;
		_DebugLevel(56);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_M)) {
		// 魔王適正。
		transform->SetLocalPosition(-145.69, 190.0f, 264.72f);
		_nowEXP = 0;
		_DebugLevel(61);
	}
}
void Player::_DebugLevel(int lv)
{
	// 次のレベルのパラメータを設定。
	_PlayerParam->ParamReset(_ParamTable[lv]);
	//HPが上がったのでHPバーのHP設定しなおす。
	_HPBar->Reset(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)),true);
	//MPが上がったのでMPバーのMP設定しなおす。
	//_MPBar->Reset(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)),true);
}
#endif // _DEBUG

//プレイヤーに装備をセット(中でアイテムコードを見て武器か防具をセット)。
void Player::SetEquipment(HoldItemBase* equi) {
	if (equi == nullptr) {
		return;
	}

	//防具。
	if (equi->GetInfo()->TypeID == Item::ItemCodeE::Armor) {
		//装備している防具と装備しようとしている防具が同じなら外す。
		if (static_cast<HoldArmor*>(equi) == _Equipment->armor) {
			_Equipment->armor->SetIsEquipFalse();
			_Equipment->armor = nullptr;
			return;
		}
		//前に装備していた防具を外す。
		else if (_Equipment->armor != nullptr) {

			_Equipment->armor->SetIsEquipFalse();
			_Equipment->armor = nullptr;
		}
		//防具。
		_Equipment->armor = static_cast<HoldArmor*>(equi);
		//装備フラグをtrueにする。
		_Equipment->armor->SetIsEquipTrue();

		INSTANCE(Inventory)->SaveArmor();
	}
	else
		//武器。
	{
		//装備している防具と装備しようとしている防具が同じなら外す。
		if (static_cast<HoldWeapon*>(equi) == _Equipment->weapon) {
			_Equipment->weapon->SetIsEquipFalse();
			_Equipment->weapon = nullptr;
			return;
		}
		else if (_Equipment->weapon != nullptr) {
			//前に装備していた武器を外す。
			_Equipment->weapon->SetIsEquipFalse();
			_Equipment->weapon = nullptr;
		}
		//武器。
		_Equipment->weapon = static_cast<HoldWeapon*>(equi);
		//装備フラグをtrueにする。
		_Equipment->weapon->SetIsEquipTrue();

		INSTANCE(Inventory)->SaveWeapon();
	}
}

//ゲーム開始時にインベントリから装備している武具を探し装備し直す。
void Player::Re_SetEquipment() {

	//武具分装備フラグを探す。
	for (int type = static_cast<int>(Item::ItemCodeE::Armor); type <= static_cast<int>(Item::ItemCodeE::Weapon); type++)
	{
		//武具のインベントリリストを取得。
		vector<HoldItemBase*> vector = INSTANCE(Inventory)->GetInventoryList(static_cast<Item::ItemCodeE>(type));
		for (auto itr = vector.begin(); itr != vector.end();)
		{
			//装備されている。
			if ((*itr) != nullptr&& static_cast<HoldEquipment*>((*itr))->GetIsEquip() == true) {
				//プレイヤーに装備。
				SetEquipment((*itr));
				break;
			}
			else
			{
				itr++;
			}
		}
	}
}


void Player::SetNPC(NPC * npc)
{
	//そのまま設定
	if ((_NearNPC == nullptr) || (npc == nullptr))
	{
		_NearNPC = npc;
	}
	else
	{
		_NearNPC->SetIsSpeak(false);
		_NearNPC = npc;
	}
}

void Player::AnimationEventControl()
{
	/*******************/
	/*		攻撃	   */
	/*******************/
	//攻撃1
	{
		float eventframe = 0.5f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack01, eventframe, static_cast<AnimationEvent>(&Player::Attack1));
	}
	//攻撃2
	{
		float eventframe = 0.5f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack02, eventframe, static_cast<AnimationEvent>(&Player::Attack2));

	}
	//攻撃3
	{
		float eventframe = 0.5f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack03, eventframe, static_cast<AnimationEvent>(&Player::Attack3));

	}
	//攻撃4
	{
		float eventframe = 0.5f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack04, eventframe, static_cast<AnimationEvent>(&Player::Attack4));
	}
	//攻撃5
	{
		float eventframe = 1.1f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack05, eventframe, static_cast<AnimationEvent>(&Player::Attack5));
	}
}

void Player::Attack1()
{
	//攻撃時のサウンド再生。
	_AttackSoound->Play(false);
	//攻撃ボイス再生
	_AttackBoiceSound[(int)Player::AttackBoice::Attack1]->Play(false);
	//攻撃コリジョン作成
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 120)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 2.5f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}

void Player::Attack2()
{
	//攻撃時のサウンド再生。
	_AttackSoound->Play(false);
	//攻撃ボイス再生
	_AttackBoiceSound[(int)Player::AttackBoice::Attack2]->Play(false);
	//攻撃コリジョン作成
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack02", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 100)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 2.5f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}

void Player::Attack3()
{
	//攻撃時のサウンド再生。
	_AttackSoound->Play(false);
	//攻撃ボイス再生
	_AttackBoiceSound[(int)Player::AttackBoice::Attack1]->Play(false);
	//攻撃コリジョン作成
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack03", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 120)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 2.5f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}

void Player::Attack4()
{
	//攻撃時のサウンド再生。
	_AttackSoound->Play(false);
	//攻撃ボイス再生
	_AttackBoiceSound[(int)Player::AttackBoice::Attack2]->Play(false);
	//攻撃コリジョン作成
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack04", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 100)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 2.5f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}

void Player::Attack5()
{
	//攻撃時のサウンド再生。
	_AttackSoound->Play(false);
	//攻撃ボイス再生
	_AttackBoiceSound[(int)Player::AttackBoice::Attack3]->Play(false);
	//攻撃コリジョン作成
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack05", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 200)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(2.5f, 2.5f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}
