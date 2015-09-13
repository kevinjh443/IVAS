DWORD WINAPI net_listen(LPVOID lpParaneter)
{
	mainwindow.THREAD_OFF();
	FILE *p_f=0;
	net.netstart();
	net.set_ip("192.168.1.1");
	net.netinti();
	for(;;)
	{
		if(mainwindow.THREAD())
		{
			net_r=1;
			p_f=fopen("pic.jpg","wb");
			Sleep(500);
			net.netrecv(rec_pic);
			if(rec_pic[0]!='\0')
			{
				fwrite(rec_pic,sizeof(rec_pic),1,p_f);
				rec_pic[0]='\0';
			}
			fclose(p_f);
			net_r=2;

		}
		else
			Sleep(2000);

	}

	return 0;
}