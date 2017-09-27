#pragma once
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "fbEngine\_Object\_Component\Component.h"

//パーティクルを使ったエフェクトを発生させるクラス。
class ParticleEffect :public Component
{
public:
	ParticleEffect(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
		Initialize();
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~ParticleEffect() {};

public:
	//エフェクトで使うエミッターとパーティクルの初期化。
	//必ず呼んで。
	void Initialize();

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
	
	//エフェクトを発生させるかどうかのフラグ。
	//tureで発生。falseで発生させない。
	void SetEffectFlag(bool flag) {
		_ParticleEmitter->SetEmitFlg(flag);
	}

private:
	ParticleEmitter*	_ParticleEmitter = nullptr;				//パーティクル発生元。
	ParticleEmitter*	_HeelParticleAssistEmitter = nullptr;	//回復エフェクトを補助するパーティクルの発生元。
	ParticleParameter	_ParticleParam;							//パーティクルパラメーター。
	ParticleParameter	_HeelParticleAssistParam;				//回復エフェクトを補助するパーティクルパラメーター。
};

