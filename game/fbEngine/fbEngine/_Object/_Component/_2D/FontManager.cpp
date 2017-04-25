#include "FontManager.h"

FontManager*  FontManager::_Instance = nullptr;

FontManager::FontManager()
{
	//NULL�w�肵�ĉ�ʑS�̂̃f�o�C�X�R���e�L�X�g�擾
	HDC hdc = GetDC(NULL);
	//�������f�o�C�X�R���e�L�X�g�쐬
	_HDC = CreateCompatibleDC(hdc);
	ReleaseDC(NULL, hdc);//��������Ȃ��B

	//�t�H���g�̑������`
	_LFont = {
		(LONG)FontSize,	//�����̍��� 0���ƃf�t�H -���ƃf�t�H����������	//�傫�߂ɐ������Č�Ŕ{����ς���
		0,			//���� 0���Ɨ��ĕ��ɍ��킹�Ă����
		0,			//��������̕����H
		0,			//�X���H
		FW_BOLD,	//�����̑����@0���f�t�H�@�}�N����`������(FW_)
		0,			//�C�^���b�N�̂ɂȂ邩�ǂ����̃t���O
		0,			//�����t���O
		0,			//�ŏ������t���O
		SHIFTJIS_CHARSET,			//�L�����N�^�Z�b�g
		OUT_TT_ONLY_PRECIS,			//�o�͐��x
		CLIP_DEFAULT_PRECIS,		//�N���b�s���O���x
		CLEARTYPE_QUALITY,			//�o�͕i��
		DEFAULT_PITCH | FF_MODERN,	//�s�b�`�ƃt�@�~���A�w��
		NULL						//�t�H���g�X�^�C��32byte�܂�
	};

	_GradFlg = GGO_GRAY4_BITMAP; // GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
	_Grad = 0; // �K���̍ő�l
	switch (_GradFlg) {
	case GGO_GRAY2_BITMAP: _Grad = 4; break;
	case GGO_GRAY4_BITMAP: _Grad = 16; break;
	case GGO_GRAY8_BITMAP: _Grad = 64; break;
	}
}

FontManager::~FontManager()
{
	//�f�o�C�X�R���e�L�X�g�폜
	DeleteDC(_HDC);
}

void FontManager::Createfont(const wchar_t* string,const char* Style)
{
	//�g���܂킹����
	if (strcmp(_LFont.lfFaceName, Style) == 0)
	{
		//TODO:: �t�H���g���g���܂킷����
		int a = 0;
	}
	//�t�H���g�X�^�C���̓R�s�[
	strcpy_s(_LFont.lfFaceName, 32, Style);
	//�E�B���h�E�Y��API�Ńt�H���g�쐬
	//�_���t�H���g�̃n���h���擾
	_HFont = CreateFontIndirect(&_LFont);
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	//�߂�l�Ƃ��đO�ɐݒ肵�Ă����t�H���g�̏�񂪋A���Ă���B
	HFONT oldFont = (HFONT)SelectObject(_HDC, _HFont);

	//�X�^�C���̃}�b�v�̃n�b�V��
	const int stylehash = Support::MakeHash(Style);
	//�X�^�C����map���o�^����Ă��邩�H
	std::map<int, FontManager::FontMap*>::iterator array = _FontMapArray.find(stylehash);
	if (array == _FontMapArray.end())
	{
		//������Ȃ������̂ŐV�����o�^
		array = _FontMapArray.insert(std::make_pair(stylehash, new FontMap)).first;
	}
	//�C�e���[�^���璆�g���擾(�����̃}�b�v)
	FontManager::FontMap* map = array->second;

	//������̏I�[�܂Ń��[�v
	for (short i = 0; string[i] != '\0'; i++)
	{
		//�����𕶎��R�[�h�ɕϊ�
		int code = (int)string[i];
		//map�ɕ������o�^����Ă��邩
		if (map->find(code) == map->end())
		{
			//�o�^����Ă��Ȃ��̂ŐV��������
			FontData* data = _CreateOutLineFontTexture(string[i],code);

			//�}�b�v�ɒǉ�
			map->insert(std::make_pair(code, data));
		}
	}
	//�t�H���g�̃e�N�X�`����������Ȃ炢��Ȃ��̂ŉ��
	DeleteObject(_HFont);
}

FontData* FontManager::Findfont(const wchar_t& wchar,const char* Style)
{
	int stylehash = Support::MakeHash(Style);
	//�X�^�C������t�H���g�}�b�v�擾
	FontMap* map = _FontMapArray[stylehash];
	if(map)
	{
		//�t�H���g��Ԃ�
		return map->at(wchar);
	}
	return nullptr;
}

FontData * FontManager::_CreateFontTexture(const int & code)
{
	//�f�o�C�X�R���e�L�X�g����t�H���g�̏����擾
	TEXTMETRIC tm;
	GetTextMetrics(_HDC, &tm);
	GLYPHMETRICS gm;
	CONST MAT2 mat = { { 0, 1 },{ 0, 0 },{ 0, 0 },{ 0, 1 } };
	//�T�C�Y���s���Ȃ̂Ŗ߂�l�ŃT�C�Y���擾
	DWORD size = GetGlyphOutlineW(_HDC, code, _GradFlg, &gm, 0, NULL, &mat);
	BYTE *pMono = new BYTE[size];
	//�������̃|�C���^�擾
	GetGlyphOutlineW(_HDC, code, _GradFlg, &gm, size, pMono, &mat);
	//���p�X�y�[�X���������(�S�p�͒m���B)
	if (code == ' ')
	{
		gm.gmBlackBoxX = gm.gmCellIncX;
	}
	// �e�N�X�`���쐬
	TEXTURE* tex = new TEXTURE();
	//4�o�C�g�A���C���ȃr�b�g�}�b�v�t�H���g�̉����𓾂�
	int fontWidth = (gm.gmBlackBoxX + 3) / 4 * 4;
	int fontHeight = gm.gmBlackBoxY;
	//��̃e�N�X�`���쐬
	(*graphicsDevice()).CreateTexture(fontWidth, fontHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex->pTexture, NULL);

	// �e�N�X�`���Ƀt�H���g�r�b�g�}�b�v������������
	D3DLOCKED_RECT lockedRect;
	tex->pTexture->LockRect(0, &lockedRect, NULL, 0);  // ���b�N
	DWORD *pTexBuf = (DWORD*)lockedRect.pBits;   // �e�N�X�`���������ւ̃|�C���^

	for (int y = 0; y < fontHeight; y++) {
		for (int x = 0; x < fontWidth; x++) {
			//���v�Z
			DWORD alpha = pMono[y * fontWidth + x] * 255 / _Grad;
			//�t�H���g�̃f�t�H���g�J���[(�����Ȕ��F)
			DWORD color = D3DCOLOR_ARGB(0, 255, 255, 255);
			//�����v�Z���āA�e�N�X�`���ւ̏�������
			pTexBuf[y * fontWidth + x] = (alpha << 24) | color;
		}
	}

	tex->pTexture->UnlockRect(0);  // �A�����b�N
	SAFE_DELETE_ARRAY(pMono);
	//�T�C�Y��ݒ�
	tex->Size = Vector2(fontWidth, fontHeight);
	//�f�[�^�������ă|�C���^��Ԃ��B
	return new FontData(tex, gm);
}

FontData * FontManager::_CreateOutLineFontTexture(const wchar_t & wc, const int & code)
{
	TEXTMETRICA tm;
	GLYPHMETRICS gm;
	MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	GetTextMetricsA(_HDC, &tm);
	//GLYPHMETRICS �̂ݎ擾
	GetGlyphOutlineW(_HDC, code, GGO_METRICS, &gm, 0, 0, &mat);
	static float pensize = 6.0f;

	//���p�X�y�[�X���������(�S�p�͒m���B)
	if (code == ' ')
	{
		gm.gmBlackBoxX = gm.gmCellIncX;
	}


	RECT charRegion = {     // LT - RB
		gm.gmptGlyphOrigin.x - pensize / 2,
		tm.tmAscent - gm.gmptGlyphOrigin.y - pensize / 2,
		gm.gmptGlyphOrigin.x + gm.gmBlackBoxX + pensize / 2,
		tm.tmAscent - gm.gmptGlyphOrigin.y + gm.gmBlackBoxY + pensize / 2
	};

	RECT charWH = { 0, 0, (gm.gmBlackBoxX + pensize ) , (gm.gmBlackBoxY + pensize ) };
	int bmpW = charWH.right;
	int bmpH = charWH.bottom;
	//�r�b�g�}�b�v
	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = bmpW;
	bmpInfo.bmiHeader.biHeight = -bmpH;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	//�r�b�g�}�b�v�ւ̃A�h���X
	unsigned char *bmp = 0;
	HBITMAP hBitMap = CreateDIBSection(0, &bmpInfo, DIB_RGB_COLORS, (void**)&bmp, 0, 0);
	HBITMAP oldBMP = (HBITMAP)SelectObject(_HDC, hBitMap);

	// BMP�w�i��F�ŏ�����
	HBRUSH bgBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 255));
	//�h��Ԃ��B
	FillRect(_HDC, &charWH, bgBrush);
	DeleteObject(bgBrush);

	// �p�X�F�͗΁A�h�͐�
	HPEN   hPen = (HPEN)CreatePen(PS_SOLID, pensize, RGB(0, 255, 0));
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
	HPEN   oldPen = (HPEN)SelectObject(_HDC, hPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(_HDC, hBrush);

	SetBkMode(_HDC, TRANSPARENT);
	BeginPath(_HDC);
	TextOutW(_HDC, -charRegion.left, -charRegion.top, &wc, 1);
	EndPath(_HDC);
	StrokeAndFillPath(_HDC);

	SelectObject(_HDC, oldPen);
	SelectObject(_HDC, oldBrush);
	//SelectObject(_HDC, oldFont);
	SelectObject(_HDC, oldBMP);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	//DeleteObject(hFont);

	// DirectX�̃e�N�X�`����BMP�̐F�������g���t�H���g�����
	int texW = charWH.right;
	int texH = charWH.bottom ;
	// �e�N�X�`���쐬
	TEXTURE* tex = new TEXTURE();
	(*graphicsDevice()).CreateTexture(texW, texH, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex->pTexture, 0);
	D3DLOCKED_RECT lockR;
	//�e�N�X�`�����b�N
	if (SUCCEEDED(tex->pTexture->LockRect(0, &lockR, 0, 0))) {
		//�r�b�g�擾
		char *d = (char*)lockR.pBits;
		unsigned BMPPitch = (charWH.right * 3 + 3) / 4 * 4;
		//�e�N�X�`���ɏ������ރ��[�v
		for (int y = 0; y < texH; y++) {
			for (int x = 0; x < texW; x++) {
				//�����ɏ������߂΂����H
				unsigned &v = *((unsigned*)d + x + y * texW);   // �e�N�X�`���̃s�N�Z���ʒu
				unsigned alph = 0;
				unsigned edge = 0;
				unsigned fill = 0;

				alph = bmp[y * BMPPitch + x * 3 + 0];
				edge = bmp[y * BMPPitch + x * 3 + 1];
				fill = bmp[y * BMPPitch + x * 3 + 2];

				// ���ߓx������ꍇ�̓G�b�W�F���̗p
				// �s�����̏ꍇ�̓u�����h�F���̗p
				Color edgeC, fillC;
				edgeC = Color::black;
				fillC = Color::white;
				unsigned a = 0xff - alph;
				if (a < 0xff) {
					// ������
					unsigned r = (edgeC.r * 255);
					unsigned g = (edgeC.g * 255);
					unsigned b = (edgeC.b * 255);
					a = (a * 255) >> 8;
					v = a << 24 | r << 16 | g << 8 | b;
				}
				else {
					// �s����
					unsigned r = ((int)(edgeC.r * 255 * edge) >> 8) + ((int)(fillC.r * 255 * fill) >> 8);
					unsigned g = ((int)(edgeC.g * 255 * edge) >> 8) + ((int)(fillC.r * 255 * fill) >> 8);
					unsigned b = ((int)(edgeC.b * 255 * edge) >> 8) + ((int)(fillC.r * 255 * fill) >> 8);
					a = ((255 * edge) >> 8) + ((255 * fill) >> 8);
					v = a << 24 | r << 16 | g << 8 | b;
				}
			}
		}
		tex->pTexture->UnlockRect(0);
	}

	DeleteObject(hBitMap);
	//�T�C�Y��ݒ�
	tex->Size = Vector2(texW, texH);
	//�f�[�^�������ă|�C���^��Ԃ��B
	return new FontData(tex, gm);
}
