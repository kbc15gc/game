#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Ground : public GameObject
{
public:
	Ground(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;

	void SetIsDL(bool is)
	{
		_Model->SetModelEffect(ModelEffectE::RECEIVE_DirectionalLight, is);
	}

private:

	SkinModel* _Model = nullptr;

};