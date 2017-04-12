#pragma once

class CCharacterController;
class SkinModel;
class Animation;
class EnemyState;

// 基底クラス。
// エネミーのキャラクター。
class EnemyCharacter :
	public GameObject
{
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
	// ステート配列の添え字を列挙。
	// ※継承先で使用するものも含めてすべてのステートをここに列挙する。
	enum class State{ NotAction };

public:
	// 引数はオブジェクトの名前。
	EnemyCharacter(const char* name);
	~EnemyCharacter();


	virtual void Awake()override;

	virtual void Start()override;

	virtual void Update()override;


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

protected:
	Components _MyComponent;	// このクラスで使用するコンポーネント。
	float _Radius = 0.0f;	// コリジョンサイズ(幅)。
	float _Height = 0.0f;	// コリジョンサイズ(高さ)。
private:
	EnemyState* _NowState = nullptr;	// 現在のステート。
	State _NowStateIdx;			// 現在のステートの添え字。
	vector<unique_ptr<EnemyState>> _MyState;	// このクラスが持つすべてのステートを登録。

	char _FileName[FILENAME_MAX];	// モデルのファイル名。
};