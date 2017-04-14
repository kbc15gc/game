#pragma once

#include "fbEngine\Animation.h"
#include "SearchViewAngle.h"

class CCharacterController;
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

	enum class State { Wandering = 0,Discovery, Wait ,Translation };

	// アニメーションデータ配列の添え字。
	// ※0番なら待機アニメーション、1番なら歩くアニメーション。
	// ※この列挙子を添え字として、継承先のクラスでアニメーショ番号のテーブルを作成する。
	enum class AnimationType { Idle = 0, Walk, Dash, Max };

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
	

	// エネミーのアニメーション再生関数(ループ)。
	// 引数：	アニメーションタイプ。
	//			補間時間。
	inline void PlayAnimation_Loop(const AnimationType AnimationType, const float InterpolateTime) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(AnimationType)].No, InterpolateTime);
	}

	// エネミーのアニメーション再生関数(指定回数ループ)。
	// 引数：	アニメーションタイプ。
	//			補間時間。
	//			ループ回数。
	inline void PlayAnimation(const AnimationType AnimationType, const float InterpolateTime, const unsigned int LoopCount) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(AnimationType)].No, InterpolateTime, LoopCount);
	}

	// 外部からステート切り替えを行おうとする場合はこちらを呼ぶ。
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

	// 初期位置取得。
	inline const Vector3& GetInitPos()const {
		return _InitPos;
	}

	// 現在のステートを取得。
	inline EnemyState* GetNowState()const {
		return _NowState;
	}
protected:
	// ステート切り替え関数。
	void _ChangeState(State next);

	// アニメーションタイプにアニメーションデータを関連付ける関数。
	// 引数：	アニメーションタイプの列挙子。
	//			第1引数に関連付けたいアニメーションデータ。
	inline void _ConfigAnimationType(AnimationType Type, const AnimationData& Data) {
		_AnimationData[static_cast<unsigned int>(Type)] = Data;
	}

	// 現在のステートの処理が終了したときに呼ばれるコールバック関数。
	// 引数は終了したステートのタイプ。
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

	// パラメーターで剛体を生成。
	virtual void _BuildCollision();

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
	EnemyState* _NowState = nullptr;	// 現在のステート。

	Vector3 _InitPos;	// 初期位置。

	SearchViewAngle _SearchView;	// 視野角判定。
	float _ViewAngle = 0.0f;		// 視野角(度)。
	float _ViewRange = 0.0f;		// 見える距離。

	float _AttackRange = 0.0f;	// 攻撃可能範囲。
private:
	State _NowStateIdx;		// 現在のステートの添え字。
	vector<unique_ptr<EnemyState>> _MyState;	// このクラスが持つすべてのステートを登録。

	char _FileName[FILENAME_MAX];	// モデルのファイル名。

	Vector3 _MoveSpeed;	// 最終的な移動量(最終的にキャラクターコントローラに渡される)。

};