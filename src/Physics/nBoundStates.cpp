#include <physics.h>

N::BoundStates:: BoundStates (void)
{
}

N::BoundStates::~BoundStates(void)
{
}

UUIDs N::BoundStates::toUuids(void)
{
  UUIDs U                     ;
  if (count()<=0) return U    ;
  for (int i=0;i<count();i++) {
    U << ME[i].uuid           ;
  }                           ;
  return U                    ;
}

bool N::BoundStates::contains(BoundState & bound)
{
  if (count()<=0) return false           ;
  for (int i=0;i<count();i++)            {
    if ( ME [ i ] == bound ) return true ;
  }                                      ;
  return false                           ;
}
