#pragma once

#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "SearchViewAngle.h"
#include "../Component/ObjectRotation.h"
#include "fbEngine\CharacterController.h"
#include "GameObject\Component\CharacterParameter.h"
#include "GameObject\Component\ParameterBar.h"
#include "GameObject\Component\ObjectSpawn.h"
#include "GameObject\TextImage\AttackValue2D.h"
#include "GameObject\Component\AnimationEvent.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\Player\Player.h"
#include "fbEngine\_Object\_Component\_Physics\CharacterExtrude.h"
#include "GameObject\ItemManager\DropItem\DropItem.h"

class SkinModel;
class Animation;
class EnemyState;
class ParticleEffect;
class BuffDebuffICon;

class EnemyAttack;
class EnemySingleAttack;
class EnemyBreathAttack;

// 基底クラス。
// エネミーのキャラクター。
class EnemyCharacter :
	public GameObject
{
public:
	// プレイヤーと最も近いエネミーの情報。
	struct NearEnemyInfo {
		NearEnemyInfo(float len, EnemyCharacter* obj) {
			length = len;
			object = obj;
		}
		float length = FLT_MAX;
		EnemyCharacter* object = nullptr;
	};

	// 自分がどの種類のエネミーか。
	// ※このクラスを継承して新種エネミーを作成したらここに種別を追加すること。
	enum class EnemyType{Born = 0,BossDrarian,Drarian, Golem};

	// ステート配列の添え字を列挙。
	// ※継承先で使用するものも含めてすべてのステートをここに列挙する。
	// ※追加する際はこのクラスの_BuildState関数に記述した順番になっているかをしっかり確認すること。
	// ※ステートを追加した際はここだけでなくこのクラス内の_BuildState関数も更新すること。
	enum class State {None = -1, Wandering = 0,Discovery, Chace,Threat, Speak,StartAttack, Attack ,Wait ,Translation, Fall,Damage,Death};

	// アニメーションデータテーブルの添え字。
	// ※0番なら待機アニメーション、1番なら歩くアニメーション。
	// ※この列挙子を添え字として、継承先のクラスでアニメーション番号のテーブルを作成する。
	// ※攻撃モーションはモデルによって数が異なるのでここでは扱わない。
	enum class AnimationType { None = -1,Idle = 0, Walk, Dash, Threat, Fall,Damage, Death,Max };

	// サウンドデータテーブルの添え字。
	enum class SoundIndex{None = -1, Attack1 = 0, Attack2, Attack3, Threat,Damage,Death, Max};

	// サウンドデータ構造体。
	struct SoundData {
		char Path[FILENAME_MAX];	// サウンド名(.wavも入れてね)。
		bool Is3D = false;		// 3Dサウンドか。
		bool IsLoop = false;	// ループ再生か。
		SoundSource* Source = nullptr;	// サウンド再生オブジェクト。
	};
private:
	// このクラスで使用するコンポーネント。
	// ※コンポーネントは勝手に削除されるため、このクラスでは削除しない。
	// ※このクラス独自のメンバ変数と住み分けを行うために構造体化する。
	// ※ここで登録されているものがエネミーにアタッチされているコンポーネントのすべてではないので注意。
	struct Components {
		SkinModel* Model = nullptr;	// このオブジェクトのモデル。
		Animation* Animation = nullptr;	// このオブジェクトのアニメーション。
		Collider* Collider = nullptr;	// コリジョン形状。
		CCharacterController* CharacterController = nullptr;		// キャラクターコントローラ。
		ObjectRotation* RotationAction = nullptr;	// オブジェクトを回転させるクラス。
		CharacterParameter* Parameter = nullptr;//エネミーのパラメーター。
		ParameterBar* HPBar = nullptr;			// ゲージHP用。
		ObjectSpawn* Spawner = nullptr;		// リスポーン設定できる。
		AnimationEventPlayer* AnimationEventPlayer = nullptr;	// アニメーションにイベントを設定できる。
		ParticleEffect* ParticleEffect = nullptr;	// パーティクルエフェクト。
		BuffDebuffICon*	BuffDebuffICon = nullptr;	// バフデバフアイコン。
		CharacterExtrude* CharacterExtrude = nullptr;	// このエネミーが他のオブジェクトを押し出す。
		vector<RigidBody*> ExtrudeCollisions;	// キャラクターコントローラの押し戻しに使用する剛体。
	};

	struct CollisionInfo {
		float radius = 0.0f;	// コリジョンサイズ(幅)。
		float height = 0.0f;	// コリジョンサイズ(高さ)。
		Vector3 offset = Vector3::zero;	// 差分。
		Collision_ID id = Collision_ID::ENEMY;	// コリジョン属性。
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
	inline EnemyAttack* AttackSelect() {
		_nowAttack = _AttackSelectSubClass();
		return _nowAttack;
	}

	// エネミーのアニメーション再生関数(ループ)。
	// 引数：	アニメーションタイプ(テーブルのほう)。
	//			補間時間。
	inline void PlayAnimation_Loop(const AnimationType animationType, const float interpolateTime) {
		PlayAnimation_OriginIndex(_AnimationNo[static_cast<unsigned int>(animationType)], interpolateTime,-1);
	}

	// エネミーのアニメーション再生関数(指定回数ループ)。
	// 引数：	アニメーションタイプ(テーブルのほう)。
	//			補間時間。
	//			ループ回数(デフォルトは1)。
	inline void PlayAnimation(const AnimationType animationType, const float interpolateTime, const int loopCount = 1) {
		PlayAnimation_OriginIndex(_AnimationNo[static_cast<unsigned int>(animationType)], interpolateTime, loopCount);
	}

	// エネミーのアニメーション再生関数(指定回数ループ)。
	// 引数：	アニメーションタイプ(モデルごとのアニメーション番号、-1で再生しない)。
	//			補間時間。
	//			ループ回数(-1で無限ループ)。
	inline void PlayAnimation_OriginIndex(const int animationNum, const float interpolateTime, const int loopCount = 1) {
		if (animationNum == -1) {
			// アニメーションを再生しない。
			return;
		}
		_MyComponent.Animation->PlayAnimation(animationNum, interpolateTime, loopCount);
	}

	// 音再生関数。
	// 引数：	効果音テーブルの添え字。
	inline void EnemyPlaySound(const SoundIndex idx) {
		if (_SoundData[static_cast<int>(idx)].Source) {
			// サウンドソースが作成されている。
			_SoundData[static_cast<int>(idx)].Source->Play(_SoundData[static_cast<int>(idx)].IsLoop);
		}
	}

	// エネミーがアニメーションを再生しているか。
	inline bool GetIsPlaying() {
		return _MyComponent.Animation->GetPlaying();
	}

	// エネミーが追跡範囲外に出たか判定。
	bool IsOutsideDiscovery();

	// 引数の値を加算した結果、エネミーが徘徊範囲外に出たか判定。
	bool IsOutsideWandering(const Vector3& Add);

	// 自分が発生させたもの以外の攻撃コリジョンと衝突した瞬間呼ばれるコールバック。
	// ※引数は衝突した攻撃コリジョン。
	// ※処理が少ないうちはinlineのままでいいよ(だいたい3行以上の処理をするようになるまで)。

	virtual void HitAttackCollisionEnter(AttackCollision* hitCollision);

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

	// ダメージのけぞり設定関数。
	// 引数：	のけぞるか。
	//			何回に一回のけぞるか(デフォルトは1)。
	void ConfigDamageReaction(bool isMotion, unsigned short probability = 1);

	// エネミーにダメージを与える処理。
	// 引数：	ダメージ情報。
	void GiveDamage(const CharacterParameter::DamageInfo& info);

	// 攻撃生成関数。
	// 引数：	位置(ローカル座標)。
	//			回転。
	//			拡縮。
	//			寿命(0.0より小さい値で無限)。
	//			親(デフォルトはnull)。
	//			魔法攻撃か(デフォルトはfalse)。
	//			防御無視攻撃か(デフォルトはfalse)。
	//			ダメージ率(攻撃の種類などによる攻撃力に対する割合、この値に0.01f掛けた値を攻撃力に乗算する、単位はパーセント)。
	// 戻り値:  生成した攻撃。
	inline AttackCollision* CreateAttack(const Vector3& localPos, const Quaternion& rot, const Vector3& scale, float life, Transform* parent = nullptr,bool isMagic = false,bool isThroughDamage = false,int percentage = 100) {
		//攻撃コリジョン作成。
		unsigned int priorty = 1;
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
		attack->Create(move(_MyComponent.Parameter->GiveDamageMass(isMagic, isThroughDamage,nullptr, percentage)),localPos, rot, scale, AttackCollision::CollisionMaster::Enemy, life, 0.0f, parent);
		return attack;
	}


	// 全パラメーター設定。
	// 引数：	HPバーに設定する色(重ねる場合は先に追加したものから表示される)。
	//			各種パラメーター。
	inline void SetParamAll(const vector<BarColor>& color,int param[CharacterParameter::Param::MAX]) const{
		_MyComponent.Parameter->ParamReset(param);
		_MyComponent.HPBar->Create(color, static_cast<float>(_MyComponent.Parameter->GetMaxHP()), static_cast<float>(_MyComponent.Parameter->GetParam(CharacterParameter::Param::HP)), true, false, transform, Vector3(0.0f, 2.0f, 0.0f), Vector2(0.5f, 0.5f),6,false, false);
		_MyComponent.BuffDebuffICon->SetHpBarTransform(_MyComponent.HPBar->GetTransform());
		
	}
	// 全パラメーター設定。
	// 引数：	HPバーに設定する色(重ねる場合は先に追加したものから表示される)。
	//			各種パラメーター。
	inline void SetParamAll(const vector<BarColor>& color, const vector<int>& param) const {
		_MyComponent.Parameter->ParamReset(param);
		_MyComponent.HPBar->Create(color, static_cast<float>(_MyComponent.Parameter->GetMaxHP()), static_cast<float>(_MyComponent.Parameter->GetParam(CharacterParameter::Param::HP)), true, false, transform, Vector3(0.0f, 2.0f, 0.0f), Vector2(0.5f, 0.5f),6 ,false,false);
		_MyComponent.BuffDebuffICon->SetHpBarTransform(_MyComponent.HPBar->GetTransform());
	}

	// モデルファイルのパスを設定。
	inline void SetFileName(const char* name) {
		if (strlen(name) >= FILENAME_MAX) {
			// ファイル名文字数オーバー。
			abort();
		}
		strcpy(_FileName, name);
	}

	inline const char* GetFileName()const {
		return _FileName;
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

	// ドロップする経験値。
	inline void SetDropEXP(int exp) {
		_dropExp = exp;
	}
	inline int GetDropEXP() const {
		return _dropExp;
	}

	// ドロップする所持金。
	inline void SetDropMoney(int money) {
		_dropMoney = money;
	}
	inline int GetDropMoney() const {
		return _dropMoney;
	}

	inline float GetWalkSpeed()const {
		return _walkSpeed;
	}

	inline float GetDashSpeed()const {
		return _walkSpeed * 5.0f;

	}

	inline int GetParam(CharacterParameter::Param param)const {
		if (_MyComponent.Parameter) {
			return _MyComponent.Parameter->GetParam(param);
		}
		else {
			return 0;
		}
	}

	inline int GetPigmentParam(CharacterParameter::Param param)const {
		if (_MyComponent.Parameter) {
			return _MyComponent.Parameter->GetPigmentParam(param);
		}
		else {
			return 0;
		}
	}

	inline int GetMaxHP()const {
		if (_MyComponent.Parameter) {
			return _MyComponent.Parameter->GetMaxHP();
		}
		else {
			return 0;
		}
	}

	inline int GetMaxMP()const {
		if (_MyComponent.Parameter) {
			return _MyComponent.Parameter->GetMaxMP();
		}
		else {
			return 0;
		}
	}

	// エネミー共通のアニメーション列挙子から各モデルのアニメーション番号を取得。
	inline int GetAnimationNo(AnimationType type)const {
		return _AnimationNo[static_cast<int>(type)];
	}

	// 初期ステート取得。
	inline const State GetInitState()const {
		return _initState;
	}

	// 現在エネミーが選択している攻撃の発動距離を返却。
	float GetNowSelectAttackRange()const;

	inline EnemyAttack* GetNowSelectAttack() {
		return _nowAttack;
	}



	// 死亡時のドロップ処理。
	inline void Drop() {
		_DropSubClass();
		_Player->TakeDrop(GetDropEXP(), GetDropMoney());
	}

	/**
	* アイテム効果.
	*/
	bool ItemEffect(Item::ItemInfo* info);

	/**
	* エフェクト用更新.
	*/
	void EffectUpdate();

	inline State GetNowStateIndex()const {
		return _NowStateIdx;
	}

	inline short GetDamageMotionRandNum()const {
		return _damageMotionRandNum;
	}

	inline int GetNowPlayAnimation()const {
		return _MyComponent.Animation->GetPlayAnimNo();
	}

protected:
	// ステート切り替え関数。
	// ※Noneを渡すとステートがオフになる。
	void _ChangeState(State next);

	// アニメーションテーブル作成関数。
	// 引数：	アニメーション終了時間の格納用配列(この配列に終了時間を設定する、添え字はモデルに設定されているアニメーション番号)。
	// 受け取る配列内の値はデフォルトで-1となっているので、アニメーションの終了時間が1秒以上のものは設定しなくてよい。
	// ※純粋仮想関数。
	virtual void _BuildAnimationSubClass(vector<double>& datas) = 0;

	// アニメーションタイプにアニメーションデータを関連付ける関数。
	// 引数：	アニメーションタイプの列挙子。
	//			各モデルのアニメーション番号。
	inline void _ConfigAnimationType(AnimationType Type, const int animNo) {
		_AnimationNo[static_cast<unsigned int>(Type)] = animNo;
	}

	// 引数のパラメータをサウンドテーブルに登録する関数。
	// 引数：	登録するサウンドのタイプ(列挙子)。
	//			waveファイルの名前(.wavまで含めて)。
	//			3Dサウンドにするか。
	//			ループ再生するか。
	void _ConfigSoundData(SoundIndex idx, char* filePath, bool is3D = false, bool isLoop = false);

	// 現在のステートの処理が終了したときに呼ばれるコールバック関数。
	// 引数は終了したステートのタイプ。
	// ※この関数は各ステートが自発的に終了した場合にのみ呼び出される。
	// ※このクラスのChangeStateRequestで終了した場合は呼ばれない。
	// ※ローカルステートの終了時にも呼ばれない。
	virtual void _EndNowStateCallback(State EndStateType) {};

private:

	// HPバーの描画状態更新処理。
	void _BarRenderUpdate();

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

	// キャラクターコントローラ押し出しコンポーネント用の剛体作成関数。
	// コリジョン属性や形状などを設定し、作成する。
	// ※処理自体は継承先に委譲。
	virtual void _CreateExtrudeCollision() = 0;

	// キャラクターコントローラ押し出しコンポーネントのパラメーターを設定する関数。
	// 衝突するコリジョン属性や重力の値などをここで設定する。
	// ※処理自体は継承先に委譲。
	virtual void _ConfigCharacterExtrude() = 0;

	// パラメーターで剛体を生成。
	void _BuildCollision();

	// モデルデータ初期化関数。
	void _BuildModelData();

	// 継承先で汎用的に使用するすべてのステートを登録する関数。
	// ※ステートを追加したら必ずこの関数内に記述を追加する
	// ※追加する際はこのクラスのState列挙体に対応する順番になっているかをしっかり確認すること。
	// ※ステートを追加した際はここだけでなくこのクラス内のState列挙体も更新すること。
	void _BuildState();

	// 絶対に他のクラスでも使わないステートはこっちに登録。
	virtual void _BuildStateSubClass() {};

	// アニメーションのテーブル作成に使用するデータを作成。
	void _BuildAnimation();

	// アニメーションイベントを設定する関数。
	// ※処理自体は継承先に委譲。
	virtual void _ConfigAnimationEvent() = 0;

	// 効果音のテーブル作成。
	// ※処理自体は継承先に委譲。
	virtual void _BuildSoundTable() = 0;

	// 死亡時ドロップ処理。
	// ※継承先によって異なる処理。
	virtual inline void _DropSubClass() = 0;

	// 攻撃処理を決定する関数。
	// 戻り値：	実行したい攻撃処理クラスのポインタ。
	// ※複数攻撃パターンがある場合は攻撃処理を分岐させる。
	// ※継承先で実装。
	virtual EnemyAttack* _AttackSelectSubClass() = 0;

protected:
	Components _MyComponent;	// このクラスで使用するコンポーネント。

	vector<unique_ptr<EnemyState>> _MyState;	// このクラスが持つすべてのステートを登録。

	CollisionInfo _collisionInfo;

	int _AnimationNo[static_cast<int>(AnimationType::Max)];	// 各アニメーションタイプのアニメーション番号と再生時間の配列。
	SoundData _SoundData[static_cast<int>(SoundIndex::Max)];

	State _NowStateIdx;		// 現在のステートの添え字。
	EnemyState* _NowState = nullptr;	// 現在のステート。
	State _initState;	// 初期ステート。

	float _Gravity = -0.98f;	// 重力。

	Vector3 _InitPos;	// 初期位置。

	SearchViewAngle _SearchView;	// 視野角判定。
	float _ViewAngle = 0.0f;		// 視野角(度)。
	float _ViewRange = 0.0f;		// 見える距離。

	float _WanderingRange = 0.0f;	// 徘徊範囲。

	float _discoveryRange = 0.0f;	// 追跡範囲。

	float _walkSpeed = 0.0f;		// 歩行速度。

	short _damageMotionRandNum = 1;	// 怯む確率(攻撃時以外)。

	bool _isDamageMotion = true;			// のけぞりモーションを再生するか。
	bool _isDamageMotionRandom = true;		// のけぞりモーションをランダムで再生するか(ランダムにしない場合は必ずのけぞる)。
	unsigned short _damageMotionProbability = 1;	// のけぞる確率(この変数に設定された回数に1回はのけぞる)。

	EnemyAttack* _nowAttack = nullptr;

private:
	int _dropExp;	// 落とす経験値。
	int _dropMoney; // 落とす金額。

	char _FileName[FILENAME_MAX];	// モデルのファイル名。

	Vector3 _MoveSpeed;	// 最終的な移動量(最終的にキャラクターコントローラに渡される)。

	Player* _Player = nullptr;			//プレイヤー

public:
	static NearEnemyInfo nearEnemyInfo;
};

