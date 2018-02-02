#pragma once
#include "GameObject.h"

class Particle;

//パーティクル生成パラメータ
struct ParticleParameter {
	//初期化。
	void Init()
	{
		memset(this, 0, sizeof(ParticleParameter));
		size = Vector2(1, 1);
		initAlpha = 1.0f;
		brightness = 1.0f;
		isBillboard = true;
		mulColor = Color::white;
		isParent = false;
		isZTest = true;
	}
	const char* texturePath;						//!<テクスチャのファイルパス(Asset/Textureの中にあるもの(Asset/Textureよりも先のパスの指定のみでOK))。
	Vector3		initVelocity;						//!<初速度(エミッターと親子関係を組んでいる場合はエミッターの座標系、そうでない場合はワールド座標)。
	Vector2		size;								//パーティクルサイズ
	float		life;								//!<寿命。単位は秒(0より小さい値で無限)。
	float		intervalTime;						//!<発生時間。単位は秒。
	Vector3		initPositionRandomMargin;			//!<初期位置のランダム幅。
	Vector3		initVelocityVelocityRandomMargin;	//!<初速度のランダム幅。
	Vector3		addVelocityRandomMargih;			//!<速度の積分のときのランダム幅。
	Vector3		gravity;							//!<重力。
	bool		isFade;								//!<死ぬときにフェードアウトする？
	float		fadeTime;							//!<フェードする時間。
	float		initAlpha;							//!<初期アルファ値。
	bool		isBillboard;						//!<ビルボード？
	float		brightness;							//!<輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
	int			alphaBlendMode;						//!<0半透明合成、1加算合成。
	Color		mulColor;							//!<乗算カラー。
	bool		isParent;							//!<エミッターと親子関係を組むか。
	bool isZTest;
};


	
//パーティクルの発生機	
class ParticleEmitter : public GameObject {
public:
	ParticleEmitter(char* name) :GameObject(name) {};
	~ParticleEmitter() {
		ReleaseParticleAll();
	};
	/*!
	 *@brief	初期化。
	 *@_Param[in]	random		乱数生成に使用する乱数生成機。
	 *@_Param[in]	camera		パーティクルの描画で使用するカメラ。
	 *@_Param[in]	_Param		パーティクル生成用のパラメータ。
	 *@_Param[in]	emitPosition	エミッターの座標。
	 */
	void Init(const ParticleParameter& _Param);
	void Start() override;
	void Update() override;
	void Render() override;
	/*!
	*@brief	パーティクルに力を加える。
	*@_Param[in]	applyForce		乱数生成に使用する乱数生成機。
	*/
	void ApplyForce(Vector3& applyForce);

	// 初速度再設定。
	// ※既に作成されたパーティクルに対しては無効。
	// ※これから作成されるものに対してのみ有効。
	inline void ResetInitVelocity(const Vector3& newVelocity) {
		_Param.initVelocity = newVelocity;
	}

	// パラメーター再設定(作成済みのパーティクルのパラメーターも変更)。
	void ResetParameterAlreadyCreated(const ParticleParameter& param);

	// この関数を呼んでからAchievedCreateParticleEnd関数が呼ばれるまでに生成されたパーティクルへのポインタを取得し続ける関数。
	// 引数：	生成したパーティクルを格納する配列へのポインタ。
	void AchievedCreateParticleStart(vector<Particle*>* array);

	// 生成したパーティクルの取得終了。
	void AchievedCreateParticleEnd();

	void SetEmitFlg(bool b);

	inline bool GetEmitFlg()const {
		return emit;
	}

	// パラメーター設定(作成済みのパーティクルには無効)。
	inline void SetParam(const ParticleParameter& param) {
		_Param = param;
	}
	inline const ParticleParameter& GetParam()const {
		return _Param;
	}

	inline const Vector3& GetInitVelocity() const{
		return _Param.initVelocity;
	}
	
	// 生存しているパーティクルのうち、最初に生成されたものを返却する関数。
	// ※リストにパーティクルが格納されてない場合はnullが返却される。
	Particle* GetParticleBegin()const;
	// 生存しているパーティクルのうち、最後に生成されたものを返却する関数。
	// ※リストにパーティクルが格納されてない場合はnullが返却される。
	Particle* GetParticleEnd()const;

	void ReleaseParticleAll();
	//パーティクル生成
	void Emit();
private:
	bool	emit;//生成フラグ
	float					_Timer;			//!<タイマー
	ParticleParameter		_Param;			//!<パラメータ。
	std::list<Particle*>	_ParticleList;	//!<パーティクルのリスト。
	vector<Particle*>* _achievedArray = nullptr;
};