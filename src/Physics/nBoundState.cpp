#include <physics.h>

N::BoundState:: BoundState (void)
              : QByteArray (    )
              , uuid       (0   )
{
}

N::BoundState::~BoundState(void)
{
}

UUIDs N::BoundState::toUuids(void)
{
  UUIDs U                   ;
  int ps = particles ( )    ;
  if (ps<=0) return U       ;
  SUID * u = (SUID *)data() ;
  for (int i=0;i<ps;i++)    {
    U << u[i]               ;
  }                         ;
  return U                  ;
}

int N::BoundState::particles(void)
{
  return size ( ) / sizeof(SUID) ;
}

bool N::BoundState::contains(SUID puid)
{
  int ps = particles ( )        ;
  if (ps<=0) return false       ;
  SUID * u = (SUID *)data()     ;
  for (int i=0;i<ps;i++)        {
    if (u[i]==puid) return true ;
  }                             ;
  return false                  ;
}

void N::BoundState::sort(void)
{
  int ps = particles()      ;
  if (ps<=1) return         ;
  UUIDs U                   ;
  SUID * u = (SUID *)data() ;
  for (int i=0;i<ps;i++)    {
    U << u[i]               ;
  }                         ;
  qSort(U.begin(),U.end())  ;
  for (int i=0;i<ps;i++)    {
    u[i] = U[i]             ;
  }                         ;
}

int N::BoundState::append(SUID puid)
{
  QByteArray :: append ( (const char *)&puid , sizeof(SUID) ) ;
  sort                 (                                    ) ;
  return particles     (                                    ) ;
}

int N::BoundState::take(int index)
{
  if ( index >= particles ( ) ) return particles ( )     ;
  remove           ( sizeof(SUID)*index , sizeof(SUID) ) ;
  sort             (                                   ) ;
  return particles (                                   ) ;
}

int N::BoundState::operator << (SUID puid)
{
  return append ( puid ) ;
}
