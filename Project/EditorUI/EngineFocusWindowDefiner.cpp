#include "EngineFocusWindow.hpp"
#include "../Engine.hpp"
#include "UIAddon.hpp"
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include "../Component/TransformComponent.hpp"
#include "../Component/HitboxComponent.hpp"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "EditorField.hpp"
#include "../Component/GameObject.h"
#include "../Component/RenderComponent.h"
#include "../Engine.hpp"
#include "EditorFieldGroup.hpp"
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
// maximum mumber of lines the output console should have
static const WORD MAX_CONSOLE_LINES = 500;
int currentID = 0;
std::vector<int> recycleID;
WNDPROC prevWndEditor;
WNDPROC prevWndTextMulti;
WNDPROC prevWndText;
WNDPROC prevWndButton;
bool isDouble(char a);
std::string isLetter(std::string line);
LRESULT CALLBACK WndProcEditorFocus(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
EngineFocus::EngineFocus()
{
	windowDefinedName = "EngineFocus";
	titleBarDisplay = "EngineFocus";
	wc = {0};
	//The First Windows structure
	wc.lpszClassName = windowDefinedName;
	wc.lpfnWndProc = WndProcEditorFocus;						// This function is called by windows
	wc.style = CS_HREDRAW | CS_VREDRAW;;							// Catch double-clicks
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(NULL,IDI_WINLOGO);
	wc.hIconSm = 0;//LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.lpszMenuName = NULL;//MAKEINTRESOURCE(MENU_ID);;							// No menu
	wc.cbClsExtra = 0;														// No extra bytes after the window class
	wc.cbWndExtra = 0;														// structure or the window instance
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);;

	//wc.hInstance = hInst;
	//wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);;
	if(!RegisterClassEx(&wc))
		MessageBox(NULL,"Error Registering The Window Class","Error",MB_OK | MB_ICONERROR);
}

#pragma region HWND handler Related
int EditorUI::RequestID()
{
	if(!recycleID.empty())
		return recycleID[0];
	else
		return 	currentID++;
}
void EditorUI::RecycleID(int i)
{
	recycleID.push_back(i);
}
RECT EditorUI::GetLocalCoordinates(HWND hWnd)
{
	RECT Rect;
	GetWindowRect(hWnd,&Rect);
	MapWindowPoints(HWND_DESKTOP,GetParent(hWnd),(LPPOINT)&Rect,2);
	//Rect.right -= Rect.left;
	return Rect;
}
RECT EditorUI::GetClientCoordinates(HWND hWnd)
{
	RECT Rect;
	GetWindowRect(hWnd,&Rect);
	return Rect;
}

bool isDouble(char a)
{
	if(!(a > 0 && a < 255))
		return false;
	else if(isdigit(a))
		return true;
	else if(a == '.')
		return true;
	else if(a == ',')
		return true;
	else
		return false;
}
std::string isLetter(std::string line)
{
	std::string newValue;
	char c;
	for(int i = 0; i < line.length(); i++)
	{
		c = line.at(i);
		if(
			(c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '.' ||
			c == '!' ||
			c == '\n' ||
			c == '\\'
			)
		{
			newValue.push_back(c);
		}
	}
	return newValue;
}
#pragma endregion
#pragma region WndProc
LRESULT CALLBACK WndProcText(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	//LOW: Set a limit of char to TextBox
	switch(msg)
	{
		case WM_CHAR:
		{
			switch(wParam)
			{
#pragma region Enter
			case(VK_RETURN) :
			{
				TCHAR txt[1024];
				DWORD start = 0;
				DWORD end = 0;
				GetWindowText(hWnd, txt, 1024);
				std::string value(txt);
				std::string newValue = "";
				int id = GetWindowLong(hWnd, GWL_ID);
				SendMessage(hWnd, EM_GETSEL, reinterpret_cast<WPARAM>(&start), reinterpret_cast<WPARAM>(&end));

				//TODO: Move this out towards a seperate WndProc
				//Field Object
				std::map<int, BaseField*>::iterator it;
				for (auto i = Engine::Window.focus.componentFieldGroup.begin(); i != Engine::Window.focus.componentFieldGroup.end(); i++)
				{
					it = i->second.field.find(id);
					if (it != i->second.field.end())
						break;
				}
				if (!value.empty())
				{
					if (it->second->flags & FieldFlag::Decimal)
					{
#pragma region Decimal
						for (size_t i = 0; i < value.size(); i++)
						{
							if (isDouble(value.at(i)))
								newValue.push_back(value[i]);
						}
						if (!newValue.empty())
						{
							if (value[0] == '-')
								newValue.insert(newValue.begin(), value[0]);
						}
						else
							newValue = "0";
#pragma endregion
					}
					else
						newValue = isLetter(value);
				}
				else
					newValue = '0';
				SetWindowTextA(hWnd, newValue.c_str());
				SendMessage(hWnd, EM_SETSEL, start, end);
				Engine::Window.setValue(it->second, newValue);
			}
							return 0;
#pragma endregion
				default:
					break;
			}
		}
		default:
			break;
	}
	return CallWindowProc(prevWndText,hWnd,msg,wParam,lParam);
}
LRESULT CALLBACK WndProcEditorFocus(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	int xDelta;     // xDelta = new_pos - current_pos
	int xNewPos;    // new position
	int yDelta = 0;
	SCROLLINFO scrollNfo;

	int xCurrentScroll;

	GetScrollInfo(hWnd,SB_HORZ,&scrollNfo);
	xCurrentScroll = scrollNfo.nPos;
	switch(msg)
	{
		// User clicked the scroll bar shaft left of the scroll box.
		case WM_VSCROLL:
		{
			SCROLLINFO si = {sizeof(SCROLLINFO),SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS,0,0,0,0,0};
			GetScrollInfo(hWnd,SB_VERT,&si);
			int scrollSpeed = 16;
#pragma region wParam
			switch(wParam)
			{
				case SB_PAGEDOWN:
				{
					scrollSpeed = 64;
					if(si.nPos + scrollSpeed <= si.nMax)
					{
						ScrollWindowEx(hWnd,0,-scrollSpeed,NULL,NULL,NULL,0,SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
						si.nPos += scrollSpeed;
					}
					else
					{
						ScrollWindowEx(hWnd,0,-(si.nMax - si.nPos),NULL,NULL,NULL,0,SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
						si.nPos += (si.nMax - si.nPos);
					}
					SetScrollInfo(hWnd,SB_VERT,&si,false);
					break;
				}
				case SB_LINEDOWN:
				{
					scrollSpeed = 16;
					if(si.nPos + scrollSpeed <= si.nMax)
					{
						ScrollWindowEx(hWnd,0,-scrollSpeed,NULL,NULL,NULL,0,SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
						si.nPos += scrollSpeed;
					}
					else
					{
						ScrollWindowEx(hWnd,0,-(si.nMax - si.nPos),NULL,NULL,NULL,0,SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
						si.nPos += (si.nMax - si.nPos);
					}
					SetScrollInfo(hWnd,SB_VERT,&si,false);
					break;
				}
				case SB_LINEUP:
				{
					scrollSpeed = -16;
					//Negative + Negative = Positive
					if(si.nPos + scrollSpeed >= 0)
					{
						ScrollWindowEx(hWnd,0,-scrollSpeed,NULL,NULL,NULL,0,SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
						si.nPos += scrollSpeed;
					}
					else
					{
						ScrollWindowEx(hWnd,0,-si.nPos,NULL,NULL,NULL,0,SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
						si.nPos += si.nPos;
					}
					SetScrollInfo(hWnd,SB_VERT,&si,false);

					break;
				}
				case SB_PAGEUP:
				{
					scrollSpeed = -64;
					//Negative + Negative = Positive
					if(si.nPos + scrollSpeed >= 0)
					{
						ScrollWindowEx(hWnd,0,-scrollSpeed,NULL,NULL,NULL,0,SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
						si.nPos += scrollSpeed;
					}
					else
					{
						if(si.nPos < 64)
						{
							ScrollWindowEx(hWnd,0,-si.nPos,NULL,NULL,NULL,0,SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
							std::cout << si.nPos << std::endl;
							si.nPos += si.nPos;
						}
					}
					SetScrollInfo(hWnd,SB_VERT,&si,false);
					break;
				}
				case SB_THUMBPOSITION:
				{
					int move = HIWORD(wParam);
					si.nPos = move;
					SetScrollInfo(hWnd,SB_VERT,&si,false);

					break;
				}
				default:
					break;
			}
#pragma endregion
		}
		case WM_MOUSEACTIVATE:
		{
			SetFocus(hWnd);
			break;
		}

		default:
		{
			break;
		}
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
LRESULT CALLBACK WndProcTextMulti(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_CHAR:
		{
			switch(wParam)
			{
				#pragma region VK_RETURN
				case(VK_RETURN) :
				{
					TCHAR txt[1024];
					DWORD start = 0;
					DWORD end = 0;
					int id = GetWindowLong(hWnd,GWL_ID);
					SendMessage(hWnd,EM_GETSEL,reinterpret_cast<WPARAM>(&start),reinterpret_cast<WPARAM>(&end));
					GetWindowText(hWnd,txt,1024);
					std::string value(txt);
					std::string newValue = "";

					std::map<int,BaseField*>::iterator it;
					for(auto i = Engine::Window.focus.componentFieldGroup.begin(); i != Engine::Window.focus.componentFieldGroup.end(); i++)
					{
						it = i->second.field.find(id);
						if(it != i->second.field.end())
							break;
					}
					if(!value.empty())
					{
						newValue = isLetter(value);
					}
					else
						newValue = "";
					SetWindowTextA(hWnd,newValue.c_str());
					SendMessage(hWnd,EM_SETSEL,start,end);
					Engine::Window.setValue(it->second,newValue);
					break;
				}
#pragma endregion
				default:
					break;
			}
		}
		default:
			break;
	}
	return CallWindowProc(prevWndTextMulti,hWnd,msg,wParam,lParam);
}
#pragma endregion
#pragma region Add HWND
HWND EngineFocus::addEditor(HWND phWnd,std::string name,int x,int y,int width,int height,int groupID)
{
	HWND hWnd = CreateWindowEx
		(
		WS_EX_CONTROLPARENT,		// Extended possibilites for variation
		windowDefinedName,			// Classname
		titleBarDisplay,			// Title Text
		WS_CHILD | LBS_DISABLENOSCROLL | WS_VISIBLE | WS_THICKFRAME | WS_VSCROLL,	// default window
		x,							// Windows decides the position
		y,							// where the window ends up on the screen
		width,						// The programs width
		height,						// and height in pixels
		phWnd,						// The window is a child-window to desktop
		NULL,						// No menu
		Engine::Window.hInstance,	// Program Instance handler
		NULL						// No Window Creation data
		//this
		);

	//ShowWindow(hWnd,1);

	/*
	if(!prevWndEditor)
	prevWndEditor = (WNDPROC)GetWindowLong(hWnd,GWL_WNDPROC);

	SetWindowLong(hWnd,GWL_WNDPROC,(LONG_PTR)WndProcEditorFocus);
	//*/
	return hWnd;
}
HWND EngineFocus::addComponentGroup(HWND phWnd,std::string name,int x,int y,int width,int height,int groupID)
{
	HWND hWnd =
		CreateWindowEx
		(
		0,
		"EngineFocus",
		name.c_str(),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
		x,y,
		width,height,
		phWnd,
		(HMENU)groupID,
		Engine::Window.hInstance,
		NULL
		);

	return hWnd;
}
HWND EngineFocus::addButton(HWND phWnd,std::string buttonDisplayName,int x,int y,int width,int height,int buttonID)
{
	HWND hWnd;
	hWnd = CreateWindowEx
		(
		0,
		"Button",
		buttonDisplayName.c_str(),
		BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		x,y,
		width,height,
		phWnd,
		(HMENU)buttonID,
		Engine::Window.hInstance,
		NULL
		);
	return hWnd;
}
HWND EngineFocus::addTextbox(HWND phWnd,std::string text,int x,int y,int width,int height,int textboxID)
{
	HWND hWnd;
	hWnd = CreateWindowEx
		(
		0,
		"Edit",
		text.c_str(),
		WS_VISIBLE | WS_CHILD | WS_TABSTOP,
		x,y,
		width,height,
		phWnd,
		(HMENU)textboxID,
		Engine::Window.hInstance,
		NULL
		);
	if(!prevWndText)
		prevWndText = (WNDPROC)GetWindowLong(hWnd,GWL_WNDPROC);
	SetWindowLong(hWnd,GWL_WNDPROC,(LONG_PTR)WndProcText);
	return hWnd;
}
HWND EngineFocus::addTextboxInt(HWND phWnd,std::string text,int x,int y,int width,int height,int textboxID)
{
	HWND hWnd;
	hWnd = CreateWindowEx
		(
		0,
		"Edit",
		text.c_str(),
		WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_AUTOHSCROLL | WS_TABSTOP,
		x,y,
		width,height,
		phWnd,
		(HMENU)textboxID,
		Engine::Window.hInstance,
		NULL
		);
	if(!prevWndText)
		prevWndText = (WNDPROC)GetWindowLong(hWnd,GWL_WNDPROC);
	SetWindowLong(hWnd,GWL_WNDPROC,(LONG_PTR)WndProcText);
	return hWnd;
}
HWND EngineFocus::addTextboxMulti(HWND phWnd,std::string text,int x,int y,int width,int height,int textboxID)
{
	HWND hWnd;
	hWnd = CreateWindowEx
		(
		0,
		"Edit",
		text.c_str(),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		x,y,
		width,height,
		phWnd,
		(HMENU)textboxID,
		Engine::Window.hInstance,
		NULL
		);
	if(!prevWndTextMulti)
		prevWndTextMulti = (WNDPROC)GetWindowLong(hWnd,GWL_WNDPROC);
	SetWindowLong(hWnd,GWL_WNDPROC,(LONG_PTR)WndProcTextMulti);
	return hWnd;
}
HWND EngineFocus::addLabel(HWND phWnd,std::string labelDisplay,int x,int y,int width,int height,int labelID)
{
	HWND hWnd;
	hWnd = CreateWindowEx
		(
		0,
		"static",
		labelDisplay.c_str(),
		WS_VISIBLE | WS_CHILD,
		x,y,
		width,height,
		phWnd,
		(HMENU)labelID,
		Engine::Window.hInstance,
		NULL
		);

	return hWnd;
}
#pragma endregion
void EditorUI::RedirectIOToConsole()
{
	using namespace std;
	{
		int hConHandle;
		long lStdHandle;
		CONSOLE_SCREEN_BUFFER_INFO coninfo;
		FILE *fp;

		// allocate a console for this app
		AllocConsole();

		// set the screen buffer to be big enough to let us scroll text
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&coninfo);
		coninfo.dwSize.Y = MAX_CONSOLE_LINES;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),coninfo.dwSize);

		// redirect unbuffered STDOUT to the console
		lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
		hConHandle = _open_osfhandle(lStdHandle,_O_TEXT);

		fp = _fdopen(hConHandle,"w");

		*stdout = *fp;

		setvbuf(stdout,NULL,_IONBF,0);

		// redirect unbuffered STDIN to the console

		lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
		hConHandle = _open_osfhandle(lStdHandle,_O_TEXT);

		fp = _fdopen(hConHandle,"r");
		*stdin = *fp;
		setvbuf(stdin,NULL,_IONBF,0);

		// redirect unbuffered STDERR to the console
		lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
		hConHandle = _open_osfhandle(lStdHandle,_O_TEXT);

		fp = _fdopen(hConHandle,"w");

		*stderr = *fp;

		setvbuf(stderr,NULL,_IONBF,0);

		// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
		// point to console as well
		ios::sync_with_stdio();
	}
}