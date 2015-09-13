my_msg msg;
my_netlink net;
my_window mainwindow;
my_window thisissola;
my_window thisissola1;
my_window thisissola2;
my_window internet;
my_window inwindow;
my_window map;
my_window start;
my_window end;
my_print map_p;
my_print open;
my_print mainp;
my_print inp;
my_print inwinback;
my_print start_p;
my_print end_p;
my_PC pc;
my_mci a1;
my_mci a2;
my_mci a3;
int is_play0 = 0;
int is_play1 = 0;
int is_play2 = 0;
int is_weather = 0;
int is_read = 0;
int lu=0;//路径标识

char strin[255]="\0";
char map_s[255]="\0";
char str_out[255]="\0";

int is_speak = 0;

int map_f=0;//地图导航是否启动

int saipvoi(char *str);
void look_good();

int talk=0;//对话开关

int on_move=0;

int is_ch=0;//语音点播模式是否打开

int is_run=0;//视频运行情况

char rec_pic[320*240]="\0";//接受图片的buff
int net_r=0;//0为开始接受，1正在接受，2接受结束

char but_str[5][20]={"\0","\0","\0","\0","\0"};

POINT p;
RECT r;
RECT r1;
HWND hWndEdit;

HWND hinbutt16;

LRESULT CALLBACK incallback(HWND , UINT , WPARAM , LPARAM );
void run(char *);
typedef BOOL(*UPDATELAYEREDWINDOWFUNCTION)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);