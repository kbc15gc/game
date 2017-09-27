#include "stdafx.h"
#include "ParticleEffect.h"

void ParticleEffect::Initialize() {
	_ParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("ParticleEffect", 8);
	_HeelParticleAssistEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("HeelParticleEffectAssist", 8);
	_ParticleParam.Init();
}

void ParticleEffect::HeelEffect(Transform* parent) {
	//回復に使用するパーティクルパラメーターを設定。
	_ParticleParam.Init();
	_ParticleParam.texturePath = "ItemEfectBase.png";
	_ParticleParam.alphaBlendMode = 1;
	_ParticleParam.addVelocityRandomMargih = Vector3::zero;
	_ParticleParam.brightness = 10.0f;
	_ParticleParam.fadeTime = 1.0f;
	_ParticleParam.gravity = 0.0f;
	_ParticleParam.initAlpha = 1.0f;
	_ParticleParam.initPositionRandomMargin = Vector3(1.0f, 1.0f, 1.0f);
	_ParticleParam.initVelocity = Vector3::up * 3.0f;
	_ParticleParam.initVelocityVelocityRandomMargin = Vector3::zero;
	_ParticleParam.intervalTime = 0.07f;
	_ParticleParam.isBillboard = true;
	_ParticleParam.isFade = true;
	_ParticleParam.life = 0.001f;
	_ParticleParam.size = Vector2(0.05f, 0.05f);
	_ParticleParam.mulColor = Color::green;


	_ParticleEmitter->transform->SetParent(parent);
	_ParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_ParticleEmitter->Init(_ParticleParam);
	SetEffectFlag(true);

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
	_HeelParticleAssistParam.size = Vector2(0.01f, 0.01f);
	_HeelParticleAssistParam.mulColor = Color::yellow;


	_HeelParticleAssistEmitter->transform->SetParent(parent);
	_HeelParticleAssistEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
	_HeelParticleAssistEmitter->Init(_HeelParticleAssistParam);
	SetEffectFlag(true);
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