#include "stdafx.h"
#include "ParticleEffect.h"

void ParticleEffect::Awake() {
	_BuffParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BuffParticleEffect", 8);
	_DebuffParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("DebuffParticleEffect", 8);
	_HeelHpParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("HeelHPParticleEffect", 8);
	_HeelMpParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("HeelMpParticleEffect", 8);
	_HeelParticleAssistEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("HeelParticleEffectAssist", 8);

	_LevelUPParticleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("LevelUPEffect", 8);

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
	_HeelParticleAssistParam.isParent = true;
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
	_HeelHpParticleParam.isParent = true;

	//Hp回復に使用するエミッターを設定。
	_HeelHpParticleEmitter->transform->SetParent(transform);
	_HeelHpParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_HeelHpParticleEmitter->Init(_HeelHpParticleParam);


	//回復エフェクトのアシストエミッター設定。
	_HeelParticleAssistEmitter->transform->SetParent(transform);
	_HeelParticleAssistEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.3f, 0.0f));
	_HeelParticleAssistEmitter->Init(_HeelParticleAssistParam);

	//Hp回復エフェクトのエミット開始。
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

	//Mp回復に使用するエミッターを設定。
	_HeelMpParticleEmitter->transform->SetParent(transform);
	_HeelMpParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_HeelMpParticleEmitter->Init(_HeelMpParticleParam);

	//回復エフェクトのアシストエミッター設定。
	_HeelParticleAssistEmitter->transform->SetParent(transform);
	_HeelParticleAssistEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.3f, 0.0f));
	_HeelParticleAssistEmitter->Init(_HeelParticleAssistParam);

	//Mp回復エフェクトのエミット開始。
	SetHeelMpEffectFlag(true);
}

void ParticleEffect::BuffEffect() {
	//バフに使用するパーティクルパラメーターを設定。
	_BuffParticleParam.Init();
	_BuffParticleParam.texturePath = "ItemEfectBase.png";
	_BuffParticleParam.alphaBlendMode = 3;
	_BuffParticleParam.addVelocityRandomMargih = Vector3::zero;
	_BuffParticleParam.brightness = 1.0f;
	_BuffParticleParam.fadeTime = 0.1f;
	_BuffParticleParam.gravity = 0.0f;
	_BuffParticleParam.initAlpha = 1.0f;
	_BuffParticleParam.initPositionRandomMargin = Vector3(0.5f, 0.5f, 0.5f);
	_BuffParticleParam.initVelocity = Vector3::up * 5.0f;
	_BuffParticleParam.initVelocityVelocityRandomMargin = Vector3::up;
	_BuffParticleParam.intervalTime = 0.01f;
	_BuffParticleParam.isBillboard = true;
	_BuffParticleParam.isFade = true;
	_BuffParticleParam.life = 0.001f;
	_BuffParticleParam.size = Vector2(0.01f, 0.3f);
	_BuffParticleParam.mulColor = Color::red;
	_BuffParticleParam.isParent = true;

	_BuffParticleEmitter->transform->SetParent(transform);
	_BuffParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_BuffParticleEmitter->Init(_BuffParticleParam);
	SetBuffEffectFlag(true);
}

void ParticleEffect::DeBuffEffect() {
	//デバフに使用するパーティクルパラメーターを設定。
	_DebuffParticleParam.Init();
	_DebuffParticleParam.texturePath = "ItemEfectBase.png";
	_DebuffParticleParam.alphaBlendMode = 3;
	_DebuffParticleParam.addVelocityRandomMargih = Vector3::zero;
	_DebuffParticleParam.brightness = 1.0f;
	_DebuffParticleParam.fadeTime = 0.1f;
	_DebuffParticleParam.gravity = 0.0f;
	_DebuffParticleParam.initAlpha = 1.0f;
	_DebuffParticleParam.initPositionRandomMargin = Vector3(0.5f, 0.5f, 0.5f);
	_DebuffParticleParam.initVelocity = Vector3::down * 5.0f;
	_DebuffParticleParam.initVelocityVelocityRandomMargin = Vector3::down;
	_DebuffParticleParam.intervalTime = 0.01f;
	_DebuffParticleParam.isBillboard = true;
	_DebuffParticleParam.isFade = true;
	_DebuffParticleParam.life = 0.001f;
	_DebuffParticleParam.size = Vector2(0.01f, 0.3f);
	_DebuffParticleParam.mulColor = Color::blue;
	_DebuffParticleParam.isParent = true;

	_DebuffParticleEmitter->transform->SetParent(transform);
	_DebuffParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 2.4f, 0.0f));
	_DebuffParticleEmitter->Init(_DebuffParticleParam);
	SetDebuffEffectFlag(true);
}

void ParticleEffect::FireFly()
{	
	//水辺の蛍にどうぞ。
	_FireFlyParticleParam.Init();
	_FireFlyParticleParam.texturePath = "ItemEfectBase.png";
	_FireFlyParticleParam.alphaBlendMode = 1;
	_FireFlyParticleParam.addVelocityRandomMargih = Vector3::up;
	_FireFlyParticleParam.brightness = 5.0f;
	_FireFlyParticleParam.fadeTime = 0.5f;
	_FireFlyParticleParam.gravity = 0.0f;
	_FireFlyParticleParam.initAlpha = 1.0f;
	_FireFlyParticleParam.initPositionRandomMargin = Vector3(10.0f, 10.0f, 10.0f);
	_FireFlyParticleParam.initVelocity = Vector3::up * 2.0f;
	_FireFlyParticleParam.initVelocityVelocityRandomMargin = Vector3(-10.0f, -10.0f,-10.0f);
	_FireFlyParticleParam.intervalTime = 0.01f;
	_FireFlyParticleParam.isBillboard = true;
	_FireFlyParticleParam.isFade = true;
	_FireFlyParticleParam.life = 0.01f;
	_FireFlyParticleParam.size = Vector2(0.1f, 0.1f);
	_FireFlyParticleParam.mulColor = Color::green;

	_FireFlyParticleEmitter->transform->SetParent(transform);
	_FireFlyParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_FireFlyParticleEmitter->Init(_FireFlyParticleParam);
	SetFireFlyEffectFlag(true);
}

void ParticleEffect::LevelUpEffect()
{
	//バフに使用するパーティクルパラメーターを設定。
	_LevelUPParticleParam.Init();
	_LevelUPParticleParam.texturePath = "par.png";
	_LevelUPParticleParam.alphaBlendMode = 1;
	_LevelUPParticleParam.addVelocityRandomMargih = Vector3::zero;
	_LevelUPParticleParam.brightness = 5.0f;
	_LevelUPParticleParam.fadeTime = 0.5f;
	_LevelUPParticleParam.gravity = 0.0f;
	_LevelUPParticleParam.initAlpha = 1.0f;
	_LevelUPParticleParam.initPositionRandomMargin = Vector3(0.5f, 0.5f, 0.5f);
	_LevelUPParticleParam.initVelocity = Vector3::up * 5.0f;
	_LevelUPParticleParam.initVelocityVelocityRandomMargin = Vector3(2.0f, 1.0f, 2.0f);
	_LevelUPParticleParam.intervalTime = 0.01f;
	_LevelUPParticleParam.isBillboard = true;
	_LevelUPParticleParam.isFade = true;
	_LevelUPParticleParam.life = 0.001f;
	_LevelUPParticleParam.size = Vector2(0.1f, 0.1f);
	_LevelUPParticleParam.mulColor = Color::yellow;
	_LevelUPParticleParam.isParent = true;

	_LevelUPParticleEmitter->transform->SetParent(transform);
	_LevelUPParticleEmitter->transform->SetLocalPosition(Vector3(0.0f, 0.6f, 0.0f));
	_LevelUPParticleEmitter->Init(_LevelUPParticleParam);
	SetLevelUPEffectFlag(true);
}

void ParticleEffect::Update() {

	//Hp回復エフェクトを消す処理。
	if (_IsHeelHpFlag) {
		_TotalHeelHpEffectTime += Time::DeltaTime();
		if (_TotalHeelHpEffectTime > 1.0f) {
			SetHeelHpEffectFlag(false);
			_TotalHeelHpEffectTime = 0.0f;
		}
	}

	//Mp回復エフェクトを消す処理。
	if (_IsHeelMpFlag) {
		_TotalHeelMpEffectTime += Time::DeltaTime();
		if (_TotalHeelMpEffectTime > 1.0f) {
			SetHeelMpEffectFlag(false);
			_TotalHeelMpEffectTime = 0.0f;
		}
	}

	//レベルアップエフェクトを消す処理。
	if (_IsLevelUPFlag) {
		_TotalLevelUPEffectTime += Time::DeltaTime();
		if (_TotalLevelUPEffectTime > 2.0f) {
			SetLevelUPEffectFlag(false);
			_TotalLevelUPEffectTime = 0.0f;
		}
	}
}