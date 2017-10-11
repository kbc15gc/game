#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerState/PlayerStateRun.h"
#include "PlayerState/PlayerStateIdol.h"
#include "PlayerState/PlayerStateAttack.h"
#include "PlayerState\/PlayerStateDeath.h"
#include "PlayerState\PlayerStateStop.h"
#include "PlayerState\PlayerStateImpact.h"
#include "AttackCollision.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Component\CharacterParameter.h"
#include"GameObject\Component\ObjectRotation.h"
#include "GameObject\Component\ParameterBar.h"
#include "GameObject\Component\OutputData.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\HoldEquipment.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon.h"
#include "GameObject\TextImage\LevelUpImage.h"
#include "GameObject\Component\BuffDebuffICon.h"
#include "GameObject\Village\NPC.h"
#include "GameObject\Component\AnimationEvent.h"

class SkinModel;
class Animation;
class ParameterBar;
class ItemManager;
class ParticleEffect;
class BuffDebuffICon;
class HistoryManager;

namespace
{
	const int MAXLV = 100;

	struct ExperiencePointTableInfo
	{
		int ExperiencePoint;	//レベルアップ毎に必要な経験値
		int param[CharacterParameter::Param::MAX];	// 各種パラメータ。
	};

	const Support::DATARECORD ExperiencePointTableInfoData[] =
	{
		{ "ExperiencePoint",Support::DataTypeE::INT, offsetof(struct ExperiencePointTableInfo,ExperiencePoint),sizeof(int) },
		{ "param",	Support::DataTypeE::INTARRAY, offsetof(struct ExperiencePointTableInfo,param),	sizeof(ExperiencePointTableInfo::param) },
	};

}


namespace {
	//プレイヤーが何を装備しているのかをまとめた構造体(防具と武器)。
	struct PlayerEquipment
	{
		HoldArmor* armor = nullptr;		//防具。
		HoldWeapon* weapon = nullptr;	//武器。
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
		Impact,				//ダメージを受けた
		Death,				//死亡
		Stop,				//止まる
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
		AnimationImpact,							//ダメージを受けた
		AnimationDeath,								//死亡
		AnimationNum,								//アニメーションの数
	};
	// プレイヤーのパラメーター
	enum class Parameter {
		EXP = 0,
		HP,
		MP,
		ATK,
		DEF,
		DEX,
		AGI
	};
	//プレイヤー攻撃ボイス
	enum class AttackBoice
	{
		Start = 0,
		Attack1 = Start,
		Attack2,
		Attack3,
		End = Attack3
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
	void PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum = -1);
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
		_Model->SetEnable(is);
	}
	//プレイヤー解放
	void Releace();
	//敵が落とした物(経験値、お金)を受け取る。
	void TakeDrop(int dropexp, int money)
	{
		_nowEXP += dropexp;
		// お金はインベントリに格納。
		INSTANCE(Inventory)->AddPlayerMoney(money);
	}

	inline void HeelHP() {

	}

	int GetParam(CharacterParameter::Param param)
	{
		return _PlayerParam->GetParam(param);
	}
	int GetPigmentParam(CharacterParameter::Param param) {
		return _PlayerParam->GetPigmentParam(param);
	}
	int GetBuffParam(CharacterParameter::Param param) {
		return _PlayerParam->GetBuffParam(param);
	}
	int GetDebuffParam(CharacterParameter::Param param) {
		return _PlayerParam->GetDebuffParam(param);
	}

	int GetMaxHP() {
		return _PlayerParam->GetMaxHP();
	}
	int GetMaxMP() {
		return _PlayerParam->GetMaxMP();
	}
	// 現在の経験値量を取得。
	int GetExp() const {
		return _nowEXP;
	}
	// レベルアップに必要な経験値を取得。
	int GetNextLevelExp()const {
		return _EXPTable[_PlayerParam->GetParam(CharacterParameter::LV) - 1];
	}
	//プレイヤーをストップさせるフラグ。
	void PlayerStopEnable()
	{
		ChangeState(State::Stop);
	}
	//プレイヤーストップから戻す関数。
	//ステートアイドルに戻ります。
	void PlayerStopDisable()
	{
		ChangeState(State::Idol);
	}

	//プレイヤーに装備をセット(中でアイテムコードを見て武器か防具をセット)。
	void SetEquipment(HoldItemBase* equi);

	//ゲーム開始時にインベントリから装備している武具を探し装備し直す。
	void Re_SetEquipment();

	//プレイヤーの装備構造体を取得。
	inline PlayerEquipment* GetEquipment() {
		return _Equipment;
	}

	//プレイヤーのHpBarを取得。
	inline ParameterBar* GetPlayerHpBar() {
		return _HPBar;
	}

	//プレイヤーのMpBarを取得。
	inline ParameterBar* GetPlayerMpBar() {
		return _MPBar;
	}

	//バフデバフアイコンを取得。
	inline BuffDebuffICon* GetBuffDebuffICon() {
		return _BuffDebuffICon;
	}


	/**
	* アイテムが使用された.
	*/
	bool ItemEffect(Item::ItemInfo* info);
	
	/**
	* エフェクト用更新.
	*/
	void EffectUpdate();

	//アニメーションイベント
	void AnimationEventControl();
	//攻撃1
	void Attack1();
	//攻撃2
	void Attack2();
	//攻撃3
	void Attack3();
	//攻撃4
	void Attack4();
	//攻撃5
	void Attack5();

private:
	//プレイヤーがダメージを受ける処理
	void _Damage();
	//経験値テーブルをCSVからロード。
	void _LoadEXPTable();
	// レベルアップ。
	// 引数：		レベルアップに必要な経験値の値。
	void _LevelUP();
	//話す
	void Speak();

#ifdef _DEBUG
	//デバッグ機能
	void _DebugPlayer();
	//デバッグレベル
	void _DebugLevel(int lv);
#endif // _DEBUG

private:
	friend class PlayerStateAttack;
	friend class PlayerStateDeath;
	friend class PlayerStateIdol;
	friend class PlayerStateRun;
	friend class PlayerStateImpact;

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
	//プレイヤーステートイン朴
	PlayerStateImpact _ImpactState;
	//プレイヤーステートデス
	PlayerStateDeath _DeathState;
	//プレイヤーステートストップ
	PlayerStateStop _StopState;
	//プレイヤーのパラメーター
	CharacterParameter* _PlayerParam = nullptr;
	// 回転。
	ObjectRotation* _Rotation = nullptr;
	// HPバー。
	ParameterBar* _HPBar = nullptr;
	// MPバー。
	ParameterBar* _MPBar = nullptr;
#ifdef _DEBUG
	// デバッグ用データ出力コンポーネント。
	OutputData* _outputData = nullptr;
#endif
	//デバッグ
	bool _Debug = false;
	//レベルアップに必要な経験値のテーブル(LV - 1が添え字)。
	std::vector<int> _EXPTable;
	int _nowEXP = 0;	// 現在の経験値。
	// レベルごとのパラメーターテーブル。
	vector<vector<int>> _ParamTable = vector<vector<int>>(MAXLV,vector<int>(CharacterParameter::MAX,0));
	//プレイヤーの装備。
	PlayerEquipment* _Equipment = nullptr;
	//パーティクルエフェクト。
	ParticleEffect*	_ParticleEffect = nullptr;
	//バフデバフアイコン。
	BuffDebuffICon* _BuffDebuffICon = nullptr;
	//レベルアップイメージ
	LevelUpImage* _LevelUpImage;
	//歴史書
	HistoryManager* _HistoryManager = nullptr;
	//NPCと話すときジャンプしないため
	bool _Speak;
	//プレイヤーがダメージ受けた時のSE
	SoundSource* _DamageSound = nullptr;
	//レベルアップ時の音
	SoundSource* _LevelUpSound = nullptr;
	//回復サウンド
	SoundSource* _HeelSound = nullptr;
	//ステータスアップサウンド
	SoundSource* _StatusUpSound = nullptr;
	//ステータスダウンサウンド
	SoundSource* _StatusDownSound = nullptr;
	//プレイヤー死亡サウンド
	SoundSource* _DeathSound = nullptr;
	//攻撃時のSE
	SoundSource* _AttackSoound = nullptr;
	//攻撃ボイス
	vector<SoundSource*> _AttackBoiceSound;
	//攻撃ボイスENUM
	AttackBoice	_AttackBoice;

	//アニメーションイベント
	AnimationEventPlayer* _AnimationEventPlayer = nullptr;
};