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
#include "GameObject\Component\ParameterBar.h"
#include "GameObject\Component\OutputData.h"


class SkinModel;
class Animation;
class ParameterBar;
class HistoryBook;

namespace
{
	const int MAXLV = 100;

	struct ExperiencePointTableInfo
	{
		int ExperiencePoint;	//レベルアップ毎に必要な経験値
		int HP;					//レベルごとのHP
		int MP;					//レベルごとのMP
		int ATK;				//レベルごとのATK
		int DEF;				//レベルごとのDEF
		int DEX;				//レベルごとのDEX
		int AGI;				//レベルごとのAGI
	};

	const Support::DATARECORD ExperiencePointTableInfoData[] =
	{
		{ "ExperiencePoint",Support::DataTypeE::INT, offsetof(struct ExperiencePointTableInfo,ExperiencePoint),sizeof(int) },
		{ "HP", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,HP),sizeof(int)},
		{ "MP", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,MP),sizeof(int)},
		{ "ATK", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,ATK),sizeof(int)},
		{ "DEF", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,DEF),sizeof(int)},
		{ "DEX", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,DEX),sizeof(int)},
		{ "AGI", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,AGI),sizeof(int)},
	};
}

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
		AnimationAttack03,							//攻撃３
		AnimationAttack04,							//攻撃04
		AnimationAttack05,							//攻撃03
		AnimationAttackEnd = AnimationAttack05,
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

	// 自分が発生させたもの以外の攻撃コリジョンと衝突した瞬間呼ばれるコールバック。
	// ※引数は衝突した攻撃コリジョン。
	// ※処理が少ないうちはinlineのままでいいよ(だいたい3行以上の処理をするようになるまで)。
	inline virtual void HitAttackCollisionEnter(AttackCollision* hitCollision);

	// 自分が発生させたもの以外の攻撃コリジョンに衝突ている間呼ばれるコールバック。
	// ※引数は衝突した攻撃コリジョン。
	// ※処理が少ないうちはinlineのままでいいよ(だいたい3行以上の処理をするようになるまで)。
	inline virtual void HitAttackCollisionStay(AttackCollision* hitCollision) {
		OutputDebugString("Stay");
	}

	// 自分が発生させたもの以外の攻撃コリジョンとの衝突から外れたら呼ばれるコールバック。
	// ※引数は衝突した攻撃コリジョン。
	// ※処理が少ないうちはinlineのままでいいよ(だいたい3行以上の処理をするようになるまで)。
	inline virtual void HitAttackCollisionExit(AttackCollision* hitCollision) {
		OutputDebugString("Exit");
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
	//モデルを表示するかフラグをセット。
	void SetEnable(bool is)
	{
		_Model->enable = is;
	}
	//プレイヤー解放
	void Releace();
	//敵が落とした物(経験値、お金)を受け取る。
	void TakeDrop(int dropexp,int money)
	{
		_PlayerParam->AddParam(CharacterParameter::EXP, dropexp);
		_PlayerParam->AddParam(CharacterParameter::MONEY, money);
	}

	int* GetParamPt(CharacterParameter::Param param)
	{
		return _PlayerParam->GetParamPt(param);
	}

private:
	//プレイヤーがダメージを受ける処理
	void _Damage();
	//経験値テーブルをCSVからロード。
	void _LoadEXPTable();
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
	//最初のポジション
	Vector3 _StartPos;
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
	//レベルアップ時の音
	SoundSource* _LevelUP = nullptr;
	//プレイヤーのパラメーター
	CharacterParameter* _PlayerParam = nullptr;
	// 回転。
	ObjectRotation* _Rotation = nullptr;
	// HPバー。
	ParameterBar* _HPBar = nullptr;
	// MPバー。
	ParameterBar* _MPBar = nullptr;
	//ヒストリーブック
	HistoryBook* _HistoryBook = nullptr;
#ifdef _DEBUG
	// デバッグ用データ出力コンポーネント。
	OutputData* _outputData = nullptr;
#endif
	//デバッグ
	bool _Debug = false;

	//経験値テーブル
	std::vector<int> _EXPTable;
	//レベルごとのHP
	std::vector<int> _HPTable;
	//レベルごとのMP
	std::vector<int> _MPTable;
	//レベルごとのATK
	std::vector<int> _ATKTable;
	//レベルごとのDEF
	std::vector<int> _DEFTable;
	//レベルごとのDEX
	std::vector<int> _DEXTable;
	//レベルごとのAGI
	std::vector<int> _AGITable;
};