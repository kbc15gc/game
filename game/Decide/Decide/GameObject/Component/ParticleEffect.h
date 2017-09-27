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

	//回復のエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void HeelEffect(Transform* parent);

	//バフのエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void BuffEffect(Transform* parent);

	//デバフのエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void DeBuffEffect(Transform* parent);

	//蛍っぽいエフェクトを発生させる。
	//引数はエフェクトを発生させる基点。
	void FireFly(Transform* parent);
	
	//回復エフェクトを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetHeelEffectFlag(bool flag) {
		_HeelParticleEmitter->SetEmitFlg(flag);
		_HeelParticleAssistEmitter->SetEmitFlg(flag);
		_IsHeelFlag = flag;
	}

	//バフデバフを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetEffectFlag(bool flag) {
		_ParticleEmitter->SetEmitFlg(flag);
	}

private:
	ParticleEmitter*	_ParticleEmitter = nullptr;				//回復パーティクル発生元。
	ParticleParameter	_HeelParticleParam;						//回復パーティクルパラメーター。
	ParticleEmitter*	_HeelParticleEmitter = nullptr;			//回復エフェクトを補助するパーティクルの発生元。
	ParticleEmitter*	_HeelParticleAssistEmitter = nullptr;	//回復エフェクトを補助するパーティクルの発生元。
	ParticleParameter	_ParticleParam;							//パーティクルパラメーター。
	ParticleParameter	_HeelParticleAssistParam;				//回復エフェクトを補助するパーティクルパラメーター。
	bool				_IsHeelFlag;							//回復エフェクトが始まった。
	float				_TotalHeelEffectTime = 0.0f;			//回復エフェクトが始まって何秒経ったかの合計。
};

