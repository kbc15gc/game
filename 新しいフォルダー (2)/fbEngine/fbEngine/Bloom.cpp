#include "Bloom.h"
#include "Effect.h"
#include "RenderTargetManager.h"
#include "Vertex.h"
#include "VertexDefinition.h"

void Bloom::Create()
{
	if (_Vertex == nullptr)
	{
		_Vertex = new Vertex();

		//�|�W�V������`
		VERTEX_POSITION position[] = {
			{ -1.0f, -1.0f, 0.0f, 1.0f },//����
			{ -1.0f, 1.0f, 0.0f, 1.0f },//����
			{ 1.0f, -1.0f, 0.0f, 1.0f },//�E��
			{ 1.0f, 1.0f, 0.0f, 1.0f },//�E��
		};
		//UV��`
		VERTEX_TEXCOORD texcoord[] = {
			{ 0.0f, 1.0f },//����
			{ 0.0f, 0.0f },//����
			{ 1.0f, 1.0f },//�E��
			{ 1.0f, 0.0f },//�E��
		};

		//���_�錾(���_���ǂ̂悤�ɍ\������Ă��邩)
		D3DVERTEXELEMENT9 elements[] = {
			{ 0, 0              , D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // ���_���W
			{ 1, 0              , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD   , 0 }, // UV
			D3DDECL_END()
		};

		_Vertex->Initialize(fbEngine::PRIMITIVETYPE::TRIANGLESTRIP, 4);
		_Vertex->CreateVertexBuffer(position, 4, sizeof(VERTEX_POSITION), elements[0]);
		_Vertex->CreateVertexBuffer(texcoord, 4, sizeof(VERTEX_TEXCOORD), elements[1]);
		_Vertex->CreateDeclaration();
	}

	//�L���t���O���R�s�[
	_IsEnable = true;

	if (_IsEnable)
	{
		//�����𑜓x�̃T�C�Y���v�Z
		int w = g_WindowSize.x / 2;
		int h = g_WindowSize.y / 2;

		//�P�x�����_�����O�^�[�Q�b�g�̍쐬
		_LuminanceRT = new RenderTarget();
		_LuminanceRT->Create(g_WindowSize, D3DFMT_A16B16G16R16F);

		for (int i = 0; i < NUM_DOWN_SAMPLING_RT / 2; i++)
		{
			//�V�t�g�ʂ̌v�Z
			int shift = i + 1;

			//�_�E���T���v�����O�pRT�̓Y�������v�Z
			int baseIndex = i * 2;

			//�V�t�g���ăT�C�Y�����������Ă���(�k���o�b�t�@�̍쐬)

			//���u���[�pRT�̍쐬
			_DownSamplingRT[baseIndex] = new RenderTarget();
			_DownSamplingRT[baseIndex]->Create(Vector2(w >> shift, h >> (shift - 1)), D3DFMT_A16B16G16R16F);
			//�c�u���[�pRT�̍쐬
			_DownSamplingRT[baseIndex + 1] = new RenderTarget();
			_DownSamplingRT[baseIndex + 1]->Create(Vector2(w >> shift, h >> shift), D3DFMT_A16B16G16R16F);
		}

		//�ڂ��������pRT�̍쐬
		_CombineRT = new RenderTarget();
		_CombineRT->Create(Vector2(w >> 2, h >> 2), D3DFMT_A16B16G16R16F);

		//�G�t�F�N�g�̃��[�h
		_Effect = EffectManager::LoadEffect("Bloom.fx");
	}
}

void Bloom::Render()
{
	if (_IsEnable)
	{
		//�d�݌v�Z
		_UpdateWeight(25.0f);

		//�P�x���o
		{

			//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
			(*graphicsDevice()).SetRenderTarget(0, _LuminanceRT->buffer);
			(*graphicsDevice()).SetDepthStencilSurface(_LuminanceRT->depth);

			//�e�N�X�`���̃N���A
			(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);

			_Effect->SetTechnique("SamplingLuminance");

			_Effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			_Effect->BeginPass(0);

			//�e�N�X�`���̐ݒ�
			//�I�t�X�N���[���������
			_Effect->SetTexture("g_Scene", INSTANCE(SceneManager)->GetOffScreenTexture()->pTexture);

			_Effect->CommitChanges();

			//�摜�`��
			_Vertex->DrawPrimitive();

			_Effect->EndPass();
			_Effect->End();

		}//�P�x���o
		D3DXVECTOR3 l, r;
		l = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
		r = D3DXVECTOR3(0.2125f, 0.7154f, 0.0721f);
		float a = D3DXVec3Dot(&l, &r);

		 //�P�x���ڂ���
		{
			//���[�v�pRT�N���X�̃|�C���^
			RenderTarget* prevRT = _LuminanceRT;
			//�_�E���T���v�����O�pRT�̓Y����
			int rtIndex = 0;

			for (int i = 0; i < NUM_DOWN_SAMPLING_RT / 2; i++)
			{
				//XBlur
				{
					//�_�E���T���v�����O�pRT��XBlur�p�������_�����O�^�[�Q�b�g�ɐݒ�
					(*graphicsDevice()).SetRenderTarget(0, _DownSamplingRT[rtIndex]->buffer);
					(*graphicsDevice()).SetDepthStencilSurface(_DownSamplingRT[rtIndex]->depth);

					//�摜���N���A
					(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);

					//�e�N�j�b�N��ݒ�
					_Effect->SetTechnique("XBlur");

					_Effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
					_Effect->BeginPass(0);

					//�摜�T�C�Y���v�Z
					float size[2] =
					{
						static_cast<float>(prevRT->texture->Size.x),
						static_cast<float>(prevRT->texture->Size.y)
					};
					//�I�t�Z�b�g���v�Z
					float offset[] =
					{
						16.0f / static_cast<float>(prevRT->texture->Size.x),
						0.0f
					};

					//��ʃT�C�Y��ݒ�
					_Effect->SetValue("g_LuminanceTexSize", size, sizeof(size));
					//�I�t�Z�b�g��ݒ�
					_Effect->SetValue("g_Offset", offset, sizeof(size));
					//�d�݂�ݒ�
					_Effect->SetValue("g_Weight", _Weights, sizeof(_Weights));

					//�e�N�X�`����ݒ�
					_Effect->SetTexture("g_Blur", prevRT->texture->pTexture);

					_Effect->CommitChanges();

					//�`��
					_Vertex->DrawPrimitive();

					_Effect->EndPass();
					_Effect->End();
				}//XBlur

				 //YBlur�p��ݒ�
				prevRT = _DownSamplingRT[rtIndex];
				rtIndex++;

				//YBlur
				{
					//�_�E���T���v�����O�pRT��YBlur�p�������_�����O�^�[�Q�b�g�ɐݒ�
					(*graphicsDevice()).SetRenderTarget(0, _DownSamplingRT[rtIndex]->buffer);
					(*graphicsDevice()).SetDepthStencilSurface(_DownSamplingRT[rtIndex]->depth);

					//�摜���N���A
					(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);

					_Effect->SetTechnique("YBlur");

					_Effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
					_Effect->BeginPass(0);

					//�T�C�Y���v�Z
					float size[2] =
					{
						static_cast<float>(prevRT->texture->Size.x),	//����
						static_cast<float>(prevRT->texture->Size.y)	//����
					};
					//�I�t�Z�b�g���v�Z
					float offset[] =
					{
						0.0f,										//����
						16.0f / static_cast<float>(prevRT->texture->Size.y),	//����
					};

					//��ʃT�C�Y��ݒ�
					_Effect->SetValue("g_LuminanceTexSize", size, sizeof(size));
					//�I�t�Z�b�g��ݒ�
					_Effect->SetValue("g_Offset", offset, sizeof(size));
					//�d�݂�ݒ�
					_Effect->SetValue("g_Weight", _Weights, sizeof(_Weights));

					//�e�N�X�`���̐ݒ�
					_Effect->SetTexture("g_Blur", prevRT->texture->pTexture);

					_Effect->CommitChanges();

					//�`��
					_Vertex->DrawPrimitive();

					_Effect->EndPass();
					_Effect->End();
				}

				//XBlur�p��ݒ�
				prevRT = _DownSamplingRT[rtIndex];
				rtIndex++;

			}//YBlur

		}//�P�x���ڂ���

		 //�{�P�t�B���^�[�̍���
		{

			//�A���t�@�u�����h���Ȃ��ɐݒ�
			(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			(*graphicsDevice()).SetRenderTarget(0, _CombineRT->buffer);
			(*graphicsDevice()).SetDepthStencilSurface(_CombineRT->depth);

			//�摜���N���A
			(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);

			float offset[] =
			{
				0.5f / static_cast<float>(_CombineRT->texture->Size.x),
				0.5f / static_cast<float>(_CombineRT->texture->Size.y),
			};

			_Effect->SetTechnique("Combine");

			_Effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			_Effect->BeginPass(0);

			//�e�N�X�`���̐ݒ�
			_Effect->SetTexture("g_CombineTex00", _DownSamplingRT[1]->texture->pTexture);
			_Effect->SetTexture("g_CombineTex01", _DownSamplingRT[3]->texture->pTexture);
			_Effect->SetTexture("g_CombineTex02", _DownSamplingRT[5]->texture->pTexture);
			_Effect->SetTexture("g_CombineTex03", _DownSamplingRT[7]->texture->pTexture);
			_Effect->SetTexture("g_CombineTex04", _DownSamplingRT[9]->texture->pTexture);

			_Effect->SetValue("g_Offset", offset, sizeof(offset));

			_Effect->CommitChanges();

			//�`��
			_Vertex->DrawPrimitive();

			_Effect->EndPass();
			_Effect->End();

		}//�{�P�t�B���^�[�̍���

		 //�u���[��
		{

			float offset[] =
			{
				0.5f / static_cast<float>(_CombineRT->texture->Size.x),
				0.5f / static_cast<float>(_CombineRT->texture->Size.y)
			};

			//���C�������_�[�ɕύX
			INSTANCE(RenderTargetManager)->BeforeRT();
			

			//���Z�����B
			(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			_Effect->SetTechnique("Final");

			_Effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			_Effect->BeginPass(0);

			_Effect->SetTexture("g_Blur", _CombineRT->texture->pTexture);
			_Effect->SetValue("g_Offset", offset, sizeof(offset));

			_Effect->CommitChanges();

			//�`��
			_Vertex->DrawPrimitive();

			_Effect->EndPass();
			_Effect->End();

			(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		}//�u���[��

	}
}

void Bloom::Release()
{
}

void Bloom::_UpdateWeight(const float& dis)
{
	float total = 0;

	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		_Weights[i] = expf(-0.5f*(float)(i*i) / dis);

		total += 2.0f * _Weights[i];
	}

	//�K�i��
	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		_Weights[i] /= total;
	}
}
