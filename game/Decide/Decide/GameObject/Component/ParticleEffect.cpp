#include "stdafx.h"
#include "ParticleEffect.h"

void ParticleEffect::Awake() {
	_ParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("ParticleEffect", 8);
	_HeelHpParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("HeelHPParticleEffect", 8);
	_HeelMpParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("HeelMpParticleEffect", 8);
	_HeelParticleAssistEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("HeelParticleEffectAssist", 8);

	//回復エフェクトのアシストに使用するパーティクルパラメーターを設定。
	_HeelParticleAssistParam.Init();
	_HeelParticleAssistParam.texturePath = "ItemEfectBase.png";
	_HeelParticleAssistParam.alphaBlendMode = 1;
	_HeelParticleAssistParam.addVelocityRandomMargih = Vector3::zero;
	_HeelParticleAssistParam.brightness = 7.0f;
	_HeelParticleAssistParam.fadeTime = 1.0f;
	_HeelParticleAssistParam.gravity = 0.0f;
	_HeelParticleAssistParam.initAlpha = 1.0f;
	_HeelParticleAssistParam.initPositionRandomMargin = Vector3(1.0f, 1.0f, 1.0f);
	_HeelParticleAssistParam.initVelocity = Vector3::up * 3.0f;
	_HeelParticleAssistParam.initVelocityVelocityRandomMargin = Vector3::zero;
	_HeelParticleAssistParam.intervalTime = 0.001f;
	_HeelParticleAssistParam.isBillboard = true;
	_HeelParticleAssistParam.isFade = true;
	_HeelParticleAssistParam.life = 0.001f;
	_HeelParticleAssistParam.size = Vector2(0.03f, 0.03f);
	_HeelParticleAssistParam.mulColor = Color::yellow;
}

void ParticleEffect::HeelHpEffect() {
	//Hp回復に使用するパーティクルパラメーターを設定。
	_HeelHpParticleParam.Init();
	_HeelHpParticleParam.texturePath = "ItemEfectBase.png";
	_HeelHpParticleParam.alphaBlendMode = 1;
	_HeelHpParticleParam.addVelocityRandomMargih = Vector3::zero;
	_HeelHpParticleParam.brightness = 10.0f;
	_HeelHpParticleParam.fadeTime = 1.0f;
	_HeelHpParticleParam.gravity = 0.0f;
	_HeelHpParticleParam.initAlpha = 1.0f;
	_HeelHpParticleParam.initPositionRandomMargin = Vector3(1.0f, 1.0f, 1.0f);
	_HeelHpParticleParam.initVelocity = Vector3::up * 3.0f;
	_HeelHpParticleParam.initVelocityVelocityRandomMargin = Vector3::zero;
	_HeelHpParticleParam.intervalTime = 0.07f;
	_HeelHpParticleParam.isBillboard = true;
	_HeelHpParticleParam.isFade = true;
	_HeelHpParticleParam.life = 0.001f;
	_HeelHpParticleParam.size = Vector2(0.05f, 0.05f);
	_HeelHpParticleParam.mulColor = Color::green;


	_HeelHpParticleEmitter->transform->SetParent(transform);
	_HeelHpParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_HeelHpParticleEmitter->Init(_HeelHpParticleParam);


	_HeelParticleAssistEmitter->transform->SetParent(transform);
	_HeelParticleAssistEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.3f, 0.0f));
	_HeelParticleAssistEmitter->Init(_HeelParticleAssistParam);
	SetHeelHpEffectFlag(true);
}

void ParticleEffect::HeelMpEffect() {
	//Mp回復に使用するパーティクルパラメーターを設定。
	_HeelMpParticleParam.Init();
	_HeelMpParticleParam.texturePath = "ItemEfectBase.png";
	_HeelMpParticleParam.alphaBlendMode = 1;
	_HeelMpParticleParam.addVelocityRandomMargih = Vector3::zero;
	_HeelMpParticleParam.brightness = 10.0f;
	_HeelMpParticleParam.fadeTime = 1.0f;
	_HeelMpParticleParam.gravity = 0.0f;
	_HeelMpParticleParam.initAlpha = 1.0f;
	_HeelMpParticleParam.initPositionRandomMargin = Vector3(1.0f, 1.0f, 1.0f);
	_HeelMpParticleParam.initVelocity = Vector3::up * 3.0f;
	_HeelMpParticleParam.initVelocityVelocityRandomMargin = Vector3::zero;
	_HeelMpParticleParam.intervalTime = 0.07f;
	_HeelMpParticleParam.isBillboard = true;
	_HeelMpParticleParam.isFade = true;
	_HeelMpParticleParam.life = 0.001f;
	_HeelMpParticleParam.size = Vector2(0.05f, 0.05f);
	_HeelMpParticleParam.mulColor = Color::blue * 0.7f;


	_HeelMpParticleEmitter->transform->SetParent(transform);
	_HeelMpParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_HeelMpParticleEmitter->Init(_HeelMpParticleParam);


	_HeelParticleAssistEmitter->transform->SetParent(transform);
	_HeelParticleAssistEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.3f, 0.0f));
	_HeelParticleAssistEmitter->Init(_HeelParticleAssistParam);
	SetHeelMpEffectFlag(true);
}

void ParticleEffect::BuffEffect() {
	//バフに使用するパーティクルパラメーターを設定。
	_ParticleParam.Init();
	_ParticleParam.texturePath = "ItemEfectBase.png";
	_ParticleParam.alphaBlendMode = 3;
	_ParticleParam.addVelocityRandomMargih = Vector3::zero;
	_ParticleParam.brightness = 1.0f;
	_ParticleParam.fadeTime = 0.5f;
	_ParticleParam.gravity = 0.0f;
	_ParticleParam.initAlpha = 1.0f;
	_ParticleParam.initPositionRandomMargin = Vector3(0.5f, 0.5f, 0.5f);
	_ParticleParam.initVelocity = Vector3::up * 5.0f;
	_ParticleParam.initVelocityVelocityRandomMargin = Vector3::up;
	_ParticleParam.intervalTime = 0.01f;
	_ParticleParam.isBillboard = true;
	_ParticleParam.isFade = true;
	_ParticleParam.life = 0.01f;
	_ParticleParam.size = Vector2(0.01f, 0.3f);
	_ParticleParam.mulColor = Color::red;

	_ParticleEmitter->transform->SetParent(transform);
	_ParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_ParticleEmitter->Init(_ParticleParam);
	SetEffectFlag(true);
}

void ParticleEffect::DeBuffEffect() {
	//デバフに使用するパーティクルパラメーターを設定。
	_ParticleParam.Init();
	_ParticleParam.texturePath = "ItemEfectBase.png";
	_ParticleParam.alphaBlendMode = 3;
	_ParticleParam.addVelocityRandomMargih = Vector3::zero;
	_ParticleParam.brightness = 1.0f;
	_ParticleParam.fadeTime = 0.5f;
	_ParticleParam.gravity = 0.0f;
	_ParticleParam.initAlpha = 1.0f;
	_ParticleParam.initPositionRandomMargin = Vector3(0.5f, 0.5f, 0.5f);
	_ParticleParam.initVelocity = Vector3::down * 5.0f;
	_ParticleParam.initVelocityVelocityRandomMargin = Vector3::down;
	_ParticleParam.intervalTime = 0.01f;
	_ParticleParam.isBillboard = true;
	_ParticleParam.isFade = true;
	_ParticleParam.life = 0.01f;
	_ParticleParam.size = Vector2(0.01f, 0.3f);
	_ParticleParam.mulColor = Color::blue;

	_ParticleEmitter->transform->SetParent(transform);
	_ParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 1.6f, 0.0f));
	_ParticleEmitter->Init(_ParticleParam);
	SetEffectFlag(true);
}

void ParticleEffect::FireFly()
{	
	//水辺の蛍にどうぞ。
	_ParticleParam.Init();
	_ParticleParam.texturePath = "ItemEfectBase.png";
	_ParticleParam.alphaBlendMode = 1;
	_ParticleParam.addVelocityRandomMargih = Vector3::up;
	_ParticleParam.brightness = 5.0f;
	_ParticleParam.fadeTime = 0.5f;
	_ParticleParam.gravity = 0.0f;
	_ParticleParam.initAlpha = 1.0f;
	_ParticleParam.initPositionRandomMargin = Vector3(10.0f, 10.0f, 10.0f);
	_ParticleParam.initVelocity = Vector3::up * 2.0f;
	_ParticleParam.initVelocityVelocityRandomMargin = Vector3(-10.0f, -10.0f,-10.0f);
	_ParticleParam.intervalTime = 0.01f;
	_ParticleParam.isBillboard = true;
	_ParticleParam.isFade = true;
	_ParticleParam.life = 0.01f;
	_ParticleParam.size = Vector2(0.1f, 0.1f);
	_ParticleParam.mulColor = Color::green;

	_ParticleEmitter->transform->SetParent(transform);
	_ParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_ParticleEmitter->Init(_ParticleParam);
	SetEffectFlag(true);
}

void ParticleEffect::Update() {
	if (_IsHeelHpFlag) {
		_TotalHeelHpEffectTime += Time::DeltaTime();
		if (_TotalHeelHpEffectTime > 1.0f) {
			SetHeelHpEffectFlag(false);
			_TotalHeelHpEffectTime = 0.0f;
		}
	}

	if (_IsHeelMpFlag) {
		_TotalHeelMpEffectTime += Time::DeltaTime();
		if (_TotalHeelMpEffectTime > 1.0f) {
			SetHeelMpEffectFlag(false);
			_TotalHeelMpEffectTime = 0.0f;
		}
	}
}