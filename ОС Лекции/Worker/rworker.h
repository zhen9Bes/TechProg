//---------------------------------------------------------------------------

#ifndef workerH
#define workerH
#include <windows.h>

//Вспомогательный класс, очень помогает защищать данные.
//Защищает данные буквально до самой смерти
class RMutexLocker
{
    HANDLE hMutex;
public:
    RMutexLocker( HANDLE mutex )
        :hMutex( mutex )
    {
        WaitForSingleObject( hMutex, INFINITE );
    }
    ~RMutexLocker()
    {
        ReleaseMutex( hMutex );
    }
};

class RWorker
{
    HANDLE hUpdateEvt; //событие "данные обновились"
    HANDLE hStopEvt;   //событие "конец работы"

    HANDLE hMutex;  //защита данных
    int    DATA;    //очень важные данные

    HANDLE hThread; //дескриптор потока
public:
    RWorker();
    ~RWorker();

    int     getData();
    void    setData( int val );
    bool    isDataUpdated();

    void    startWork();
    void    stopWork ();

private:
    static DWORD WINAPI ThreadFunc( LPVOID ptr );
};


//---------------------------------------------------------------------------
#endif
 
