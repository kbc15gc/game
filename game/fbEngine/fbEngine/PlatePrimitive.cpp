#include "PlatePrimitive.h"
#include "Effect.h"
#include "Vertex.h"
#include "VertexDefinition.h"
#include "Camera.h"
#include "Light.h"

Vertex* PlatePrimitive::_Vertex = nullptr;

PlatePrimitive::PlatePrimitive(char * name) :
	GameObject(name),
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
	_Size = Vector2(1.0f, 1.0f);
	_Pivot = Vector2(0.5f, 0.5f);
	_Texture = nullptr;
}

void PlatePrimitive::Start()
{
	_Camera = GameObjectManager::mainCamera;
	_Light = GameObjectManager::mainLight;
}

void PlatePrimitive::Update()
{

}

void PlatePrimitive::Render()
{
	D3DXMATRIX  matWorld, matSize;

	D3DXMatrixIdentity(&matSize);
	D3DXMatrixIdentity(&matWorld);
	//�T�C�Y��ݒ�
	D3DXMatrixScaling(&matSize, _Size.x, _Size.y, 1.0f);
	//�s�{�b�g���ړ�
	matWorld._41 = _Pivot.x;
	matWorld._42 = _Pivot.y;
	//_Pivot * �T�C�Y�@*�@���[���h(�X�P�[���@*�@��]�@*�@�|�W�V����)
	matWorld = matWorld * matSize * transform->GetWorldMatrix();

	//Z�o�b�t�@
	(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�j�b�N�ݒ�
	_Effect->SetTechnique("NormalRender");

	_Effect->Begin(NULL, 0);
	_Effect->BeginPass(0);

	_Effect->SetMatrix("g_worldMatrix", &matWorld);
	_Effect->SetMatrix("g_rotationMatrix", transform->GetRotateMatrixAddress());
	_Effect->SetMatrix("g_viewMatrix", &(D3DXMATRIX)_Camera->GetViewMat());
	_Effect->SetMatrix("g_projectionMatrix", &(D3DXMATRIX)_Camera->GetProjectionMat());

	_Effect->SetValue("g_blendColor", _BlendColor, sizeof(Color));

	const int num = _Light->GetNum();
	Vector4 dir[System::MAX_LIGHTNUM];
	Color color[System::MAX_LIGHTNUM];
	const vector<DirectionalLight*>& vec = _Light->GetLight();
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
	Vector3 campos = _Camera->transform->GetPosition();
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

	_Vertex->DrawPrimitive();

	_Effect->EndPass();
	_Effect->End();

	//�ύX�����X�e�[�g�����ɖ߂�
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void PlatePrimitive::SetTexture(TEXTURE* t)
{
	_Size = t->Size;
	_Texture = t->pTexture;
}

void PlatePrimitive::SetPivot(Vector2 v2)
{
	_Pivot = v2;
}

void PlatePrimitive::SetPivot(float x, float y)
{
	_Pivot = Vector2(x, y);
}

void PlatePrimitive::SetBlendColor(Color c)
{
	_BlendColor = c;
}
