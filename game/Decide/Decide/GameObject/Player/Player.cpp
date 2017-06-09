#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include <string>
#include <sstream>

Player::Player(const char * name) :
	GameObject(name),
	//キャラクターコントローラーNULL
	_CharacterController(NULL),
	//重力設定
	_Gravity(-50.0f),
	//現在のステート
	_CurrentState(NULL),
	//走るステート
	_RunState(this),
	//アイドルステート
	_IdolState(this),
	//攻撃ステート
	_AttackState(this),
	//死亡ステート
	_DeathState(this)
{
}

Player::~Player()
{
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
	//高さ設定
	_Height = 1.5f;
	//半径設定
	_Radius = 0.5f;
	//カプセルコライダー作成
	coll->Create(_Radius, _Height);
	//スキンモデル作成
	SkinModelData* modeldata = new SkinModelData();
	//モデルデータ作成
	modeldata->CloneModelData(SkinModelManager::LoadModel("Player.X"), _Anim);
	//モデル設定
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, true);
	_Model->SetAllBlend(Color::white * 13);
	//キャラクターコントローラー初期化
	{
		_CharacterController->Init(this, transform, _Radius, _Height, Vector3(0.0f, _Height / 2, 0.0f), Collision_ID::PLAYER, coll, _Gravity);
		// 以下衝突を取りたい属性(横方向)を指定。
		{
			// ※テスト用(後で直してね)。
			_CharacterController->AttributeXZ_AllOff();	// 全衝突無視。
			//_CharacterController->AddAttributeXZ(Collision_ID::ATTACK);	// 攻撃コリジョンを追加。
		}
		// 以下衝突を取りたい属性(縦方向)を指定。
		{
			// ※テスト用(後で直してね)。
			_CharacterController->AttributeY_AllOn();	// 全衝突。
			_CharacterController->SubAttributeY(Collision_ID::ENEMY);	// エネミーを削除。
			_CharacterController->SubAttributeY(Collision_ID::ATTACK);
		}
		//キャラクターコントローラーの重力設定
		_CharacterController->SetGravity(_Gravity);
	}
	//HPのテキスト表示
	{
		_HPText = INSTANCE(GameObjectManager)->AddNew<TextObject>("HPText", _Priority);
		_HPText->Initialize(L"", 70.0f);
		_HPText->SetFormat((int)fbText::TextFormatE::CENTER | (int)fbText::TextFormatE::UP);
		_HPText->transform->SetLocalPosition(Vector3(1150, 630,0));
	}
	//MPのテキスト表示
	{
		_MPText = INSTANCE(GameObjectManager)->AddNew<TextObject>("MPText", _Priority);
		_MPText->Initialize(L"", 70.0f);
		_MPText->SetFormat((int)fbText::TextFormatE::CENTER | (int)fbText::TextFormatE::UP);
		_MPText->transform->SetLocalPosition(Vector3(1150, 680, 0));
	}
	//ダメージSE初期化
	_DamageSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DamageSE", 0);
	_DamageSE->Init("Asset/Sound/Damage_01.wav");
}

void Player::Start()
{
	//モデルにカメラ設定
	_Model->SetCamera(INSTANCE(GameObjectManager)->mainCamera);
	//モデルにライト設定
	_Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//アニメーションの終了時間設定
	_AnimationEndTime[(int)AnimationNo::AnimationIdol] = -1.0;			//アイドル
	_AnimationEndTime[(int)AnimationNo::AnimationWalk] = -1.0;			//歩く
	_AnimationEndTime[(int)AnimationNo::AnimationRun] = 0.68;			//走る
	_AnimationEndTime[(int)AnimationNo::AnimationAttack01] = -1.0f;		//攻撃１
	_AnimationEndTime[(int)AnimationNo::AnimationAttack02] = -1.0;		//攻撃２
	_AnimationEndTime[(int)AnimationNo::AnimationDeath] = -1.0;			//死亡
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
	transform->SetLocalPosition(Vector3(374, 69, -1275));
	//移動速度初期化
	_MoveSpeed = Vector3::zero;
	//初期プレイヤー状態（待機）
	_State = State::Idol;
	//攻撃アニメーションステートの初期化
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;
	//プレイヤーのパラメーター初期化。
	_PlayerParam->ParamInit(100, 50, 5, 4, 3, 1);
	//レベル初期化
	_Level = 1;
}

void Player::Update()
{
	if (_CurrentState != NULL)
	{
		//ステートアップデート
		_CurrentState->Update();
	}
	//HPのテキストを表示更新
	string hp = to_string(_PlayerParam->_HP);
	string mp = to_string(_PlayerParam->_MP);
	_HPText->SetString(hp.data());
	_MPText->SetString(mp.data());
	//ライフが0になると死亡する。

	//アニメーションコントロール
	AnimationControl();
	//トランスフォーム更新
	transform->UpdateTransform();
}

void Player::ChangeState(State nextstate)
{
	if (_CurrentState != NULL) {
		//現在のステートを抜けるときの処理
		_CurrentState->Leave();
	}
	switch (nextstate)
	{
		//待機状態
	case State::Idol:
		_CurrentState = &_IdolState;
		break;
		//走る状態
	case State::Run:
		_CurrentState = &_RunState;
		break;
		//攻撃状態
	case State::Attack:
		_CurrentState = &_AttackState;
		break;
		//死亡状態
	case State::Death:
		_CurrentState = &_DeathState;
		//デフォルト
	default:
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
	//ジャンプアニメーション
	if (_CharacterController->IsJump())
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
		else if(_State == State::Idol)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
		}
		//アタックアニメーション
		else if (_State == State::Attack)
		{
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
				PlayAnimation(_NextAttackAnimNo, 0.1f, 1);
				_NowAttackAnimNo = _NextAttackAnimNo;
				_NextAttackAnimNo = AnimationNo::AnimationInvalid;
			}

			
		}
		//死亡アニメーション
		else if (_State == State::Death)
		{
			PlayAnimation(AnimationNo::AnimationDeath, 0.1f, 1);
		}
	}
}
