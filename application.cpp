
#include "application.h"

Application::Application(HINSTANCE* hinst,
				   HINSTANCE* hprevinst,
				   LPSTR* lpcmdln,
				   int* ncmdshw) : hinstance(hinst), hprevinstance(hprevinst), lpcmdline(lpcmdln), ncmdshow(ncmdshw)
{

}

bool Application::CreateApp(UINT X, UINT Y, std::string wclassname, std::string wname)
{
	//определение класса окна WINDCLASSEX
	windowsclass.cbSize = sizeof(WNDCLASSEXA);
	windowsclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowsclass.lpfnWndProc = MainProc;
	windowsclass.cbClsExtra = 0;
	windowsclass.cbWndExtra = 0;
	windowsclass.hInstance = *hinstance;
	windowsclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowsclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowsclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	windowsclass.lpszMenuName = NULL;
	windowsclass.lpszClassName = wclassname.c_str();
	windowsclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	//регистрация класса
	if(!RegisterClassEx(&windowsclass))
	{ return 0; }
	
	//создание окна
	if(!(hWnd = CreateWindowEx(0, //стиль окна
		wclassname.c_str(),				 //клсс
		wname.c_str(),  //имя окна
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0,0,						 //левый верхний угол
		X, Y,					 //ширина и высота
		NULL,						 //дескриптор родительского окна
		NULL,						 //дескриптор меню
		*hinstance,					 //дескриптор приложения
		NULL)))						 //указатель на данные окна
	{ return 0; }

	return true;
}

void Application::DelApp()
{

}

void Application::SetMainProc( LRESULT (CALLBACK *proc)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) )
{
	MainProc = proc;
}

HWND Application::Wnd()
{
	return hWnd;
}

//bool Application::MsgLoop()
//{
//	if(true)
//    {
//    	ShowWindow(hWnd, SW_SHOWDEFAULT);//рисовать окно
//    	UpdateWindow(hWnd);				 //обновить окно
//    	ZeroMemory(&msg, sizeof(msg));
//    	while(msg.message != WM_QUIT)
//    	{
//    		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//    		{
//    			TranslateMessage(&msg);
//    			DispatchMessage(&msg);
//    		}
//    		else
//			{
//				
//			}
//    	}
//    }
//
//	return 0;
//}

Application::~Application()
{

}