#include "myclass.h"
#include "var.h"
#include <wininet.h>
#pragma comment(lib,"wininet.lib")
#include "ai.h"
#include "other.h"
#include "shell.h"
#include "callback.h"
#include "sapi.h" 
#include "xiancheng.h"
#include "net.h"



int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MessageBoxA(0,"��һ��ʹ�ñ�ϵͳ������������ʶ��ģ����򵼳��򣨽̳̿�����������������޷�ʹ�������ϳ���ʶ��ģ��","��ܰ��ʾ",0);
	mainp.set_bmp("main.bmp");
	AI.create_and_read("ai.txt","mainword.txt","knowledge.txt");

	HANDLE thnet;
	DWORD WINAPI net_listen(LPVOID lpParaneter);
	mainwindow.craete_new_road(thnet,net_listen);

	LRESULT CALLBACK sola(HWND , UINT , WPARAM , LPARAM );//�ص���������
	if(thisissola.reg_win(hInstance,0,sola,TEXT("ert")))//����ע��
		thisissola.set_hwnd(thisissola.create_win(hInstance,nCmdShow,TEXT("�ٴε�������㲥�ر�")
		,0,WS_VISIBLE|WS_POPUP|WS_EX_LAYERED,700,300,800,400));//��������
	open.set_bmp("1.bmp",1);
	SetWindowPos(thisissola.get_hwnd(),0,0,0,open.get_bmp_w(),open.get_bmp_h()+50,2);


	if(thisissola1.reg_win(hInstance,0,sola,TEXT("ert1")))//����ע��
		thisissola1.set_hwnd(thisissola1.create_win(hInstance,nCmdShow,TEXT("�ٴε���������Źر�")
		,0,WS_VISIBLE|WS_POPUP|WS_EX_LAYERED,700,300,800,400));//��������
	ShowWindow(thisissola1.get_hwnd(),SW_HIDE);


	if(thisissola2.reg_win(hInstance,0,sola,TEXT("ert2")))//����ע��
		thisissola2.set_hwnd(thisissola2.create_win(hInstance,nCmdShow,TEXT("�ٴε��������ʾ�ر�")
		,0,WS_VISIBLE|WS_POPUP|WS_EX_LAYERED,700,300,800,400));//��������
	ShowWindow(thisissola2.get_hwnd(),SW_HIDE);




	LRESULT CALLBACK callback(HWND , UINT , WPARAM , LPARAM );//�ص���������

	mainwindow.set_icon("123.ico");
	if(mainwindow.reg_win(hInstance,0,callback,TEXT("asd")))//����ע��
		mainwindow.set_hwnd(mainwindow.create_win(hInstance,nCmdShow,TEXT("������ʿ���ܸ���ϵͳ")
		,0,WS_OVERLAPPEDWINDOW,250,150,800+10,392+105));//��������
	ShowWindow(mainwindow.get_hwnd(),SW_HIDE);


	
	

	//create_button(hInstance,mainwindow.get_hwnd());
	//���봰��
#if 1
	

	create_in();

#endif
	HANDLE hth1;
	DWORD WINAPI funhear(LPVOID lpParaneter);
	mainwindow.craete_new_road(hth1,funhear);

	LRESULT CALLBACK map_callback(HWND , UINT , WPARAM , LPARAM );//�ص���������
	if(map.reg_win(hInstance,0,map_callback,TEXT("map")))//����ע��
		map.set_hwnd(map.create_win(hInstance,nCmdShow,TEXT("MAP"),0,WS_VISIBLE|WS_POPUP|WS_SIZEBOX,
		0,50,0,0,mainwindow.get_hwnd()));//��������

	map_p.set_bmp("�ٶȵ�ͼ.bmp",1);

	SetWindowPos(map.get_hwnd(),0,0,0,map_p.get_bmp_w(),map_p.get_bmp_h(),2);

	

	


	ShowWindow(map.get_hwnd(),SW_HIDE);
	GetWindowRect(map.get_hwnd(),&r1);

	LRESULT CALLBACK start_callback(HWND , UINT , WPARAM , LPARAM );//�ص���������
	if(start.reg_win(hInstance,0,start_callback,TEXT("map_s")))//����ע��
		start.set_hwnd(start.create_win(hInstance,nCmdShow,TEXT("MAP"),0,
		WS_VISIBLE|WS_POPUP|WS_EX_LAYERED,
		0,0,100,100,map.get_hwnd()));//��������

	ShowWindow(start.get_hwnd(),SW_HIDE);
	start_p.set_bmp("o1.bmp",1);

	SetWindowPos(start.get_hwnd(),0,r1.left,r1.top,start_p.get_bmp_w(),start_p.get_bmp_h(),2);

	LRESULT CALLBACK end_callback(HWND , UINT , WPARAM , LPARAM );//�ص���������
	if(end.reg_win(hInstance,0,end_callback,TEXT("map_end")))//����ע��
		end.set_hwnd(end.create_win(hInstance,nCmdShow,TEXT("MAP"),0,
		WS_VISIBLE|WS_POPUP|WS_EX_LAYERED,
		0,0,100,100,map.get_hwnd()));//��������

	ShowWindow(end.get_hwnd(),SW_HIDE);
	end_p.set_bmp("o2.bmp",1);

	int x=40;
	HWND hinbutt7 = CreateWindow(TEXT("BUTTON"),TEXT("��������ģʽ"),   WS_CHILD|WS_VISIBLE,
		0,432-x,143,60,	
		mainwindow.get_hwnd(),
		(HMENU)7000,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	HWND hinbutt8 = CreateWindow(TEXT("BUTTON"),TEXT("��������ģʽ"),WS_CHILD|WS_VISIBLE,
		143,432-x,142,60,	
		mainwindow.get_hwnd(),
		(HMENU)8000,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);
	

	HWND hinbutt3 = CreateWindow(TEXT("BUTTON"),TEXT("�����㲥"),WS_CHILD|WS_VISIBLE,
		300-15,432-x,100,60,	
		mainwindow.get_hwnd(),
		(HMENU)2000,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	HWND hinbutt4 = CreateWindow(TEXT("BUTTON"),TEXT("��������"),WS_CHILD|WS_VISIBLE,
		400-15,432-x,100,60,	
		mainwindow.get_hwnd(),
		(HMENU)3000,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	HWND hinbutt5 = CreateWindow(TEXT("BUTTON"),TEXT("��������"),WS_CHILD|WS_VISIBLE,
		500-15,432-x,100,60,	
		mainwindow.get_hwnd(),
		(HMENU)4000,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	HWND hinbutt6 = CreateWindow(TEXT("BUTTON"),TEXT("��ѯʱ��"),WS_CHILD|WS_VISIBLE,
		600-15,432-x,100,60,	
		mainwindow.get_hwnd(),
		(HMENU)5000,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	HWND hinbutt2 = CreateWindow(TEXT("BUTTON"),TEXT("������ʾ"),WS_CHILD|WS_VISIBLE,
		700-15,432-x,110,60,	
		mainwindow.get_hwnd(),
		(HMENU)1000,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);
	
	HANDLE hth2;
	DWORD WINAPI funread(LPVOID lpParaneter);
	thisissola.craete_new_road(hth1,funread);


	cqupt.read("����.txt");
	
	SetWindowPos(end.get_hwnd(),0,r1.left,r1.top,end_p.get_bmp_w(),end_p.get_bmp_h(),2);

	//KillProcess(GetProcess(L"QQ.exe"));
	//this_way("asd.asd");
	out_message_win(0,0,mainwindow.get_hInstance(),mainwindow.get_hwnd(),800);
	ShowWindow(output_win.get_hwnd(),SW_HIDE);

	saipvoi("��ӭʹ��������ʿ���ܸ���ϵͳ��");


	LRESULT CALLBACK n_callback(HWND , UINT , WPARAM , LPARAM );//�ص���������
		if(internet.reg_win(hInstance,0,n_callback,TEXT("net")))//����ע��
			 internet.set_hwnd(internet.create_win(hInstance,nCmdShow,TEXT("window_n"),0,
			 WS_VISIBLE|WS_POPUP|WS_EX_LAYERED,200,200,200,250,mainwindow.get_hwnd()));//��������
		ShowWindow(internet.get_hwnd(),SW_HIDE);

		HWND hb0 = CreateWindow(TEXT("BUTTON"),TEXT("�ر�"),WS_CHILD|WS_VISIBLE,
		0,0,200,50,	
		internet.get_hwnd(),
		(HMENU)100,
		(HINSTANCE) GetWindowLong(internet.get_hwnd(), GWL_HINSTANCE),
		0);
		HWND hb1 = CreateWindow(TEXT("BUTTON"),TEXT("���ֵ㲥"),WS_CHILD|WS_VISIBLE,
		0,50,200,100,	
		internet.get_hwnd(),
		(HMENU)1000,
		(HINSTANCE) GetWindowLong(internet.get_hwnd(), GWL_HINSTANCE),
		0);
		HWND hb2 = CreateWindow(TEXT("BUTTON"),TEXT("��̨�㲥"),WS_CHILD|WS_VISIBLE,
		0,150,200,100,	
		internet.get_hwnd(),
		(HMENU)2000,
		(HINSTANCE) GetWindowLong(internet.get_hwnd(), GWL_HINSTANCE),
		0);

	return (int)msg.message_while(3);
}


