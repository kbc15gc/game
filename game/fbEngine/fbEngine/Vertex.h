#pragma once

//�v���~�e�B�u�^�C�v.
namespace fbEngine
{
	enum class PRIMITIVETYPE {
		TRIANGLELIST = D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST,	//�g���C�A���O�����X�g�B
		TRIANGLESTRIP = D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP,  //�g���C�A���O���X�g���b�v�B
	};
}

//���_������
class Vertex:Noncopyable
{
public:
	~Vertex();
	//������
	// �`����@
	// ���_��
	void Initialize(fbEngine::PRIMITIVETYPE type,int vertexnum);
	//���_�o�b�t�@�̍쐬
	// vertexinfo ���_���̔z��
	// vertexnum�@���_��
	// vertexsize ���_�T�C�Y
	void CreateVertexBuffer(void* vertexinfo,int vertexnum,int vertexsize, D3DVERTEXELEMENT9 element);
	//���_�o�b�t�@�ƃG�������g����f�R���[�V�����쐬
	void CreateDeclaration();
	//�`��
	void DrawPrimitive();
private:
	//�`����@
	D3DPRIMITIVETYPE _PrimitiveType;
	//�|���S����
	int _PolygonNum;
	//���_�o�C�g��
	vector<int> _VertexSize;
	//���_�o�b�t�@
	vector<IDirect3DVertexBuffer9*> _VertexBuffer;
	//���_�錾
	vector<D3DVERTEXELEMENT9> _Element;
	//�f�R���[�V����
	IDirect3DVertexDeclaration9 *_Declaration;
};