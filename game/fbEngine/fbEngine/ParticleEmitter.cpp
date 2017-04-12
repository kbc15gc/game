#include "ParticleEmitter.h"
#include "Particle.h"

/*!
 * @brief	�p�[�e�B�N�������@
 */
void ParticleEmitter::Init(const ParicleParameter& _Param )
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
	//�p�[�e�B�N���̃A�b�v�f�[�g
	list<Particle*>::iterator p = _ParticleList.begin();
	while (p != _ParticleList.end()) {
		//��
		if ((*p)->IsDead()) {
			SAFE_DELETE(*p);
			p = _ParticleList.erase(p);
		}
		//��
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
	*@brief	�p�[�e�B�N���ɗ͂�������B
	*@_Param[in]	applyForce		���������Ɏg�p���闐�������@�B
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
void ParticleEmitter::Emit()
{
	//����
	if (emit)
	{
		_Timer += Time::DeltaTime();
		if (_Timer >= _Param.intervalTime) {
			//�p�[�e�B�N���𐶐��B
			Particle* p = new Particle("particle");
			p->Awake();
			p->Init(_Param, transform->GetPosition());
			_Timer = 0.0f;
			_ParticleList.push_back(p);
		}
	}
}