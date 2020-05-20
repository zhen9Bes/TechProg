//---------------------------------------------------------------------------
#pragma hdrstop
#include "rworker.h"

RWorker::RWorker()
{
    //������� � ������ �����������, ���������� ��������
    hUpdateEvt = CreateEvent( NULL, TRUE, TRUE, NULL );
    //������� � ������ �����������, ���������� ����������
    hStopEvt   = CreateEvent( NULL, TRUE, TRUE, NULL );
    //������, ���� ���������
    hMutex = CreateMutex( NULL, FALSE, "" );

    DATA    = 0;
    hThread = INVALID_HANDLE_VALUE; //����� ���� �� �������
}

RWorker::~RWorker()
{
    stopWork();  //���� ��������, ���������������
    CloseHandle( hUpdateEvt );
    CloseHandle( hStopEvt );
    CloseHandle( hMutex );
}

void    RWorker::startWork()
{
    if( hThread != INVALID_HANDLE_VALUE ) return; //��� �������
    ResetEvent( hStopEvt ); //����� ������� ������ ���������� ���� ���������
    hThread = CreateThread( NULL, 0, ThreadFunc, this, 0, NULL ); //����������� � ������
}

void    RWorker::stopWork ()
{
    if( hThread == INVALID_HANDLE_VALUE ) return; //�� �������
    SetEvent( hStopEvt ); //������� ���� ���������
    //����, ����� ����� ������� ��� ���� � �����������
    if( WaitForSingleObject( hThread, 5000 ) != WAIT_OBJECT_0 )
    {
        //�������� ���� �����, �� �����������, "��������" ������, ����������, ����� �� �������
        TerminateThread( hThread, 1 );
    }
    CloseHandle( hThread );
    hThread = INVALID_HANDLE_VALUE;
}

int  RWorker::getData()
{
    RMutexLocker guard( hMutex );
    ResetEvent( hUpdateEvt );
    return DATA;
}

void RWorker::setData( int val )
{
    RMutexLocker guard( hMutex );
    DATA = val;
    SetEvent( hUpdateEvt );
}

bool RWorker::isDataUpdated()
{
    return WaitForSingleObject( hUpdateEvt, 0 ) == WAIT_OBJECT_0;
}


DWORD WINAPI RWorker::ThreadFunc( LPVOID ptr )
{
    RWorker* _this = (RWorker*)ptr;
    if( !_this ) return 1; //������ ��� ���� ������, �� ����� ��������

    for(;;)
    {
        //������� ������ ������
        _this->setData( _this->getData() + 1 );
        //������, ����� �� ���� ����� ������
        if( WaitForSingleObject( _this->hStopEvt, 1000 ) != WAIT_TIMEOUT )
            break;
    }
    return 0;
}



//---------------------------------------------------------------------------
#pragma package(smart_init)
 
