//---------------------------------------------------------------------------

#ifndef workerH
#define workerH
#include <windows.h>

//��������������� �����, ����� �������� �������� ������.
//�������� ������ ��������� �� ����� ������
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
    HANDLE hUpdateEvt; //������� "������ ����������"
    HANDLE hStopEvt;   //������� "����� ������"

    HANDLE hMutex;  //������ ������
    int    DATA;    //����� ������ ������

    HANDLE hThread; //���������� ������
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
