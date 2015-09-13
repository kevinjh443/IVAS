#include <tlhelp32.h> 
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
void run(char *str);
char * this_way(char *str);
int GetProcess(WCHAR *str);
int KillProcess(DWORD Pid);

class way //储存各种起点终点     @明天查画直线的函数
{
	struct s_e
	{
		int s_x;
		int s_y;
		int e_x;
		int e_y;
		int lu;

		s_e *next;
	};
public:

	s_e *first;
	s_e *more;
	s_e *pass;

	~way()
	{
		more=first;
		for(int i=0;more;i++)
		{
			pass=more;
			more=more->next;
			free(pass);
		}
	}

	void read(char *flie)
	{
		FILE *f=fopen(flie,"r");
		if(!(f))
		{
			MessageBoxA(0,"文本打开失败",0,0);
			return ;
		}

		while(!feof(f))
		{
			more=(s_e *)malloc(sizeof(s_e));

			fscanf(f,"%d\t%d\t%d\t%d\t%d\n",&more->e_x,&more->e_y,&more->s_x,&more->s_y,&more->lu);


			more->next=0;

			if(first==0)first=more;
			if(pass!=0)pass->next=more;
			pass=more;

		}
		fclose(f);




	}

	void start_end(int x)//路段编号
	{
		more=first;

		while(more)
		{
			if(x==more->lu)
			{
				
				GetWindowRect(map.get_hwnd(),&r1);

				SetWindowPos(end.get_hwnd(),0,more->e_x+r1.left-10,more->e_y+r1.top-25,
					end_p.get_bmp_w(),end_p.get_bmp_h(),SWP_SHOWWINDOW);

				SetWindowPos(start.get_hwnd(),0,more->s_x+r1.left-10,more->s_y+r1.top-25,
					end_p.get_bmp_w(),end_p.get_bmp_h(),SWP_SHOWWINDOW);

				ShowWindow(start.get_hwnd(),SW_SHOW);
				ShowWindow(end.get_hwnd(),SW_SHOW);

				break;
			}
			more=more->next;
		}
		if(!more)
		{
			MessageBoxA(0,"未找到路径",0,0);
		}
	}

	//KillProcess(GetProcess(""));
	void vod(int flag=0,int l=0)
	{
		char str[300];
		if(flag==0)
		{
			saipvoi("到达目的地！");
			if(is_run==1)KillProcess(GetProcess(L"BRoad.exe"));
			if(is_run==2){KillProcess(GetProcess(L"ZebraCRO.exe"));KillProcess(GetProcess(L"TrfcLights.exe"));}
			if(is_run==3)KillProcess(GetProcess(L"Ladder.exe"));
		}
		if(flag==1)
		{
			if(is_run==2){KillProcess(GetProcess(L"ZebraCRO.exe"));KillProcess(GetProcess(L"TrfcLights.exe"));}
			if(is_run==3)KillProcess(GetProcess(L"Ladder.exe"));
			if(is_run!=1)
			{
				saipvoi("如果检测到盲道则爆鸣");
				run("BRoad.exe");
			}
			is_run=1;
			sprintf(str,"向前走%d米",l);
			saipvoi(str);
		}
		if(flag==2)
		{
			if(is_run==2){KillProcess(GetProcess(L"ZebraCRO.exe"));KillProcess(GetProcess(L"TrfcLights.exe"));}
			if(is_run==3)KillProcess(GetProcess(L"Ladder.exe"));
			if(is_run!=1)
			{
				saipvoi("如果检测到盲道则爆鸣");
				run("BRoad.exe");
			}
			is_run=1;
			sprintf(str,"向后走%d米",l);
			saipvoi(str);
		}
		if(flag==3)
		{
			if(is_run==2){KillProcess(GetProcess(L"ZebraCRO.exe"));KillProcess(GetProcess(L"TrfcLights.exe"));}
			if(is_run==3)KillProcess(GetProcess(L"Ladder.exe"));
			if(is_run!=1)
			{
				saipvoi("如果检测到盲道则爆鸣");
				run("BRoad.exe");
			}
			is_run=1;
			sprintf(str,"向左走%d米",l);
			saipvoi(str);
		}
		if(flag==4)
		{
			if(is_run==2){KillProcess(GetProcess(L"ZebraCRO.exe"));KillProcess(GetProcess(L"TrfcLights.exe"));}
			if(is_run==3)KillProcess(GetProcess(L"Ladder.exe"));
			if(is_run!=1)
			{
				saipvoi("如果检测到盲道则爆鸣");
				run("BRoad.exe");
			}
			is_run=1;
			sprintf(str,"向右走%d米",l);
			saipvoi(str);
		}
		if(flag==5)
		{
			if(is_run==1)KillProcess(GetProcess(L"BRoad.exe"));
			saipvoi("如果检测到斑马线则爆鸣");
			run("ZebraCRO.exe");//TrfcLights
			run("TrfcLights.exe");
			is_run=2;
			sprintf(str,"前方%d米处过马路",l);
			saipvoi(str);
		}
		if(flag==6)
		{
			if(is_run==1)KillProcess(GetProcess(L"BRoad.exe"));
			is_run=3;
			saipvoi("如果检测到楼梯则爆鸣");
			run("Ladder.exe");
			sprintf(str,"前方%d米处有楼梯",l);
			saipvoi(str);
		}

		
	}

	int selete(char *str)
	{

		if(strstr(strin,"到了")||strstr(strin,"继续"))
		{
			return 0;
		}

		if(strstr(strin,"停止导航"))
		{
			if(is_run==1)KillProcess(GetProcess(L"BRoad.exe"));
			if(is_run==2){KillProcess(GetProcess(L"ZebraCRO.exe"));KillProcess(GetProcess(L"TrfcLights.exe"));}
			if(is_run==3)KillProcess(GetProcess(L"Ladder.exe"));
			return -2;
		}

		if(strstr(strin,"继续教育")&&!strstr(map_s,"继续教育"))
		{
			strcpy(map_s,strin);
			return 1;
		}
		else if(strstr(strin,"七教")&&!strstr(map_s,"七教"))
		{
			strcpy(map_s,strin);
			return 2;
		}
		else if(strstr(strin,"阳光")&&!strstr(map_s,"阳光"))
		{
			strcpy(map_s,strin);
			return 3;
		}
		else if(strstr(strin,"医院")&&!strstr(map_s,"医院"))
		{
			strcpy(map_s,strin);
			return 4;
		}
		else if(strstr(strin,"邮政")&&!strstr(map_s,"邮政"))
		{
			strcpy(map_s,strin);
			return 5;
		}else if(strstr(strin,"色彩")&&!strstr(map_s,"色彩"))
		{
			strcpy(map_s,strin);
			return 6;
		}
		else if(strstr(strin,"茶楼")&&!strstr(map_s,"茶楼"))
		{
			strcpy(map_s,strin);
			return 7;
		}
		else if(strstr(strin,"汉堡")&&!strstr(map_s,"汉堡"))
		{
			strcpy(map_s,strin);
			return 8;
		}
		else if((strstr(strin,"商业")&&!strstr(map_s,"商业"))||
			(strstr(strin,"眼镜")&&!strstr(map_s,"眼镜"))
			)
		{
			strcpy(map_s,strin);
			return 9;
		}
		else if((strstr(strin,"茶饮")&&!strstr(map_s,"茶饮"))||
			(strstr(strin,"移动")&&!strstr(map_s,"移动"))||
			(strstr(strin,"网吧")&&!strstr(map_s,"网吧"))||
			(strstr(strin,"一心")&&!strstr(map_s,"一心"))
			)
		{
			strcpy(map_s,strin);
			return 10;
		}

		else if((strstr(strin,"大排档")&&!strstr(map_s,"大排档"))||
			(strstr(strin,"面庄")&&!strstr(map_s,"面庄"))||
			(strstr(strin,"百度")&&!strstr(map_s,"百度"))||
			(strstr(strin,"鱼庄")&&!strstr(map_s,"鱼庄"))||
			(strstr(strin,"老鸭汤")&&!strstr(map_s,"老鸭汤"))||
			(strstr(strin,"甜品")&&!strstr(map_s,"甜品"))||
			(strstr(strin,"火锅")&&!strstr(map_s,"火锅"))
			)
		{
			strcpy(map_s,strin);
			return 11;
		}


		return -1;


	}


	void where_go(int flag)
	{
		static int last=0;
		static int step=1;
		if(flag==0)flag=last;
		if(step==0&&flag!=last) step=1;
		if(flag==-2)
		{
			step=1;
			talk=0;
			return;
		}
		switch(flag)
		{
		case 1://继续教育学院
			switch(step)
			{
			case 1:
				start_end(1);
				vod(3,50);
				step+=1;
				break;
			case 2:
				start_end(2);
				vod(4,200);
				step+=1;
				break;
			case 3:
				start_end(3);
				vod(4,50);
				step=0;
				break;
			default:
				vod();
			}
			break;

		case 2://7教
			switch(step)
			{
			case 1:
				start_end(1);
				vod(3,50);
				step+=1;
				break;
			case 2:
				start_end(2);
				vod(4,200);
				step+=2;
				break;
			case 4:
				start_end(4);
				vod(1,50);
				step+=1;
				break;
			case 5:
				start_end(5);
				vod(3,50);
				step+=1;
				break;
			case 6:
				start_end(6);
				vod(3,50);
				step=0;
				break;
			default:
				vod();
			}

			break;

		case 3://阳光
			switch(step)
			{
			case 1:
				start_end(1);
				vod(3,50);
				step+=1;
				break;
			case 2:
				start_end(2);
				vod(4,200);
				step+=2;
				break;
			case 4:
				start_end(4);
				vod(1,50);
				step+=3;
				break;
			case 7:
				start_end(7);
				vod(4,100);
				vod(6,10);
				step+=1;
				break;
			case 8:
				start_end(8);
				vod(4,100);
				step=0;
				break;
			default:
				vod();
			}
			break;

		case 9://眼镜店
			switch(step)
			{
			case 1:
				start_end(1);
				vod(3,50);
				step+=1;
				break;
			case 2:
				start_end(2);
				vod(4,200);
				step+=2;
				break;
			case 4:
				start_end(4);
				vod(1,50);
				step+=3;
				break;
			case 7:
				start_end(7);
				vod(4,100);
				vod(6,10);
				step+=5;
				break;
			case 12:
				start_end(12);
				vod(1,150);
				step+=1;
				break;
			case 13:
				start_end(13);
				vod(1,30);
				vod(5,2);
				step=-1;
				break;
			default:
				vod();
				step=0;

			}
			break;

		}

		if(flag!=0)
			last=flag;

	}

};
way cqupt;

int KillProcess(DWORD Pid)
{ 
	//打开进程得到进程句柄 
	HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,Pid);
	if(hProcess==NULL)
	{
		MessageBoxA(0,"进程无法结束，请激活视频运行窗口，按ESC",0,0);
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

int GetProcess(WCHAR *str)
{
#if 1
	PROCESSENTRY32 pe32;
	pe32.dwSize=sizeof(pe32);
	//获得系统内所有进程快照
	HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap==INVALID_HANDLE_VALUE)
	{
		MessageBoxA(0,"无法打开进程，请关闭杀毒软件",0,0);
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
	if(!bProcess)return 0;
	CloseHandle(hProcessSnap);
	return pe32.th32ProcessID;
#else
	return 0;
#endif

}


char * this_way(char *str)
{
	char szFilePath[500] = {0};
	DWORD dwError = GetModuleFileNameA(NULL, szFilePath, ARRAYSIZE(szFilePath));
	if(dwError > 0)
	{
		PathRemoveFileSpecA(szFilePath);
		strcat(szFilePath,"\\");
		strcat(szFilePath,str);
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


void run(char *str)
{
#if 1
	
	char temp[500];
	sprintf(temp,"start %s",this_way(str));
	system(temp);
#endif
}//KillProcess(GetProcess(""));



void get_weather(char *filename)
{
	FILE *wp=fopen(filename,"r");
	char str[100]="\0";

	while(!feof(wp))
	{
		if(is_weather==1)
		{
			is_weather=0;
			break;
		}
		fscanf(wp,"%s\n",str);

		saipvoi(str);


	}
	fclose(wp);
}