//---------------------------------------------------------------------------

#ifndef mformH
#define mformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "rworker.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo;
    TButton *StartBtn;
    TButton *StopBtn;
    TTimer *Timer;
    void __fastcall StartBtnClick(TObject *Sender);
    void __fastcall StopBtnClick(TObject *Sender);
    void __fastcall TimerTimer(TObject *Sender);
private:	// User declarations
    RWorker    worker;
public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
