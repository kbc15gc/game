#include "stdafx.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"

// EnemyAttackB

void EnemyAttack::Init(float attackRange, int animType, float interpolate, int animLoopNum) {
	_AttackRange = attackRange;
	_animType = animType;
	_interpolate = interpolate;
	_animLoopNum = animLoopNum;
}


// EnemySingleAttackB

EnemySingleAttack::EnemySingleAttack(EnemyCharacter* object) :EnemyAttack(object) {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
}
void EnemySingleAttack::Entry() {
	_enemyObject->LookAtObject(*_player);
};

bool EnemySingleAttack::Update() {
	return !_isPlaying;
}

// EnemyBreathAttackB
EnemyBreathAttack::EnemyBreathAttack(EnemyCharacter* object) :EnemyAttack(object) {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
}

void EnemyBreathAttack::Entry() {
	_enemyObject->LookAtObject(*_player);
}

bool EnemyBreathAttack::Update() {
	return !_isPlaying;
}
