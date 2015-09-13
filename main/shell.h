
int is_mci=0;

void com(char *strin)
{
	if(strstr(strin,"关闭")&&strstr(strin,"系统"))
	{
		is_read = 0;
		saipvoi("正在关闭系统！");
		Sleep(2000);
		exit(0);
	}

	if(talk==0)
	{

		if(strstr(strin,"打开")&&strstr(strin,"系统"))
		{
			is_read = 0;
			ShowWindow(thisissola.get_hwnd(),SW_HIDE);
			look_good();
			saipvoi("智能行走辅助系统正在启动！");
		}
		else if(strstr(strin,"打开")&&strstr(strin,"语音"))
		{
			is_read = 0;
			talk=1;
			ShowWindow(output_win.get_hwnd(),SW_SHOW);
			ShowWindow(inwindow.get_hwnd(),SW_SHOW);
			saipvoi("自由聊天已启动！");


		}
		else if(strstr(strin,"打开")&&strstr(strin,"导航"))
		{
			is_read = 0;
			saipvoi("导航模块已启动！");
			talk=2;
			ShowWindow(map.get_hwnd(),SW_SHOW);
			ShowWindow(start.get_hwnd(),SW_SHOW);
			ShowWindow(end.get_hwnd(),SW_SHOW);
			GetWindowRect(map.get_hwnd(),&r1);
			ShowWindow(inwindow.get_hwnd(),SW_SHOW);

		}
		else if(strstr(strin,"播放")||strstr(strin,"音乐"))
		{
			is_read = 0;
			BOOL ping = InternetCheckConnection(L"http://www.baidu.com", 1, 0);
			if (ping)
			{
				run("music.html");
			}
			else
			{

				is_play1 = 1;
				SetWindowLong(thisissola.get_hwnd(),GWL_STYLE,WS_OVERLAPPEDWINDOW);
				ShowWindow(thisissola.get_hwnd(),SW_SHOW);
				a1.mic_avi_play(L"2.wav",thisissola.get_hwnd());
				SetWindowPos(thisissola.get_hwnd(),0,0,0,a1.mci_getWidth(),a1.mci_getHight()+50,SWP_SHOWWINDOW);
			}
		}
		else if(strstr(strin,"点播")||strstr(strin,"电台"))
		{
			BOOL ping = InternetCheckConnection(L"http://www.baidu.com", 1, 0);
			if (ping)
			{
				run("radio.html");
			}
			else
			{
				saipvoi("网络无法连接！");
			}
		}
		else if(strstr(strin,"新闻"))
		{
			is_read = 0;
			SetWindowLong(thisissola1.get_hwnd(),GWL_STYLE,WS_OVERLAPPEDWINDOW);
			ShowWindow(thisissola1.get_hwnd(),SW_SHOW);
			a2.mic_avi_play(L"new.wmv",thisissola1.get_hwnd());
			SetWindowPos(thisissola1.get_hwnd(),0,0,0,a2.mci_getWidth(),a2.mci_getHight()+50,2);
			is_play2=1;

		}

	}
	else  if(talk==1)
	{
		is_read = 0;

		if(strstr(strin,"退出")&&strstr(strin,"语音"))
		{
			talk=0;
			ShowWindow(inwindow.get_hwnd(),SW_HIDE);
			out_message_win(3);
		}
		else
			saipvoi(AI.find(strin));


	}

	else  if(talk==2)
	{
		is_read = 0;
		if(strstr(strin,"关闭")&&strstr(strin,"导航"))
		{
			talk=0;
		}
		else
		{
			cqupt.where_go(cqupt.selete(strin));

		}
	}

}


void look_good()
{
	int t=100;

	ShowWindow(mainwindow.get_hwnd(),SW_SHOW);
	SetLayeredWindowAttributes(mainwindow.get_hwnd(),RGB(255,255,255),50,LWA_ALPHA|LWA_COLORKEY);
	mainp.set_bmp("m1.bmp");
	mainwindow.update_win(mainwindow.get_hwnd());
	Sleep(t);

	SetLayeredWindowAttributes(mainwindow.get_hwnd(),RGB(255,255,255),90,LWA_ALPHA|LWA_COLORKEY);
	mainp.set_bmp("m2.bmp");
	mainwindow.update_win(mainwindow.get_hwnd());
	Sleep(t);

	SetLayeredWindowAttributes(mainwindow.get_hwnd(),RGB(255,255,255),130,LWA_ALPHA|LWA_COLORKEY);
	mainp.set_bmp("m3.bmp");
	mainwindow.update_win(mainwindow.get_hwnd());
	Sleep(t);

	SetLayeredWindowAttributes(mainwindow.get_hwnd(),RGB(255,255,255),180,LWA_ALPHA|LWA_COLORKEY);
	mainp.set_bmp("m4.bmp");
	mainwindow.update_win(mainwindow.get_hwnd());
	Sleep(t);

	SetLayeredWindowAttributes(mainwindow.get_hwnd(),RGB(255,255,255),250,LWA_ALPHA|LWA_COLORKEY);
	mainp.set_bmp("m5.bmp");
	mainwindow.update_win(mainwindow.get_hwnd());
	Sleep(t);

	SetLayeredWindowAttributes(mainwindow.get_hwnd(),RGB(255,255,255),255,LWA_ALPHA|LWA_COLORKEY);
	mainp.set_bmp("m6.bmp");
	mainwindow.update_win(mainwindow.get_hwnd());
	Sleep(t);

	SetLayeredWindowAttributes(mainwindow.get_hwnd(),RGB(255,255,255),255,LWA_ALPHA|LWA_COLORKEY);
	mainp.set_bmp("main3.bmp");
	mainwindow.update_win(mainwindow.get_hwnd());
}


HWND hinbutt10,hinbutt11,hinbutt12,hinbutt13,hinbutt14,hinbutt15,hinbutt17;
void create_in()
{
	if(inwindow.reg_win((HINSTANCE) GetWindowLong(mainwindow.get_hwnd(),
		GWL_HINSTANCE),0,/**/incallback/**/,TEXT("in")))
		inwindow.set_hwnd(inwindow.create_win(
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), 
		GWL_HINSTANCE),SW_SHOW,TEXT("inwindow"),0,WS_VISIBLE|WS_POPUP|WS_SIZEBOX,
		800,200,
		515,400,
		mainwindow.get_hwnd()
		));
	ShowWindow(inwindow.get_hwnd(),SW_HIDE);

	//输入框创建
	hWndEdit = CreateWindow(
		TEXT("EDIT"),
		NULL,
		WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE|ES_WANTRETURN,
		20,30,	
		360,
		75,
		inwindow.get_hwnd(),
		NULL,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		NULL);

	//创建按钮
	HWND hinbutt = CreateWindow(TEXT("BUTTON"),TEXT("发送"),WS_CHILD|WS_VISIBLE,
		380,30,100,75,	
		inwindow.get_hwnd(),
		(HMENU)1000,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	hinbutt10 = CreateWindowA("BUTTON","阳光图文",WS_CHILD|WS_VISIBLE,
		20,150,100,40,	
		inwindow.get_hwnd(),
		(HMENU)100,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	hinbutt11 = CreateWindowA("BUTTON","眼睛店",WS_CHILD|WS_VISIBLE,
		140,150,100,40,	
		inwindow.get_hwnd(),
		(HMENU)200,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	hinbutt12 = CreateWindowA("BUTTON","继续",WS_CHILD|WS_VISIBLE,
		260,150,100,40,	
		inwindow.get_hwnd(),
		(HMENU)300,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);




	hinbutt13 = CreateWindowA("BUTTON","查询天气",WS_CHILD|WS_VISIBLE,
		20,150,100,40,	
		inwindow.get_hwnd(),
		(HMENU)400,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	hinbutt14 = CreateWindowA("BUTTON","今天星期几？",WS_CHILD|WS_VISIBLE,
		20,200,150,40,		
		inwindow.get_hwnd(),
		(HMENU)500,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	hinbutt15 = CreateWindowA("BUTTON","告诉我今天的日期",WS_CHILD|WS_VISIBLE,
		20,250,180,40,	
		inwindow.get_hwnd(),
		(HMENU)600,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	hinbutt17 = CreateWindowA("BUTTON","退出聊天",WS_CHILD|WS_VISIBLE,
		405,350,100,40,	
		inwindow.get_hwnd(),
		(HMENU)700,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);

	hinbutt16 = CreateWindow(TEXT("BUTTON"),TEXT("退出导航"),WS_CHILD|WS_VISIBLE,
		390,150,100,40,	
		inwindow.get_hwnd(),
		(HMENU)800,
		(HINSTANCE) GetWindowLong(mainwindow.get_hwnd(), GWL_HINSTANCE),
		0);
}


void createbut1()
{
	ShowWindow(hinbutt12,SW_HIDE);
	ShowWindow(hinbutt11,SW_HIDE);
	ShowWindow(hinbutt10,SW_HIDE);
	ShowWindow(hinbutt16,SW_HIDE);

	ShowWindow(hinbutt13,SW_SHOW);
	ShowWindow(hinbutt14,SW_SHOW);
	ShowWindow(hinbutt15,SW_SHOW);
	ShowWindow(hinbutt17,SW_SHOW);
}

void createbut2()
{
	ShowWindow(hinbutt12,SW_SHOW);
	ShowWindow(hinbutt11,SW_SHOW);
	ShowWindow(hinbutt10,SW_SHOW);
	ShowWindow(hinbutt16,SW_SHOW);

	ShowWindow(hinbutt13,SW_HIDE);
	ShowWindow(hinbutt14,SW_HIDE);
	ShowWindow(hinbutt15,SW_HIDE);
	ShowWindow(hinbutt17,SW_HIDE);
}