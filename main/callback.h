LRESULT CALLBACK callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		PAINTSTRUCT ps;  
		HDC hdc;	
	case WM_CREATE:

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		mainp.laodbmp_and_output(hdc);
		//	inwinback.png_out(hWnd,L"in3.png");
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		GetCursorPos(&p);
		GetWindowRect(hWnd,&r);
#if 0
		sprintf(strin,"x=%d,y=%d",p.x-r.left,p.y-r.top);
		MessageBoxA(0,strin,0,0);
#else

		if(p.x-r.left>133&&p.x-r.left<365&&p.y-r.top>65&&p.y-r.top<359)
		{
			talk=2;
			ShowWindow(map.get_hwnd(),SW_SHOW);
			ShowWindow(start.get_hwnd(),SW_SHOW);
			ShowWindow(end.get_hwnd(),SW_SHOW);
			GetWindowRect(map.get_hwnd(),&r1);
			ShowWindow(inwindow.get_hwnd(),SW_SHOW);
			ShowWindow(hinbutt16,SW_SHOW);
			saipvoi("导航系统已启动！");
			mainwindow.THREAD_ON();
		}
		else if(p.x-r.left>393&&p.x-r.left<624&&p.y-r.top>69&&p.y-r.top<362)
		{

			ShowWindow(output_win.get_hwnd(),SW_SHOW);
			ShowWindow(inwindow.get_hwnd(),SW_SHOW);
			talk=1;
		}
#endif
		break;

	case WM_MOUSEMOVE:
		GetCursorPos(&p);
		GetWindowRect(hWnd,&r);

		if(p.x-r.left>133&&p.x-r.left<365&&p.y-r.top>65&&p.y-r.top<359)
		{

			if(on_move==0)
			{
				mainp.set_bmp("main1.bmp");
				mainp.update_win(hWnd);
			}
			on_move=1;
		}
		else if(p.x-r.left>393&&p.x-r.left<624&&p.y-r.top>69&&p.y-r.top<362)
		{

			if(on_move==0)
			{
				mainp.set_bmp("main2.bmp");
				mainp.update_win(hWnd);
			}
			on_move=1;
		}

		else if(p.x-r.left>699&&p.x-r.left<797&&p.y-r.top>325&&p.y-r.top<430)
		{

			if(on_move==0)
			{
				mainp.set_bmp("main.bmp");
				mainp.update_win(hWnd);
			}
			on_move=1;
		}

		else
		{

			if(on_move==1)
			{
				mainp.set_bmp("main3.bmp");
				mainp.update_win(hWnd);
			}
			on_move=0;
		}

		break;

	case WM_COMMAND:
		switch(wParam)
		{	
		case 1000:
			is_read = 0;
			if(is_play0==0)
			{
				SetWindowLong(thisissola2.get_hwnd(),GWL_STYLE,WS_OVERLAPPEDWINDOW);
				ShowWindow(thisissola2.get_hwnd(),SW_SHOW);
				a3.mic_avi_play(L"1.wmv",thisissola2.get_hwnd());
				SetWindowPos(thisissola2.get_hwnd(),0,0,0,a3.mci_getWidth(),a3.mci_getHight()+50,SWP_SHOWWINDOW);
				is_play0=1;
			}
			else
			{
				a3.mic_stop();
				ShowWindow(thisissola2.get_hwnd(),SW_HIDE);
				is_play0=0;
			}
			break;
		case 2000:
			///////////////////////////
			{
				is_read = 0;
				if(is_play1==1)
				{
						//a1.mic_stop();
					//mciSendString("stop");
					PlaySound(NULL,NULL,NULL);
						ShowWindow(thisissola.get_hwnd(),SW_HIDE);
						is_play1=0;
						break;
				}
				BOOL ping = InternetCheckConnection(L"http://www.baidu.com", 1, 0);
				if (ping)
				{
					ShowWindow(internet.get_hwnd(),SW_SHOW);
				}
				else
				{
					saipvoi("无法建立网络了连接，播放本地资源吗");
					MessageBoxA(0,"无法建立连接，播放本地资源",0,0);

					if(is_play1==0)
					{

						//SetWindowLong(thisissola.get_hwnd(),GWL_STYLE,WS_OVERLAPPEDWINDOW);
						//ShowWindow(thisissola.get_hwnd(),SW_SHOW);
						//mciSendString("play lcmusic1.wav","",0,NULL);//播放音乐
						srand(time(0));
						int flag_music=rand()%2;
						if (0 == flag_music)
						{
							PlaySound(L"lcmusic1.wav",NULL,SND_FILENAME|SND_ASYNC);
							is_play1=1;
						}else{
							PlaySound(L"lcmusic2.wav",NULL,SND_FILENAME|SND_ASYNC);
							is_play1=1;
						}
						
						is_play1=1;
						/*
						srand(time(0));
						int flag_music=rand()%2;
						
							if (0==flag_music)
							{
								a1.mic_avi_play(L"lcmusic1.wav",thisissola.get_hwnd());
							SetWindowPos(thisissola.get_hwnd(),0,0,0,a1.mci_getWidth(),a1.mci_getHight()+50,2);
							
							}else{
								a1.mic_avi_play(L"lcmusic2.wav",thisissola.get_hwnd());
								SetWindowPos(thisissola.get_hwnd(),0,0,0,a1.mci_getWidth(),a1.mci_getHight()+50,2);
							
							}
							is_play1=1;
						*/
					}
					
				}
			}
			////////////////////////////////////
			break;
		case 3000:
			is_read = 0;
			if(is_play2==0)
			{
				SetWindowLong(thisissola1.get_hwnd(),GWL_STYLE,WS_OVERLAPPEDWINDOW);
				ShowWindow(thisissola1.get_hwnd(),SW_SHOW);
				a2.mic_avi_play(L"new.wmv",thisissola1.get_hwnd());
				SetWindowPos(thisissola1.get_hwnd(),0,0,0,a2.mci_getWidth(),a2.mci_getHight()+50,2);
				is_play2=1;
				//run("WinExec_百度搜索.htm");
			}
			else
			{
				a2.mic_stop();
				ShowWindow(thisissola1.get_hwnd(),SW_HIDE);
				is_play2=0;
			}
			break;
		case 4000:
			run("ws_weather.exe");
			is_read = 1;

			break;
		case 5000:
			is_read = 0;
			AI.ai_fun("今天几号");
			AI.ai_fun("现在时间");
			AI.ai_fun("星期几");
			break;
		case 7000:
			if(talk == 1)
			{
				ShowWindow(output_win.get_hwnd(),SW_HIDE);
				ShowWindow(inwindow.get_hwnd(),SW_HIDE);
				talk=0;
			}
			is_read = 0;
			saipvoi("导航模块已启动！");
			talk=2;
			ShowWindow(map.get_hwnd(),SW_SHOW);
			ShowWindow(start.get_hwnd(),SW_SHOW);
			ShowWindow(end.get_hwnd(),SW_SHOW);
			GetWindowRect(map.get_hwnd(),&r1);
			ShowWindow(inwindow.get_hwnd(),SW_SHOW);
			break;
		case 8000:
			if(talk == 2)
			{
				if(is_run==1)KillProcess(GetProcess(L"BRoad.exe"));
				if(is_run==2){KillProcess(GetProcess(L"ZebraCRO.exe"));KillProcess(GetProcess(L"TrfcLights.exe"));}
				if(is_run==3)KillProcess(GetProcess(L"Ladder.exe"));
				ShowWindow(map.get_hwnd(),SW_HIDE);
				ShowWindow(start.get_hwnd(),SW_HIDE);
				ShowWindow(end.get_hwnd(),SW_HIDE);
				ShowWindow(inwindow.get_hwnd(),SW_HIDE);
			}
			is_read = 0;
			talk=1;
			ShowWindow(output_win.get_hwnd(),SW_SHOW);
			ShowWindow(inwindow.get_hwnd(),SW_SHOW);
			saipvoi("自由聊天已启动！");
			break;

		}
		break;

	case WM_RBUTTONDOWN:
		break;

	case WM_DESTROY:	
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}


LRESULT CALLBACK sola(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		PAINTSTRUCT ps;  
		HDC hdc;		
	case WM_CREATE:
		SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)^WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd,RGB(255,255,255),0,LWA_COLORKEY);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		open.laodbmp_and_output(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		//ShowWindow(thisissola.get_hwnd(),SW_HIDE);
		//look_good();
		break;
	case WM_DESTROY:	
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK start_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		PAINTSTRUCT ps;  
		HDC hdc;		
	case WM_CREATE:
		SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)^WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd,RGB(255,255,255),0,LWA_COLORKEY);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		start_p.laodbmp_and_output(hdc);
		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK end_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		PAINTSTRUCT ps;  
		HDC hdc;		
	case WM_CREATE:
		SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)^WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd,RGB(255,255,255),0,LWA_COLORKEY);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		end_p.laodbmp_and_output(hdc);
		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

#if 1

LRESULT CALLBACK incallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		PAINTSTRUCT ps;  
		HDC hdc;		
	case WM_CREATE:

		break;

	case WM_LBUTTONDOWN:
		SendMessage(hWnd,WM_NCLBUTTONDOWN,HTCAPTION,lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		inp.set_bmp("in2.bmp");
		inp.laodbmp_and_output(hdc);
		SetBkMode(hdc,TRANSPARENT);
		TextOutA(hdc,0,0,"若语音无法输入，请使用本窗口键入信息",36);
		TextOutA(hdc,20,120,"快捷输入：",10);
		if(talk == 1)
		{
			createbut1();
		}
		else if(talk == 2)
		{
			createbut2();
			TextOutA(hdc,0,200,"*起点为计算机科学与技术学院",
				strlen("*起点为计算机科学与技术学院"));
			TextOutA(hdc,0,230,"*导航为仿真，为体现楼梯，盲道，斑马线，红绿的识别情况",
				strlen("*导航为仿真，为体现楼梯，盲道，斑马线，红绿的识别情况"));
			TextOutA(hdc,0,260,"*实际内容与重庆邮电大学实际路况并非全部相符",
				strlen("*实际内容与重庆邮电大学实际路况并非全部相符"));
			TextOutA(hdc,0,290,"*导航模式的仿真是确定了目的地后将路径分为多段路程",
				strlen("*导航模式的仿真是确定了目的地后将路径分为多段路程"));
			TextOutA(hdc,0,320,"*输入“到了”或者“继续”这到下一段路",
				strlen("*输入“到了”或者“继续”这到下一段路"));
			TextOutA(hdc,0,350,"*输入“停止导航”或点击map窗口的“退出导航”，则退出",
				strlen("*输入“停止导航”或点击map窗口的“退出导航”，则退出"));
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_COMMAND:
		switch(wParam)
		{	
		case 1000:
			GetWindowTextA(hWndEdit,strin,300-1);           //获取编辑框内容

			SetWindowText(hWndEdit, NULL);//湛蓝的青空
			if(talk==1)
			{
				if(strstr(strin,"退出")&&strstr(strin,"聊天"))
				{
					talk=0;
					ShowWindow(inwindow.get_hwnd(),SW_HIDE);
					out_message_win(3);
				}
				else
				{
					if(!AI.ai_fun(strin))
					{
						strcpy(str_out,AI.find(strin));
					if(str_out)
					out_message_win(1,str_out);
					if(is_speak==0)
						saipvoi(str_out);
					//AI.find(strin);
					}
				}
			}
			else if(talk==2)
			{
				cqupt.where_go(cqupt.selete(strin));
			}

			break;

		case 100:
			SetWindowText(hWndEdit, L"阳光图文");
			break;

		case 200:
			SetWindowText(hWndEdit, L"眼睛店");
			break;

		case 300:
			SetWindowText(hWndEdit, L"继续");
			break;

		case 400:
			SetWindowText(hWndEdit, L"查询天气");
			break;

		case 500:
			SetWindowText(hWndEdit, L"今天星期几?");
			break;

		case 600:
			SetWindowText(hWndEdit, L"告诉我今天的日期");
			break;

		case 700:
			ShowWindow(output_win.get_hwnd(),SW_HIDE);
			ShowWindow(inwindow.get_hwnd(),SW_HIDE);
			talk=0;
			break;

		case 800:
			talk = 0;
			if(is_run==1)KillProcess(GetProcess(L"BRoad.exe"));
			if(is_run==2){KillProcess(GetProcess(L"ZebraCRO.exe"));KillProcess(GetProcess(L"TrfcLights.exe"));}
			if(is_run==3)KillProcess(GetProcess(L"Ladder.exe"));
			ShowWindow(map.get_hwnd(),SW_HIDE);
			ShowWindow(start.get_hwnd(),SW_HIDE);
			ShowWindow(end.get_hwnd(),SW_HIDE);
			ShowWindow(inwindow.get_hwnd(),SW_HIDE);
			break;
		}
		break;
	case WM_DESTROY:	
		PostQuitMessage(0);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
#endif


LRESULT CALLBACK map_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		PAINTSTRUCT ps;  
		HDC hdc;
		FILE *f;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		map_p.laodbmp_and_output(hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		SendMessage(hWnd,WM_NCLBUTTONDOWN,HTCAPTION,lParam);
		break;

	case WM_RBUTTONDOWN:
#if 1
		GetCursorPos(&p);
		GetWindowRect(hWnd,&r);
		sprintf(strin,"x=%d,y=%d",p.x-r.left,p.y-r.top);
		MessageBoxA(0,strin,0,0);
#endif
		break;



	case WM_DESTROY:	
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}



LRESULT CALLBACK n_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
	switch (message)
	{
	PAINTSTRUCT ps;  
	HDC hdc;		

	case WM_COMMAND:
		switch(wParam)
		{	
		case 100:
			ShowWindow(internet.get_hwnd(),SW_HIDE);
			break;
		case 1000:
			run("music.html");
			break;
		case 2000:
			run("radio.html");
			break;
		}
	break;

	default:
	return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}