
#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include <string>

class Application
{
	protected:
		WNDCLASSEX windowsclass; //класс окна
		HWND hWnd;				 //дескриптор окна
		MSG msg;				 //идентификатор сообшения
		LRESULT (CALLBACK *MainProc)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); //указатель на оконную процедуру

		HINSTANCE* hinstance;
		HINSTANCE* hprevinstance;
		LPSTR* lpcmdline;
		int* ncmdshow;
	public:
		Application(HINSTANCE* hinst, HINSTANCE* hprevinst, LPSTR* lpcmdln, int* ncmdshw);
		virtual bool CreateApp(UINT X, UINT Y, std::string wclassname, std::string wname);
		virtual void DelApp();
		virtual void SetMainProc( LRESULT (CALLBACK *proc)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) ); //установка оконной ппоцедуры
		virtual HWND Wnd();
		//virtual bool MsgLoop();
		virtual ~Application();
};

#endif