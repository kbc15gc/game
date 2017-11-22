#include"fbstdafx.h"
#include "Sprite.h"
#include "_Effect\EffectManager.h"
#include "_Object\Vertex.h"

Vertex* Sprite::_Vertex = nullptr;

Sprite::Sprite(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name()),
	_Texture(nullptr),
	_Pivot(0.5f, 0.5f),
	_UV(0.0f, 1.0f, 0.0f, 1.0f),
	_BlendColor(Color::white),
	_ClipColor(Color::zero),
	_FadeTime(10.0f),
	_FTimer(0.0f),
	_FadeLine(1.0f),
	_SpriteEffect(((DWORD)fbSprite::SpriteEffectE::NONE)),
	_DoubleSided(false)
{
	
}

void Sprite::Start()
{
	//���_�o�b�t�@�쐬
	if(_Vertex == nullptr)
	{
		_Vertex = new Vertex();
		//��ŏ㉺���]������̂ł����č����ɂ���B
		//�|�W�V������`
		VERTEX_POSITION position[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f },//����
			{ 1.0f, 0.0f, 0.0f, 1.0f },//�E��
			{ 0.0f, 1.0f, 0.0f, 1.0f },//����
			{ 1.0f, 1.0f, 0.0f, 1.0f },//�E��
		};
		//UV��`
		VERTEX_TEXCOORD texcoord[] = {
			{ 0.0f, 0.0f },//����
			{ 1.0f, 0.0f },//�E��
			{ 0.0f, 1.0f },//����
			{ 1.0f, 1.0f },//�E��
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
}

void Sprite::Update()
{
	Fade();
}

void Sprite::ImageRender()
{
	//�e�N�X�`�����Ȃ��̂Ȃ�`�悵�Ȃ�
	if (_Texture == nullptr)
		return;
	//�e�G�t�F�N�g
	{
		if (_SpriteEffect & (DWORD)fbSprite::SpriteEffectE::OUTLINE)
			_CreateOutLine();

		if (_SpriteEffect & (DWORD)fbSprite::SpriteEffectE::SHADOW)
			_CreateShadow();
	}

	//�`��
	_RenderSprite();
}

void Sprite::SetEffectFlg(const DWORD& e)
{
	_SpriteEffect = e;
}

bool Sprite::SetEffectFlg(const DWORD& e, bool f)
{
	//���ɗL�����ǂ����H
	if ((_SpriteEffect & e) > 0 && f == false)
	{
		//������
		_SpriteEffect = _SpriteEffect - e;
		return true;
	}
	else if ((_SpriteEffect & e) == 0 && f == true)
	{
		//�L����
		_SpriteEffect = _SpriteEffect + e;
		return true;
	}
	return false;
}

void Sprite::_RenderSprite()
{
	RECT rect;
	//�E�B���h�E�n���h������N���C�A���g�G���A�̃T�C�Y���擾
	HWND hWnd = FindWindow("DECIDE", NULL);
	GetClientRect(hWnd, &rect);
	float w = (float)rect.right - (float)rect.left;
	float h = (float)rect.bottom - (float)rect.top;
	Vector2 mag = Vector2(w, h) / g_StartWindowSize;


	//�V�F�[�_�t�@�C���ǂݍ���
	_Effect = EffectManager::LoadEffect("Sprite.fx");

	D3DXMATRIX  matWorld, matSize, matScale, matRot, matTrans;
	//�摜�̃T�C�Y��ݒ�
	D3DXMatrixScaling(&matSize, _Size.x * mag.x, _Size.y * mag.y, 1.0f);
	//�ݒ肳�ꂽ�X�P�[����ݒ�
	D3DXMatrixScaling(&matScale, transform->GetScale().x, transform->GetScale().y, transform->GetScale().z);
	//��]
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(transform->GetAngle().z));
	//�ړ�
	D3DXMatrixTranslation(&matTrans, transform->GetPosition().x * mag.x, transform->GetPosition().y * mag.y, transform->GetPosition().z);

	//�摜�T�C�Y�@*�@�X�P�[���@*�@��]�@*�@�|�W�V����
	matWorld = matSize * matScale *matRot * matTrans;

	//�ˉe�ϊ��s��
	//�X�N���[���̃T�C�Y�Ɏ��߂�B
	D3DXMATRIX proj(
		2.0f / w, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0 / h, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	//�����p�A�����̂��ߏ�ƕ�����������
	D3DXMATRIX adjustment
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,	//�}�C�i�X�͏㉺�𔽓]�����邽��
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f		//��ʂ̒��S����ɂȂ��Ă���̂�X��-1.0f,Y��1.0f�����I�t�Z�b�g����B
	);

	D3DXMATRIX wp = matWorld * (proj*adjustment);

	if (_DoubleSided)
	{
		//���ʕ`��
		(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}else
	{
		//�\�ʕ`��
		(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�j�b�N�ݒ�
	_Effect->SetTechnique("SpriteTech");

	_Effect->Begin(NULL, 0);
	_Effect->BeginPass(0);

	//�s��
	_Effect->SetMatrix("g_WP", &wp);

	//�e�N�X�`��
	_Effect->SetTexture("g_texture", _Texture->pTexture);
	//�n�_�ƒ���
	Vector2 uvstart(_UV.x, _UV.z), uvlen(_UV.y - _UV.x, _UV.w - _UV.z);
	//UV�ݒ�
	_Effect->SetValue("g_UVStart", &uvstart, sizeof(Vector2));
	_Effect->SetValue("g_UVLength", &uvlen, sizeof(Vector2));
	//pivot�ݒ�(�X�P�[�����O���]�̊�_)
	_Effect->SetFloat("g_Pivotx", _Pivot.x);
	_Effect->SetFloat("g_Pivoty", _Pivot.y);

	//�F�ݒ�
	_Effect->SetValue("g_BlendColor", _BlendColor, sizeof(Color));
	//�����F�ݒ�
	_Effect->SetValue("g_ClipColor", _ClipColor, sizeof(Color));
	Vector4 flg = { (float)((int)_SpriteEffect & 2),0,0,0 };
	//�t���O���M
	_Effect->SetValue("g_EffectFlg", &flg, sizeof(Vector4));
	//������A���{�[�_�[���C���I�ȃA��
	_Effect->SetFloat("g_FadeLine", _FadeLine);

	//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
	_Effect->CommitChanges();
	//�`��
	_Vertex->DrawPrimitive();

	_Effect->EndPass();
	_Effect->End();

	//�ύX�����X�e�[�g�����ɖ߂�
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Sprite::Fade()
{
	//Fade���ǂ����H
	if((_SpriteEffect & (DWORD)fbSprite::SpriteEffectE::FADE) > 0)
	{
		//�^�C�}�[����
		_FTimer += Time::DeltaTime();
		//�t�F�[�h�C�����A�E�g���H
		int fadein = 1;
		//�ꂯ���ڂ̃r�b�g�Ŕ���
		if ((_SpriteEffect & fadein) > 0)
		{
			//�t�F�[�h�C��(0.0->1.0)
			_FadeLine = min(1.0f, (_FTimer / _FadeTime));
		}
		else
		{
			//�t�F�[�h�A�E�g(1.0->0.0)
			_FadeLine = max(0.0f, 1.0f - (_FTimer / _FadeTime));
		}
	}
}

void Sprite::_CreateShadow()
{
	//�ύX�O�ɒl��ۑ�
	Color colorbuf = _BlendColor;
	Vector3 posbuf, pos;
	pos = posbuf = transform->GetPosition();
	//�F������
	_BlendColor = Color::black;
	//���炷��
	Vector2 offset = Vector2(_Texture->Size.x * 0.05f, _Texture->Size.y * 0.05f);
	
	pos.x += offset.x * transform->GetScale().x;
	pos.y += offset.y * transform->GetScale().y;
	transform->SetPosition(pos);

	_RenderSprite();

	//�߂�
	_BlendColor = colorbuf;
	transform->SetPosition(posbuf);
}

void Sprite::_CreateOutLine()
{
	//�ύX�O�ɒl��ۑ�
	Color colorbuf = _BlendColor;
	Vector3 posbuf, pos;
	pos = posbuf = transform->GetPosition();
	//�F������
	_BlendColor = Color::black;
	//�ړ���
	float offset = 1.0f;
	//�㉺���E�Ɉړ������ĕ`��
	FOR(i,4)
	{
		switch (i)
		{
		case 0:
			pos.x += offset;
			break;
		case 1:
			pos.x -= offset;
			break;
		case 2:
			pos.y += offset;
			break;
		case 3:
			pos.y -= offset;
			break;
		default:
			break;
		}
		transform->SetPosition(pos);
		_RenderSprite();
		transform->SetPosition(posbuf);
	}

	//�߂�
	_BlendColor = colorbuf;
	transform->SetPosition(posbuf);
}
