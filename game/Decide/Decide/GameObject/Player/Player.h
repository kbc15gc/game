#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerState/PlayerStateRun.h"
#include "PlayerState/PlayerStateIdol.h"
#include "PlayerState/PlayerStateAttack.h"
#include "PlayerState\/PlayerStateDeath.h"
#include "AttackCollision.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Component\CharacterParameter.h"
#include"GameObject\Component\ObjectRotation.h"

class SkinModel;
class Animation;
class ParameterBar;

class Player : public GameObject
{
public:
	//プレイヤーのステート
	enum class State
	{
		Idol = 0,			//アイドル
		Run,				//走る
		Attack,				//攻撃
		Death,				//死亡
		StateNum,
	};
	//アニメーションのナンバー
	enum class AnimationNo
	{
		AnimationInvalid = -1,						//無効
		AnimationIdol,								//アイドル	
		AnimationWalk,								//歩き
		AnimationRun,								//走る
		AnimationJump,								//ジャンプ
		AnimationAttackStart,			
		AnimationAttack01 = AnimationAttackStart,	//攻撃01
		AnimationAttack02,							//攻撃02
		AnimationAttack03,							//攻撃03
		AnimationAttackEnd = AnimationAttack03,
		AnimationDeath,								//死亡
		AnimationNum,								//アニメーションの数
	};
	Player(const char* name);
	~Player();
	void Awake()override;
	void Start()override;
	void Update()override;
	//状態変更
	// nextstate 次のステート
	void ChangeState(State nextstate);
	//アニメーション再生
	// animno アニメーションのナンバー
	// interpolatetime 補間時間
	// loopnum ループ回数 (デフォルトは-1)
	void PlayAnimation(AnimationNo animno, float interpolatetime , int loopnum = -1);
	//アニメーションコントロール
	void AnimationControl();

	// 自分が発生させたもの以外の攻撃コリジョンに衝突したら呼ばれるコールバック。
	// ※引数は衝突した攻撃コリジョン。
	// ※処理が少ないうちはinlineのままでいいよ(だいたい3行以上の処理をするようになるまで)。
	// エネミーが作った攻撃。
	void HitAttackCollision(AttackCollision* hitCollision) {
		OutputDebugString("とりあえずブレイクポイント設定できるようにするね。");
		if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Enemy)
		{
			_PlayerParam->SubParam(CharacterParameter::Param::HP,1);	//ダメージを受ける(とりあえず、ライフを１ずつ減らす)
			_DamageSE->Play(false);//ダメージを受けたときのSE
		}
	}

	//セットステート
	void SetState(State state)
	{
		_State = state;
	}
	//ゲットステート
	State GetState()
	{
		return _State;
	}
	//キャラクターコントローラーゲット
	CCharacterController& GetCharaCon() const
	{
		return*_CharacterController;
	}
private:
	friend class PlayerStateAttack;
	friend class PlayerStateDeath;
	friend class PlayerStateIdol;
	friend class PlayerStateRun;

	//コンポーネントとかアドレスの保持が必要なものたち
	//モデル
	SkinModel* _Model = nullptr;
	//アニメーション
	Animation* _Anim = nullptr;
	//高さ
	float _Height;
	//半径
	float _Radius;
	//最終的な移動量
	Vector3 _MoveSpeed;
	//進行
	Vector3 _Dir;
	//重力
	float _Gravity;
	//プレイヤーの状態
	State _State;
	//アニメーションの終了時間
	double _AnimationEndTime[(int)AnimationNo::AnimationNum];
	//キャラクターコントローラー
	CCharacterController* _CharacterController = nullptr;
	//現在のプレイヤーのステート
	PlayerState*	_CurrentState = nullptr;
	//現在の攻撃アニメーションステート
	AnimationNo	_NowAttackAnimNo;
	//次の攻撃アニメーションステート
	AnimationNo _NextAttackAnimNo;
	//プレイヤーステートラン
	PlayerStateRun	_RunState;
	//プレイヤーステートアイドル
	PlayerStateIdol	_IdolState;
	//プレイヤーステートアタック
	PlayerStateAttack _AttackState;
	//プレイヤーステートデス
	PlayerStateDeath _DeathState;
	//プレイヤーがダメージ受けた時のSE
	SoundSource* _DamageSE = nullptr;
	//プレイヤーのパラメーター
	CharacterParameter* _PlayerParam = nullptr;
	//プレイヤーのレベル
	int _Level;
	//HPのテキスト表示
	TextObject* _HPText;
	//MPのテキスト表示
	TextObject* _MPText;
	// 回転。
	ObjectRotation* _Rotation = nullptr;
	// HPバー。
	ParameterBar* _HPBar = nullptr;
	// MPバー。
	ParameterBar* _MPBar = nullptr;
};