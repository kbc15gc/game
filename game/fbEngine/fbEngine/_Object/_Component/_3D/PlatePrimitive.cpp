#include "PlatePrimitive.h"

#include "_Effect\EffectManager.h"
#include "_Object\Vertex.h"
#include "_Object\_Component\_3D\Camera.h"
#include "_Object\_Component\_3D\Light.h"

Vertex* PlatePrimitive::_Vertex = nullptr;

PlatePrimitive::PlatePrimitive(GameObject* g, Transform* t) :
	Component(g, t, typeid(this).name()),
	_IsBillboard(true),
	_BlendMode(fbEngine::BlendModeE::Trans),
	_Effect(nullptr),
	_Size(Vector2(1.0f,1.0f)),
	_Pivot(Vector2(0.5f, 0.5f)),
	_UV(0.0f, 1.0f, 0.0f, 1.0f),
	_Texture(nullptr),
	_BlendColor(Color::white)
{
	//���_�o�b�t�@�쐬
	if (_Vertex == nullptr)
	{
		_Vertex = new Vertex();
		//�|�W�V������`
		VERTEX_POSITION position[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f },//����
			{ 1.0f, 0.0f, 0.0f, 1.0f },//�E��
			{ 0.0f, -1.0f, 0.0f, 1.0f },//����
			{ 1.0f, -1.0f, 0.0f, 1.0f },//�E��
		};
		//UV��`
		VERTEX_TEXCOORD texcoord[] = {
			{ 0.0f, 0.0f },//����
			{ 1.0f, 0.0f },//�E��
			{ 0.0f, 1.0f },//����
			{ 1.0f, 1.0f },//�E��
		};
		//�@����`
		VERTEX_NORMAL normal[] = {
			{ 0.0f, 0.0f, -1.0f, 1.0f },//����
			{ 0.0f, 0.0f, -1.0f, 1.0f },//�E��
			{ 0.0f, 0.0f, -1.0f, 1.0f },//����
			{ 0.0f, 0.0f, -1.0f, 1.0f },//�E��
		};

		//���_�錾(���_���ǂ̂悤�ɍ\������Ă��邩)
		D3DVERTEXELEMENT9 elements[] = {
			//�X�g���[���ԍ�,�I�t�Z�b�g�l,�f�[�^�^,?,�Z�}���e�B�N�X,�Z�}���e�B�N�X�ԍ�
			{ 0, 0, D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION, 0 } , // ���_���W
			{ 1, 0, D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 0 } , // UV
			{ 2, 0,	D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0 },//�@��
		};

		_Vertex->Initialize(fbEngine::PRIMITIVETYPE::TRIANGLESTRIP, 4);
		_Vertex->CreateVertexBuffer(position, 4, sizeof(VERTEX_POSITION), elements[0]);
		_Vertex->CreateVertexBuffer(texcoord, 4, sizeof(VERTEX_TEXCOORD), elements[1]);
		_Vertex->CreateVertexBuffer(normal, 4, sizeof(VERTEX_NORMAL), elements[2]);
		_Vertex->CreateDeclaration();
	}
}

void PlatePrimitive::Awake()
{
	//�G�t�F�N�g�ǂݍ���
	_Effect = EffectManager::LoadEffect("Primitive.fx");
}

void PlatePrimitive::Start()
{
	_Camera = INSTANCE(GameObjectManager)->mainCamera;
	_Light = INSTANCE(GameObjectManager)->mainLight;
}

void PlatePrimitive::Update()
{
	if (_IsBillboard)
	{
		//�r���{�[�h�������s���B
		const D3DXMATRIX& mCameraRot = INSTANCE(GameObjectManager)->mainCamera->transform->GetRotateMatrix();
		Quaternion qRot;
		qRot.SetRotation(Vector3(mCameraRot.m[2][0], mCameraRot.m[2][1], mCameraRot.m[2][2]), 0.0f);
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, (D3DXQUATERNION*)&qRot);
		rot = mCameraRot*rot;
		transform->SetRotateMatrix(rot);
	}
}

void PlatePrimitive::Render()
{
	D3DXMATRIX  matWorld, matSize;

	D3DXMatrixIdentity(&matSize);
	D3DXMatrixIdentity(&matWorld);
	//�T�C�Y��ݒ�
	D3DXMatrixScaling(&matSize, _Size.x, _Size.y, 1.0f);
	//�s�{�b�g���ړ�
	matWorld._41 = -_Pivot.x;//���ɂ��炷
	matWorld._42 = _Pivot.y;//��ɂ��炷
	//_Pivot * �T�C�Y�@*�@���[���h(�X�P�[���@*�@��]�@*�@�|�W�V����)
	matWorld = matWorld * matSize * transform->GetWorldMatrix();

	//Z�o�b�t�@
	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);
	(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//���u�����h
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	switch (_BlendMode)
	{
	case fbEngine::Trans:
		//��Z����
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//���n(���ɏ�����Ă���)���Ƃ̃u�����h���[�h
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case fbEngine::Add:
		//���Z����
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	default:
		break;
	}

	//�e�N�j�b�N�ݒ�
	_Effect->SetTechnique("NormalRender");

	_Effect->Begin(NULL, 0);
	_Effect->BeginPass(0);

	//�n�_�ƒ���
	Vector2 uvstart(_UV.x, _UV.z), uvlen(_UV.y - _UV.x, _UV.w - _UV.z);
	//UV�ݒ�
	_Effect->SetValue("g_UVStart", &uvstart, sizeof(Vector2));
	_Effect->SetValue("g_UVLength", &uvlen, sizeof(Vector2));

	_Effect->SetMatrix("g_worldMatrix", &matWorld);
	_Effect->SetMatrix("g_rotationMatrix", transform->GetRotateMatrixAddress());
	_Effect->SetMatrix("g_viewMatrix", &(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetViewMat());
	_Effect->SetMatrix("g_projectionMatrix", &(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetProjectionMat());

	_Effect->SetValue("g_blendColor", _BlendColor, sizeof(Color));

	const int num = INSTANCE(GameObjectManager)->mainLight->GetNum();
	Vector4 dir[System::MAX_LIGHTNUM];
	Color color[System::MAX_LIGHTNUM];
	const vector<DirectionalLight*>& vec = INSTANCE(GameObjectManager)->mainLight->GetLight();
	FOR(idx,num)
	{
		dir[idx] = vec[idx]->Direction();
		color[idx] = vec[idx]->GetColor();
	}
	//���C�g�̌�����]���B
	_Effect->SetValue("g_diffuseLightDirection", &dir, sizeof(Vector4)*System::MAX_LIGHTNUM);
	//���C�g�̃J���[��]���B
	_Effect->SetValue("g_diffuseLightColor", &color, sizeof(Color)*System::MAX_LIGHTNUM);
	//���C�g���Z�b�g
	_Effect->SetInt("g_LightNum", num);
	//����
	_Effect->SetVector("g_ambientLight", &D3DXVECTOR4(0.5, 0.5, 0.5, 1));
	//�J�����̃|�W�V�����Z�b�g
	Vector3 campos = INSTANCE(GameObjectManager)->mainCamera->transform->GetPosition();
	_Effect->SetValue("g_cameraPos", &D3DXVECTOR4(campos.x, campos.y, campos.z, 1.0f), sizeof(D3DXVECTOR4));

	//�e�N�X�`�����i�[����Ă���΃Z�b�g
	if (_Texture != NULL)
	{
		_Effect->SetTexture("g_Texture", _Texture);
		_Effect->SetBool("Texflg", true);
	}
	//�e�N�X�`�����Ȃ��Ȃ�J���[�Z�b�g
	else
	{
		//���F
		_Effect->SetValue("g_Color", &Color::white, sizeof(Color));
		_Effect->SetBool("Texflg", false);
	}
	//���˃t���O
	_Effect->SetBool("Spec", false);

	//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
	_Effect->CommitChanges();
	//�|���S���`��
	_Vertex->DrawPrimitive();

	_Effect->EndPass();
	_Effect->End();

	//�ύX�����X�e�[�g�����ɖ߂�
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);
}

void PlatePrimitive::SetBillboard(const bool & f)
{
	_IsBillboard = f;
}

void PlatePrimitive::SetBlendMode(fbEngine::BlendModeE mode)
{
	_BlendMode = mode;
}

void PlatePrimitive::SetTexture(TEXTURE* t)
{
	_Size = t->Size;
	_Texture = t->pTexture;
}

void PlatePrimitive::SetSize(const Vector2 & size)
{
	_Size = size;
}

void PlatePrimitive::SetPivot(const Vector2& v2)
{
	_Pivot = v2;
}

void PlatePrimitive::SetPivot(float x, float y)
{
	_Pivot = Vector2(x, y);
}

void PlatePrimitive::SetUV(const Vector4 & uv)
{
	_UV = uv;
}

void PlatePrimitive::SetBlendColor(const Color& c)
{
	_BlendColor = c;
}