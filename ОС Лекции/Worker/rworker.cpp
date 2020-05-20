//---------------------------------------------------------------------------
#pragma hdrstop
#include "rworker.h"

RWorker::RWorker()
{
    //—обытие с ручным управлением, изначально активное
    hUpdateEvt = CreateEvent( NULL, TRUE, TRUE, NULL );
    //—обытие с ручным управлением, изначально неактивное
    hStopEvt   = CreateEvent( NULL, TRUE, TRUE, NULL );
    //ћютекс, пока свободный
    hMutex = CreateMutex( NULL, FALSE, "" );

    DATA    = 0;
    hThread = INVALID_HANDLE_VALUE; //поток пока не запущен
}

RWorker::~RWorker()
{
    stopWork();  //если работали, останавливаемс€
    CloseHandle( hUpdateEvt );
    CloseHandle( hStopEvt );
    CloseHandle( hMutex );
}

void    RWorker::startWork()
{
    if( hThread != INVALID_HANDLE_VALUE ) return; //уже запущен
    ResetEvent( hStopEvt ); //перед началом работы сбрасываем флаг остановки
    hThread = CreateThread( NULL, 0, ThreadFunc, this, 0, NULL ); //запускаемс€ в работу
}

void    RWorker::stopWork ()
{
    if( hThread == INVALID_HANDLE_VALUE ) return; //не запущен
    SetEvent( hStopEvt ); //взводим флаг остановки
    //ждем, когда поток заметит наш флаг и остановитс€
    if( WaitForSingleObject( hThread, 5000 ) != WAIT_OBJECT_0 )
    {
        //прождали п€ть минут, не остановилс€, "приболел" видимо, пристрелим, чтобы не мучалс€
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
    if( !_this ) return 1; //забыли нам дать объект, не будем работать

    for(;;)
    {
        //сделаем важную работу
        _this->setData( _this->getData() + 1 );
        //поспим, гл€д€ на флаг конца работы
        if( WaitForSingleObject( _this->hStopEvt, 1000 ) != WAIT_TIMEOUT )
            break;
    }
    return 0;
}



//---------------------------------------------------------------------------
#pragma package(smart_init)
 
