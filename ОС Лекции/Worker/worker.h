//---------------------------------------------------------------------------

#ifndef workerH
#define workerH
#include <windows.h>

//Вспомогательный класс, очень помогает защищать данные.
//Защищает данные буквально до самой смерти
class MutexLocker
{
    HANDLE hMutex;
public:
    MutexLocker( HANDLE mutex )
        :hMutex( mutex )
    {
        WaitForSingleObject( hMutex, INFINITE );
    }
    ~MutexLocker()
    {
        ReleaseMutex( hMutex );
    }
};

class Worker
{
    HANDLE hUpdateEvt; //событие "данные обновились"
    HANDLE hStopEvt;   //событие "конец работы"

    HANDLE hMutex;  //защита данных
    int    DATA;    //очень важные данные

    HANDLE hThread; //дескриптор потока
public:
    Worker();
    ~Worker();

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
