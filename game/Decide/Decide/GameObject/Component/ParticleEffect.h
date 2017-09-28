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
	void HeelHpEffect(Transform* parent);

	//MP回復のエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void HeelMpEffect(Transform* parent);

	//バフのエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void BuffEffect(Transform* parent);

	//デバフのエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void DeBuffEffect(Transform* parent);

	//蛍っぽいエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void FireFly(Transform* parent);
	
	//Hp回復エフェクトを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetHeelHpEffectFlag(bool flag) {
		_HeelHpParticleEmitter->SetEmitFlg(flag);
		_HeelHpParticleAssistEmitter->SetEmitFlg(flag);
		_IsHeelHpFlag = flag;
	}


	//Mp回復エフェクトを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetHeelMpEffectFlag(bool flag) {
		_HeelMpParticleEmitter->SetEmitFlg(flag);
		_HeelMpParticleAssistEmitter->SetEmitFlg(flag);
		_IsHeelMpFlag = flag;
	}

	//バフデバフを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetEffectFlag(bool flag) {
		_ParticleEmitter->SetEmitFlg(flag);
	}

private:
	ParticleEmitter*	_ParticleEmitter = nullptr;				//パーティクル発生元。
	ParticleParameter	_ParticleParam;							//パーティクルパラメーター。
	ParticleParameter	_HeelHpParticleParam;					//Hp回復パーティクルパラメーター。
	ParticleEmitter*	_HeelHpParticleEmitter = nullptr;		//Hp回復エフェクトパーティクルの発生元。
	ParticleEmitter*	_HeelHpParticleAssistEmitter = nullptr;	//Hp回復エフェクトを補助するパーティクルの発生元。	
	ParticleParameter	_HeelHpParticleAssistParam;				//Hp回復エフェクトを補助するパーティクルパラメーター。
	ParticleParameter	_HeelMpParticleParam;					//Mp回復パーティクルパラメーター。
	ParticleEmitter*	_HeelMpParticleEmitter = nullptr;		//Mp回復エフェクトパーティクルの発生元。
	ParticleEmitter*	_HeelMpParticleAssistEmitter = nullptr;	//Mp回復エフェクトを補助するパーティクルの発生元。	
	ParticleParameter	_HeelMpParticleAssistParam;				//Mp回復エフェクトを補助するパーティクルパラメーター。
	bool				_IsHeelHpFlag;							//Hp回復エフェクトが始まった。
	float				_TotalHeelHpEffectTime = 0.0f;			//Hp回復エフェクトが始まって何秒経ったかの合計。
	bool				_IsHeelMpFlag;							//Mp回復エフェクトが始まった。
	float				_TotalHeelMpEffectTime = 0.0f;			//Mp回復エフェクトが始まって何秒経ったかの合計。
};

