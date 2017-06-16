#pragma once

#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "SearchViewAngle.h"
#include "../Component/ObjectRotation.h"
#include "fbEngine\CharacterController.h"
#include "AttackCollision.h"
#include "GameObject\Component\CharacterParameter.h"
#include "GameObject\Component\ParameterBar.h"

class SkinModel;
class Animation;
class EnemyState;

// 基底クラス。
// エネミーのキャラクター。
class EnemyCharacter :
	public GameObject
{
public:
	// ステート配列の添え字を列挙。
	// ※継承先で使用するものも含めてすべてのステートをここに列挙する。
	// ※追加する際はこのクラスの_BuildState関数に記述した順番になっているかをしっかり確認すること。
	// ※ステートを追加した際はここだけでなくこのクラス内の_BuildState関数も更新すること。

	enum class State { Wandering = 0,Discovery, StartAttack, Attack ,Wait ,Translation, Fall };

	// アニメーションデータ配列の添え字。
	// ※0番なら待機アニメーション、1番なら歩くアニメーション。
	// ※この列挙子を添え字として、継承先のクラスでアニメーショ番号のテーブルを作成する。
	enum class AnimationType { Idle = 0, Walk, Dash, Attack, Fall, Max };

	// アニメーションデータ構造体。
	struct AnimationData {
		unsigned int No;	// アニメーション番号。
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
	// ※継承先で実装。
	// ※複数攻撃パターンがある場合はここでローカルステートに遷移する際に分岐させる。
	virtual EnemyCharacter::State AttackSelect() = 0;

	// 攻撃判定用コリジョン作成関数。
	// 引数：	コリジョンを発生させるフレーム。
	//			コリジョン発生位置。
	//			コリジョン回転。
	//			コリジョンのサイズ。
	// ※生成されるコリジョン形状はボックスです。
	void CreateAttackCollision(const int eventFrame, const Vector3& pos, const Vector3& angle, const Vector3& size);

	// エネミーのアニメーション再生関数(ループ)。
	// 引数：	アニメーションタイプ。
	//			補間時間。
	inline void PlayAnimation_Loop(const AnimationType animationType, const float interpolateTime) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(animationType)].No, interpolateTime);
	}

	// エネミーのアニメーション再生関数(指定回数ループ)。
	// 引数：	アニメーションタイプ。
	//			補間時間。
	//			ループ回数。
	inline void PlayAnimation(const AnimationType animationType, const float interpolateTime, const int loopCount) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(animationType)].No, interpolateTime, loopCount);
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
			_MyComponent.Parameter->DamageMass(hitCollision->GetDamage(), _MyComponent.Parameter->GetParam(CharacterParameter::DEF));
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
private:
	vector<unique_ptr<EnemyState>> _MyState;	// このクラスが持つすべてのステートを登録。

	char _FileName[FILENAME_MAX];	// モデルのファイル名。

	Vector3 _MoveSpeed;	// 最終的な移動量(最終的にキャラクターコントローラに渡される)。
};