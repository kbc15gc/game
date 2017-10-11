#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\SplitSpace.h"
#include "GameObject\AttackValue2D.h"
#include "..\History\HistoryManager.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\Component\BuffDebuffICon.h"

namespace
{
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
	//話しているか
	_Speak(false)
{
	//経験値テーブルをロード
	_LoadEXPTable();
}

Player::~Player()
{
	char text[256];
	sprintf(text, "~Player address %x\n", *this);
	OutputDebugString(text);
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
	_MPBar = AddComponent<ParameterBar>();
	//アニメーションイベント。
	//_AnimationEventPlayer = AddComponent<AnimationEventPlayer>();
	//高さ設定
	_Height = 1.3f;
	//半径設定
	_Radius = 0.8f;
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
	//_Model->SetAllBlend(Color::white * 13);

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
	_CharacterController->AddAttributeXZ(Collision_ID::BUILDING);	// 建物のコリジョン追加。
	// 以下衝突を取りたい属性(縦方向)を指定。
	_CharacterController->AttributeY_AllOn();	// 全衝突。
	_CharacterController->SubAttributeY(Collision_ID::ENEMY);	// エネミーを削除。
	_CharacterController->SubAttributeY(Collision_ID::ATTACK);	//攻撃コリジョン削除。
	//キャラクターコントローラーの重力設定
	_CharacterController->SetGravity(_Gravity);

	//プレイヤーのパラメーター初期化。
	_PlayerParam->ParamReset(_ParamTable[0]);
	
	// HPのバーを表示。
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Green);
		_HPBar->Create(Colors, _PlayerParam->GetMaxHP(), _PlayerParam->GetParam(CharacterParameter::HP),true, true, NULL);
	}
	// MPのバーを表示。
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Blue); //175.0f, 21.9f, 0.0f
		_MPBar->Create(Colors, _PlayerParam->GetMaxMP(), _PlayerParam->GetParam(CharacterParameter::MP), true, true, _HPBar->GetTransform(), Vector3(0.0f, 40.0f, 0.0f), Vector2(1.0f, 1.0f));
	}
	//ダメージSE初期化
	_DamageSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DamageSound", 0);
	_DamageSound->Init("Asset/Sound/Damage_01.wav");
	//レベルアップ音初期化
	_LevelUpSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("LevelUpSound", 0);
	_LevelUpSound->Init("Asset/Sound/Player/lvup.wav");
	_LevelUpSound->SetVolume(1.0f);
	//死亡ボイス初期化
	_DeathSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DeathSound", 0);
	_DeathSound->Init("Asset/Sound/Player/death.wav");
	//回復SE初期化
	_HeelSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("HeelSound", 0);
	_HeelSound->Init("Asset/Sound/Player/heal02.wav");
	//ステータスアップサウンド初期化
	_StatusUpSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StatusUpSound", 0);
	_StatusUpSound->Init("Asset/Sound/Player/statusup.wav");
	_StatusUpSound->SetVolume(2.0f);
	//ステータスダウンサウンド初期化
	_StatusDownSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StatusDownSound", 0);
	_StatusDownSound->Init("Asset/Sound/Player/statusdown.wav");
	_StatusDownSound->SetVolume(2.0f);
	//攻撃サウンド初期化
	_AttackSoound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_AttackSoound->Init("Asset/Sound/Player/PlayerAttack_00.wav");
	//攻撃ボイス初期化
	_AttackBoiceSound.push_back(INSTANCE(GameObjectManager)->AddNew<SoundSource>("Attack1", 0));
	_AttackBoiceSound.push_back(INSTANCE(GameObjectManager)->AddNew<SoundSource>("Attack2", 0));
	_AttackBoiceSound.push_back(INSTANCE(GameObjectManager)->AddNew<SoundSource>("Attack3", 0));
	_AttackBoiceSound[(int)AttackBoice::Attack1]->Init("Asset/Sound/Player/attack1.wav");
	_AttackBoiceSound[(int)AttackBoice::Attack2]->Init("Asset/Sound/Player/attack1.wav");
	_AttackBoiceSound[(int)AttackBoice::Attack3]->Init("Asset/Sound/Player/attack2.wav");
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
	//初期アニメーションとしてアイドルを再生
	PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
	//初期ステート設定
	ChangeState(State::Idol);
	//ポジション
	_StartPos = Vector3(-202.0f, 58.0f, -156.0f);
	transform->SetLocalPosition(_StartPos);
	//移動速度初期化
	_MoveSpeed = Vector3::zero;
	//攻撃アニメーションステートの初期化
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;

	//レベルアップイメージ初期化
	_LevelUpImage = INSTANCE(GameObjectManager)->AddNew<LevelUpImage>("LevelUP", 9);

	//ゲーム開始時にインベントリから装備している武具を探し装備し直す。
	Re_SetEquipment();

	// 初期位置に移動。
	_CharacterController->Execute();
}

void Player::Update()
{
	//カレントステートがNULLでない && ストップステートじゃない場合更新
	if (_CurrentState != nullptr)
	{
		//ステートアップデート
		_CurrentState->Update();
	}
	if (_HPBar != nullptr)
	{
		//HPバーの更新
		_HPBar->Update();
	}
	if (_MPBar != nullptr)
	{
		//MPバーの更新
		_MPBar->Update();
	}
	
	if (_PlayerParam)
	{
		//レベルアップするか。
		if (_EXPTable.size() > 0 &&
			_nowEXP >= _EXPTable[_PlayerParam->GetParam(CharacterParameter::LV) - 1])
		{
			_LevelUP();
		}
		//ダメージを受ける処理。
		//_Damage();
		//エフェクト
		EffectUpdate();
		//@todo for debug
#ifdef _DEBUG
		_DebugPlayer();
#endif // _DEBUG
	}
	
	//アニメーションコントロール
	AnimationControl();
	// ※トランスフォームを更新すると内部でオイラー角からクォータニオンを作成する処理が呼ばれる。
	// ※オイラー角を使用せず直接クォータニオンを触る場合はこの処理を呼ぶとオイラー角の値で作成されたクォータニオンで上書きされる。
	// ※都合が悪いのでとりあえずコメントアウト。
		////トランスフォーム更新
		//transform->UpdateTransform();

	//NPCと話すか
	Speak();
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
	//アニメーションスピードは基本１
	_Anim->SetAnimeSpeed(NormalAnimationSpeed);
	//死亡アニメーション
	if (_State == State::Death)
	{
		PlayAnimation(AnimationNo::AnimationDeath, 0.1f, 1);
		return;
	}
	//ダメージを受けたアニメーション
	if (_State == State::Impact)
	{
		PlayAnimation(AnimationNo::AnimationImpact, 0.2f, 1);
		return;
	}
	//ジャンプアニメーション
	//ストップじゃないならジャンプする。
	if (_CharacterController->IsJump() && _State != State::Stop && !_Speak)
	{
		PlayAnimation(AnimationNo::AnimationJump, 0.1f, 0);
	}
	else
	{
		//走るアニメーション
		if (_State == State::Run)
		{
			PlayAnimation(AnimationNo::AnimationRun, 0.2f);
		}
		//アイドルアニメーション
		else if (_State == State::Idol)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
		}
		//プレイヤーストップならアイドルアニメーション
		else if (_State == State::Stop)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
		}
		//アタックアニメーション
		else if (_State == State::Attack)
		{
			//攻撃の時はスピードを変更。
			_Anim->SetAnimeSpeed(AttackAnimationSpeed);
			if (_NextAttackAnimNo == AnimationNo::AnimationAttackStart)
			{
				//攻撃開始
				PlayAnimation(_NextAttackAnimNo, 0.1f, 1);
				_NowAttackAnimNo = _NextAttackAnimNo;
				_NextAttackAnimNo = AnimationNo::AnimationInvalid;
			}
			else if (_NextAttackAnimNo != AnimationNo::AnimationInvalid)
			{
				//連撃
				//Animation::PlayAnimInfo* info = new Animation::PlayAnimInfo((UINT)_NextAttackAnimNo, 0.1f, 0.7f, 1);
				//_Anim->AddAnimationQueue(info);
				//アニメーションキューに追加。
				_Anim->AddAnimationQueue(new Animation::PlayAnimInfo((UINT)_NextAttackAnimNo, 0.1f, 0.7f, 1));
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
		ChangeState(State::Impact);

		// ダメージを与える処理
		int damage = _PlayerParam->ReciveDamage(*hitCollision->GetDamageInfo(), _Equipment->armor);
		_HPBar->SubValue(damage);
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
		attackvalue->Init(damage, hitCollision->GetDamageInfo()->isCritical, 1.5f, Vector3(0.0f, _Height, 0.0f),c);
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
	_MPBar = nullptr;
	for (auto &p : _AttackBoiceSound)
	{
		p = nullptr;
	}
	_AttackBoiceSound.clear();
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
			_HPBar->SetValue(_PlayerParam->GetParam(CharacterParameter::Param::HP));
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
		if (_MPBar) {
			_MPBar->SetValue(_PlayerParam->GetParam(CharacterParameter::Param::MP));
		}
		
		_HeelSound->Play(false);
		
		returnValue = true;
	}

	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
		int value = info->effectValue[idx];
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

			_PlayerParam->Buff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(value), info->time);
			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
			_BuffDebuffICon->BuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			_StatusUpSound->Play(false);

			returnValue = true;
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
			_PlayerParam->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
			_BuffDebuffICon->DebuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			_StatusDownSound->Play(false);

			returnValue = true;
		}
	}
	return returnValue;
}

/**
* エフェクト用更新.
*/
void Player::EffectUpdate()
{
	bool isBuffEffect = false;
	bool isDeBuffEffect = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::Param::DEX; idx++) {

		if (_PlayerParam->GetBuffParam((CharacterParameter::Param)idx) > 0.0f)
		{
			isBuffEffect = true;
		}
		else
		{
			_BuffDebuffICon->DeleteBuffIcon((CharacterParameter::Param)idx);
		}

		if (_PlayerParam->GetDebuffParam((CharacterParameter::Param)idx) > 0.0f)
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
	}

	//海に入るとダメージを食らう。
	static float time = 0.0f;
	time += Time::DeltaTime();
	//海の中の場合。
	//HPが0以上なら。
	//デバッグ時でない。
	//2秒間隔で。

	if (transform->GetLocalPosition().y < 48.5f && _PlayerParam->GetParam(CharacterParameter::HP) > 0 && _Debug == false)
	{
		if (fmod(time, 2.0f) >= 1.0f)
		{
			//最大HPの1割ずつ減る。
			_HPBar->SubValue(_PlayerParam->ReciveDamageThrough(_PlayerParam->GetMaxHP() * 0.1f));
			time = 0.0f;
		}
	}

}

//経験値テーブルをロード。
void Player::_LoadEXPTable()
{
	std::vector<std::unique_ptr<ExperiencePointTableInfo>> exptbaleinfo;
	Support::LoadCSVData<ExperiencePointTableInfo>("Asset/Data/PlayerParameter.csv", ExperiencePointTableInfoData, ARRAYSIZE(ExperiencePointTableInfoData), exptbaleinfo);
	
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
	_HPBar->Reset(_PlayerParam->GetParam(CharacterParameter::HP), _PlayerParam->GetParam(CharacterParameter::HP),true);
	//MPが上がったのでMPバーのMP設定しなおす。
	_MPBar->Reset(_PlayerParam->GetParam(CharacterParameter::MP), _PlayerParam->GetParam(CharacterParameter::MP),true);
	//レベルアップ時のイメージ表示。
	_LevelUpImage->Init();
	//レベルアップ時の音再生。
	_LevelUpSound->Play(false);
	//レベルアップエフェクト
	/*_ParticleEffect->LevelUpEffect();
	_ParticleEffect->SetLevelUPEffectFlag(true);*/
}

void Player::Speak()
{
	//NPCを取得
	vector<vector<NPC*>> npc;
	npc = INSTANCE(HistoryManager)->GetNPCList();
	//村
	for (auto village : npc)
	{
		//サイズが0ならコンティニュー
		if (village.size() == 0)
		{
			continue;
		}
		//NPC
		for (auto npc : village)
		{
			if (npc == nullptr)
			{
				continue;
			}
			//NPCからプレイヤーのベクトル
			Vector3 dir = npc->transform->GetPosition() - transform->GetPosition();
			float len = dir.Length();
			//範囲内かどうか
			if (npc->GetRadius() >= len)
			{
				//地面についていれば話しかけれる
				if (_CharacterController->IsOnGround())
				{
					//会話のためHPバーなどを消す。
					_HPBar->RenderDisable();
					_MPBar->RenderDisable();
					//話すフラグセット
					npc->SetIsSpeak(true);
					//プレイヤー話すフラグ設定
					//ジャンプしなくなる
					_Speak = true;
					//これ以上処理は続けない
					break;
				}
			}
			else
			{
				//話すNPCがいないので
				npc->SetIsSpeak(false);
				//話し終わると
				if (_Speak)
				{
					_HPBar->RenderEnable();
					_MPBar->RenderEnable();
					_Speak = false;
				}
			}
		}
		//話す状態ならもう回さない。
		if (_Speak)
		{
			break;
		}
	}
}

#ifdef _DEBUG
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
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Iron);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_3))
	{
		//所持リストに追加.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Oil);
	}
	//経験値を増やす。
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_1))
	{
		TakeDrop(100, 100);
	}
	int level = _PlayerParam->GetParam(CharacterParameter::LV);
	//レベルを上げる。
	if (level <= 95)
	{
		if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_2))
		{
			level += 5;
			_DebugLevel(level);
		}
	}
	if (level >= 6)
	{
		//レベル下げる。
		if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_3))
		{
			level -= 5;
			_DebugLevel(level);
		}
	}
}
void Player::_DebugLevel(int lv)
{
	// 次のレベルのパラメータを設定。
	_PlayerParam->ParamReset(_ParamTable[lv]);
	//HPが上がったのでHPバーのHP設定しなおす。
	_HPBar->Reset(_PlayerParam->GetParam(CharacterParameter::HP), _PlayerParam->GetParam(CharacterParameter::HP),true);
	//MPが上がったのでMPバーのMP設定しなおす。
	_MPBar->Reset(_PlayerParam->GetParam(CharacterParameter::MP), _PlayerParam->GetParam(CharacterParameter::MP),true);
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


void Player::AnimationEventControl()
{
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
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 120)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 1.5f, 1.5f), AttackCollision::CollisionMaster::Player, 0.5f, 0.0f, transform);
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
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 100)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 1.5f, 1.5f), AttackCollision::CollisionMaster::Player, 0.5f, 0.0f, transform);
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
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 120)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 1.5f, 1.5f), AttackCollision::CollisionMaster::Player, 0.5f, 0.0f, transform);
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
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 100)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 1.5f, 1.5f), AttackCollision::CollisionMaster::Player, 0.5f, 0.0f, transform);
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
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 150)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 1.5f, 1.5f), AttackCollision::CollisionMaster::Player, 0.5f, 0.0f, transform);
	}
}