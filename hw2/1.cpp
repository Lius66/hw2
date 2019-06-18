#pragma once
#include<DuiLib/UIlib.h>
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

struct stop_watch {
	int hour, minute, second, microsecond;
}sw;
void UpdateTime();
void ResetTime();

class CDuiFrameWnd :public WindowImplBase {
public:
	virtual LPCTSTR    GetWindowClassName() const { return _T("DUIMainFrame"); }
	virtual CDuiString GetSkinFile() { return _T("duilib.xml"); }
	virtual CDuiString GetSkinFolder() { return _T(""); }

	void InitWindow() {
		show_time_txt = static_cast <CTextUI*> (m_PaintManager.FindControl(_T("show_time_txt")));
		start_btn = static_cast <CButtonUI*> (m_PaintManager.FindControl(_T("start_btn")));
		reset_btn = static_cast <CButtonUI*> (m_PaintManager.FindControl(_T("reset_btn")));
		pause_btn = static_cast <CButtonUI*> (m_PaintManager.FindControl(_T("pause_btn")));
		count_btn = static_cast <CButtonUI*> (m_PaintManager.FindControl(_T("count_btn")));

	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		LRESULT lRes = 0;
		if (uMsg == WM_TIMER) {
			UpdateTime();
			TCHAR current_time[1024];
			wsprintf(current_time, "%d : %d : %d : %d", sw.hour, sw.minute, sw.second, sw.microsecond);
			show_time_txt->SetText(current_time);
		}
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	void Notify(TNotifyUI& msg) {
		if (msg.sType == DUI_MSGTYPE_CLICK) {
			CDuiString strName = msg.pSender->GetName();
			if (strName == _T("start_btn"))
			{
				msg.pSender->SetVisible(false);
				pause_btn->SetVisible(true);

				reset_btn->SetVisible(false);
				count_btn->SetVisible(true);

				::SetTimer(m_hWnd, 2019, 10,NULL);
			}
			else if (strName == _T("pause_btn")) {

				count_btn->SetVisible(false);
				reset_btn->SetVisible(true);

				msg.pSender->SetVisible(false);
				start_btn->SetVisible(true);
				::KillTimer(m_hWnd, 2019);
			}
			else if (strName == _T("reset_btn")) {
				//msg.pSender->SetVisible(false);
				//count_btn->SetVisible(true);
				time_count = 0;
				::KillTimer(m_hWnd, 2019);
				ResetTime();
				TCHAR current_time[1024];
				wsprintf(current_time, "%d : %d : %d : %d", sw.hour, sw.minute, sw.second, sw.microsecond);
				show_time_txt->SetText(current_time);
			}
			else if (strName == _T("count_btn")) {
				time_count++;
				TCHAR current_time[1024];
				wsprintf(current_time, "count %d:    %d : %d : %d : %d",time_count, sw.hour, sw.minute, sw.second, sw.microsecond);
				MessageBox(m_hWnd, current_time, NULL, NULL);

			}

		}
		/*if (msg.sType == DUI_MSGTYPE_TIMER) {
			UpdateTime();
			MessageBox(m_hWnd, _T("timer..."), NULL, NULL);
			TCHAR current_time[1024];
			wsprintf(current_time, "%d%d%d%d", sw.hour, sw.minute, sw.second, sw.microsecond);
			show_time_txt->SetText(current_time);
		}*/
		
		__super::Notify(msg);
	}
	

private:
	CTextUI* show_time_txt;
	CButtonUI* start_btn;
	CButtonUI* reset_btn;
	CButtonUI* pause_btn;
	CButtonUI* count_btn;
	int time_count = 0;

};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//设置窗口关联的实例
	CPaintManagerUI::SetInstance(hInstance);

	//创建窗口
	CDuiFrameWnd duiFrame;
	duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
//	duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_DIALOG, 0);

	duiFrame.CenterWindow();//居中
	duiFrame.ShowModal();
	return 0;
}

void UpdateTime()
{
	sw.microsecond++;
	if (sw.microsecond == 100) {
		sw.microsecond = 0;
		sw.second++;
		if (sw.second == 60) {
			sw.second = 0;
			sw.minute++;
			if (sw.minute == 60) {
				sw.minute = 0;
				sw.hour++;
				if (sw.hour == 24) {
					sw.hour = 0;
				}
			}
		}
	}
}

void ResetTime() {
	sw.hour = 0;
	sw.minute = 0;
	sw.second = 0;
	sw.microsecond = 0;
}