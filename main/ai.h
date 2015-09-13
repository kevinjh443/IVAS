#if 0

这是个奇迹，
	一个机器人为了被赋予的「心」而歌唱，
	是如此地真实。

	但是这份奇迹是那么地短暂。
	对她来说「心」是那么地庞大，
	使她无法承受这份重量。
	这个机器人就这么启动着并永远地坐在那。

	她微笑着，
	看上去就像个天使…

#endif
#include <math.h>
#define BSIZE 200

struct lisp_data//反馈层信息
{
	char ex_str[200];//样例输入语句
	char str[BSIZE];//返回的话
	char main_str[10][20];
	long int lisp;

	lisp_data *next;
};

class my_ai_link
{
	long int now_lisp;//当前上下文匹配链参数

	int learn; //学习模式

	char Nouns[20];//记录的名词
	char Nouns_m[100];//记录名词的解释

	int change;//是否可以转移话题

	char file_1[255],file_2[255],file_3[255];

	struct link_str
	{
		int id;//与结果集对应
		char ex_str[200];//样例输入语句
		char str[BSIZE];//返回的话
		char main_str[50];
		//对应关键字表，实现任何数量的关键字存储识别，规则：1241120143424,124112表示第一个词，用0划分开
		//每个字用3位数表示，1最小，比如第一个数就为111，最后一个为999，共可以表达888个字
		long  lisp;
		//上下文匹配链，实现任何数量的语句上下文关联
		//规则：1232，表示优先对前缀为1232的语句进行搜索，若未找到则对123前缀的搜索，以此类推。最小为0阶
		//对话的开始只对0阶做搜索。
		int flag;
		//标记该词条返回上层网络搜索是否适用与上述未找到的情况。
		link_str * next;
		link_str *last;
	};

	link_str *first;
	link_str *more;
	link_str *pass;

	struct key//sigle
	{
		char word[3];
		int id;
		key *next;
	};

	key *head;
	key *add;
	key *temp;

	struct Key_word//关键词表，对关键词与输入句对比后，将结果存入结果表
	{
		int flag;//1为存在此关键字,若不符合lisp，全为0
		char keyword[15];
		Key_word(){flag=0;}
	}key_word[10];

	struct RES//结果集，可以在链表中找出res最大的为最接近结果的数据
	{
		int x[10];//一个关键词匹配给元素为1，未匹配-1，flag为0则为0
		int id;
		int res()//返回该组关键词的命中率
		{
			int ret=0;
			for(int q=0;q<10;q++)
			{
				ret+=x[q];
			}
			return ret;
		}
		RES *next;//链状的结果集，每一节点对应一组关键词
	};
	RES *top;//链表顶部
	RES *up;
	RES *down;

	struct knowledge
	{
		int id;//编号
		char str[20];//记录的词语
		char mean[200];//相应的解释
		int get_n;//相对的取词作用范围，正代表向右取，负代表向左取
		int do_c;//执行的相应操作，0代表没有

		knowledge *next;
	};
	knowledge *k_1;
	knowledge *k_2;
	knowledge *k_3;


	void res_free()
	{
		up=top;
		while(up)
		{
			down=up;
			up=up->next;
			free(down);
			down=0;
		}
		top = 0;
		up = 0;
	}

	void word_to_str( char *x,int list)//将link_str中存储的main_str在key中找字并转化成字符串存在Key_word中
	{//49='1'//list为more->lisp
		int num3[3];
		int count=0;
		int zero=0;

		count=strlen(x);

		for(int j=0;j<10;j++)//每次调用，数据清空
		{
			key_word[j].flag=0;
			memset(key_word[j].keyword,0,15);
		}

		if(run_last_lisp(list)!=now_lisp)return;

		for(int i=0;i<count;i+=3)
		{
			if(x[i]-48==0)
			{
				zero++;
				i-=2;
				continue;
			}

			num3[0]=x[i]-48;
			num3[1]=x[i+1]-48;
			num3[2]=x[i+2]-48;

			add=head;
			while(add)
			{
				if(add->id==num3[0]*100+num3[1]*10+num3[2])
				{
					if(key_word[zero].flag!=1)key_word[zero].keyword[0]='\0';
					strcat(key_word[zero].keyword,add->word);
					key_word[zero].flag=1;
					break;
				}
				add=add->next;
			}if(!add)
			{
				MessageBoxA(0,"缺少关键字",0,0);
			}



		}

	}

	int list_bit(int x)//统计现在位于层数
	{
		int i=1;
		int ret=1;

		while(x/i!=0)
		{
			i*=10;
			ret+=1;
		}

		return ret;
	}

	int run_last_lisp(int x)//进行上一层
	{
		int t=x;
		int w=list_bit(x);
		int ret=0;

		if(w==1)return 0;

		for(;w>1;w--)
		{
			ret=((t/(int)pow((double)10,w-1))*(int)pow((double)10,w-2))+ret;
			t=t%(int)pow((double)10,w-1);
		}

		return ret;
	}

	void free_lisp_data(lisp_data *x)//释放层信息
	{
		lisp_data *te;
		while(x)
		{
			te=x;
			x=x->next;
			free(te);
		}
	}

	int count_key_word(char *str)//统计关键词个数
	{
		int r=1;
		for(int i=0;str[i]!='\0';i++)
		{
			if(str[i]==48)
				r++;
		}
		return r;
	}

public:

	my_ai_link()
	{
		now_lisp=0;
		head=NULL;
		add=NULL;
		temp=NULL;
		first=NULL;
		more=NULL;
		pass=NULL;
		learn=0;
		change=0;
		Nouns[0]='\0';

	}

	int get_nowlisp()//返回外接现在层数
	{
		return now_lisp;
	}

	void up_nowlisp()//调用函数去找上一层
	{
		now_lisp=run_last_lisp(now_lisp);
	}

	void down_nowlisp(int x)//直接进行下一层（1-9：rand）
	{
		now_lisp=now_lisp*10+x;
	}

	void create_and_read(char *filename,char *keyfile,char *kown)//初始化，构建整个系统链表
	{
		int i=0;
		strcpy(file_1,filename);
		strcpy(file_2,keyfile);
		strcpy(file_3,kown);
		FILE *f=fopen(filename,"r");
		FILE *k=fopen(keyfile,"r");
		FILE *l=fopen(kown,"r");

		if(!f||!k||!l)
		{
			MessageBoxA(0,"文本打开失败",0,0);
			return ;
		}

		while(!feof(f))
		{
			more=(link_str *)malloc(sizeof(link_str));

			fscanf(f,"%s\n%s\t%s\t%ld\t%d\n",more->ex_str,more->str,&more->main_str,&more->lisp,&more->flag);
			more->id=i;
			i++;

			more->next=0;

			if(first==0)
			{
				first=more;
				first->last=NULL;
			}

			if(pass!=0)
			{
				pass->next=more;
				more->last=pass;
			}

			pass=more;

		}


		while(!feof(k))
		{
			add=(key *)malloc(sizeof(key));

			fscanf(k,"%s\t%d\n",add->word,&add->id);

			add->next=0;

			if(head==0)
			{
				head=add;
			}

			if(temp!=0)
			{
				temp->next=add;
			}

			temp=add;

		}

		while(!feof(l))
		{
			k_2=(knowledge *)malloc(sizeof(knowledge));

			fscanf(l,"%d\t%s\t%s\t%d\t%d\n",&k_2->id,k_2->str,k_2->mean,&k_2->get_n,&k_2->do_c);

			k_2->next=0;

			if(k_1==0)
			{
				k_1=k_2;
			}

			if(k_3!=0)
			{
				k_3->next=k_2;
			}

			k_3=k_2;

		}
		fclose(l);
		fclose(f);
		fclose(k);
	}

	void save_word(char *word ,int k)//存储更新关键字库
	{
		FILE *f=fopen(file_2,"a");
		fprintf(f,"%s\t%d\n",word,k);
		while(add)
		{
			if(add->next==0)
				temp=add;

			add=add->next;
		}
		add=(key *)malloc(sizeof(key));
		add->id=k;
		strcpy(add->word,word);

		temp->next=add;
		add->next=0;
		fclose(f);
	}

	void save_Nouns(char *Ns,char *Ns_m)//存储更新知识库
	{
		FILE *l=fopen(file_3,"a");
		while(k_2)
		{
			if(k_2->next==0)
				k_3=k_2;

			k_2=k_2->next;
		}
		k_2=(knowledge *)malloc(sizeof(knowledge));
		k_2->id=k_3->id+1;
		k_2->get_n=0;
		k_2->do_c=0;
		fprintf(l,"%d\t%s\t%s\t%d\t%d\n",k_2->id,Ns,Ns_m,0,0);

		strcpy(k_2->str,Ns);
		strcpy(k_2->mean,Ns_m);

		k_3->next=k_2;
		k_2->next=0;
		fclose(l);
	}

	int chack_key_word(char* str)//检测输入关键字是否存在,无0排除，
	{
		char sa[3];
		int flag=0;
		int l=strlen(str);

		for(int i=0;i<l;i+=2)
		{
			sa[0]=str[i];
			sa[1]=str[i+1];
			sa[2]='\0';

			add=head;
			while(add->next)
			{
				if(strstr(add->word,sa))
				{
					flag=add->id+flag*(int)pow((double)10,i+1);
					break;
				}
				add=add->next;
				if(add->next)
				{
					if(strstr(add->next->word,sa))
					{
						flag=add->next->id+flag*(int)pow((double)10,i+1);////////////////////////////
						break;
					}
				}
			}
			if(!add->next)
			{
				if((add->id+1)%10)
				{
					flag=add->id+1+flag*(int)pow((double)10,i+1);
					save_word(sa,add->id+1);
				}
				else 
				{
					flag=add->id+2+flag*(int)pow((double)10,i+1);
					save_word(sa,add->id+2);
				}


			}	
		}
		return flag;
	}

	long long str_to_long(char *str)//输入的一组词，含0(用"."来分开)
	{
		char in[50]="\0";
		long long m=0;
		for(int i=0,j=0,k=0;i<strlen(str);i++,k++)//未验证
		{
			if(str[i]!=' ')
				in[k]=str[i];
			else
			{
				in[k]='\0';
				m*=10;
				if(j!=0)
					m = chack_key_word(in) + m * (int)pow((double)10,k/2*3);//i-1
				else
					m = chack_key_word(in);
				j++;
				memset(in,0,50);
				k=-1;
			}
		}

		return m;


	}

	lisp_data *one;
	lisp_data *two;
	lisp_data *three;

	lisp_data *now_lisp_data()//now_lisp，//本层所有数据
	{
		more=first;


		int t=0;
		while(more)
		{
			t=more->lisp/10;
			if(t==now_lisp)
			{
				two=(lisp_data *)malloc(sizeof(lisp_data));
				strcpy(two->ex_str,more->ex_str);
				strcpy(two->str,more->str);
				word_to_str(more->main_str,more->lisp);
				two->lisp=more->lisp;
				two->next=0;
				for(int i=0;i<10;i++)
				{
					if(key_word[i].flag==1)
					{
						strcpy(two->main_str[i],key_word[i].keyword);

					}
					else
					{
						two->main_str[i][0]='\0';
					}
				}

				if(!one) one=two;
				if(three!=0) three->next=two;
				three=two;
			}

			more=more->next;
		}

		return one;

	}

	char * find(char *str)
		//利用每一个Key_word[10]中的一组关键词对str进行查找，并把结果存入RES中
		//再对RES中的数据进行分析，筛选出结果，根据id匹配到link_str中返回str的答复
	{


		if(char *out=this->change_？(str))
		{
			return out;
		}

		if(char *out=this->learn_one(str))
		{
			return out;
		}

		

		static int lop=1;//递归相关
		more=first;
		while(more)//统计阶段
		{
			up=(RES*)malloc(sizeof(RES));
			up->id=more->id;
			this->word_to_str(more->main_str,more->lisp);
			for(int i=0;i<10;i++)
			{
				if((key_word[i].flag&&lop)||(key_word[i].flag&&more->flag))
				{
					if(strstr(str,key_word[i].keyword))
					{
						if(count_key_word(more->main_str)==1)
							up->x[i]=2;
						else
							up->x[i]=1;
					}
					else
					{
						up->x[i]=0;

					}

				}
				else
				{
					up->x[i]=0;
				}
			}



			if(top==NULL) top=up;

			if(down) down->next=up;

			down=up;

			up->next=NULL;


			more=more->next;

		}

		//反馈阶段
		up=top;
		int rst_id[2][5]={{0,0,0,0,0},{0,0,0,0,0}};
		int t;
		int inti=0;
		while(up)
		{
			t=up->res();
			if(t>rst_id[0][inti] && t>=2)
			{
				rst_id[0][inti]=t;
				rst_id[1][inti]=up->id;
			}
			/*
			else if(t==rst_id[0][inti])
			{
			if(rst_id[0][inti+1]<t)
			{
			rst_id[0][inti+1]=t;
			rst_id[1][inti+1]=up->id;
			}
			else if(rst_id[0][inti+1]==t)
			{
			if(rst_id[0][inti+2]<t)
			{
			rst_id[0][inti+2]=t;
			rst_id[1][inti+2]=up->id;
			}
			else if(rst_id[0][inti+2]==t)
			{
			if(rst_id[0][inti+3]<t)
			{
			rst_id[0][inti+3]=t;
			rst_id[1][inti+3]=up->id;
			}
			else if(rst_id[0][inti+4]<t)
			if(rst_id[0][inti+4]<t)
			{
			rst_id[0][inti+4]=t;
			rst_id[1][inti+4]=up->id;
			}
			else if(rst_id[0][inti+4]==t)
			{
			MessageBoxA(0,"相同数据溢出！",0,0);
			}
			}

			}

			}*/
			up=up->next;
		}
		//寻址阶段
		if(rst_id[0][0]>0)
		{
			more=first;
			while(more)
			{
				if(rst_id[1][0]==more->id)
				{
					now_lisp=more->lisp;
					res_free();
					return more->str;
				}
				more=more->next;
			}
		}

		//当前层没找到的情况
		if(now_lisp==0)
		{
			res_free();
			//MessageBoxA(0,"未找到数据！",0,0);
			str=do_not_fine(str);

			///////////////////////
			if(strstr(str,"烫"))
			{
				return "这种时候不想‘烫’了";
			}
			//////////////////////
			return str;

		}


		lop=0;
		now_lisp=run_last_lisp(now_lisp);
		return find(str);

		//目前一旦完全（包括之上的层里）找不到，lisp全部返回，未来改进

	}




	~my_ai_link()
	{
		more=first;
		while(more)
		{
			pass=more;
			more=more->next;
			free(pass);
		}

		add=head;
		while(add)
		{
			temp=add;
			add=add->next;
			free(temp);
		}

		k_2=k_1;
		while(k_2)
		{
			k_3=k_2;
			k_2=k_2->next;
			free(k_3);
		}



		free_lisp_data(one);

	}

	char *get_Nouns(char *str)//源，取词个数，
		//-75 -60
	{
		char r[20]="\0";
		int flag=0;
		int n=2;
		int len;
		char *str1=0;
		k_2=k_1;
		while(k_2)
		{
			if((str1=strstr(str,k_2->str))&&k_2->get_n!=0)
			{
				flag=2;
				n=k_2->get_n;
				break;
			}
			k_2=k_2->next;
		}

		if(flag==2)
		{
			if(n<0)
			{
				len=strlen(str)-strlen(str1)+k_2->get_n;
				if(len<0)len=0;
				for(int i=len,j=0;;i++)
				{
					r[j++]=str[i];
					if(-n*2>strlen(str)-strlen(str1))
						n=(strlen(str)-strlen(str1))/2;
					if(j==n*2)
					{
						r[j+1]='\0';
						strcpy(this->Nouns,r);
						return (char *)r;
					}
				}

			}
			else
			{
				len=strlen(str)-strlen(str1)+strlen(k_2->str);
				for(int i=len,j=0;i>0;i++)
				{
					r[j++]=str[i];
					if(j==n*2)
					{
						r[j+1]='\0';
						strcpy(this->Nouns,r);
						return (char *)r;
					}
				}
			}}
	return 0;
}

char *change_？(char *str)//find前
{
	char out[100];
	char *out1;

	if(change == 1)
		if( (!strcmp(str,"是的")) || (!strcmp(str,"是这样的")) || (!strcmp(str,"不错")) || (!strcmp(str,"对")) )
		{
			change = 0;
			sprintf(out,"哦,%s啊~",Nouns_m);
			out1=out;
			return out1;
		}
		else 
			change = 0;

	return 0;
}

char *change_one(char *str)
{
	char outstr[100];
	////////////////////////////////
	k_2=k_1;
	while(k_2)
	{
		if(strstr(str,k_2->str)&&k_2->get_n==0)
		{

			sprintf(outstr,"原来您说的是%s啊",k_2->mean);
			return outstr;
		}

		k_2=k_2->next;
	}

	////////////////////////////////
	if(strstr(str,"为什么"))
	{
		strcpy(outstr,"我也不知道为什么");
		return outstr;
	}
	else if(strstr(str,"理由"))
	{
		strcpy(outstr,"我也不知道什么理由");
		return outstr;
	}
	else if(strstr(str,"是啊"))
	{
		strcpy(outstr,"呵呵");
		return outstr;
	}
	return 0;
}

char *learn_one(char *str)//记录名词解释  find前
{
	char *str1=0;
	if(learn == 1)
	{
		chack_key_word(str);
		learn = 0;
		if(str1=strstr(str,"是"))
		{
			str1=str1+2;
			strcpy(Nouns_m,str1);
		}
		else 
			strcpy(Nouns_m,str);

		save_Nouns(Nouns,Nouns_m);

		return "我记住了！";
	}
	return 0;
}



char *do_not_fine(char *str)//没找到应答的句子情况
{
	char *outstr=0;
	char out2[20]="\0";
	UINT flag=0;
	srand(time(0));
	flag=rand()%2;
	if(Nouns[0]=='\0')flag=1;

	if(outstr=change_one(str))
	{
		return outstr;
	}
	else
	{

		if((outstr=get_Nouns(str)) && flag == 1)
		{
			strcat(outstr,"是什么？");
			learn = 1;
			flag=0;
			return outstr;
		}


		if(Nouns[0]!='\0')
		{

			sprintf(out2,"那是与%s相关吗？",Nouns);
			change = 1;
			outstr = out2;
			return outstr;
		}
	}
	return "这个我真的不知道";
}
#if 0
//非移植性附加
void input_data()
{
	char tenp_str[100];

	while(more)
	{
		if(!more->next)
			pass=more;
		more=more->next;
	}

	more=(link_str *)malloc(sizeof(link_str));
	strcpy(more->ex_str,strEdit);
	strcpy(more->str,strEdit0);
	sprintf(tenp_str,"%llu",str_to_long(strEdit1));
	strcpy(more->main_str,tenp_str);
	more->lisp=atol(strEdit2);
	now_lisp=more->lisp;

	if(IDYES==MessageBoxA(editwindow.get_hwnd(),"是否可在返回上层从使用？",0,MB_YESNO))
	{
		more->flag=1;
	}
	else 
	{
		more->flag=0;
	}

	pass->next=more;
	more->last=pass;
	more->next=0;

	FILE *f=fopen("ai.txt","a");
	fprintf(f,"%s\n%s\t%s\t%ld\t%d\n",more->ex_str,more->str,more->main_str,more->lisp,more->flag);
	fclose(f);

	text_print();
}
#endif


int ai_fun(char *str)//功能函数 if(!AI.ai_fun(strEdit))AI.fine()
{
	char outstr[50];
	
	if(strstr(str,"星期几"))
	{
		pc.get_week(outstr);
		saipvoi(outstr);
		return 1;
	}
	else if(strstr(str,"今天几号")||(strstr(str,"今天")&&strstr(str,"日期")))
	{
		pc.get_date(outstr);
		saipvoi(outstr);
		return 1;
	}
	else if(strstr(str,"几点")||(strstr(str,"现在")&&strstr(str,"时间")))
	{
		pc.get_time(outstr);
		saipvoi(outstr);
		return 1;
	}
	else if(strstr(str,"查询天气"))
	{
		run("ws_weather.exe");
		is_read = 1;
		return 1;
	}
	return 0;
}

};


my_ai_link AI;



class my_heart : public my_ai_link
{

public:


};