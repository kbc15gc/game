#include "stdafx.h"
#include "Vertex.h"

Vertex::~Vertex()
{
	//�V�F�A�[�h�|�C���^�Ȃ̂ŎQ�Ƃ��Ȃ��Ȃ������������͂��I
	_VertexBuffer.clear();
	SAFE_DELETE(_Declaration);
}

void Vertex::Initialize(fbEngine::PRIMITIVETYPE type, int vertexnum)
{
	switch (type)
	{
	case fbEngine::PRIMITIVETYPE::TRIANGLELIST:
		_PolygonNum = vertexnum / 3;
		break;
	case fbEngine::PRIMITIVETYPE::TRIANGLESTRIP:
		_PolygonNum = vertexnum - 2;
		break;
	default:
		return;
		break;
	}
	_PrimitiveType = (D3DPRIMITIVETYPE)type;
}

void Vertex::CreateVertexBuffer(void * vertexinfo, int vertexnum, int vertexsize, D3DVERTEXELEMENT9 element)
{
	int Vnum;
	switch (_PrimitiveType)
	{
	case (D3DPRIMITIVETYPE)fbEngine::PRIMITIVETYPE::TRIANGLELIST:
		Vnum = _PolygonNum * 3;
		break;
	case (D3DPRIMITIVETYPE)fbEngine::PRIMITIVETYPE::TRIANGLESTRIP:
		Vnum = _PolygonNum + 2;
		break;
	}

	//FB_ASSERT(vertexnum <= Vnum, "���_������Ă܂����B");

	IDirect3DVertexBuffer9* buffer;
	int length = vertexsize * vertexnum;
	//�|�W�V�����̒��_�o�b�t�@�쐬
	(*graphicsDevice()).CreateVertexBuffer(
		length,						//���_�o�b�t�@�̃T�C�Y(�T�C�Y*���_��)
		D3DUSAGE_WRITEONLY,
		0,							//�Œ�@�\�ł͂Ȃ��̂Ŕ�e�u�e���_�ō쐬����
		D3DPOOL_MANAGED,
		&buffer,					//���_�o�b�t�@
		NULL
	);

	//���_�o�b�t�@�̐擪�A�h���X�ւ̎Q�Ƃ��i�[����B
	VOID* pVertices;
	//�A�h���X�擾
	buffer->Lock(
		0,						//���b�N����擪
		length,					//���b�N�����
		(void**)&pVertices,		//�擪�A�h���X
		D3DLOCK_DISCARD			//�t���O
	);
	//�A�h���X�̎Q�Ƃ֒��_��`���R�s�[
	memcpy(pVertices, vertexinfo, length);
	//���b�N����
	buffer->Unlock();

	//���_�T�C�Y�ێ�
	_VertexSize.push_back(vertexsize);
	//�X�g���[���ԍ�
	element.Stream = _VertexBuffer.size();
	//�G�������g�ێ�
	_Element.push_back(element);
	//���_�o�b�t�@�ێ�
	_VertexBuffer.push_back(buffer);
}

void Vertex::CreateDeclaration()
{
	//�G�������g�̔z��쐬
	D3DVERTEXELEMENT9* elements = new D3DVERTEXELEMENT9[_Element.size() + 1];
	unsigned short i = 0;
	for (i = 0; i < _Element.size(); i++)
	{
		elements[i] = _Element[i];
	}
	//����
	elements[i] = D3DDECL_END();
	//�f�R���[�V�����쐬
	(*graphicsDevice()).CreateVertexDeclaration(elements, &_Declaration);
	//��������Ȃ�����폜
	SAFE_DELETE_ARRAY(elements);
}

void Vertex::DrawPrimitive()
{
	//�f�R���[�V�����ݒ�
	(*graphicsDevice()).SetVertexDeclaration(_Declaration);
	//�X�g���[���Z�b�g
	for (unsigned short i = 0; i < _VertexBuffer.size(); i++)
	{
		(*graphicsDevice()).SetStreamSource(i, _VertexBuffer[i], 0, _VertexSize[i]);
	}
	//�`��
	(*graphicsDevice()).DrawPrimitive(_PrimitiveType, 0, _PolygonNum);
}
