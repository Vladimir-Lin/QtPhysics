#include <physics.h>

#define EUUID 94242282865164288ull
#define RSize 24

N::StandardParticles:: StandardParticles (QWidget * parent,Plan * p)
                     : MatterContainer   (          parent,       p)
{
  Configure ( ) ;
}

N::StandardParticles::~StandardParticles(void)
{
}

QSize N::StandardParticles::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( RSize * 11 , RSize * 8 ) ) ;
}

N::StandardMatter * N::StandardParticles::standard (int i)
{
  StandardMatter * sm = new StandardMatter ( this , i ) ;
  return sm                                             ;
}

void N::StandardParticles::Configure(void)
{
  setWindowTitle ( tr("Standard Model of Elementary Particles") ) ;
  setMinimumSize ( 12 * 11 , 12 * 8                             ) ;
  for (int i=1;i<=18;i++)                                         {
    MatterBox      * mb = new MatterBox ( this , plan )           ;
    StandardMatter * sm = standard      ( i           )           ;
    mb -> setPainter ( sm )                                       ;
    addMatter ( i , mb , QRect ( 0 , 0 , RSize , RSize )        ) ;
  }                                                               ;
  /////////////////////////////////////////////////////////////////
  PositionMaps [  1 ] =  1                                        ;
  PositionMaps [  2 ] =  4                                        ;
  PositionMaps [  3 ] =  7                                        ;
  PositionMaps [  4 ] = 10                                        ;
  PositionMaps [  5 ] =  2                                        ;
  PositionMaps [  6 ] =  5                                        ;
  PositionMaps [  7 ] =  8                                        ;
  PositionMaps [  8 ] = 11                                        ;
  PositionMaps [  9 ] =  3                                        ;
  PositionMaps [ 10 ] =  6                                        ;
  PositionMaps [ 11 ] =  9                                        ;
  PositionMaps [ 12 ] = 12                                        ;
  PositionMaps [ 13 ] = 13                                        ;
  PositionMaps [ 14 ] = 14                                        ;
  PositionMaps [ 15 ] = 15                                        ;
  PositionMaps [ 16 ] = 16                                        ;
  PositionMaps [ 17 ] = 17                                        ;
  PositionMaps [ 18 ] = 18                                        ;
}

void N::StandardParticles::paintEvent(QPaintEvent * event)
{ Q_UNUSED              ( event ) ;
  QPainter   p          ( this  ) ;
  int        w = height (       ) ;
  int        h = width  (       ) ;
  int        r = w / 8            ;
  if ( ( h / 11 ) < r )           {
    r = h / 11                    ;
  }                               ;
  /////////////////////////////////
  int tw = r * 4                  ;
  int ps = r / 2                  ;
  QSize   T    ( tw , ps        ) ;
  QPoint  L                       ;
  QPoint  Q                       ;
  QPoint  G                       ;
  QString leptons                 ;
  QString quarks                  ;
  QString bosons                  ;
  leptons = tr ("Leptons"       ) ;
  quarks  = tr ("Quarks"        ) ;
  bosons  = tr ("Gauge Bosons"  ) ;
  /////////////////////////////////
  p . save (                    ) ;
  p . translate ( h / 2 , w / 2 ) ;
  p . rotate    ( -90.0         ) ;
  L . setY ( - (r*5) - ps       ) ;
  L . setX ( - (r*4)            ) ;
  Q . setY ( - (r*5) - ps       ) ;
  Q . setX ( 0                  ) ;
  G . setY (   (r*3)            ) ;
  G . setX ( - (r*4)            ) ;
  /////////////////////////////////
  QColor BC                       ;
  QColor LC                       ;
  QColor QC                       ;
  QColor GC                       ;
  BC = QColor(255,255,255)        ;
  LC = QColor( 85,204, 34)        ;
  QC = QColor(204,144,255)        ;
  GC = QColor(255, 96, 96)        ;
  // Higgs , (248,224, 96)
  /////////////////////////////////
  QFont f = p.font()              ;
  f . setPixelSize ( ps   )       ;
  f . setBold      ( true )       ;
  p . setFont      ( f    )       ;
  /////////////////////////////////
  p . setPen   ( BC )             ;
  p . setBrush ( BC )             ;
  p . drawRect ( - ( w / 2 )      ,
                 - ( h / 2 )      ,
                 w                ,
                 h              ) ;
  /////////////////////////////////
  p . setPen   ( LC )             ;
  p . setBrush ( LC )             ;
  p . drawText                    (
    QRect(L,T)                    ,
    Qt::AlignLeft                 |
    Qt::AlignVCenter              ,
    leptons                     ) ;
  /////////////////////////////////
  p . setPen   ( QC )             ;
  p . setBrush ( QC )             ;
  p . drawText                    (
    QRect(Q,T)                    ,
    Qt::AlignLeft                 |
    Qt::AlignVCenter              ,
    quarks                      ) ;
  /////////////////////////////////
  p . setPen   ( GC )             ;
  p . setBrush ( GC )             ;
  p . drawText                    (
    QRect(G,T)                    ,
    Qt::AlignLeft                 |
    Qt::AlignVCenter              ,
    bosons                      ) ;
  /////////////////////////////////
  p . restore ( )                 ;
}

bool N::StandardParticles::FocusIn(void)
{
  return true ;
}

bool N::StandardParticles::FocusOut(void)
{
  return false ;
}

bool N::StandardParticles::Relocation(void)
{
  int    w = width  (       )      ;
  int    h = height (       )      ;
  int    r = h / 8                 ;
  QPoint C ( w / 2 , h / 2 )       ;
  if ( ( w / 11 ) < r ) r = w / 11 ;
  //////////////////////////////////
  QMap<int,QRect> M                ;
  QSize           W ( r*2 , r*2 )  ;
  QPoint          S = C            ;
  S -= QPoint ( r * 5 , r * 4 )    ;
  //////////////////////////////////
  for (int i=1;i<=18;i++)          {
    int    x = ( i - 1 ) / 4       ;
    int    y = ( i - 1 ) % 4       ;
    int    z = PositionMaps[i]     ;
    QPoint A                       ;
    x *= 2                         ;
    x *= r                         ;
    y *= 2                         ;
    y *= r                         ;
    A  = QPoint ( x , y )          ;
    A += S                         ;
    M [ i ] = QRect ( A , W )      ;
    ////////////////////////////////
    MatterBox * mb                 ;
    mb = MatterUuid [z]            ;
    MatterRect [ mb ] = M [ i ]    ;
    mb -> setGeometry ( M [ i ]  ) ;
  }                                ;
  //////////////////////////////////
  return true                      ;
}

bool N::StandardParticles::Menu(QPoint pos)
{
  return true ;
}
