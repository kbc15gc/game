#pragma once

#include "BreathObject.h"


class LaserBreath :
	public BreathObject
{
public:
	// 引数はオブジェクトの名前。
	LaserBreath(const char* name) :BreathObject(name) {

	}
	~LaserBreath() {
		INSTANCE(GameObjectManager)->AddRemoveList(_particleEmitter);
	}

	// ブレスオブジェクトを与えたパラメータで作成。
	// 引数：	ブレスを出すキャラクター。
	//			ブレス発生位置(ローカル座標、親はキャラクターのTransform)。
	//			ブレスのスピード。
	//			ブレスの粒子生成待ち時間。
	//			どの軸で回すか。
	//			回転量(キャラクターの正面から何度回すか)。
	//			乗算カラー。
	//			攻撃倍率。
	void Create(EnemyCharacter* obj, const Vector3& emitPosLocal,float power,float interval, const Vector3& axis,float deg,const char* fileName = "t1.png",const Vector2& size = Vector2(0.5f, 0.5f), const float brightness = 2.0f,Color mul = Color::white,int atk = 100);

	void Awake()override;

	void Update()override;

	// ブレス発射開始。
	void _BreathStartSubClass()override;

	// ブレス発射終了。
	inline void _BreathEndSubClass()override {
		testBreathEnd = true;
		_isCreateBreathEnd = true;
		_particleEmitter->SetEmitFlg(false);
		_particleEmitter->AchievedCreateParticleEnd();
		_particleEmitter->SetActive(false);
	}

	void BreathStop()override;

	void BreathPlay()override;


private:
	// 衝突判定コリジョンの更新。
	void _UpdateCollision()override;


private:
	Particle* _start = nullptr;	// 攻撃時、最初に生成されたパーティクル(ブレスの先頭)。
	Particle* _end = nullptr;	// 攻撃時、最後に生成されたパーティクル(ブレスの終端)。
	Vector3 _breathScale;

	ParticleEmitter* _particleEmitter = nullptr;
	ParticleParameter _initParticleParam;
	Vector3 _velocity;
	float _rad;		// キャラクターの正面からどれくらい回したブレスか。
	Vector3 _axis;	// どの軸を中心に回したか。
	Color _mulColor = Color::white;
	float _power;
	int _atk;
	
public:
	bool testBreathEnd = false;
};