#pragma once
#include "GameObject.h"
#include "ParticleEmitter.h"

class Vertex;


//パーティクルクラス
class Particle :public GameObject
{
public:
	enum eState {
		eStateRun,
		eStateFadeOut,
		eStateDead,
	};
	Particle(char* name) :GameObject(name) {}
	void Awake()override;
	void LateUpdate()override;
	void Render() override;

	void Init(const ParticleParameter& param,Transform* parent);
	/*!
	*@brief	パーティクルに力を加える。
	*@param[in]	_ApplyForce		乱数生成に使用する乱数生成機。
	*/
	void ApplyForce(Vector3& _ApplyForce)
	{
		this->_ApplyForce = _ApplyForce;
	}
	bool IsDead()
	{
		return _IsDead;
	}
	void SetIsDead(bool flg) {
		_IsDead = flg;
	}

	void SetParam(const ParticleParameter& param);

	inline const Vector3& GetVelocity() {
		return _Velocity;
	}

	inline void SetIsAutoDelete(bool flg) {
		_isAutoDelete = flg;
	}

	inline void SetEmitterTransform(Transform* parent) {
		_parent = parent;
	}

	inline float GetLife()const {
		return _Life;
	}
private:		
	static Vertex* _Vertex;						//頂点


	TEXTURE*		_Texture;					//画像
	Effect*			_Effect;					//シェーダーエフェクト。
	Camera*			_Camera;					//カメラ。
	float			_Life;						//ライフ(0より小さい値で無限)。
	float			_Timer;						//タイマー。
	Vector3			_Velocity;					//速度。
	Vector3			_Gravity;					//重力。
	float			_RotateZ;					//Z軸周りの回転。
	Vector3			_AddVelocityRandomMargih;	//速度の積分のときのランダム幅。
	bool			_IsDead;					//死亡フラグ。
	bool			_IsFade;					//死ぬときにフェードアウトする？
	float			_FadeTIme;					//フェードの時間。
	eState			_State;						//状態。
	float			_InitAlpha;					//初期アルファ。
	float			_Alpha;						//アルファ。
	bool			_IsBillboard;				//ビルボード？
	Vector3			_ApplyForce;				//外部から加わる力。
	float			_Brightness;				//輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
	int				_AlphaBlendMode;			//0半透明合成、1加算合成。
	Color			_MulColor;					//乗算カラー。

	bool _isParent;	// エミッターと親子関係を組むか。

	bool _isZTest = true;

	Transform* _parent = nullptr;	// エミッターのTransform(親子関係を組む組まないにかかわらず、エミッターの座標系での向きなどを使ってパーティクルの位置を更新するので必要)。

	bool _isAutoDelete = true;	// パーティクルが自発的に削除されるか。
};