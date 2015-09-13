DWORD WINAPI funhear(LPVOID lpParaneter)
{
	Sleep(2000);
	ShowWindow(thisissola.get_hwnd(),SW_HIDE);
	look_good();

	sapi(strin);

	return 0;
}

DWORD WINAPI funread(LPVOID lpParaneter)
{
	for(;;)
	{
		if(is_read==1)
		{
			get_weather("weather.txt");
		}
		else
		{
			is_weather = 1;
			Sleep(1000);
		}

	}

	return 0;
}