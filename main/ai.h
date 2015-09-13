#if 0

���Ǹ��漣��
	һ��������Ϊ�˱�����ġ��ġ����質��
	����˵���ʵ��

	��������漣����ô�ض��ݡ�
	������˵���ġ�����ô���Ӵ�
	ʹ���޷��������������
	��������˾���ô�����Ų���Զ�������ǡ�

	��΢Ц�ţ�
	����ȥ�������ʹ��

#endif
#include <math.h>
#define BSIZE 200

struct lisp_data//��������Ϣ
{
	char ex_str[200];//�����������
	char str[BSIZE];//���صĻ�
	char main_str[10][20];
	long int lisp;

	lisp_data *next;
};

class my_ai_link
{
	long int now_lisp;//��ǰ������ƥ��������

	int learn; //ѧϰģʽ

	char Nouns[20];//��¼������
	char Nouns_m[100];//��¼���ʵĽ���

	int change;//�Ƿ����ת�ƻ���

	char file_1[255],file_2[255],file_3[255];

	struct link_str
	{
		int id;//��������Ӧ
		char ex_str[200];//�����������
		char str[BSIZE];//���صĻ�
		char main_str[50];
		//��Ӧ�ؼ��ֱ�ʵ���κ������Ĺؼ��ִ洢ʶ�𣬹���1241120143424,124112��ʾ��һ���ʣ���0���ֿ�
		//ÿ������3λ����ʾ��1��С�������һ������Ϊ111�����һ��Ϊ999�������Ա��888����
		long  lisp;
		//������ƥ������ʵ���κ���������������Ĺ���
		//����1232����ʾ���ȶ�ǰ׺Ϊ1232����������������δ�ҵ����123ǰ׺���������Դ����ơ���СΪ0��
		//�Ի��Ŀ�ʼֻ��0����������
		int flag;
		//��Ǹô��������ϲ����������Ƿ�����������δ�ҵ��������
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

	struct Key_word//�ؼ��ʱ��Թؼ����������ԱȺ󣬽������������
	{
		int flag;//1Ϊ���ڴ˹ؼ���,��������lisp��ȫΪ0
		char keyword[15];
		Key_word(){flag=0;}
	}key_word[10];

	struct RES//��������������������ҳ�res����Ϊ��ӽ����������
	{
		int x[10];//һ���ؼ���ƥ���Ԫ��Ϊ1��δƥ��-1��flagΪ0��Ϊ0
		int id;
		int res()//���ظ���ؼ��ʵ�������
		{
			int ret=0;
			for(int q=0;q<10;q++)
			{
				ret+=x[q];
			}
			return ret;
		}
		RES *next;//��״�Ľ������ÿһ�ڵ��Ӧһ��ؼ���
	};
	RES *top;//������
	RES *up;
	RES *down;

	struct knowledge
	{
		int id;//���
		char str[20];//��¼�Ĵ���
		char mean[200];//��Ӧ�Ľ���
		int get_n;//��Ե�ȡ�����÷�Χ������������ȡ������������ȡ
		int do_c;//ִ�е���Ӧ������0����û��

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

	void word_to_str( char *x,int list)//��link_str�д洢��main_str��key�����ֲ�ת�����ַ�������Key_word��
	{//49='1'//listΪmore->lisp
		int num3[3];
		int count=0;
		int zero=0;

		count=strlen(x);

		for(int j=0;j<10;j++)//ÿ�ε��ã��������
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
				MessageBoxA(0,"ȱ�ٹؼ���",0,0);
			}



		}

	}

	int list_bit(int x)//ͳ������λ�ڲ���
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

	int run_last_lisp(int x)//������һ��
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

	void free_lisp_data(lisp_data *x)//�ͷŲ���Ϣ
	{
		lisp_data *te;
		while(x)
		{
			te=x;
			x=x->next;
			free(te);
		}
	}

	int count_key_word(char *str)//ͳ�ƹؼ��ʸ���
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

	int get_nowlisp()//����������ڲ���
	{
		return now_lisp;
	}

	void up_nowlisp()//���ú���ȥ����һ��
	{
		now_lisp=run_last_lisp(now_lisp);
	}

	void down_nowlisp(int x)//ֱ�ӽ�����һ�㣨1-9��rand��
	{
		now_lisp=now_lisp*10+x;
	}

	void create_and_read(char *filename,char *keyfile,char *kown)//��ʼ������������ϵͳ����
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
			MessageBoxA(0,"�ı���ʧ��",0,0);
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

	void save_word(char *word ,int k)//�洢���¹ؼ��ֿ�
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

	void save_Nouns(char *Ns,char *Ns_m)//�洢����֪ʶ��
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

	int chack_key_word(char* str)//�������ؼ����Ƿ����,��0�ų���
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

	long long str_to_long(char *str)//�����һ��ʣ���0(��"."���ֿ�)
	{
		char in[50]="\0";
		long long m=0;
		for(int i=0,j=0,k=0;i<strlen(str);i++,k++)//δ��֤
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

	lisp_data *now_lisp_data()//now_lisp��//������������
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
		//����ÿһ��Key_word[10]�е�һ��ؼ��ʶ�str���в��ң����ѽ������RES��
		//�ٶ�RES�е����ݽ��з�����ɸѡ�����������idƥ�䵽link_str�з���str�Ĵ�
	{


		if(char *out=this->change_��(str))
		{
			return out;
		}

		if(char *out=this->learn_one(str))
		{
			return out;
		}

		

		static int lop=1;//�ݹ����
		more=first;
		while(more)//ͳ�ƽ׶�
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

		//�����׶�
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
			MessageBoxA(0,"��ͬ���������",0,0);
			}
			}

			}

			}*/
			up=up->next;
		}
		//Ѱַ�׶�
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

		//��ǰ��û�ҵ������
		if(now_lisp==0)
		{
			res_free();
			//MessageBoxA(0,"δ�ҵ����ݣ�",0,0);
			str=do_not_fine(str);

			///////////////////////
			if(strstr(str,"��"))
			{
				return "����ʱ���롮�̡���";
			}
			//////////////////////
			return str;

		}


		lop=0;
		now_lisp=run_last_lisp(now_lisp);
		return find(str);

		//Ŀǰһ����ȫ������֮�ϵĲ���Ҳ�����lispȫ�����أ�δ���Ľ�

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

	char *get_Nouns(char *str)//Դ��ȡ�ʸ�����
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

char *change_��(char *str)//findǰ
{
	char out[100];
	char *out1;

	if(change == 1)
		if( (!strcmp(str,"�ǵ�")) || (!strcmp(str,"��������")) || (!strcmp(str,"����")) || (!strcmp(str,"��")) )
		{
			change = 0;
			sprintf(out,"Ŷ,%s��~",Nouns_m);
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

			sprintf(outstr,"ԭ����˵����%s��",k_2->mean);
			return outstr;
		}

		k_2=k_2->next;
	}

	////////////////////////////////
	if(strstr(str,"Ϊʲô"))
	{
		strcpy(outstr,"��Ҳ��֪��Ϊʲô");
		return outstr;
	}
	else if(strstr(str,"����"))
	{
		strcpy(outstr,"��Ҳ��֪��ʲô����");
		return outstr;
	}
	else if(strstr(str,"�ǰ�"))
	{
		strcpy(outstr,"�Ǻ�");
		return outstr;
	}
	return 0;
}

char *learn_one(char *str)//��¼���ʽ���  findǰ
{
	char *str1=0;
	if(learn == 1)
	{
		chack_key_word(str);
		learn = 0;
		if(str1=strstr(str,"��"))
		{
			str1=str1+2;
			strcpy(Nouns_m,str1);
		}
		else 
			strcpy(Nouns_m,str);

		save_Nouns(Nouns,Nouns_m);

		return "�Ҽ�ס�ˣ�";
	}
	return 0;
}



char *do_not_fine(char *str)//û�ҵ�Ӧ��ľ������
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
			strcat(outstr,"��ʲô��");
			learn = 1;
			flag=0;
			return outstr;
		}


		if(Nouns[0]!='\0')
		{

			sprintf(out2,"������%s�����",Nouns);
			change = 1;
			outstr = out2;
			return outstr;
		}
	}
	return "�������Ĳ�֪��";
}
#if 0
//����ֲ�Ը���
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

	if(IDYES==MessageBoxA(editwindow.get_hwnd(),"�Ƿ���ڷ����ϲ��ʹ�ã�",0,MB_YESNO))
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


int ai_fun(char *str)//���ܺ��� if(!AI.ai_fun(strEdit))AI.fine()
{
	char outstr[50];
	
	if(strstr(str,"���ڼ�"))
	{
		pc.get_week(outstr);
		saipvoi(outstr);
		return 1;
	}
	else if(strstr(str,"���켸��")||(strstr(str,"����")&&strstr(str,"����")))
	{
		pc.get_date(outstr);
		saipvoi(outstr);
		return 1;
	}
	else if(strstr(str,"����")||(strstr(str,"����")&&strstr(str,"ʱ��")))
	{
		pc.get_time(outstr);
		saipvoi(outstr);
		return 1;
	}
	else if(strstr(str,"��ѯ����"))
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