#include "EffectManager.h"
#include "Effect.h"

map<char*, Effect*> EffectManager::effectDictinary;

EffectManager::~EffectManager()
{
	map<char*, Effect*>::iterator it = effectDictinary.begin();
	while (it != effectDictinary.end())
	{
		SAFE_RELEASE(it->second);
		it++;
	}
	effectDictinary.clear();
}

Effect* EffectManager::LoadEffect(char* filename)
{
	Effect* effect = nullptr;

	auto it = effectDictinary.find(filename);
	//�������ɃG�t�F�N�g���o�^����ĂȂ�
	if (it == effectDictinary.end()) 
	{
		char* filepath = new char[64];
		strcpy_s(filepath, strlen("Asset/Shader/")+1, "Asset/Shader/");
		strcat_s(filepath, strlen(filepath) + strlen(filename)+1, filename);
		ID3DXEffect* ieffect;	//Effect�ւ̃|�C���^
		LPD3DXBUFFER  compileErrorBuffer = NULL;
		//Effect�t�@�C���ǂݍ���
		HRESULT hr = D3DXCreateEffectFromFile(
			graphicsDevice(),
			filepath,
			NULL,
			NULL,
#ifdef _DEBUG
			D3DXSHADER_DEBUG,
#else
			D3DXSHADER_SKIPVALIDATION,
#endif
			NULL,
			&ieffect,
			&compileErrorBuffer
			);

		if (FAILED(hr)) {
			MessageBox(NULL, (LPCSTR)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		}

		effect = new Effect(filename);
		effect->SetEffect(ieffect);
		//�����ɓo�^
		effectDictinary[filename] = effect;
	}
	else
	{
		effect = it->second;
	}

	return effect;
}