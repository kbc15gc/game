#include "Game.h"

CGraphicsDevice g_graphicsDevice;
HINSTANCE hInst;                                // ���݂̃C���^�[�t�F�C�X
HWND g_MainWindow;
Vector2 g_WindowSize;

//�R�[���o�b�N�֐�
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void Initialize();
void Update();
void Draw();

//�A�v���P�[�V�����̎n�܂�
int GameStart(_In_ HINSTANCE hInstance,
	_In_ int nCmdShow,
	unsigned int IDC)
{
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC));

    MSG msg;

	Initialize();

	// ���C�� ���b�Z�[�W ���[�v:
	do
	{
		//���b�Z�[�W�����݂��邩����
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
			DispatchMessage(&msg);		//���b�Z�[�W���E�B���h�E�v���V�[�W���ɑ��o
		}
		else
		{
			Update();
			Draw();
		}

	} while (msg.message != WM_QUIT);

    return (int) msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
ATOM MyRegisterClass(HINSTANCE hInstance,
	UINT        style,
	WNDPROC     lpfnWndProc,
	int         cbClsExtra,
	int         cbWndExtra,
	HICON       hIcon,
	HCURSOR     hCursor,
	HBRUSH      hbrBackground,
	LPCWSTR     lpszMenuName,
	LPCWSTR     lpszClassName,
	HICON       hIconSm)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = style;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = lpfnWndProc;
	wcex.cbClsExtra = cbClsExtra;
	wcex.cbWndExtra = cbWndExtra;
	wcex.hInstance = hInstance;
	wcex.hIcon = hIcon;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FBENGIN));
	wcex.hCursor = hCursor;//LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = hbrBackground;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = lpszMenuName;//MAKEINTRESOURCEW(IDC_FBENGIN);
	wcex.lpszClassName = lpszClassName;// szWindowClass;
	wcex.hIconSm = hIconSm;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   if (!g_MainWindow)
   {
      return FALSE;
   }
   //�f�o�C�X������
   graphicsDevice().InitD3d(g_MainWindow);
   //�E�B���h�E�\��
   ShowWindow(g_MainWindow, nCmdShow);
   //�E�B���h�E�X�V
   UpdateWindow(g_MainWindow);

   return TRUE;
}

//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:    ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND  - �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT    - ���C�� �E�B���h�E�̕`��
//  WM_DESTROY  - ���~���b�Z�[�W��\�����Ė߂�
//
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_COMMAND:
//        {
//            int wmId = LOWORD(wParam);
//            // �I�����ꂽ���j���[�̉��:
//           /* switch (wmId)
//            {
//            case IDM_ABOUT:
//                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//                break;
//            case IDM_EXIT:
//                DestroyWindow(hWnd);
//                break;
//            default:
//                return DefWindowProc(hWnd, message, wParam, lParam);
//            }*/
//        }
//        break;
//    case WM_PAINT:
//        {
//            PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(hWnd, &ps);
//            // TODO: HDC ���g�p����`��R�[�h�������ɒǉ����Ă�������...
//            EndPaint(hWnd, &ps);
//        }
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}

void Initialize()
{
	INSTANCE(InputManager)->Initialize();
	INSTANCE(PhysicsWorld)->Start();
	Random::Seed(timeGetTime());
}

void Update() 
{
	Time::Update();
	INSTANCE(SoundEngine)->Update();
	INSTANCE(InputManager)->Update();
	INSTANCE(SceneManager)->UpdateScene();
}

void Draw() 
{
	//��ʂ𔒐F�ŃN���A
	(*graphicsDevice()).Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(255, 255, 255),
		1.0f,
		0);

	//�V�[���J�n
	if (SUCCEEDED((*graphicsDevice()).BeginScene()))
	{
		INSTANCE(SceneManager)->DrawScene();
		//�V�[���I��
		(*graphicsDevice()).EndScene();
	}
	(*graphicsDevice()).Present(NULL, NULL, NULL, NULL);
}