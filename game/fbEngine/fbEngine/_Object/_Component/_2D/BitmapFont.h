#pragma once
#include "_Object\_Component\Component.h"

class Vertex;

class BitmapFont :public Component
{
public:
	BitmapFont(GameObject* g, Transform* t);
	void Start()override;
	void Update()override;
	void Render()override;

	void LoadFont(string filename);
	void SetText(string text);
private:
	//�G�t�F�N�g
	Effect* _Effect;
	//�r�b�g�}�b�v�t�H���g�̃e�N�X�`���B
	TEXTURE* _Bitmap;
	//������B
	string _Text;
	//�e�L�X�g�T�C�Y�B
	Vector2 _TextSize;

	//
	LPDIRECT3DVERTEXBUFFER9 _PosBuffer;
	LPDIRECT3DVERTEXBUFFER9 _UVBuffer;
	//�f�R���[�V����
	LPDIRECT3DVERTEXDECLARATION9 _Declaration;	
};