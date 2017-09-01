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
	//パーティクルのアップデート
	list<Particle*>::iterator p = _ParticleList.begin();
	while (p != _ParticleList.end()) {
		//死
		if ((*p)->IsDead()) {
			SAFE_DELETE(*p);
			p = _ParticleList.erase(p);
		}
		//生
		else {
			(*p)->Update();
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

void ParticleEmitter::Emit()
{
	//生成
	if (emit)
	{
		_Timer += Time::DeltaTime();
		if (_Timer >= _Param.intervalTime) {
			//パーティクルを生成。
			Particle* p = new Particle("particle");
			p->Awake();
			p->Init(_Param, transform->GetPosition());
			_Timer = 0.0f;
			_ParticleList.push_back(p);
		}
	}
}