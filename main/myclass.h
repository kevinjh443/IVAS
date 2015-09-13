

	//包含头文件
#include <windows.h>
#include<winuser.h>
#pragma  comment(lib,"user32.lib")
#pragma comment(lib, "WINMM.LIB")
#include <mmsystem.h> 
#include "Windef.h"
#include <time.h>
#include <string.h>
#include <shellapi.h>
#include <tlhelp32.h> 
#include <stdio.h>

#define WM_SHELLNOTIFY WM_USER+5

#include <Digitalv.h>

	///*如果不使用网络连接要注释掉
#pragma  comment(lib,"Ws2_32.lib")
	//*/
	//////////////////gdi+
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
	///////////////////
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
	///////////////////
	//主程序模版
	///////////////////
	//全局变量：my_msg msg;my_window mainwindow;
	//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
	//{
	//	UNREFERENCED_PARAMETER(hPrevInstance);
	//	UNREFERENCED_PARAMETER(lpCmdLine);
	//	...
	//	return (int)msg.message_while();
	//}
	////////////////////////////////////////////////////////////////////////////////////////////
	//创建窗口模版WS_OVERLAPPEDWINDOW
	//	LRESULT CALLBACK callback(HWND , UINT , WPARAM , LPARAM );//回调函数声明
	//	if(mainwindow.reg_win(hInstance,0,callback,TEXT("asd")))//窗体注册
	//		 mainwindow.set_hwnd(mainwindow.create_win(hInstance,nCmdShow,TEXT("window")));//窗体生成
	////////////////////////////////////////////////////////////////////////////////////////////
	//消息响应模版
	/*LRESULT CALLBACK callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
	switch (message)
	{
	PAINTSTRUCT ps;  
	HDC hdc;		
	case WM_CREATE:
	DragAcceptFiles(hWnd,TRUE);//拖拽
	break;
	case WM_PAINT:
	hdc = BeginPaint(hWnd, &ps);
	//
	EndPaint(hWnd, &ps);
	break;
	case WM_LBUTTONDOWN:
	break;

	case WM_SHELLNOTIFY:
		if( lParam==WM_RBUTTONDOWN )
			SetForegroundWindow(hWnd);
		if( lParam==WM_LBUTTONDBLCLK )//双击事件
			SetForegroundWindow(hWnd);
		break;

	case WM_DROPFILES:
	break;

	case WM_COMMAND:
	break;

	case WM_DESTROY:	
	PostQuitMessage(0);
	break;
	default:
	return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}*/
	///////////////////线程模版
	//HANDLE hth;
	//DWORD WINAPI xxx(LPVOID lpParaneter);
	//xxx.craete_new_road(hth,xxx);
	//...
	/*
	DWORD WINAPI xxx(LPVOID lpParaneter)
	{
	basedata.THREAD_ON();
	for(;;)
	{
	if(basedata.THREAD())
	{
	Sleep(500);
	...
	}
	else
	Sleep(2000);

	}

	return 0;
	}
	*/
	/*
	SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);//总在最前
	窗体透明
	SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(HDD,GWL_EXSTYLE)^WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd,RGB(255,255,255),255,LWA_ALPHA|LWA_COLORKEY);
	*/
	///////////////////
	//#ifdef UNICODE
	//#undef UNICODE
	//#endif
	///////////////////
	//////////////////////////
	//class:BASE
	/////////////////////////
class BASE
{
	HWND hWnd;
	HINSTANCE hInstance;
	int thread;
public:
	BASE()
	{
		hWnd=0;
		hInstance=0;
		thread=0;
	}
	void set_hwnd(HWND hwnd)
	{
		this->hWnd=hwnd;
	}

	HWND get_hwnd()
	{
		return this->hWnd;
	}

	void set_hInstance(HINSTANCE hinstance)
	{
		hInstance=hinstance;
	}

	HINSTANCE get_hInstance()
	{
		return hInstance;
	}

	void update_win(HWND hwnd, int tpye=0,RECT *rect = NULL)
	{
		
		InvalidateRect(hwnd,rect,tpye);
		UpdateWindow(hwnd);
	}


	HANDLE craete_new_road(HANDLE &hth1,LPTHREAD_START_ROUTINE FUN)
	{
		hth1=CreateThread(NULL,0,FUN,NULL,0,NULL);
		CloseHandle(hth1);
		return hth1;
	}

	BOOL THREAD()
	{
		return this->thread;
	}

	void THREAD_ON()
	{
		this->thread=1;
	}

	void THREAD_OFF()
	{
		this->thread=0;
	}


};
BASE basedata;

class my_msg
{
	MSG msg;
	HACCEL hAccelTable;//快捷键
	ULONG_PTR gdiplusStartupToken;
public:

	my_msg()
	{
		hAccelTable=0;
	}
	void set_hAccelTable(HINSTANCE hInstance,int IDC)
	{
		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC));
	}
	//
	int message_while(int tpye=1,void (*OnIdle)(void)=NULL)
	{
		if(tpye==1)
		{
			while (GetMessage(&msg, NULL, 0, 0))
			{
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			return (int) msg.wParam;
		}

		if(tpye==2)
		{

			while ( true )
			{
				if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
				{
					if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					continue;
				}
				if ( WM_QUIT == msg.message )
				{
					break;
				}
				OnIdle();
			}


			return (int) msg.wParam;

		}
		if(tpye==3)
		{
			while (GetMessage(&msg, NULL, 0, 0))
			{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
			}
			 Gdiplus::GdiplusShutdown(gdiplusStartupToken);
			return (int) msg.wParam;
		}
	}

	void png_out(HWND hwnd,WCHAR* filename,int flag=0,int x=0,int y=0,int w=0,int h=0)//flag:0,拉伸到窗口 1：否
	{
		Gdiplus::GdiplusStartupInput gdiInput;
		Gdiplus::GdiplusStartup(&gdiplusStartupToken,&gdiInput,NULL);

		RECT wndRect;
		::GetWindowRect(hwnd,&wndRect);
		SIZE wndSize = {wndRect.right-wndRect.left,wndRect.bottom-wndRect.top};
		HDC hdc = ::GetDC(hwnd);
		HDC memDC = ::CreateCompatibleDC(hdc);
		HBITMAP memBitmap = ::CreateCompatibleBitmap(hdc,wndSize.cx,wndSize.cy);
		::SelectObject(memDC,memBitmap);

		Gdiplus::Image image(filename);
		Gdiplus::Graphics graphics(memDC);
		int xasd=image.GetWidth();
		if(flag==0)
			graphics.DrawImage(&image,x,y,wndSize.cx,wndSize.cy);
		else if(flag==1)
			graphics.DrawImage(&image,x,y,image.GetWidth(),image.GetHeight());
		else
			graphics.DrawImage(&image,x,y,w,h);

		HDC screenDC = GetDC(NULL);
		POINT ptSrc = {0,0};

		BLENDFUNCTION blendFunction;
		blendFunction.AlphaFormat = AC_SRC_ALPHA;
		blendFunction.BlendFlags = 0;
		blendFunction.BlendOp = AC_SRC_OVER;
		blendFunction.SourceConstantAlpha = 255;
		UpdateLayeredWindow(hwnd,screenDC,&ptSrc,&wndSize,memDC,&ptSrc,0,&blendFunction,2);

		::DeleteDC(memDC);
		::DeleteObject(memBitmap);
	}

	MSG get_mag()
	{
		return msg;
	}
};


class my_window : public BASE
{
	HWND hWnd;
	HICON hIcon;
	HCURSOR hCursor;
	LPCWSTR lpszMenuName;
	HBRUSH hbrBackground;
	WNDCLASSEX wcex;
	NOTIFYICONDATA nti;

public:

	my_window()
	{
		hIcon=0;
		hCursor=0;
		lpszMenuName=0;
		hbrBackground=0;

	}

	void set_menu(int IDC)
	{
		lpszMenuName=MAKEINTRESOURCE(IDC);
	}

	void set_icon(char *icon_str)
	{
		hIcon=(HICON) LoadImageA(NULL,icon_str,IMAGE_ICON,0,0,LR_LOADFROMFILE);
		//SendMessage(hWnd,STM_SETICON,IMAGE_ICON,(LPARAM)(UINT)hIcon);
	}

	void set_Cursor(char *Cursor_str)
	{
		hCursor=(HCURSOR) LoadImageA(NULL,Cursor_str,IMAGE_CURSOR,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
	}

	void set_background(UINT style)
	{
		hbrBackground=(HBRUSH)style;
	}

	ATOM reg_win(HINSTANCE hInstance,UINT style,WNDPROC WndProc,LPCWSTR szWindowClass)
	{


		wcex.cbSize = sizeof(WNDCLASSEX);

		if(style)
			wcex.style			= style;
		else
			wcex.style			= (CS_HREDRAW | CS_VREDRAW);

		wcex.lpfnWndProc	= WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;

		if(!hIcon)
			wcex.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
		else
			wcex.hIcon			= hIcon;

		if(!hCursor)
			wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		else
			wcex.hCursor		= hCursor;

		if(!hbrBackground)
			wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		else
			wcex.hbrBackground	= (HBRUSH)hbrBackground;

		wcex.lpszMenuName	= lpszMenuName;
		wcex.lpszClassName	= szWindowClass;
		wcex.hIconSm		= NULL;

		if(!&wcex)
			MessageBoxA(0,"error: cannot Register Class!",0,0);

		return RegisterClassEx(&wcex);
	}

	HWND create_win2(//png
		HINSTANCE hInstance, 
		int nCmdShow,
		LPCWSTR wintitle,
		LPCWSTR wintext=NULL,
		UINT winstyle=WS_OVERLAPPEDWINDOW,
		int winx=CW_USEDEFAULT,
		int winy=0,
		int width=CW_USEDEFAULT,
		int height=0,
		HWND father=0,
		HMENU  hmune=0,
		LPVOID lpParam=0
		)
	{

		if(!wintext)
			wintext=wcex.lpszClassName;

		 hWnd = CreateWindowExW(WS_EX_LAYERED|WS_EX_TOOLWINDOW,wintext,wintitle, winstyle,
			winx, winy, width, height, father, hmune, hInstance, lpParam);

		 

		if (!(hWnd))
		{
			MessageBoxA(0,"error: cannot create window",0,0);
			return 0;
		}

		

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		return hWnd;
	}


	HWND create_win(
		HINSTANCE hInstance, 
		int nCmdShow,
		LPCWSTR wintitle,
		LPCWSTR wintext=NULL,
		UINT winstyle=WS_OVERLAPPEDWINDOW,
		int winx=CW_USEDEFAULT,
		int winy=0,
		int width=CW_USEDEFAULT,
		int height=0,
		HWND father=0,
		HMENU  hmune=0,
		LPVOID lpParam=0
		)
	{

		if(!wintext)
			wintext=wcex.lpszClassName;

		hWnd = CreateWindowW(wintext,wintitle, winstyle,
			winx, winy, width, height, father, hmune, hInstance, lpParam);

		if (!hWnd)
		{
			MessageBoxA(0,"error: cannot create window",0,0);
			return 0;
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		return hWnd;
	}

	void DROPFILES_ON()//WM_DROPFILES
	{
		DragAcceptFiles(this->hWnd,TRUE);
	}

	void DROPFILES_OFF()//WM_DROPFILES
	{
		DragAcceptFiles(this->hWnd,0);
	}

	LPWSTR dropfiles(MSG msg)
	{
		LPWSTR szFileName=(LPWSTR)malloc(MAX_PATH*sizeof(WCHAR));
		DragQueryFile((HDROP)msg.wParam,0,szFileName,MAX_PATH);
		return szFileName;
	}

	void create_NIF(char *icon)
	{
		nti.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
		nti.hIcon = (HICON)LoadImageA(NULL,icon,IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
		nti.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE; 
		nti.hWnd = hWnd; 
		nti.uID = 101;
		nti.uCallbackMessage =WM_SHELLNOTIFY;
		nti.dwInfoFlags=NIIF_INFO;
		nti.dwState=NIS_SHAREDICON; 
		nti.uVersion = NOTIFYICON_VERSION;

		Shell_NotifyIcon(NIM_ADD, &nti);
	}
};







/////////////////////////////
//class header:string.h windoes.h
///////////////////////////

class my_print : public BASE
{

	char bmp_str[50];
	int out_x;
	int out_y;
	int bmp_w;
	int bmp_h;

public:

	my_print()
	{
		out_x=0;
		out_y=0;
		bmp_str[0]='\0';


	}


	void textout(char * ,HDC,LPCWSTR,int ,int ,int,int,int,int,int,int);


	void set_bmp(char *str,int flag=0)
	{
		if(strlen(str)<50)
			strcpy(bmp_str,str);
		else
			MessageBoxA(0,"error:name to long!",0,0);

		if(flag==1)
			laod();
	}

	void setxy(int x,int y)
	{
		out_x=x;
		out_y=y;
	}

	void laod()
	{
		HBITMAP bitmap;//定义bitmap对象
		BITMAP bmp;//定义bmp结构

		bitmap =(HBITMAP) LoadImageA(NULL,bmp_str,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
		GetObject(bitmap,sizeof(bmp),&bmp);//获得bitmap信息到bmp结构中;
		DeleteObject(bitmap);//删除对象

		bmp_w=bmp.bmWidth;
		bmp_h=bmp.bmHeight;
	}

	int get_bmp_w()
	{
		return bmp_w;
	}

	int get_bmp_h()
	{
		return bmp_h;
	}

	void laodbmp_and_output(HDC hdc,int pic_w=0,int pic_h=0)//放在WM_PRINT中
	{
		HBITMAP bitmap;//定义bitmap对象
		BITMAP bmp;//定义bmp结构
		HDC DCMemory = CreateCompatibleDC(NULL);//建立内存兼容DC


		bitmap =(HBITMAP) LoadImageA(NULL,bmp_str,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
		GetObject(bitmap,sizeof(bmp),&bmp);//获得bitmap信息到bmp结构中;
		SelectObject(DCMemory,bitmap);//将BMP选入到内存DC中
		DeleteObject(bitmap);//删除对象
		if(pic_w==0&&pic_h==0)
			BitBlt(hdc,out_x,out_y,bmp.bmWidth,bmp.bmHeight,DCMemory,0,0,SRCCOPY);//将内存DC中的内容拷贝到当前窗口D
		else
			BitBlt(hdc,out_x,out_y,pic_w,pic_h,DCMemory,0,0,SRCCOPY);
		DeleteDC(DCMemory);
	}

	char *get_bmp_str()
	{
		if(bmp_str)
			return bmp_str;
		else
			MessageBoxA(0,"error:bmp empty!",0,0);
		return 0;
	}

	void other_bmp_out(HDC hdc,char *pic)
	{

		// TODO: Add your command handler code here 
		BOOL               bSuccess ; 
		DWORD              dwFileSize, dwHighSize, dwBytesRead ; 
		HANDLE             hFile ; 
		BITMAPFILEHEADER * pbmfh ; 
		BITMAPINFO       * pbmi ; 
		BYTE             * pBits ; 
		int                cxDib, cyDib ; 

		char* pstrFileName=pic; 

		hFile = CreateFileA (pstrFileName, GENERIC_READ, FILE_SHARE_READ, NULL, 
			OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ; 

		if (hFile == INVALID_HANDLE_VALUE) 
			return ; 

		dwFileSize = GetFileSize (hFile, &dwHighSize) ; 

		if (dwHighSize) 
		{ 
			CloseHandle (hFile) ; 
			return ; 
		} 

		pbmfh = (BITMAPFILEHEADER *)malloc (dwFileSize) ; 
		if (!pbmfh) 
		{ 
			CloseHandle (hFile) ; 
			return ; 
		} 

		bSuccess = ReadFile (hFile, pbmfh, dwFileSize, &dwBytesRead, NULL) ; 
		CloseHandle (hFile) ; 
		if (!bSuccess || (dwBytesRead != dwFileSize)          
			|| (pbmfh->bfType != * (WORD *) "BM")  
			|| (pbmfh->bfSize != dwFileSize)) 
		{ 
			free (pbmfh) ; 
			return  ; 
		} 


		if (pbmfh == NULL) 
		{ 
			return ; 
		} 
		// 取得位图信息结构体的指针和位图数据数组的指针 

		pbmi  = (BITMAPINFO *) (pbmfh + 1) ; 
		pBits = (BYTE *) pbmfh + pbmfh->bfOffBits ; 

		// Get the DIB width and height 

		if (pbmi->bmiHeader.biSize == sizeof (BITMAPCOREHEADER)) 
		{ 
			cxDib = ((BITMAPCOREHEADER *) pbmi)->bcWidth ; 
			cyDib = ((BITMAPCOREHEADER *) pbmi)->bcHeight ; 
		} 
		else 
		{ 
			cxDib =      pbmi->bmiHeader.biWidth ; 
			cyDib = abs (pbmi->bmiHeader.biHeight) ; 
		} 




		StretchDIBits (hdc, 0, 0, 800, 800,  
			0, 0, cxDib, cyDib, 
			pBits, pbmi, DIB_RGB_COLORS, SRCCOPY) ; 

		return;

	}


	

};




//case MM_MCINOTIFY:

class my_mci
{
	///////////////////////
	MCIDEVICEID  dev[20];
	MCI_OPEN_PARMS mciOpen;
	MCI_PLAY_PARMS mciPlay;
	MCI_BREAK_PARMS mcibreak;
	////////////////////////
	int nVirtKey;


public:

	my_mci()
	{
		nVirtKey=32;
	}

	void SetBreakKey(int key)
	{
		nVirtKey=key;
	}

	void mic_avi_play(LPWSTR say_or_wmv,HWND hWnd)
	{




		mciPlay.dwCallback   =   (DWORD)hWnd;
		mciOpen.dwCallback   =   (DWORD)hWnd;

		mciOpen.lpstrElementName=say_or_wmv;

		if(mciSendCommand(mciOpen.wDeviceID,MCI_OPEN, MCI_OPEN_ELEMENT ,
			(DWORD)&mciOpen)==0)
			dev[1]=mciOpen.wDeviceID;
		else
			mciOpen.wDeviceID=dev[1];


		mciPlay.dwFrom = 0;
		mcibreak.nVirtKey=nVirtKey;

		MCI_DGV_WINDOW_PARMS WindowParms;

		WindowParms.dwCallback = (DWORD)hWnd;
		WindowParms.hWnd = hWnd;
		WindowParms.nCmdShow = SW_SHOW;
		WindowParms.lpstrText = (LPWSTR) NULL;
		mciSendCommand( mciOpen.wDeviceID, MCI_WINDOW, MCI_DGV_WINDOW_HWND, (DWORD)&WindowParms );

		mciSendCommand(mciOpen.wDeviceID,MCI_PLAY,MCI_NOTIFY | MCI_FROM|MCI_BREAK_KEY,(DWORD)&mciPlay);



	}

	int mci_getHight()
	{
		  
        MCI_DGV_WHERE_PARMS MCIWhere;   
        if (mciSendCommand (mciOpen.wDeviceID, MCI_WHERE, MCI_DGV_WHERE_SOURCE,    
            (DWORD)(LPVOID) &MCIWhere))
		{   
            return 0;   
        } 
        return (int) MCIWhere.rc.bottom;   
	}

	int mci_getWidth()
	{
		MCI_DGV_WHERE_PARMS MCIWhere;   
        if (mciSendCommand (mciOpen.wDeviceID, MCI_WHERE, MCI_DGV_WHERE_SOURCE,   
            (DWORD)(LPVOID) &MCIWhere))
		{   
            return 0;   
        }   
        return (int) MCIWhere.rc.right;   
	}

	void mic_close()
	{
		mciSendCommand(mciOpen.wDeviceID,MCI_CLOSE,MCI_NOTIFY | MCI_FROM,(DWORD)&mciPlay);
	}

	void mic_stop()
	{
		mciSendCommand(mciOpen.wDeviceID,MCI_PAUSE,MCI_NOTIFY | MCI_FROM,(DWORD)&mciPlay);

	}

	void mic_start()
	{
		mciSendCommand(mciOpen.wDeviceID,MCI_RESUME,MCI_NOTIFY | MCI_FROM,(DWORD)&mciPlay);
	}

	void mic_full()
	{
		//MCI_MCIAVI_PLAY_FULLSCREEN
		//mciSendCommand(mciOpen.wDeviceID,MCI_RESUME,MCI_NOTIFY | MCI_FROM,(DWORD)&mciPlay);
		;
	}
};







class my_PC
{

template<typename _CharT>
int mystrcmp(const _CharT* s1, const _CharT* s2)
{
 while(*s1 && *s2 && *s1 == *s2)
 {
  ++s1;
  ++s2;
 }
 return *s1 - *s2;
}
public:
int KillProcess(DWORD Pid)
{ 
	//打开进程得到进程句柄 
	HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,Pid);
	if(hProcess==NULL)
	{
		MessageBoxA(0,"OpenProcess	error",0,0);
		return 0;
	}
	//结束进程 
	if(TerminateProcess(hProcess,0))
	{
		//printf("结束进程成功\n");
		return 1;
	}
	else
	{
		//printf("结束进程失败\n");
		return 0;
	}
}

int GetProcess(WCHAR *str)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize=sizeof(pe32);
	//获得系统内所有进程快照
	HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap==INVALID_HANDLE_VALUE)
	{
		MessageBoxA(0,"CreateToolhelp32Snapshot error",0,0);
		return 0;
	}
	//枚举列表中的第一个进程

	BOOL bProcess=Process32First(hProcessSnap,&pe32);
	while(bProcess)
	{
	
		if(!mystrcmp(pe32.szExeFile,str))
		{
			//MessageBox(0,pe32.szExeFile,0,0);//pe32.th32ProcessID
			break;
		}
		//继续枚举进程
		bProcess=Process32Next(hProcessSnap,&pe32);
	}
	CloseHandle(hProcessSnap);
	return pe32.th32ProcessID;
}

void get_date(char *d)
{
	char tstr[50];
	char rstr[10]="\0";
	char mstr[20]="\0";

	time_t now;
	now = time(0);
	strcpy(tstr,ctime(&now));
	for(int i=0,j=0,k=0;tstr[i]!='\0';i++)
	{
		if(tstr[i]==' ')
		{
			j++;
			i++;
			k=0;
		}
		if(j==1)
		{
			mstr[k++]=tstr[i];
			if(tstr[i+1]==' ')
			{
				if(!strcmp(mstr,"Jan"))
				{
					strcpy(mstr,"1月");
				}
				if(!strcmp(mstr,"Feb"))
				{
					strcpy(mstr,"2月");
				}
				if(!strcmp(mstr,"Mar"))
				{
					strcpy(mstr,"3月");
				}
				if(!strcmp(mstr,"Apr"))
				{
					strcpy(mstr,"4月");
				}
				if(!strcmp(mstr,"May"))
				{
					strcpy(mstr,"5月");
				}
				if(!strcmp(mstr,"Jun"))
				{
					strcpy(mstr,"6月");
				}
				if(!strcmp(mstr,"Jul"))
				{
					strcpy(mstr,"7月");
				}
				if(!strcmp(mstr,"Aug"))
				{
					strcpy(mstr,"8月");
				}
				if(!strcmp(mstr,"Sep"))
				{
					strcpy(mstr,"9月");
				}
				if(!strcmp(mstr,"Oct"))
				{
					strcpy(mstr,"10月");
				}
				if(!strcmp(mstr,"Nov"))
				{
					strcpy(mstr,"11月");
				}
				if(!strcmp(mstr,"Dec"))
				{
					strcpy(mstr,"12月");
				}
			}
		}
		if(j==2)
		{
			rstr[k++]=tstr[i];
		}
		if(j==3)
		{
			strcat(rstr,"日");
			strcat(mstr,rstr);
			break;
		}
	}
	//MessageBoxA(0,rstr,0,0);
	strcpy(d,mstr);
}

char * get_my_way(char *str)
{
	char szFilePath[MAX_PATH] = {0};
	DWORD dwError = GetModuleFileNameA(NULL, szFilePath, ARRAYSIZE(szFilePath));
	if(dwError > 0)
	{
		PathRemoveFileSpecA(szFilePath);
		sprintf(szFilePath,"%s\\%s",szFilePath,str);
		//MessageBoxA(0,szFilePath,0,0);
		return szFilePath;
		//printf(_T("The App path is: %s\n"), szFilePath);
	}
	else
	{

		MessageBoxA(0,"GetModuleFileName error",0,0);
		return 0;
	}
}

void get_week(char *w)
{
	char tstr[50];
	char rstr[10]="\0";

	time_t now;
	now = time(0);
	strcpy(tstr,ctime(&now));
	for(int i=0,j=0,k=0;tstr[i]!='\0';i++)
	{
		if(tstr[i]==' ')
		{
			j++;
			i++;
		}
		if(j==0)
		{
			rstr[k++]=tstr[i];
			if(tstr[i+1]==' ')
			{
				if(!strcmp(rstr,"Mon"))
				{
					strcpy(rstr,"星期一");
				}
				if(!strcmp(rstr,"Feb"))
				{
					strcpy(rstr,"星期二");
				}
				if(!strcmp(rstr,"Wed"))
				{
					strcpy(rstr,"星期三");
				}
				if(!strcmp(rstr,"Thu"))
				{
					strcpy(rstr,"星期四");
				}
				if(!strcmp(rstr,"Fir"))
				{
					strcpy(rstr,"星期五");
				}
				if(!strcmp(rstr,"Sat"))
				{
					strcpy(rstr,"星期六");
				}
				if(!strcmp(rstr,"Sun"))
				{
					strcpy(rstr,"星期天");
				}
			}
		}
		if(j==1)
		{
			break;
		}
	}
	//MessageBoxA(0,rstr,0,0);
	strcpy(w,rstr);
}

void get_time(char *t)
{
	char tstr[50];
	char rstr[10]="\0";

	time_t now;
	now = time(0);
	strcpy(tstr,ctime(&now));
	for(int i=0,j=0,k=0;tstr[i]!='\0';i++)
	{
		if(tstr[i]==' ')
		{
			j++;
			i++;
		}
		if(j==3)
		{
			rstr[k++]=tstr[i];
		}
		if(j==4)
		{
			break;
		}
	}
	//MessageBoxA(0,rstr,0,0);
	strcpy(t,rstr);
}

};

char Output_one[300]="\0";
int Reset=-1;
LRESULT CALLBACK output_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
my_window output_win;
void out_message_win(int flag,char *str=0,HINSTANCE hInstance=0,HWND f=0,int x=600,int y=0,int w=500,int h=1000)
{
	if(flag==0)
	{
		output_win.set_background((COLOR_WINDOW+0));
		if(output_win.reg_win(hInstance,0,output_callback,TEXT("OUT")))//窗体注册
			output_win.set_hwnd(output_win.create_win(hInstance,SW_SHOW,TEXT("OUTwindow"),0,13565952U,x,y,w,h,f));//窗体生成

		HWND hinbutt1 = CreateWindow(
			TEXT("BUTTON"),
			TEXT("上一层"),
			WS_CHILD|WS_VISIBLE,
			w/2-100,h-100,	
			100,
			40,	
			output_win.get_hwnd(),
			(HMENU)1000,
			hInstance,
			0);

		HWND hinbutt2 = CreateWindow(
			TEXT("BUTTON"),
			TEXT("下一层"),
			WS_CHILD|WS_VISIBLE,
			w/2,h-100,	
			100,
			40,	
			output_win.get_hwnd(),
			(HMENU)2000,
			hInstance,
			0);
	}
	else if (flag==1)
	{
		strcpy(Output_one,str);
		Reset=0;
		output_win.update_win(output_win.get_hwnd());
		
	}
	else if(flag==2)
	{
		strcpy(Output_one,str);
		Reset=1;
		output_win.update_win(output_win.get_hwnd(),1);
		
	}
	else
	{
		ShowWindow(output_win.get_hwnd(),SW_HIDE);
	}



}

LRESULT CALLBACK output_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int y=0;
	switch (message)
	{
		PAINTSTRUCT ps;  
		HDC hdc;		
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if(Reset==1)
		{
			y=0;
		}
		TextOutA(hdc,0,y,Output_one,strlen(Output_one));
		if(Reset==0)
		{
			y+=30;
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_COMMAND:
		switch(wParam)
		{	
		case 1000:
		
			break;
		case 2000:

			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}


//面向无连接udp socket

class my_netlink
{
	SOCKET socketsrv;
	SOCKADDR_IN addrsrv;
	SOCKADDR_IN addrclient;
	char *sentto_ip;
public:
	void netstart();
	void netend();
	int netinti();
	void netrecv(char *);//最好放在线程中
	void netsent(char *);
	void set_ip(char *);
};

void my_netlink::netstart()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return; 
	}

	/* The WinSock DLL is acceptable. Proceed. */
}

void my_netlink::netend()
{
	closesocket(socketsrv);
	WSACleanup();
}

int my_netlink::netinti()
{
	int res=0;
	if(socketsrv=socket(AF_INET,SOCK_DGRAM,0))//创建
	{

		if(!this->sentto_ip)
			addrsrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//(ULONG)0x00000000 INADDR_ANY
		else
			addrsrv.sin_addr.S_un.S_addr=inet_addr(sentto_ip);

		addrsrv.sin_family=AF_INET;
		addrsrv.sin_port=htons(1003);

		//绑定
		res=bind(socketsrv,(SOCKADDR *)&addrsrv,sizeof(SOCKADDR));
	}
	else
		MessageBoxA(0,"socket 绑定失败",0,0);


	return res;
}


void my_netlink::netsent(char *str)
{
	sendto(socketsrv,str,strlen(str)+1,0,(SOCKADDR *)&addrsrv,sizeof(SOCKADDR));
}

void my_netlink::netrecv(char *dest_str)//最好放在线程中
{
	int len=sizeof(SOCKADDR);
	
	recvfrom(socketsrv,dest_str,sizeof(dest_str),0,(SOCKADDR *)&addrclient,&len);

}


void my_netlink::set_ip(char *ip)
{
	if(ip)
	{
		sentto_ip=(char *)malloc((strlen(ip)+1)*sizeof(char));
		strcpy(sentto_ip,ip);
	}
	else
	{
		MessageBoxA(0,"ip为空",0,0);
	}
}