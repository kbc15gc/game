#pragma once

// �O���[�o���ϐ�:
extern HINSTANCE hInst;                                // ���݂̃C���^�[�t�F�C�X

//�Q�[���X�^�[�g
extern int GameStart(_In_ HINSTANCE hInstance,
	_In_ int nCmdShow,
	unsigned int IDC);

//�E�B���h�E�N���X�쐬
extern ATOM MyRegisterClass(HINSTANCE hInstance,
	UINT        style,
WNDPROC     lpfnWndProc,
int         cbClsExtra,
int         cbWndExtra,
HICON       hIcon,
HCURSOR     hCursor,
HBRUSH      hbrBackground,
LPCWSTR     lpszMenuName,
LPCWSTR     lpszClassName,
HICON       hIconSm);
//�C���X�^���X������
extern BOOL InitInstance(HINSTANCE h_Instance,
	int cmd);