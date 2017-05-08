/**
* �V���h�E�}�b�v�N���X�̎���.
*/
#include"ShadowMap.h"

namespace
{
	const float ShadowAreaTable[ShadowMap::SHADOWMAP_NUM][2] =
	{
		{ 20.0f	, 20.0f },
		{ 45.0f	, 45.0f },
		{ 120.0f, 120.0f },
	};
}

/**
* �R���X�g���N�^. 
*/
ShadowMap::ShadowMap()
{
	FOR(count, SHADOWMAP_NUM)
	{
		_ShadowArea[count].x = ShadowAreaTable[count][0];
		_ShadowArea[count].y = ShadowAreaTable[count][1];

		D3DXMatrixIdentity(&_LPMatrix[count]);
	}
	D3DXMatrixIdentity(&_LVMatrix);
}

/**
* �쐬.
*/
void ShadowMap::Create()
{
	int w, h;
	w = h = 2048;

	//�A�X�y�N�g��.
	_Aspect = (float)w / h;

	int size[SHADOWMAP_NUM][2] = 
	{
		{ w		,h		},
		{ w >> 1,h >> 1 },
		{ w >> 1,h >> 1 },
	};

	FOR(count, SHADOWMAP_NUM)
	{
		_ShadowMapRT[count].Create(Vector2(size[count][0], size[count][1]), D3DFMT_G16R16F, D3DFMT_D16);

		_Blur[count].Create(size[count][0], size[count][1], _ShadowMapRT[count].texture);
	}

}

/**
* �X�V.
*/
void ShadowMap::Update()
{
	//���_�ƒ����_�ŕ������v�Z.
	_LightDirection.Subtract(_LightTarget, _LightPosition);
	_LightDirection.Normalize();

	//���C�g�̏����.
	Vector3 lightUp;
	
	//�����̊p�x�����߂�.
	float angle = fabsf(_LightDirection.Dot(Vector3::up));

	if (fabsf((angle - 1.0f)) < 0.00001f)
	{
		lightUp = Vector3::right;
	}
	else
	{
		lightUp = Vector3::up;
	}

	//���C�g�̒����_.
	Vector3 lightTarget;
	lightTarget.Add(_LightPosition, _LightDirection);

	//���C�g�r���[�s��̌v�Z.
	D3DXMatrixLookAtLH(&_LVMatrix, (D3DXVECTOR3*)&_LightPosition, (D3DXVECTOR3*)&lightTarget, (D3DXVECTOR3*)&lightUp);
	_ShadowReceiverParam._LVMatrix = _LVMatrix;

	FOR(count, SHADOWMAP_NUM)
	{
		//���s���e�v���W�F�N�V�����s����v�Z
#ifdef NEKOMATA
		if (count == 0)
		{
			D3DXMatrixOrthoOffCenterLH(&_LPMatrix[count],
				0,
				_ShadowArea[count].x * _Aspect,
				_ShadowArea[count].y,
				0,
				_Near,
				_Far);
		}
		else
		{
			D3DXMatrixOrthoOffCenterLH(&_LPMatrix[count],
				_ShadowArea[count - 1].x * _Aspect,
				_ShadowArea[count].x * _Aspect,
				_ShadowArea[count].y,
				_ShadowArea[count - 1].y,
				_Near,
				_Far);
		}
#else
		D3DXMatrixOrthoLH(&_LPMatrix[count], ShadowAreaTable[count][0] * _Aspect, ShadowAreaTable[count][1], _Near, _Far);
#endif
		_ShadowReceiverParam._LPMatrix[count] = _LPMatrix[count];
	}
}

/**
* �`��.
*/
void ShadowMap::Render()
{
	//�w�ʂ̃J�����O�͂��Ȃ�.
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	FOR(count, SHADOWMAP_NUM)
	{
		INSTANCE(RenderTargetManager)->SetRT(0, &_ShadowMapRT[count]);

		(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	
		_NowRenderShadowMap = count;

		for (auto& it : _CasterModelList)
		{
			//���f���`��.
			it->RenderToShadowMap();
		}

		_Blur[count].Render();
	}

	//�w�ʂ������ŃJ�����O����.
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
	//�S�Ẵ��f�������X�g����폜.
	_CasterModelList.clear();

}

/**
* ���.
*/
void ShadowMap::Release()
{
}
