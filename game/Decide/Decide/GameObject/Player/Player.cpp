#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include <string>
#include <sstream>
#include "GameObject\SplitSpace.h"
#include "GameObject\AttackValue2D.h"
#include "..\History\HistoryManager.h"

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
	_Gravity(-55.0f),
	//現在のステート
	_CurrentState(NULL),
	//走るステート
	_RunState(this),
	//アイドルステート
	_IdolState(this),
	//攻撃ステート
	_AttackState(this),
	//死亡ステート
	_DeathState(this),
	//ストップステート
	_StopState(this),
	//デバッグか
	_Debug(false)
{
	//経験値テーブルをロード
	_LoadEXPTable();
}

Player::~Player()
{
	char text[256];
	sprintf(text, "~Player address %x\n", *this);
	OutputDebugString(text);
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
	//高さ設定
	_Height = 1.3f;
	//半径設定
	_Radius = 0.1f;
	//カプセルコライダー作成
	coll->Create(_Radius, _Height);
	//スキンモデル作成
	SkinModelData* modeldata = new SkinModelData();
	//モデルデータ作成
	modeldata->CloneModelData(SkinModelManager::LoadModel("Player.X"), _Anim);
	//モデル設定
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, true);
	//_Model->SetAllBlend(Color::white * 13);
	
	//キャラクターコントローラー初期化
	_CharacterController->Init(this, transform, Vector3(0.0f,_Height * 0.5f + _Radius,0.0f), Collision_ID::PLAYER, coll, _Gravity);
	// 以下衝突を取りたい属性(横方向)を指定。
	_CharacterController->AttributeXZ_AllOff();	// 全衝突無視。
	_CharacterController->AddAttributeXZ(Collision_ID::GROUND);		// 地面コリジョンを追加。
	_CharacterController->AddAttributeXZ(Collision_ID::ENEMY);		// 敵のコリジョン追加。
	_CharacterController->AddAttributeXZ(Collision_ID::BOSS);		// ボスのコリジョン追加。
	_CharacterController->AddAttributeXZ(Collision_ID::BUILDING);	// 建物のコリジョン追加。
	// 以下衝突を取りたい属性(縦方向)を指定。
	_CharacterController->AttributeY_AllOn();	// 全衝突。
	_CharacterController->SubAttributeY(Collision_ID::ENEMY);	// エネミーを削除。
	_CharacterController->SubAttributeY(Collision_ID::BOSS);	// ボスを削除。
	_CharacterController->SubAttributeY(Collision_ID::ATTACK);	//攻撃コリジョン削除。
	//キャラクターコントローラーの重力設定
	_CharacterController->SetGravity(_Gravity);

	//プレイヤーのパラメーター初期化。
	_PlayerParam->ParamReset(_ParamTable[0]);
	
	// HPのバーを表示。
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Green);
		_HPBar->Create(Colors, _PlayerParam->GetMaxHP(), _PlayerParam->GetParam(CharacterParameter::HP), true, NULL, Vector3(1110.0f, 660.0f, 0.0f));
	}
	// MPのバーを表示。
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Blue); //175.0f, 21.9f, 0.0f
		_MPBar->Create(Colors, _PlayerParam->GetMaxMP(), _PlayerParam->GetParam(CharacterParameter::MP), true, _HPBar->GetTransform(), Vector3(0.0f, 40.0f, 0.0f), Vector2(1.0f, 1.0f));
	}
	//ダメージSE初期化
	_DamageSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DamageSE", 0);
	_DamageSE->Init("Asset/Sound/Damage_01.wav");
	//レベルアップ音初期化
	_LevelUP_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("LevelUP", 0);
	_LevelUP_SE->Init("Asset/Sound/levelup.wav");
	_LevelUP_SE->SetVolume(2.0f);
#ifdef _DEBUG

	_outputData = AddComponent<OutputData>();
#endif
	_Equipment = new PlayerEquipment;
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
	_AnimationEndTime[(int)AnimationNo::AnimationAttack04] = -1.0f;		//攻撃3
	_AnimationEndTime[(int)AnimationNo::AnimationAttack05] = -1.0f;		//攻撃3
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
	_StartPos = Vector3(-1056.0f, 69.0f, -1947.0f);
	transform->SetLocalPosition(_StartPos);
	//移動速度初期化
	_MoveSpeed = Vector3::zero;
	//攻撃アニメーションステートの初期化
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;

	//レベルアップ時のスプライト初期化
	/*{
		_LevelUpSprite = AddComponent<Sprite>();
		_LevelUpSprite->SetTexture(LOADTEXTURE("levelup.png"));
		_LevelUpSprite->SetEnable(true);
		_LevelUpSprite->SetPivot(Vector2(0.5f, 1.0f));
	}*/

}

void Player::Update()
{

#ifdef _DEBUG
	
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
	static int level = _PlayerParam->GetParam(CharacterParameter::LV);
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
#endif // DEBUG

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
	if (_MPBar != nullptr)
	{
		//MPバーの更新
		_MPBar->Update();
	}
	//レベルアップするか。
	if (_EXPTable.size() > 0 &&
		_nowEXP >= _EXPTable[_PlayerParam->GetParam(CharacterParameter::LV) - 1])
	{
		_LevelUP();
	}
	//ダメージを受ける処理。
	_Damage();
	//アニメーションコントロール
	AnimationControl();
	// ※トランスフォームを更新すると内部でオイラー角からクォータニオンを作成する処理が呼ばれる。
	// ※オイラー角を使用せず直接クォータニオンを触る場合はこの処理を呼ぶとオイラー角の値で作成されたクォータニオンで上書きされる。
	// ※都合が悪いのでとりあえずコメントアウト。
		////トランスフォーム更新
		//transform->UpdateTransform();
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
	//ジャンプアニメーション
	//ストップじゃないならジャンプする。
	if (_CharacterController->IsJump() && _State != State::Stop)
	{
		PlayAnimation(AnimationNo::AnimationJump, 0.1f);
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
	if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Enemy && _PlayerParam->GetParam(CharacterParameter::HP) > 0)
	{
#ifdef _DEBUG
		if (_Equipment == nullptr)
		{
			// 装備用の構造体がNull。
			abort();
		}
#endif
		// ダメージを与える処理
		int damage = _PlayerParam->ReciveDamage(hitCollision->GetDamage(), hitCollision->GetIsMagic(), _Equipment->armor);
		_HPBar->SubValue(damage);
		_DamageSE->Play(false);//ダメージを受けたときのSE
		AttackValue2D* attackvalue = INSTANCE(GameObjectManager)->AddNew<AttackValue2D>("AttackValue2D", 5);
		attackvalue->Init(damage, 1.5f, Vector3(0.0f, _Height, 0.0f));
		attackvalue->transform->SetParent(transform);
	}
}

//解放。
void Player::Releace()
{
	_CharacterController = nullptr;
	_DamageSE = nullptr;
	_Rotation = nullptr;
	_PlayerParam = nullptr;
	_CurrentState = nullptr;
	_HPBar = nullptr;
	_MPBar = nullptr;
}

//攻撃を受けたとき
void Player::_Damage()
{
	//死亡ステート以外の時。
	//ライフが0になると死亡する。
	if (_PlayerParam->GetDeathFlg() == true && _State != State::Death)
	{
		ChangeState(State::Death);
	}
	//海に入るとダメージを食らう。
	if (transform->GetLocalPosition().y < 48.5f 
		&& _PlayerParam->GetParam(CharacterParameter::HP) > 0 && _Debug == false)
	{
		_HPBar->SubValue(_PlayerParam->ReciveDamage(CharacterParameter::HP, Oboreru * Time::DeltaTime()));
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
	_HPBar->Reset(_PlayerParam->GetParam(CharacterParameter::HP), _PlayerParam->GetParam(CharacterParameter::HP));
	//MPが上がったのでMPバーのMP設定しなおす。
	_MPBar->Reset(_PlayerParam->GetParam(CharacterParameter::MP), _PlayerParam->GetParam(CharacterParameter::MP));
	//レベルアップ時の音再生。
	_LevelUP_SE->Play(false);
}
#ifdef _DEBUG
void Player::_DebugLevel(int lv)
{
	// 次のレベルのパラメータを設定。
	_PlayerParam->ParamReset(_ParamTable[lv]);
	//HPが上がったのでHPバーのHP設定しなおす。
	_HPBar->Reset(_PlayerParam->GetParam(CharacterParameter::HP), _PlayerParam->GetParam(CharacterParameter::HP));
	//MPが上がったのでMPバーのMP設定しなおす。
	_MPBar->Reset(_PlayerParam->GetParam(CharacterParameter::MP), _PlayerParam->GetParam(CharacterParameter::MP));
}
#endif // _DEBUG

