#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include <string>
#include <sstream>
#include "GameObject\Component\ParameterBar.h"
#include "GameObject\SplitSpace.h"

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
	_DeathState(this),
	//デバッグか
	_Debug(false)
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
	//回転コンポーネント
	_Rotation = AddComponent<ObjectRotation>();
	// HPバー。
	_HPBar = AddComponent<ParameterBar>();
	// MPバー。
	_MPBar = AddComponent<ParameterBar>();
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
	_CharacterController->Init(this, transform, _Radius, _Height, Vector3(0.0f, _Height / 2, 0.0f), Collision_ID::PLAYER, coll, _Gravity);
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
	_PlayerParam->ParamInit(100, 100, 50, 50, 5, 3, 3, 1);
	// HPのバーを表示。
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Green);
		_HPBar->Create(Colors, _PlayerParam->GetParam(CharacterParameter::MAXHP), _PlayerParam->GetParam(CharacterParameter::HP), true, NULL, Vector3(1110, 660, 0));
	}
	// MPのバーを表示。
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Blue); //175.0f, 21.9f, 0.0f
		_MPBar->Create(Colors, _PlayerParam->GetParam(CharacterParameter::MAXMP), _PlayerParam->GetParam(CharacterParameter::MP), true, _HPBar->GetTransform(), Vector3(0.0f, 40.0f, 0.0f), Vector2(1.0f, 1.0f));
	}
	//攻撃の値のテクストオブジェクト。
	_AttackValue = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttackValueText", _Priority);
	_AttackValue->Initialize(L"", 70.0f);
	_AttackValue->SetFormat((int)fbText::TextFormatE::CENTER | (int)fbText::TextFormatE::UP);
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
	_AnimationEndTime[(int)AnimationNo::AnimationIdol] = -1.0f;			//アイドル
	_AnimationEndTime[(int)AnimationNo::AnimationWalk] = -1.0f;			//歩く
	_AnimationEndTime[(int)AnimationNo::AnimationRun] = 0.68f;			//走る
	_AnimationEndTime[(int)AnimationNo::AnimationJump] = -1.0f;			//ジャンプ
	_AnimationEndTime[(int)AnimationNo::AnimationAttack01] = -1.0f;		//攻撃1
	_AnimationEndTime[(int)AnimationNo::AnimationAttack02] = -1.0f;		//攻撃2
	_AnimationEndTime[(int)AnimationNo::AnimationAttack03] = -1.0f;		//攻撃3
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
	transform->SetLocalPosition(Vector3(378, 69, -1286));
	//移動速度初期化
	_MoveSpeed = Vector3::zero;
	//攻撃アニメーションステートの初期化
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;
	//レベル初期化
	_Level = 1;

	//// とりあえずテスト。
	//int attr = static_cast<int>(fbCollisionAttributeE::ALL) & ~(Collision_ID::PLAYER) & ~(Collision_ID::SPACE) & ~(Collision_ID::ATTACK);
	//// 空間分割コリジョン生成。
	//INSTANCE(GameObjectManager)->AddNew<SplitSpace>("SplitSpace", 1)->Split(GetComponent<SkinModel>()->GetModelData(), *transform, 3, 1, 3, attr);

	// とりあえずテスト。
	// 空間分割コリジョン生成。
	//INSTANCE(GameObjectManager)->AddNew<SplitSpace>("SplitSpace", 1)->Split(_Model->GetModelData(), *transform, 3, 4, 5);

}

void Player::Update()
{
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
	//死亡アニメーション
	if (_State == State::Death)
	{
		PlayAnimation(AnimationNo::AnimationDeath, 0.1f, 1);
		return;
	}
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
		_HPBar->SubValue(_PlayerParam->ReciveDamage(hitCollision->GetDamage()));
		_DamageSE->Play(false);//ダメージを受けたときのSE
	}
}

void Player::Releace()
{
	_CharacterController = nullptr;
	_DamageSE = nullptr;
	_AttackValue = nullptr;
	_Rotation = nullptr;
	_PlayerParam = nullptr;
	_CurrentState = nullptr;
	_HPBar = nullptr;
	_MPBar = nullptr;
}

void Player::_Damage()
{
	//死亡ステート以外の時。
	//ライフが0になると死亡する。
	if (_PlayerParam->GetParam(CharacterParameter::HP) <= 0 && _State != State::Death)
	{
		ChangeState(State::Death);
	}
	/*
	*テスト用として、海の中に入ると、じわじわとダメージを受ける。
	*/
	if (transform->GetLocalPosition().y < 48.5f 
		&& _PlayerParam->GetParam(CharacterParameter::HP) > 0 &&_Debug == false)
	{
		_PlayerParam->SubParam(CharacterParameter::HP, 2);
		_HPBar->SubValue(2);
	}
}
