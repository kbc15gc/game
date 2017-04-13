#pragma once

#include "fbEngine\Animation.h"

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
	enum class State { Wandering = 0, Translation };

	// アニメーションデータ配列の添え字。
	// ※0番なら待機アニメーション、1番なら歩くアニメーション。
	// ※この列挙子を添え字として、継承先のクラスでアニメーショ番号のテーブルを作成する。
	enum class AnimationType { Idle = 0, Walk, Max };

	// アニメーションデータ構造体。
	struct AnimationData{
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


	virtual void Awake()override;

	virtual void Start()override;

	virtual void Update()override;


	// エネミーのアニメーション再生関数(ループ)。
	// 引数：	アニメーションタイプ。
	//			補間時間。
	inline void PlayAnimation_Loop(const AnimationType AnimationType,const float InterpolateTime) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(AnimationType)].No, InterpolateTime);
	}

	// エネミーのアニメーション再生関数(指定回数ループ)。
	// 引数：	アニメーションタイプ。
	//			補間時間。
	//			ループ回数。
	inline void PlayAnimation(const AnimationType AnimationType,const float InterpolateTime,const unsigned int LoopCount) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(AnimationType)].No,InterpolateTime, LoopCount);
	}


	// モデルファイルのパスを設定。
	inline void SetFileName(const char* name) {
		if (strlen(name) >= FILENAME_MAX) {
			// ファイル名文字数オーバー。
			abort();
		}
		strcpy(_FileName, name);
	}

	// ステート配列を読み取り専用で返す関数。
	inline const vector<unique_ptr<EnemyState>>& GetMyState() const{
		return _MyState;
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
private:
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
	void _BuildState();

	// 継承先でアニメーション番号のテーブルを作成。
	// ※添え字にはこのクラス定義したAnimationType列挙体を使用。
	virtual void _BuildAnimation() = 0;
protected:
	Components _MyComponent;	// このクラスで使用するコンポーネント。
	float _Radius = 0.0f;	// コリジョンサイズ(幅)。
	float _Height = 0.0f;	// コリジョンサイズ(高さ)。
	AnimationData _AnimationData[static_cast<int>(AnimationType::Max)];	// 各アニメーションタイプのアニメーション番号と再生時間の配列。
private:
	EnemyState* _NowState = nullptr;	// 現在のステート。
	State _NowStateIdx;		// 現在のステートの添え字。
	vector<unique_ptr<EnemyState>> _MyState;	// このクラスが持つすべてのステートを登録。

	char _FileName[FILENAME_MAX];	// モデルのファイル名。
};