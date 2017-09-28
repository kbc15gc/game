#include "stdafx.h"
#include "ParticleEffect.h"

void ParticleEffect::Awake() {
	_ParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("ParticleEffect", 8);
	_HeelHpParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("HeelParticleEffect", 8);
	_HeelHpParticleAssistEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("HeelParticleEffectAssist", 8);
}

void ParticleEffect::HeelHpEffect(Transform* parent) {
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


	_HeelHpParticleEmitter->transform->SetParent(parent);
	_HeelHpParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_HeelHpParticleEmitter->Init(_HeelHpParticleParam);

	//Hp回復エフェクトのアシストに使用するパーティクルパラメーターを設定。
	_HeelHpParticleAssistParam.Init();
	_HeelHpParticleAssistParam.texturePath = "ItemEfectBase.png";
	_HeelHpParticleAssistParam.alphaBlendMode = 1;
	_HeelHpParticleAssistParam.addVelocityRandomMargih = Vector3::zero;
	_HeelHpParticleAssistParam.brightness = 7.0f;
	_HeelHpParticleAssistParam.fadeTime = 1.0f;
	_HeelHpParticleAssistParam.gravity = 0.0f;
	_HeelHpParticleAssistParam.initAlpha = 1.0f;
	_HeelHpParticleAssistParam.initPositionRandomMargin = Vector3(1.0f, 1.0f, 1.0f);
	_HeelHpParticleAssistParam.initVelocity = Vector3::up * 3.0f;
	_HeelHpParticleAssistParam.initVelocityVelocityRandomMargin = Vector3::zero;
	_HeelHpParticleAssistParam.intervalTime = 0.001f;
	_HeelHpParticleAssistParam.isBillboard = true;
	_HeelHpParticleAssistParam.isFade = true;
	_HeelHpParticleAssistParam.life = 0.001f;
	_HeelHpParticleAssistParam.size = Vector2(0.03f, 0.03f);
	_HeelHpParticleAssistParam.mulColor = Color::yellow;


	_HeelHpParticleAssistEmitter->transform->SetParent(parent);
	_HeelHpParticleAssistEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.3f, 0.0f));
	_HeelHpParticleAssistEmitter->Init(_HeelHpParticleAssistParam);
	SetHeelHpEffectFlag(true);
}

void ParticleEffect::HeelMpEffect(Transform* parent) {
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


	_HeelMpParticleEmitter->transform->SetParent(parent);
	_HeelMpParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_HeelMpParticleEmitter->Init(_HeelMpParticleParam);

	//Mp回復エフェクトのアシストに使用するパーティクルパラメーターを設定。
	_HeelMpParticleAssistParam.Init();
	_HeelMpParticleAssistParam.texturePath = "ItemEfectBase.png";
	_HeelMpParticleAssistParam.alphaBlendMode = 1;
	_HeelMpParticleAssistParam.addVelocityRandomMargih = Vector3::zero;
	_HeelMpParticleAssistParam.brightness = 7.0f;
	_HeelMpParticleAssistParam.fadeTime = 1.0f;
	_HeelMpParticleAssistParam.gravity = 0.0f;
	_HeelMpParticleAssistParam.initAlpha = 1.0f;
	_HeelMpParticleAssistParam.initPositionRandomMargin = Vector3(1.0f, 1.0f, 1.0f);
	_HeelMpParticleAssistParam.initVelocity = Vector3::up * 3.0f;
	_HeelMpParticleAssistParam.initVelocityVelocityRandomMargin = Vector3::zero;
	_HeelMpParticleAssistParam.intervalTime = 0.001f;
	_HeelMpParticleAssistParam.isBillboard = true;
	_HeelMpParticleAssistParam.isFade = true;
	_HeelMpParticleAssistParam.life = 0.001f;
	_HeelMpParticleAssistParam.size = Vector2(0.03f, 0.03f);
	_HeelMpParticleAssistParam.mulColor = Color::yellow;


	_HeelMpParticleAssistEmitter->transform->SetParent(parent);
	_HeelMpParticleAssistEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.3f, 0.0f));
	_HeelMpParticleAssistEmitter->Init(_HeelMpParticleAssistParam);
	SetHeelMpEffectFlag(true);
}

void ParticleEffect::BuffEffect(Transform* parent) {
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

	_ParticleEmitter->transform->SetParent(parent);
	_ParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_ParticleEmitter->Init(_ParticleParam);
	SetEffectFlag(true);
}

void ParticleEffect::DeBuffEffect(Transform* parent) {
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

	_ParticleEmitter->transform->SetParent(parent);
	_ParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 1.6f, 0.0f));
	_ParticleEmitter->Init(_ParticleParam);
	SetEffectFlag(true);
}

void ParticleEffect::FireFly(Transform* parent)
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

	_ParticleEmitter->transform->SetParent(parent);
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