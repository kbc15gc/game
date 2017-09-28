#include"fbstdafx.h"
#include "ParticleEmitter.h"
#include "Particle.h"

/*!
 * @brief	パーティクル生成機
 */
void ParticleEmitter::Init(const ParticleParameter& _Param )
{
	this->_Param = _Param;
	_Timer = _Param.intervalTime;
}
void ParticleEmitter::Start()
{
}
void ParticleEmitter::Update()
{
	Emit();

	//パーティクルの削除。

	if (_achievedArray) {
		// パーティクル取得処理中。
		// 取得中に削除されたパーティクルは除く。
		for (auto itr = _achievedArray->begin();itr != _achievedArray->end();) {
			if ((*itr)->IsDead()) {
				itr = _achievedArray->erase(itr);
			}
			else {
				itr++;
			}
		}
	}
	list<Particle*>::iterator p = _ParticleList.begin();
	while (p != _ParticleList.end()) {
		//死
		if ((*p)->IsDead()) {
			INSTANCE(GameObjectManager)->AddRemoveList(*p);
			p = _ParticleList.erase(p);
		}
		//生
		else {
			p++;
		}
	}
}

void ParticleEmitter::Render()
{
	for (auto p : _ParticleList) {
		p->Render();
	}
}


	/*!
	*@brief	パーティクルに力を加える。
	*@_Param[in]	applyForce		乱数生成に使用する乱数生成機。
	*/
void ParticleEmitter::ApplyForce(Vector3& applyForce)
{
	for (auto p : _ParticleList) {
		p->ApplyForce(applyForce);
	}
}

void ParticleEmitter::ResetParameterAlreadyCreated(const ParticleParameter& param) {
	_Param = param;
	for (auto particle : _ParticleList) {
		particle->SetParam(param);
	}
}

void ParticleEmitter::SetEmitFlg(bool b)
{
	emit = b;
}
Particle* ParticleEmitter::GetParticleBegin()const {
	if (_ParticleList.size() >= 1) {
		// パーティクルは生成順に格納されているので、先頭パーティクルを取得。
		return (*(_ParticleList.begin()));
	}
	else {
		return nullptr;
	}
}
Particle* ParticleEmitter::GetParticleEnd()const {
	if (_ParticleList.size() >= 1) {
		// パーティクルは生成順に格納されているので、リストの終端の一つ前を取得。
		return (*(--_ParticleList.end()));
	}
	else {
		return nullptr;
	}
}

void ParticleEmitter::ReleaseParticleAll() {
	if (_achievedArray) {
		_achievedArray->clear();
		_achievedArray = nullptr;
	}
	for (auto particle : _ParticleList) {
		INSTANCE(GameObjectManager)->AddRemoveList(particle);
	}
	_ParticleList.clear();
}

void ParticleEmitter::Emit()
{
	//生成
	if (emit)
	{
		_Timer += Time::DeltaTime();
		if (_Timer >= _Param.intervalTime) {
			//パーティクルを生成。
			Particle* p = INSTANCE(GameObjectManager)->AddNew<Particle>("particle",8);
			if (_Param.isParent) {
				// エミッターを親として設定。
				p->transform->SetParent(transform);
				p->Init(_Param, Vector3::zero);
			}
			else {
				p->Init(_Param, transform->GetPosition());
			}
			_Timer = 0.0f;
			_ParticleList.push_back(p);
			if (_achievedArray) {
				_achievedArray->push_back(p);
			}
		}
	}
}