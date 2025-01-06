/////////////////////////////
// MyDialog.cpp
//
#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"
#include "utfwstring.h"
#include <thread>
#include <iostream>
#include <ctime>
#include <cmath>
#include "wingdi.h"

#include <windowsx.h>

using namespace std;


void CMyDialog::tunerStart()
{
    freq.Initialize();
    while(true)
    {
        Sleep(5);
        if(cursShange)
        {

        }
    }
}
//------------------------------------
void CMyDialog::tunerRead()
{
    while(true)
    {
        //mtxg.lock();
        if(freq.evEnable)
        {
            sampleCount++;
            std::vector<double> nPoints;
            std::vector<double> nPointsR;
            nBand.clear();
            int nSampl=0;
            int nCount=0;
            int nData=0;
            POINT lpp;
            PatBlt(hdcMem,0,0,m_static.GetWindowRect().Width(),m_static.GetWindowRect().Height(),WHITENESS);
            BitBlt(hdcMem,0,0,m_static.GetWindowRect().Width(),m_static.GetWindowRect().Height(),hdcMaxis,0,0,SRCAND);
            nSampl=0;
            int nSign=1;
            double nData1=freq.fftVal.at(0).r*4;
            if(nData1<0.0)
            {
                nData1*=-1;
                nSign=1;
            }
            double nData2=gainConst*log(nData1*nSign);
            nData=m_static.GetWindowRect().Height()-(int)nData2*nSign-5;
            MoveToEx(hdcMem,0,nData,&lpp);
            nPoints.clear();
            nPointsR.clear();
            for(int i=startPos;i<stopPos;i++)
            {
                // szoutVal.append(std::to_wstring(i)+L" "+std::to_wstring(freq.fftVal.at(i).r)+L" "+std::to_wstring(freq.fftVal.at(i).i)+L"\n");
                nSampl++;
                if(stopPos<freq.fftVal.size())
                {
                    nPoints.push_back(freq.fftVal.at(i).r);
                    nPointsR.push_back(freq.fftValr.at(i).r);
                    if(nSampl>=freqConst)
                    {
                        nSampl=0;
                        if(leftEnable)
                        {
                            std::sort(nPoints.begin(), nPoints.end());
                            nSign=1;
                            nData1=nPoints.at(nPoints.size()-1)*4;
                            nPoints.clear();
                            if(nData1<0.0)
                            {
                                nData1*=-1;
                                nSign=1;
                            }
                            double nData2=gainConst*log(nData1*nSign);
                            nData=m_static.GetWindowRect().Height()-(int)nData2*nSign-5;
                            MoveToEx(hdcMem,nCount,m_static.GetWindowRect().Height()-5,&lpp);
                            SelectPen(hdcMem,pen);
                            LineTo(hdcMem,nCount,nData);
                        }
                        if(rightEnable)
                        {
                            std::sort(nPointsR.begin(), nPointsR.end());
                            nSign=1;
                            nData1=nPointsR.at(nPointsR.size()-1)*4;
                            nPointsR.clear();
                            if(nData1<0.0)
                            {
                                nData1*=-1;
                                nSign=1;
                            }
                            nData2=gainConst*log(nData1*nSign);
                            nData=m_static.GetWindowRect().Height()-(int)nData2*nSign-5;
                            MoveToEx(hdcMem,nCount,m_static.GetWindowRect().Height()-5,&lpp);
                            SelectPen(hdcMem,penR);
                            LineTo(hdcMem,nCount,nData);
                        }

                        nBand.push_back(nData);
                        nCount++;
                    }
                }
            }
            freq.evEnable=false;
            m_static.ShowWindow(SW_HIDE);
        }
        Sleep(4);
        // mtxg.unlock();
    }
}

// Constructor.
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
    sampleCount=0;
    vertPos=2;
    gainConst=40;
    dbScale=10;
    freqConst=4;
    currWnd=NULL;
    leftEnable=false;
    rightEnable=false;
    drawBand=false;
    pen= CreatePen(PS_SOLID,2,RGB(255,0,0));
    penR= CreatePen(PS_SOLID,2,RGB(0,0,255));
    m_aboutDialog=new  AboutDlg(IDD_ABOUT);
    m_aboutDialog->SetParent(GetHwnd());
}

// Destructor.
CMyDialog::~CMyDialog()
{
       delete  m_aboutDialog;
}

// Called when the dialog window is destroyed.
void CMyDialog::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}
void CMyDialog::OnDraw(CDC&)
{
    drawChart();
}
//-------------------------------------
void CMyDialog::drawChart()
{
    PAINTSTRUCT ps;
    m_static.ShowWindow( SW_RESTORE);
    hdc =  m_static.BeginPaint(ps);
    BitBlt(hdc,0,0,m_static.GetWindowRect().Width(),m_static.GetWindowRect().Height(),hdcMem,0,0,SRCCOPY);

    EndPaint(ps);
    ReleaseDC(hdc);
 }
// Process the dialog's window messages.
INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_VSCROLL:   return OnVScroll(wparam, lparam);
        case WM_HSCROLL:   return OnHScroll(wparam, lparam);
        case WM_MOUSEMOVE: return OnSetCursor(wparam, lparam);
        case WM_LBUTTONDOWN: m_play.EnableWindow(true);break;
        }
        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    //    catch (const std::exception& e)
    //    {
    //        // Display the exception and continue.
    //        CString str1 = e.what();
    //        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    //    }
    return 0;
}

// Handle the command notifications from controls.
BOOL CMyDialog::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDC_BUTTT:    shangeVertResol(1); break;
    case IDC_BUTTB:    shangeVertResol(0); break;
    case IDC_BUTTL:    shangeHorResol(1); break;
    case IDC_BUTTR:    shangeHorResol(0); break;
    case IDC_CHKL:    OnCheckL(); break;
    case IDC_CHKR:    OnCheckR(); break;
    case IDC_COMBO:    OnSelShange( wparam,lparam); break;
    case IDC_STATPLAY:    OnPlay( wparam,lparam); break;
    case IDC_ABOUT:    m_aboutDialog->DoModal(GetHwnd()); break;
    }

    return FALSE;
}

// Called before the dialog is displayed.
BOOL CMyDialog::OnInitDialog()
{
    // Set the application icon
    SetIconLarge(IDI_LOGO);
    SetIconSmall(IDI_LOGO);

    // Attach the controls to our CWnd member objects.
    AttachItem(IDC_STATIC4, m_static);
    AttachItem(IDC_TRACKBAR1, m_slider);
    AttachItem(IDC_CHKL, m_checkL);
    AttachItem(IDC_CHKR, m_checkR);
    AttachItem(IDC_COMBO, m_combox);
    AttachItem( IDC_BUTTT, m_butTop);
    AttachItem( IDC_BUTTB,  m_butDown);
    AttachItem( IDC_BUTTL, m_butLeft);
    AttachItem( IDC_BUTTR, m_butRight);
    AttachItem( IDC_STATPLAY, m_play);

    // Small icon sizes
    HICON icon = reinterpret_cast<HICON>(GetApp()->LoadImage(IDI_UP, IMAGE_ICON, m_butTop.GetClientRect().Width(), m_butTop.GetClientRect().Height(), LR_SHARED));
    m_butTop.SetIcon( icon);
    icon = reinterpret_cast<HICON>(GetApp()->LoadImage(IDI_DOWN, IMAGE_ICON, m_butDown.GetClientRect().Width(),  m_butDown.GetClientRect().Height(), LR_SHARED));
    m_butDown.SetIcon( icon);
    icon = reinterpret_cast<HICON>(GetApp()->LoadImage(IDI_LEFT, IMAGE_ICON, m_butLeft.GetClientRect().Width(),  m_butLeft.GetClientRect().Height(), LR_SHARED));
    m_butLeft.SetIcon( icon);
    icon = reinterpret_cast<HICON>(GetApp()->LoadImage(IDI_RIGHT, IMAGE_ICON, m_butRight.GetClientRect().Width(),  m_butRight.GetClientRect().Height(), LR_SHARED));
    m_butRight.SetIcon( icon);
    m_play.EnableWindow(false);
    freq.blockView=false;
//    HBITMAP bitmap = reinterpret_cast<HBITMAP>(GetApp()->LoadImage(IDB_PLAY, IMAGE_BITMAP,m_play.GetClientRect().Width(),m_play.GetClientRect().Height(), LR_SHARED));
//    m_play.SetBitmap(bitmap);
    m_combox.AddString(L"Input device select");
    freq.getInpDevices(&m_combox);
    m_combox.SetCurSel(0);
    PAINTSTRUCT ps;
    hdc =  m_static.BeginPaint(ps);
    hBitMap=CreateCompatibleBitmap(hdc,m_static.GetWindowRect().Width(),m_static.GetWindowRect().Height());
    hdcMem=CreateCompatibleDC(hdc);
    SelectBitmap(hdcMem,hBitMap);
    PatBlt(hdcMem,0,0,m_static.GetWindowRect().Width(),m_static.GetWindowRect().Height(),WHITENESS);

    hBitMaxis=CreateCompatibleBitmap(hdc,m_static.GetWindowRect().Width(),m_static.GetWindowRect().Height());
    hdcMaxis=CreateCompatibleDC(hdc);
    SelectBitmap(hdcMaxis,hBitMaxis);
    PatBlt(hdcMaxis,0,0,m_static.GetWindowRect().Width(),m_static.GetWindowRect().Height(),WHITENESS|SRCAND);
    ReleaseDC(hdc);
    HFONT hFont=CreateFont (16, 0, 0, 0,FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    SendMessage (GetDlgItem(IDC_STATICD).GetHwnd(), WM_SETFONT, WPARAM (hFont), TRUE);
    //  hFont=CreateFont (16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    SendMessage (GetDlgItem(IDC_CHKL).GetHwnd(), WM_SETFONT, WPARAM (hFont), TRUE);
    SendMessage (GetDlgItem(IDC_CHKR).GetHwnd(), WM_SETFONT, WPARAM (hFont), TRUE);
    SendMessage (GetDlgItem(IDC_COMBO).GetHwnd(), WM_SETFONT, WPARAM (hFont), TRUE);
    SendMessage (GetDlgItem(IDC_STATBAND).GetHwnd(), WM_SETFONT, WPARAM (hFont), TRUE);
    SendMessage (GetDlgItem(IDC_ABOUT).GetHwnd(), WM_SETFONT, WPARAM (hFont), TRUE);

    SetSliderPos(2);
    recalcSliderRange();
    recalcStartPos();
    createAxis();
    return TRUE;
}
//---------------------------------------
void CMyDialog::recalcSliderRange()
{
    int nSliderMax=1;
    nSliderMax=(MAX_FREQ_VALUE-m_static.GetWindowRect().Width())/(freqConst*freq.freqMult);
    nSliderMax/=HORIZ_GRID;
    int nGridLines=m_static.GetWindowRect().Width()/HORIZ_GRID;
    // nGridLines/=freq.freqMult;
    nGridLines-=3;
    if(nSliderMax>nGridLines)
        nSliderMax-=nGridLines;
    else
        nSliderMax=1;
    m_slider.SetTicFreq(1);
    m_slider.SetRangeMax(nSliderMax,TRUE);
    m_slider.SetRangeMin(0,TRUE);
    m_slider.SetPos(0,TRUE);
}
LRESULT CMyDialog::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Capture mouse input.
    POINT pt;
    // SetCapture();
    pt.x = GET_X_LPARAM(lparam);
    pt.y = GET_Y_LPARAM(lparam);
    return FinalWindowProc(msg, wparam, lparam);
}
LRESULT CMyDialog::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Release the capture on the mouse.
    //  ReleaseCapture();
    currWnd=NULL;
    // GetDoc().StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), false, m_penColor);
    return FinalWindowProc(msg, wparam, lparam);
}
//---------------------------------------------
// Sets the slider control's position.
void CMyDialog::SetSliderPos(int pos)
{
    switch(pos)
    {
    case 0:
        gainConst=100;
        dbScale=4;
        break;
    case 1:
        gainConst=80;
        dbScale=5;
        break;
    case 2:
        gainConst=40;
        dbScale=10;
        break;
    case 3:
        gainConst=20;
        dbScale=20;
        break;
    case 4:
        gainConst=10;
        dbScale=40;
        break;
    }
    createAxis();
}
// Called when the OK button or Enter key is pressed.
void CMyDialog::OnOK()
{
    MessageBox(L"OK Button Pressed.  Program will exit now.", L"Button", MB_OK);
    CDialog::OnOK();
}
//---------------------------------------
void CMyDialog::shangeVertResol(int nShange)
{
    switch(nShange)
    {
    case 0:
        if(vertPos<VERT_RES_MAX)
            vertPos++;
        break;
    case 1:
        if(vertPos>VERT_RES_MIN)
            vertPos--;
        break;
    }
    SetSliderPos(vertPos);
}
void CMyDialog::shangeHorResol(int nShange)
{
    switch(nShange)
    {
    case 0:
        if(freqConst<HOR_RES_MAX)
            freqConst++;
        break;
    case 1:
        if(freqConst>HOR_RES_MIN)
            freqConst--;
        break;
    }
    recalcSliderRange();
    recalcStartPos();
    createAxis();
}
// Called when Check Box 1 is clicked.
BOOL CMyDialog::OnCheckL()
{
    if(m_checkL.GetCheck()==BST_CHECKED)
    {
        HBITMAP  bitmap = reinterpret_cast<HBITMAP>(GetApp()->LoadImage(IDB_PAUSE, IMAGE_BITMAP,m_play.GetClientRect().Width(),m_play.GetClientRect().Height(), LR_SHARED));
        m_play.SetBitmap(bitmap);
        //    m_play.EnableWindow(true);
        leftEnable=true;
    }
    else
        leftEnable=false;
    createAxis();
    return TRUE;
}

// Called when Check Box 2 is clicked.
BOOL CMyDialog::OnCheckR()
{
    if(m_checkR.GetCheck()==BST_CHECKED)
    {
        HBITMAP  bitmap = reinterpret_cast<HBITMAP>(GetApp()->LoadImage(IDB_PAUSE, IMAGE_BITMAP,m_play.GetClientRect().Width(),m_play.GetClientRect().Height(), LR_SHARED));
        m_play.SetBitmap(bitmap);
        // m_play.EnableWindow(true);
        // m_play.SetFocus();
        rightEnable=true;
    }
    else
        rightEnable=false;
    createAxis();
    return TRUE;
}
//----------------------------------------------
void CMyDialog::OnSelShange(WPARAM wparam, LPARAM lparam)
{
    switch(HIWORD(wparam))
    {
    case CBN_SELCHANGE:
        HWND wnd = reinterpret_cast<HWND>(lparam);
        bool isCombo = (wnd == m_combox.GetHwnd());
        if ( isCombo)
        {
            CString deviceName;
            int item = m_combox.GetCurSel();
            int length = m_combox.GetLBTextLen(item);
            m_combox.GetLBText(item, deviceName.GetBuffer(length));
            deviceName.ReleaseBuffer();
            // ::MessageBoxW(0,std::to_wstring(m_combox.GetItemData(item)).c_str(),L"Combo",MB_OK);
            freq.setDevIndex(m_combox.GetItemData(item));
            std::thread t2(&CMyDialog::tunerStart,this);
            t2.detach();
            std::thread t3(&CMyDialog::tunerRead,this);
            t3.detach();
            m_combox.EnableWindow(false);
            drawBand=true;
            recalcStartPos();
            createAxis();
        }
        break;
    }
}
//-----------------------------------------------
BOOL CMyDialog::OnVScroll(WPARAM , LPARAM lparam)
{
    //    HWND wnd = reinterpret_cast<HWND>(lparam);
    //    int pos = 0;
    //    bool isSlider = (wnd == m_slider.GetHwnd());

    //    if (isSlider)
    //        pos = m_slider.GetPos();

    //    SetSliderPos(pos);
    return TRUE;
}
//-----------------------------------------------
BOOL CMyDialog::OnSetCursor(WPARAM , LPARAM lparam)
{
    POINT Point;
    Point.x = GET_X_LPARAM(lparam);
    Point.y = GET_Y_LPARAM(lparam);
#if 0
    std::wstring szPlace=utf8_to_wstring(std::to_string(Point.x)+" ; "+std::to_string(Point.y));
    szPlace+=utf8_to_wstring("\n"+std::to_string(m_play.GetWindowRect().left-GetWindowRect().left)+" ; "+std::to_string(m_play.GetWindowRect().left-GetWindowRect().left+m_play.GetWindowRect().Width()));
    szPlace+=utf8_to_wstring("\n"+std::to_string(m_play.GetWindowRect().top-GetWindowRect().top)+" ; "+std::to_string(m_play.GetWindowRect().top-GetWindowRect().top-m_play.GetWindowRect().Height()));
#endif
      {
//        HWND wnd=ChildWindowFromPointEx(GetHwnd(),Point,CWP_ALL);
//        bool isPlay = (wnd == m_play.GetHwnd());
        // if (isPlay)
         if(isPointinRect(Point,GetWindowRect(),m_play.GetWindowRect()))
        {
            HCURSOR hCurs1 = LoadCursor(NULL, IDC_HAND);
            SetCursor( hCurs1);
            cursShange=true;
        }
        else
        {
            cursShange=false;
            m_play.EnableWindow(false);
        }
    }
    return TRUE;
}
//---------------------------------------------
bool CMyDialog::isPointinRect(POINT pt,CRect rpar,CRect rc)
{
    bool inRect=false;
    int nLeft1=rc.left-rpar.left;
    int nLeft2=nLeft1+rc.Width();
    if((pt.x>=nLeft1)&&(pt.x<=nLeft2))
    {
        int nTop1=rc.top-rpar.top;
        int nTop2=nTop1-rc.Height();
        if((pt.y>=nTop2)&&(pt.y<=nTop1))
            inRect=true;
    }
    return inRect;
}
//-----------------------------------------------
BOOL CMyDialog::OnHScroll(WPARAM , LPARAM lparam)
{
    HWND wnd = reinterpret_cast<HWND>(lparam);
    bool isSlider = (wnd == m_slider.GetHwnd());

    if (isSlider)
    {
        recalcStartPos();
        createAxis();
    }
    return TRUE;
}
//-----------------------------------------------
BOOL CMyDialog::OnPlay(WPARAM , LPARAM lparam)
{
    if(cursShange)
    {
        HBITMAP bitmap;
        HWND wnd = reinterpret_cast<HWND>(lparam);
        bool isPlay = (wnd == m_play.GetHwnd());
        if (isPlay)
        {
            if(freq.blockView)
            {
                bitmap = reinterpret_cast<HBITMAP>(GetApp()->LoadImage(IDB_PAUSE, IMAGE_BITMAP,m_play.GetClientRect().Width(),m_play.GetClientRect().Height(), LR_SHARED));
                m_play.SetBitmap(bitmap);
                freq.blockView=false;
            }
            else
            {
                bitmap = reinterpret_cast<HBITMAP>(GetApp()->LoadImage(IDB_PLAY, IMAGE_BITMAP,m_play.GetClientRect().Width(),m_play.GetClientRect().Height(), LR_SHARED));
                m_play.SetBitmap(bitmap);
                freq.blockView=true;
            }
        }
    }
    return TRUE;
}
//-------------------------------------
void CMyDialog::recalcStartPos()
{
    startPos = m_slider.GetPos();
    startPos*=HORIZ_GRID;
    startPos*=freqConst;
    stopPos=startPos+(m_static.GetWindowRect().Width())*freqConst;
}
//-------------------------------------------------
void CMyDialog::createAxis()
{
    POINT lpp;
    PatBlt(hdcMaxis,0,0,m_static.GetWindowRect().Width(),m_static.GetWindowRect().Height(),WHITENESS|SRCAND);
    RECT rc;
    HBRUSH hBrush = CreateSolidBrush(RGB(166,203,244));
    rc.left=0;
    rc.top=0;
    rc.right=m_static.GetWindowRect().Width();
    rc.bottom= m_static.GetWindowRect().Height();
    FillRect(hdcMaxis, &rc, hBrush);
    HPEN hpen= CreatePen(PS_SOLID,1,RGB(70,138,53));
    SelectPen(hdcMaxis,hpen);
    int nLines=m_static.GetWindowRect().Height()/VERT_GRID;
    rc=m_static.GetWindowRect();
    SetTextColor(hdcMaxis, 0x00000000);
    SetBkMode(hdcMaxis,TRANSPARENT);
    rc.left=m_static.GetWindowRect().Width()-54;
    int dbValue=0;
    for(int i=0;i<=nLines;i++)
    {
        MoveToEx(hdcMaxis,0,m_static.GetWindowRect().Height()-2-VERT_GRID*i,&lpp);
        LineTo(hdcMaxis,m_static.GetWindowRect().Width(),m_static.GetWindowRect().Height()-2-VERT_GRID*i);
        rc.top=m_static.GetWindowRect().Height()-20-VERT_GRID*i;
        DrawText( hdcMaxis,utf8_to_wstring(std::to_string(dbValue)).c_str(), -1, &rc, DT_SINGLELINE | DT_NOCLIP) ;
        dbValue+=dbScale;
    }
    hpen= CreatePen(PS_SOLID,1,RGB(70,138,53));
    SelectPen(hdcMaxis,hpen);
    nLines=m_static.GetWindowRect().Width()/HORIZ_GRID;
    int freqValue=0;
    rc=m_static.GetWindowRect();
    rc.top=5;
    for(int i=0;i<=nLines;i++)
    {
        MoveToEx(hdcMaxis,HORIZ_GRID+HORIZ_GRID*i,0,&lpp);
        LineTo(hdcMaxis,HORIZ_GRID+HORIZ_GRID*i,m_static.GetWindowRect().Height()-2);
        rc.left=m_static.GetWindowRect().Width()-105-HORIZ_GRID*(nLines-i);
        DrawText( hdcMaxis,utf8_to_wstring(std::to_string(freqValue)).c_str(), -1, &rc, DT_SINGLELINE | DT_NOCLIP) ;
        freqValue=HORIZ_GRID*i*freqConst*freq.freqMult+startPos*freq.freqMult;
    }
    if(drawBand)
        SetDlgItemTextW(IDC_STATBAND,utf8_to_wstring(std::to_string(startPos*freq.freqMult)+" - "+std::to_string(freqValue)+ " Hz").c_str());
    if(leftEnable)
    {
        MoveToEx(hdcMaxis,m_static.GetWindowRect().Width()-155,40,&lpp);
        SelectPen(hdcMaxis,pen);
        LineTo(hdcMaxis,m_static.GetWindowRect().Width()-115,40);
        rc.left=m_static.GetWindowRect().Width()-112;
        rc.top=30;
        rc.right=m_static.GetWindowRect().Width()-95;
        rc.bottom=60;
        DrawText( hdcMaxis,L"Left", -1, &rc, DT_SINGLELINE | DT_NOCLIP) ;
    }
    if(rightEnable)
    {
        MoveToEx(hdcMaxis,m_static.GetWindowRect().Width()-155,70,&lpp);
        SelectPen(hdcMaxis,penR);
        LineTo(hdcMaxis,m_static.GetWindowRect().Width()-115,70);
        rc.left=m_static.GetWindowRect().Width()-112;
        rc.top=60;
        rc.right=m_static.GetWindowRect().Width()-95;
        rc.bottom=90;
        DrawText( hdcMaxis,L"Right", -1, &rc, DT_SINGLELINE | DT_NOCLIP) ;
    }
}
