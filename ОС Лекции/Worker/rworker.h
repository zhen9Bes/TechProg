//---------------------------------------------------------------------------

#ifndef workerH
#define workerH
#include <windows.h>

//��������������� �����, ����� �������� �������� ������.
//�������� ������ ��������� �� ����� ������
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
    HANDLE hUpdateEvt; //������� "������ ����������"
    HANDLE hStopEvt;   //������� "����� ������"

    HANDLE hMutex;  //������ ������
    int    DATA;    //����� ������ ������

    HANDLE hThread; //���������� ������
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
 
