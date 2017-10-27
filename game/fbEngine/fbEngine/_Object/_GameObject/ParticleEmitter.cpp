#include"fbstdafx.h"
#include "ParticleEmitter.h"
#include "Particle.h"

/*!
 * @brief	�p�[�e�B�N�������@
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

	//�p�[�e�B�N���̍폜�B

	if (_achievedArray) {
		// �p�[�e�B�N���擾�������B
		// �擾���ɍ폜���ꂽ�p�[�e�B�N���͏����B
		for (auto itr = _achievedArray->begin(); itr != _achievedArray->end();) {
			if ((*itr)->IsDead()) {
				itr = _achievedArray->erase(itr);
			}
			else {
				itr++;
			}
		}

		// �擾�������͎����폜�������ɂ�����ō폜����B
		list<Particle*>::iterator p = _ParticleList.begin();
		while (p != _ParticleList.end()) {
			//��
			if ((*p)->IsDead()) {
				INSTANCE(GameObjectManager)->AddRemoveList(*p);
				p = _ParticleList.erase(p);
			}
			//��
			else {
				p++;
			}
		}
	}
}

void ParticleEmitter::Render()
{
	//for (auto p : _ParticleList) {
	//	p->Render();
	//}
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

void ParticleEmitter::ResetParameterAlreadyCreated(const ParticleParameter& param) {
	_Param = param;
	for (auto particle : _ParticleList) {
		particle->SetParam(param);
	}
}

void ParticleEmitter::AchievedCreateParticleStart(vector<Particle*>* array) {
	_achievedArray = array;
	for (auto p : _ParticleList) {
		// �p�[�e�B�N���̎����폜���֎~����B
		p->SetIsAutoDelete(false);
	}
}

// ���������p�[�e�B�N���̎擾�I���B
void ParticleEmitter::AchievedCreateParticleEnd() {
	_achievedArray = nullptr;
	for (auto p : _ParticleList) {
		// �p�[�e�B�N���̎����폜��������B
		p->SetIsAutoDelete(true);
	}
}

void ParticleEmitter::SetEmitFlg(bool b)
{
	emit = b;
}
Particle* ParticleEmitter::GetParticleBegin()const {
	if (_ParticleList.size() >= 1) {
		// �p�[�e�B�N���͐������Ɋi�[����Ă���̂ŁA�擪�p�[�e�B�N�����擾�B
		return (*(_ParticleList.begin()));
	}
	else {
		return nullptr;
	}
}
Particle* ParticleEmitter::GetParticleEnd()const {
	if (_ParticleList.size() >= 1) {
		// �p�[�e�B�N���͐������Ɋi�[����Ă���̂ŁA���X�g�̏I�[�̈�O���擾�B
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
		// �����I�ɍ폜������B
		particle->SetIsAutoDelete(true);
	}
	_ParticleList.clear();
}

void ParticleEmitter::Emit()
{
	//����
	if (emit)
	{
		_Timer += Time::DeltaTime();
		if (_Timer >= _Param.intervalTime) {
			// PC�̏����\�͂��l���B
			short num = static_cast<short>(_Timer / _Param.intervalTime);
			// ��ȏ㐶������镪�̎��Ԃ�1�t���[���Ōo�߂��Ă����ꍇ������������B

			for (int idx = 0; idx < num; idx++) {
				//�p�[�e�B�N���𐶐��B
				Particle* p = INSTANCE(GameObjectManager)->AddNew<Particle>("particle", 8);
				if (_Param.isParent) {
					// �G�~�b�^�[��e�Ƃ��Đݒ�B
					p->transform->SetParent(transform);
					p->Init(_Param, Vector3::zero);
				}
				else {
					p->Init(_Param, transform->GetPosition());
				}
				_Timer = 0.0f;
				_ParticleList.push_back(p);
				if (_achievedArray) {
					// �����폜�֎~�B
					p->SetIsAutoDelete(false);
					_achievedArray->push_back(p);
				}
				else {
					p->SetIsAutoDelete(true);
				}
			}
		}
	}
}