/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H
#include <Windows.h>
#include "wxx_stdcontrols.h"
#include "wxx_controls.h"
#include "Frequency.h"
#include "aboutdlg.h"

enum
{
    VERT_RES_MIN=0,
    VERT_RES_MAX=4,
    HOR_RES_MIN=1,
    HOR_RES_MAX=14,
    MAX_FREQ_VALUE=21000,
    VERT_GRID=40,
    HORIZ_GRID=80
};
///////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
    virtual ~CMyDialog() override;
protected:
    // Virtual functions that override base class functions
    virtual void    OnDestroy() override;
    virtual void    OnDraw(CDC&) override;
    virtual BOOL    OnInitDialog() override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual void    OnOK() override;

private:
  //  HANDLE ghWriteEvent;
    std::vector<int> nBand;
    HPEN pen;
    HPEN penR;
    int sampleCount;
    int gainConst;
    int dbScale;
    int freqConst;
    int vertPos;
    int startPos;
    int stopPos;
    bool leftEnable;
    bool rightEnable;
    bool drawBand;
    bool cursShange;
    Frequency freq;
    HDC hdc;
    HDC hdcMem;
    HANDLE hBitMap;
    HDC hdcMaxis;
    HANDLE hBitMaxis;
    HWND currWnd;

    void tunerStart();
    void tunerRead();
    CMyDialog(const CMyDialog&) = delete;
    CMyDialog& operator=(const CMyDialog&) = delete;
    void drawChart();
    void createAxis();
    // Command handlers
    time_t curr_time;
    BOOL OnCheckL();
    BOOL OnCheckR();
    void shangeVertResol(int nShange);
    void shangeHorResol(int nShange);
    void OnSelShange(WPARAM wparam, LPARAM lparam);

    BOOL OnVScroll(WPARAM, LPARAM lparam);
    BOOL OnHScroll(WPARAM, LPARAM lparam);
    BOOL OnPlay(WPARAM , LPARAM lparam);
    BOOL OnSetCursor(WPARAM , LPARAM lparam);

    bool isPointinRect(POINT pt, CRect rpar, CRect rc);

    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);

   void SetSliderPos(int pos);
   void recalcSliderRange();
   void recalcStartPos();

    // Member variables
    CButton m_butTop;
    CButton m_butDown;
    CButton m_butLeft;
    CButton m_butRight;
    CButton m_checkL;
    CButton m_checkR;
    CStatic m_static;
    CStatic m_play;
    CSlider m_slider;
    CComboBoxEx m_combox;
    CFont m_font;
    CBitmap m_patternImage;
   AboutDlg* m_aboutDialog;

};

#endif //MYDIALOG_H
