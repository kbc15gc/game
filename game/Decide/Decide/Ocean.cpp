/**
* �C�N���X�̎���.
*/
#include"stdafx.h"
#include"Ocean.h"
#include"fbEngine\_Object\Vertex.h"
#include"fbEngine\_Effect\EffectManager.h"
#include"fbEngine\_Object\_Component\_3D\Light.h"


namespace
{
	float Tmp[6][2] =
	{
		{ 0,0 },{ 1,0 },{ 0,1 },
		{ 0,1 },{ 1,0 },{ 1,1 }
	};
}

/**
* ���߂̏�����.
*/
void Ocean::Awake()
{
	if (_Vertex == nullptr)
	{
		CreateBuffer();
	}

	_Effect = EffectManager::LoadEffect("Ocean.fx");

	_NormalTexture[0] = LOADTEXTURE("Ocean/Ocean2_Normal.png");
	_NormalTexture[1] = LOADTEXTURE("Ocean/Ocean3_Normal.png");

}

/**
* ������.
*/
void Ocean::Start()
{
	transform->SetLocalPosition(Vector3(0, 50, 0));
}

/**
* �X�V.
*/
void Ocean::Update()
{
	_Wave += 0.0003f;

	static float miti = 0;

	transform->SetLocalPosition(Vector3(0, 50 + sin(miti), 0));
	miti += 0.01;

	//�g�����X�t�H�[���̍X�V.
	transform->UpdateTransform();
}

void Ocean::CreateBuffer()
{
	_Vertex = new Vertex();

	const int SegmentCount = 100;

	vector<VERTEX_POSITION> position;
	vector<VERTEX_TEXCOORD> texcoord;
	vector<VERTEX_NORMAL> normal;
	vector<VERTEX_TANGENT> tangent;

	float polygonSize = 20000.0f / SegmentCount;
	float uvSize = 81.0f / SegmentCount;

	for (int x = 0; x < SegmentCount; x++)
	{
		for (int y = 0; y < SegmentCount; y++)
		{
			VERTEX_POSITION pos = VERTEX_POSITION(-10000.0f + (x * polygonSize), 0.0f, -10000.0f + (y * polygonSize), 1.0f);
			VERTEX_TEXCOORD tex = VERTEX_TEXCOORD(-80.0f + (x * uvSize), -80.0f + (x * uvSize));
			for (int i = 0; i < 6; i++)
			{
				position.push_back(pos + VERTEX_POSITION(polygonSize * Tmp[i][0], 0.0f, polygonSize * Tmp[i][1], 0.0f));
				texcoord.push_back(tex + VERTEX_TEXCOORD(uvSize * Tmp[i][0], uvSize * Tmp[i][1]));
				normal.push_back(VERTEX_NORMAL(0.0f, 1.0f, 0.0f, 1.0f));
				tangent.push_back(VERTEX_TANGENT(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}

	D3DVERTEXELEMENT9 elements[] = {
		{ 0, 0	, D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// ���_���W.
		{ 1, 0	, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD   , 0 }, // UV���W.
		{ 2, 0	, D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },	// �@���x�N�g��.
		{ 2, 0	, D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },	// �ڃx�N�g��.
		D3DDECL_END()
	};

	_Vertex->Initialize(fbEngine::PRIMITIVETYPE::TRIANGLELIST, position.size());
	_Vertex->CreateVertexBuffer(position.data(), position.size(), sizeof(VERTEX_POSITION), elements[0]);
	_Vertex->CreateVertexBuffer(texcoord.data(), texcoord.size(), sizeof(VERTEX_TEXCOORD), elements[1]);
	_Vertex->CreateVertexBuffer(normal.data(), normal.size(), sizeof(VERTEX_NORMAL), elements[2]);
	_Vertex->CreateVertexBuffer(tangent.data(), tangent.size(), sizeof(VERTEX_TANGENT), elements[3]);
	_Vertex->CreateDeclaration();
}

/**
* �`��.
*/
void Ocean::Render()
{
	
	//���ʕ`��.
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	//�A���t�@�e�X�g.
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//���̃e�N�X�`���͂��̂܂܂�RGBA
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//�������ރe�N�X�`����(1.0f - R(GBA))�v�Z.
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	_Effect->SetTechnique("Ocean");

	_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
	_Effect->BeginPass(0);

	_Effect->SetMatrix("g_WorldMatrix", &(D3DXMATRIX)transform->GetWorldMatrix());
	_Effect->SetMatrix("g_ViewMatrix",&(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetViewMat());
	_Effect->SetMatrix("g_ProjMatrix", &(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetProjectionMat());

	_Effect->SetTexture("g_Normal_1", _NormalTexture[0]->pTexture);
	_Effect->SetTexture("g_Normal_2", _NormalTexture[1]->pTexture);

	_Effect->SetFloat("g_Wave", _Wave);

	//���C�g.
	{
		const int num = INSTANCE(GameObjectManager)->mainLight->GetNum();
		Vector4 dir[System::MAX_LIGHTNUM];
		Color color[System::MAX_LIGHTNUM];
		ZeroMemory(dir, sizeof(Vector4)*System::MAX_LIGHTNUM);
		const vector<DirectionalLight*>& vec = INSTANCE(GameObjectManager)->mainLight->GetLight();
		FOR(i, num)
		{
			dir[i] = vec[i]->Direction();
			color[i] = vec[i]->GetColor();
			color[i].a = 5;
		}
		//���C�g�̌�����]���B
		_Effect->SetValue("g_diffuseLightDirection", &dir, sizeof(Vector4)*System::MAX_LIGHTNUM);
		//���C�g�̃J���[��]���B
		_Effect->SetValue("g_diffuseLightColor", &color, sizeof(Color)*System::MAX_LIGHTNUM);
		//���C�g���Z�b�g
		_Effect->SetInt("g_LightNum", num);
		//����
		Vector3 ambient = INSTANCE(GameObjectManager)->mainLight->GetAmbientLight();
		_Effect->SetVector("g_ambientLight", &D3DXVECTOR4(ambient.x, ambient.y, ambient.z, 1.0f));
	}

	//�J�����̃|�W�V�����Z�b�g(�X�y�L�������C�g�p)
	Vector3 campos = INSTANCE(GameObjectManager)->mainCamera->transform->GetPosition();
	_Effect->SetValue("g_cameraPos", &D3DXVECTOR4(campos.x, campos.y, campos.z, 1.0f), sizeof(D3DXVECTOR4));

	_Effect->SetTexture("g_EnvironmentMap", INSTANCE(SceneManager)->GetEnvironmentMap()->GetCubeTexture());

	_Effect->CommitChanges();

	_Vertex->DrawPrimitive();

	_Effect->EndPass();
	_Effect->End();

	//�ύX�����X�e�[�g�����ɖ߂�
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, FALSE);

}
