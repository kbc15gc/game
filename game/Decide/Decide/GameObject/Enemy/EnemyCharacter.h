#pragma once

#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "SearchViewAngle.h"
#include "../Component/ObjectRotation.h"
#include "fbEngine\CharacterController.h"
#include "GameObject\Component\CharacterParameter.h"
#include "GameObject\Component\ParameterBar.h"
#include "GameObject\Component\ObjectSpawn.h"
#include "GameObject\Component\AnimationEvent.h"

class SkinModel;
class Animation;
class EnemyState;
class EnemyAttack;

// 基底クラス。
// エネミーのキャラクター。
class EnemyCharacter :
	public GameObject
{
public:
	// 自分がどの種類のエネミーか。
	// ※このクラスを継承して新種エネミーを作成したらここに種別を追加すること。
	enum class EnemyType{Prot = 0,Drarian};

	// ステート配列の添え字を列挙。
	// ※継承先で使用するものも含めてすべてのステートをここに列挙する。
	// ※追加する際はこのクラスの_BuildState関数に記述した順番になっているかをしっかり確認すること。
	// ※ステートを追加した際はここだけでなくこのクラス内の_BuildState関数も更新すること。
	enum class State { Wandering = 0,Discovery, Threat, StartAttack, Attack ,Wait ,Translation, Fall,Damage,Death };

	// アニメーションデータ配列の添え字。
	// ※0番なら待機アニメーション、1番なら歩くアニメーション。
	// ※この列挙子を添え字として、継承先のクラスでアニメーショ番号のテーブルを作成する。
	enum class AnimationType { None = -1,Idle = 0, Walk, Dash, Threat, Attack, Fall,Damage, Death,Max };

	// アニメーションデータ構造体。
	struct AnimationData {
		int No = -1;	// アニメーション番号。
		float Time;	// 再生時間。
	};

private:
	// このクラスで使用するコンポーネント。
	// ※コンポーネントは勝手に削除されるため、このクラスでは削除しない。
	// ※このクラス独自のメンバ変数と住み分けを行うために構造体化する。
	struct Components {
		SkinModel* Model = nullptr;	// このオブジェクトのモデル。
		Animation* Animation = nullptr;	// このオブジェクトのアニメーション。
		Collider* Collider = nullptr;	// コリジョン形状。
		CCharacterController* CharacterController = nullptr;		// キャラクターコントローラ。
		ObjectRotation* RotationAction = nullptr;	// オブジェクトを回転させるクラス。
		CharacterParameter* Parameter = nullptr;//エネミーのパラメーター。
		ParameterBar* HPBar = nullptr;			// ゲージHP用。
		ObjectSpawn* Spawner = nullptr;		// リスポーン設定できる。
		AnimationEvent* AnimationEvent = nullptr;	// アニメーションにイベントを設定できる関数。
	};

public:
	// 引数はオブジェクトの名前。
	EnemyCharacter(const char* name);
	~EnemyCharacter();

	// こちらは基本的に継承しない。
	// ※継承先独自の実装は、privateにある_AwakeSubClass関数を継承して実装すること。
	void Awake()override;

	// こちらは基本的に継承しない。
	// ※継承先独自の実装は、privateにある_StartSubClass関数を継承して実装すること。
	void Start()override;

	// こちらは基本的に継承しない。
	// ※継承先独自の実装は、privateにある_UpdateSubClass関数を継承して実装すること。
	void Update()override;

	// こちらは基本的に継承しない。
	// ※継承先独自の実装は、privateにある_LateUpdateSubClass関数を継承して実装すること。
	void LateUpdate()override;
	

	// エネミーを指定したオブジェクトに向かせる処理(補間なし)。
	// 引数：	見たいオブジェクト。
	inline void LookAtObject(const GameObject& Object) {
		_MyComponent.RotationAction->RotationToObject_XZ(Object);
	}

	// エネミーを指定した方向に向かせる処理(補間なし)。
	// 引数：	向きベクトル。
	inline void LookAtDirection(const Vector3& dir) {
		_MyComponent.RotationAction->RotationToDirection_XZ(dir);
	}

	// エネミーを指定した軸と角度を用いて回転する関数。
	// 引数：	軸(単位ベクトル)。
	//			回転角度(ラジアン)。
	inline void RotationAxis(const Vector3& axis, float angle) {
		_MyComponent.RotationAction->RotationAxis(axis, angle);
	}

	// 攻撃処理を決定する関数。
	// 戻り値：	実行したい攻撃処理クラスのポインタ。
	// ※継承先で実装。
	// ※複数攻撃パターンがある場合は攻撃処理を分岐させる。
	virtual EnemyAttack* AttackSelect() = 0;

	// エネミーのアニメーション再生関数(ループ)。
	// 引数：	アニメーションタイプ(テーブルのほう)。
	//			補間時間。
	inline void PlayAnimation_Loop(const AnimationType animationType, const float interpolateTime) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(animationType)].No, interpolateTime);
	}

	// エネミーのアニメーション再生関数(指定回数ループ)。
	// 引数：	アニメーションタイプ(テーブルのほう)。
	//			補間時間。
	//			ループ回数。
	inline void PlayAnimation(const AnimationType animationType, const float interpolateTime, const int loopCount = 1) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(animationType)].No, interpolateTime, loopCount);
	}

	// エネミーのアニメーション再生関数(指定回数ループ)。
	// 引数：	アニメーションタイプ(モデルごとのアニメーション番号)。
	//			補間時間。
	//			ループ回数。
	inline void PlayAnimation_OriginIndex(const int animationNum, const float interpolateTime, const int loopCount = 1) {
		_MyComponent.Animation->PlayAnimation(animationNum, interpolateTime, loopCount);
	}

	// エネミーがアニメーションを再生しているか。
	inline bool GetIsPlaying() {
		return _MyComponent.Animation->GetPlaying();
	}

	// エネミーが徘徊範囲外に出たか判定。
	bool IsOutsideWandering();

	// 引数の値を加算した結果、エネミーが徘徊範囲外に出たか判定。
	bool IsOutsideWandering(const Vector3& Add);

	// 自分が発生させたもの以外の攻撃コリジョンと衝突した瞬間呼ばれるコールバック。
	// ※引数は衝突した攻撃コリジョン。
	// ※処理が少ないうちはinlineのままでいいよ(だいたい3行以上の処理をするようになるまで)。
	inline virtual void HitAttackCollisionEnter(AttackCollision* hitCollision) {
		if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Player)
		{
			this->GiveDamage(hitCollision->GetDamage());
		}
	}

	// 自分が発生させたもの以外の攻撃コリジョンに衝突ている間呼ばれるコールバック。
	// ※引数は衝突した攻撃コリジョン。
	// ※処理が少ないうちはinlineのままでいいよ(だいたい3行以上の処理をするようになるまで)。
	inline virtual void HitAttackCollisionStay(AttackCollision* hitCollision) {

	}

	// 自分が発生させたもの以外の攻撃コリジョンとの衝突から外れたら呼ばれるコールバック。
	// ※引数は衝突した攻撃コリジョン。
	// ※処理が少ないうちはinlineのままでいいよ(だいたい3行以上の処理をするようになるまで)。
	inline virtual void HitAttackCollisionExit(AttackCollision* hitCollision) {

	}


	// ステート処理中にステートを割り込みで切り替える関数。
	// ※外部やステート内からステート切り替えを行おうとする場合はこちらを呼ぶ。
	inline void ChangeStateRequest(State next) {
		_ChangeState(next);
	}

	// 視野角判定関数。
	// 視野角判定を行うステートから呼び出す。
	void SearchView();

	// 全パラメーター設定。
	// 引数：	HPバーに設定する色(重ねる場合は先に追加したものから表示される)。
	//			HP。
	//			HP最大値。
	//			MP。
	//			MP最大値。
	//			攻撃力。
	//			防御力。
	//			命中力。
	//			敏捷力。
	inline void SetParamAll(const vector<BarColor>& color,int hp, int maxhp, int mp, int maxmp, int atk, int def, int dex, int agi)const  {
		_MyComponent.Parameter->ParamInit(hp, maxhp, mp, maxmp, atk, def, dex, agi);
		_MyComponent.HPBar->Create(color, _MyComponent.Parameter->GetParam(CharacterParameter::Param::MAXHP), _MyComponent.Parameter->GetParam(CharacterParameter::Param::MAXHP), false, transform, Vector3(0.0f, 2.0f, 0.0f), Vector2(0.5f, 0.5f), false);
	}
	// 全パラメーター設定。
	// 引数：	HPバーに設定する色(重ねる場合は先に追加したものから表示される)。
	//			各種パラメーター。
	inline void SetParamAll(const vector<BarColor>& color,int param[CharacterParameter::Param::MAX]) const{
		_MyComponent.Parameter->ParamInit(param);
		_MyComponent.HPBar->Create(color, _MyComponent.Parameter->GetParam(CharacterParameter::Param::MAXHP), _MyComponent.Parameter->GetParam(CharacterParameter::Param::MAXHP), false, transform, Vector3(0.0f, 2.0f, 0.0f), Vector2(0.5f, 0.5f), false);
	}
	// 全パラメーター設定。
	// 引数：	HPバーに設定する色(重ねる場合は先に追加したものから表示される)。
	//			各種パラメーター。
	inline void SetParamAll(const vector<BarColor>& color, const vector<int>& param) const {
		_MyComponent.Parameter->ParamInit(param);
		_MyComponent.HPBar->Create(color, _MyComponent.Parameter->GetParam(CharacterParameter::Param::MAXHP), _MyComponent.Parameter->GetParam(CharacterParameter::Param::MAXHP), false, transform, Vector3(0.0f, 2.0f, 0.0f), Vector2(0.5f, 0.5f), false);
	}

	// エネミーにダメージを与える処理。
	// 引数：	ダメージ量。
	void GiveDamage(float damage);

	// モデルファイルのパスを設定。
	inline void SetFileName(const char* name) {
		if (strlen(name) >= FILENAME_MAX) {
			// ファイル名文字数オーバー。
			abort();
		}
		strcpy(_FileName, name);
	}

	// ステート配列を読み取り専用で返す関数。
	inline const vector<unique_ptr<EnemyState>>& GetMyState() const {
		return _MyState;
	}

	// 移動量を設定。
	inline void SetMoveSpeed(const Vector3& speed) {
		_MoveSpeed = speed;
	}

	// 移動量加算。
	inline void AddMoveSpeed(const Vector3& speed) {
		_MoveSpeed = _MoveSpeed + speed;
	}

	// 視野角設定(度)。
	inline void SetViewAngle(float angle) {
		_ViewAngle = angle;
	}

	// 見える距離設定。
	inline void SetViewRange(float range) {
		_ViewRange = range;
	}

	// 攻撃可能範囲取得。
	inline float GetAttackRange()const {
		return _AttackRange;
	}

	// 徘徊範囲返却。
	inline float GetWanderingRange()const {
		return _WanderingRange;
	}

	// 初期位置から現在の位置の移動量を返却(Y成分無視)。
	inline float GetInitToNowPosRange_XZ()const {
		Vector3 vec(_InitPos - transform->GetPosition());
		vec.y = 0.0f;
		return vec.Length();
	}

	// 初期位置から現在位置へのベクトルを返却。
	inline const Vector3& GetInitToNowPosVec() const {
		return transform->GetPosition() - _InitPos;
	}

	// 初期位置取得。
	inline const Vector3& GetInitPos()const {
		return _InitPos;
	}

	// 現在のステートを取得。
	inline EnemyState* GetNowState()const {
		return _NowState;
	}

	inline bool GetIsGround() const {
		return _MyComponent.CharacterController->IsOnGround();
	}

	inline ObjectSpawn* GetSpawner()const {
		return _MyComponent.Spawner;
	}

	inline float GetWalkSpeed()const {
		return _walkSpeed;
	}

	inline float GetDashSpeed()const {
		return _walkSpeed * 5.0f;
	}

	inline const AnimationData& GetAnimationData(AnimationType type)const {
		return _AnimationData[static_cast<int>(type)];
	}

protected:
	// ステート切り替え関数。
	void _ChangeState(State next);

	// アニメーションタイプにアニメーションデータを関連付ける関数。
	// 引数：	アニメーションタイプの列挙子。
	//			第1引数に関連付けたいアニメーションデータ。
	// ※この関数に渡されたアニメーションの終了時間が設定されます。
	inline void _ConfigAnimationType(AnimationType Type, const AnimationData& Data) {
		_AnimationData[static_cast<unsigned int>(Type)] = Data;
		// アニメーションコンポーネントにアニメーションの終了時間設定。
		_MyComponent.Animation->SetAnimationEndTime(Data.No,Data.Time);
	}

	// 現在のステートの処理が終了したときに呼ばれるコールバック関数。
	// 引数は終了したステートのタイプ。
	// ※この関数は各ステートが自発的に終了した場合にのみ呼び出される。
	// ※このクラスのChangeStateRequestで終了した場合は呼ばれない。
	// ※ローカルステートの終了時にも呼ばれない。
	virtual void _EndNowStateCallback(State EndStateType) {};

private:

	// 継承先での更新処理。
	// ※継承先で上書きして使用。
	virtual void _AwakeSubClass() = 0;

	// 継承先でのシーン切り替えによる初期化処理。
	// ※継承先で上書きして使用。
	virtual void _StartSubClass() {};

	// 継承先での更新処理。
	// ※継承先で上書きして使用。
	virtual void _UpdateSubClass() = 0;

	// 継承先での更新処理終了後に呼ばれる処理。
	// ※継承先で上書きして使用。
	virtual void _LateUpdateSubClass() {};


	// このクラスが使用するコンポーネントを追加する関数。
	virtual void _BuildMyComponents();

	// コリジョン定義関数。
	// コリジョンの形状やパラメータを設定する関数。
	// ※処理自体は継承先に委譲。
	virtual void _ConfigCollision() = 0;

	// キャラクターコントローラーのパラメーターを設定する関数。
	// 衝突するコリジョン属性や重力の値などをここで設定する。
	// ※処理自体は継承先に委譲。
	virtual void _ConfigCharacterController() = 0;

	// パラメーターで剛体を生成。
	void _BuildCollision();

	// モデルデータ初期化関数。
	void _BuildModelData();

	// 継承先で使用するすべてのステートを登録する関数。
	// ※ステートを追加したら必ずこの関数内に記述を追加する
	// ※追加する際はこのクラスのState列挙体に対応する順番になっているかをしっかり確認すること。
	// ※ステートを追加した際はここだけでなくこのクラス内のState列挙体も更新すること。
	void _BuildState();

	// 継承先でアニメーション番号のテーブルを作成。
	// ※添え字にはこのクラス定義したAnimationType列挙体を使用。
	virtual void _BuildAnimation() = 0;

	// アニメーションイベントを設定する関数。
	// ※処理自体は継承先に委譲。
	virtual void _ConfigAnimationEvent() = 0;

protected:
	Components _MyComponent;	// このクラスで使用するコンポーネント。
	float _Radius = 0.0f;	// コリジョンサイズ(幅)。
	float _Height = 0.0f;	// コリジョンサイズ(高さ)。
	AnimationData _AnimationData[static_cast<int>(AnimationType::Max)];	// 各アニメーションタイプのアニメーション番号と再生時間の配列。
	State _NowStateIdx;		// 現在のステートの添え字。
	EnemyState* _NowState = nullptr;	// 現在のステート。

	float _Gravity = -50.0f;	// 重力。

	Vector3 _InitPos;	// 初期位置。

	SearchViewAngle _SearchView;	// 視野角判定。
	float _ViewAngle = 0.0f;		// 視野角(度)。
	float _ViewRange = 0.0f;		// 見える距離。

	float _AttackRange = 0.0f;	// 攻撃可能範囲。

	float _WanderingRange = 0.0f;	// 徘徊範囲。

	float _walkSpeed = 0.0f;		// 歩行速度。
private:
	vector<unique_ptr<EnemyState>> _MyState;	// このクラスが持つすべてのステートを登録。

	char _FileName[FILENAME_MAX];	// モデルのファイル名。

	Vector3 _MoveSpeed;	// 最終的な移動量(最終的にキャラクターコントローラに渡される)。
};

// エネミーの攻撃処理。
// ※攻撃の種類ごとに作成。
// ※継承先で各関数を実装。
// ※このクラスのインスタンスを攻撃ステートに渡すことで各命令が呼び出される。
// アニメーションが設定されていれば自動で再生される。
class EnemyAttack {
public:
	virtual ~EnemyAttack() {

	}

	// 初期化関数。
	// 引数：	再生するアニメーションの種類(初期値は再生しない,モデルごとのアニメーション番号で、テーブルの番号ではない)。
	//			アニメーション補間時間(初期値は0)。
	//			アニメーションループ再生数(1でループなし、-1で無限ループ)。
	inline void Init(int animType = -1, float interpolate = 0.0f, int animLoopNum = 1) {
		_animType = animType;
		_interpolate = interpolate;
		_animLoopNum = animLoopNum;
	}
	virtual void Start() = 0;	// 攻撃ステートの最初の更新前に一度だけ呼ばれる処理。
	virtual bool Update() = 0;	// 攻撃ステートの更新処理で呼び出される処理(戻り値は終了したか)。
	virtual void Exit() = 0;	// 攻撃ステート終了時に呼び出される処理。

	// この攻撃中のステート遷移を許可するか。
	// 引数：	切り替えようとしているステート。
	// ※切り替えようとしているステートごとに処理を変えたい場合は継承先で上書き。
	inline virtual bool IsPossibleChangeState(EnemyCharacter::State next) {
		return true;
	}

	inline const int GetAnimationType()const {
		return _animType;
	}
	inline const float GetInterpolate()const {
		return _interpolate;
	}
	inline const int GetAnimationLoopNum() const {
		return _animLoopNum;
	}
	inline void SetIsPlaying(bool flg) {
		_isPlaying = flg;
	}
protected:
	int _animType = -1;	// 再生するアニメーションの種類(初期値は再生しない,モデルごとのアニメーション番号で、テーブルの番号ではない)。
	float _interpolate = 0.0f;	// アニメーション補間時間(初期値は0)。
	int _animLoopNum = 1;	// アニメーションループ再生数(1でループなし、-1で無限ループ)。
	bool _isPlaying = false;	// アニメーション再生中かのフラグ(更新処理時に攻撃ステートから設定される)。
};

// ※単攻撃(攻撃モーション一回分攻撃)。
class EnemySingleAttack :public EnemyAttack{
public:
	void Start()override {};
	bool Update()override {
		if (!_isPlaying) {
			// 攻撃モーション一度終了。
			return true;
		}
		return false;
	}
	void Exit()override{};
};