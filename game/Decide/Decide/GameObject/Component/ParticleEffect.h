#pragma once
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "fbEngine\_Object\_Component\Component.h"

//パーティクルを使ったエフェクトを発生させるクラス。
class ParticleEffect :public Component
{
public:
	ParticleEffect(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~ParticleEffect() {};
public:
	void Awake()override;

	void Update()override;

	//HP回復のエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void HeelHpEffect();

	//MP回復のエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void HeelMpEffect();

	//バフのエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void BuffEffect();

	//デバフのエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void DeBuffEffect();

	//蛍っぽいエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void FireFly();
	
	//Hp回復エフェクトを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetHeelHpEffectFlag(bool flag) {
		_HeelHpParticleEmitter->SetEmitFlg(flag);
		_HeelParticleAssistEmitter->SetEmitFlg(flag);
		_IsHeelHpFlag = flag;
	}


	//Mp回復エフェクトを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetHeelMpEffectFlag(bool flag) {
		_HeelMpParticleEmitter->SetEmitFlg(flag);
		_HeelParticleAssistEmitter->SetEmitFlg(flag);
		_IsHeelMpFlag = flag;
	}

	//バフを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetBuffEffectFlag(bool flag) {
		_BuffParticleEmitter->SetEmitFlg(flag);
	}

	//デバフを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetDebuffEffectFlag(bool flag) {
		_DebuffParticleEmitter->SetEmitFlg(flag);
	}

	//蛍を発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetFireFlyEffectFlag(bool flag) {
		_FireFlyParticleEmitter->SetEmitFlg(flag);
	}
private:
	ParticleEmitter*	_BuffParticleEmitter = nullptr;			//バフパーティクル発生元。
	ParticleParameter	_BuffParticleParam;						//バフパーティクルパラメーター。
	ParticleEmitter*	_DebuffParticleEmitter = nullptr;		//デバフパーティクル発生元。
	ParticleParameter	_DebuffParticleParam;					//デバフパーティクルパラメーター。
	ParticleParameter	_HeelHpParticleParam;					//Hp回復パーティクルパラメーター。
	ParticleEmitter*	_HeelHpParticleEmitter = nullptr;		//Hp回復エフェクトパーティクルの発生元。
	ParticleEmitter*	_HeelParticleAssistEmitter = nullptr;	//回復エフェクトを補助するパーティクルの発生元。	
	ParticleParameter	_HeelParticleAssistParam;				//回復エフェクトを補助するパーティクルパラメーター。
	ParticleParameter	_HeelMpParticleParam;					//Mp回復パーティクルパラメーター。
	ParticleEmitter*	_HeelMpParticleEmitter = nullptr;		//Mp回復エフェクトパーティクルの発生元。
	bool				_IsHeelHpFlag;							//Hp回復エフェクトが始まった。
	float				_TotalHeelHpEffectTime = 0.0f;			//Hp回復エフェクトが始まって何秒経ったかの合計。
	bool				_IsHeelMpFlag;							//Mp回復エフェクトが始まった。
	float				_TotalHeelMpEffectTime = 0.0f;			//Mp回復エフェクトが始まって何秒経ったかの合計。
	ParticleEmitter*	_FireFlyParticleEmitter = nullptr;		//蛍みたいなパーティクル発生元。
	ParticleParameter	_FireFlyParticleParam;					//蛍みたいなバフパーティクルパラメーター。
};

