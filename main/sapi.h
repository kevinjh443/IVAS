#include <atlbase.h>
#include <atlstr.h>
#include "sphelper.h"
#include <sapi.h>


#pragma comment(lib,"ole32.lib") //CoInitialize CoCreateInstance需要调用ole32.dll 
#pragma comment(lib,"sapi.lib") //sapi.lib在SDK的lib目录,必需正确配置 
int num=0,light=0;//0红1绿
inline HRESULT BlockForResult(ISpRecoContext * pRecoCtxt, ISpRecoResult ** ppResult)
{
	HRESULT hr = S_OK;
	CSpEvent event;

	while (SUCCEEDED(hr) &&
		SUCCEEDED(hr = event.GetFrom(pRecoCtxt)) &&
		hr == S_FALSE)
	{
		hr = pRecoCtxt->WaitForNotifyEvent(INFINITE);
	}

	*ppResult = event.RecoResult();
	if (*ppResult)
	{
		(*ppResult)->AddRef();
	}

	return hr;
}

const WCHAR * StopWord()
{
	const WCHAR * pchStop;

	LANGID LangId = ::SpGetUserDefaultUILanguage();

	switch (LangId)
	{
	case MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT):
		pchStop = L"}42N86\0b70e50fc0ea0e70fc/05708504608a087046";
		break;

	default:
		pchStop = L"Stop";
		break;
	}

	return pchStop;
}


int saipvoi(char *str)
{ 
	CString str1 = ("%s",str);

	//MessageBox(0,str1.AllocSysString(),0,0);
	if(str[0] == -52)
		 str1 = ("%s","这个我真的不知道");

	ISpVoice * pVoice = NULL; 

	//COM初始化： 
	if (FAILED(::CoInitialize(NULL))) 
		return FALSE; 

	//获取ISpVoice接口： 
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice); 
	if( SUCCEEDED( hr ) ) 
	{ 
			hr = pVoice->Speak(str1.AllocSysString(), 0, NULL); 
		pVoice->Release(); 
		pVoice = NULL; 
	} 

	//千万不要忘记： 
	::CoUninitialize(); 
	return TRUE; 
} 






int sapi(char * dst)
{
	HRESULT hr = E_FAIL;
	bool fUseTTS = true;            // turn TTS play back on or off
	bool fReplay = true;            // turn Audio replay on or off




	if (SUCCEEDED(hr = ::CoInitialize(NULL)))
	{
		{
			CComPtr<ISpRecoContext> cpRecoCtxt;
			CComPtr<ISpRecoGrammar> cpGrammar;
			CComPtr<ISpVoice> cpVoice;
			hr = cpRecoCtxt.CoCreateInstance(CLSID_SpSharedRecoContext);
			if(SUCCEEDED(hr))
			{
				hr = cpRecoCtxt->GetVoice(&cpVoice);
			}

			if (cpRecoCtxt && cpVoice &&
				SUCCEEDED(hr = cpRecoCtxt->SetNotifyWin32Event()) &&
				SUCCEEDED(hr = cpRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION))) && 
				SUCCEEDED(hr = cpRecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL)) &&
				SUCCEEDED(hr = cpRecoCtxt->CreateGrammar(0, &cpGrammar)) &&
				SUCCEEDED(hr = cpGrammar->LoadDictation(NULL, SPLO_STATIC)) &&
				SUCCEEDED(hr = cpGrammar->SetDictationState(SPRS_ACTIVE)))
			{
				USES_CONVERSION;

				const WCHAR * const pchStop = StopWord();
				CComPtr<ISpRecoResult> cpResult;

				//  printf( "I will repeat everything you say. Say %s to exit. ", W2A(pchStop) );

				while (SUCCEEDED(hr = BlockForResult(cpRecoCtxt, &cpResult)))
				{
					cpGrammar->SetDictationState( SPRS_INACTIVE );

					CSpDynamicString dstrText;

					if (SUCCEEDED(cpResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, 
						TRUE, &dstrText, NULL)))
					{
						strcpy(dst, W2A(dstrText));////////////////////////////
						com(dst);
						cpResult.Release();
					}
					if (_wcsicmp(dstrText, pchStop) == 0)
					{
						break;
					}

					cpGrammar->SetDictationState( SPRS_ACTIVE );
				} 
			}
		}
		::CoUninitialize();
	}
	return hr;
}
