#include "stdafx.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"

// EnemyAttack。

void EnemyAttack::Init(float attackRange, int animType, float interpolate, int animLoopNum) {
	_AttackRange = attackRange;
	_animType = animType;
	_interpolate = interpolate;
	_animLoopNum = animLoopNum;
}


// EnemySingleAttack。

EnemySingleAttack::EnemySingleAttack(EnemyCharacter* object) :EnemyAttack(object) {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
}
void EnemySingleAttack::Entry() {
	_enemyObject->LookAtObject(*_player);
};

bool EnemySingleAttack::Update() {
	if (!_isPlaying) {
		// 攻撃モーション一度終了。
		return true;
	}
	return false;
}

// EnemyBreathAttack。
EnemyBreathAttack::EnemyBreathAttack(EnemyCharacter* object) :EnemyAttack(object) {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
}

void EnemyBreathAttack::Entry() {
	_enemyObject->LookAtObject(*_player);
}

bool EnemyBreathAttack::Update() {
	if (!_isPlaying) {
		// モーション再生終了。
		return true;
	}
	return false;
}
