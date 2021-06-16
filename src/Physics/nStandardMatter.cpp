#include <physics.h>

#define EUUID 94242282865164288ull

N::StandardMatter:: StandardMatter (QObject * parent,int id)
                  : QObject        (          parent       )
                  , MatterPainter  (                       )
                  , Type           (id                     )
{
  setParticle ( id ) ;
}

N::StandardMatter::~StandardMatter (void)
{
}

void N::StandardMatter::Paint(QPainter & p,QRect region)
{
  QPen Pen ( Border )                   ;
  int  lw = region.width()              ;
  int  rw                               ;
  lw /= 24                              ;
  if (lw<0) lw = 1                      ;
  rw = lw * 2                           ;
  Pen . setWidth        ( lw          ) ;
  ///////////////////////////////////////
  QPoint P = region . topLeft ( )       ;
  QSize  W = region . size    ( )       ;
  P  += QPoint          ( lw , lw     ) ;
  W  -= QSize           ( rw , rw     ) ;
  ///////////////////////////////////////
  QRect  R              ( P , W       ) ;
  p   . save            (             ) ;
  p   . setPen          ( Pen         ) ;
  p   . drawRoundedRect ( R , rw , rw ) ;
  p   . restore         (             ) ;
  ///////////////////////////////////////
  int th = region.height() / 4          ;
  int tw = region.width () - ( rw * 3 ) ;
  int tt = region.height() - th - rw    ;
  QPoint PN ( rw * 3 / 2 , tt )         ;
  QSize  TW ( tw , th         )         ;
  PaintName ( p,Fullname,QRect(PN,TW) ) ;
  ///////////////////////////////////////
  int    xw = region.width () / 2       ;
  int    xh = region.height() / 2       ;
  QPoint CX = region.center()           ;
  QSize  WX ( xw , xh )                 ;
  CX -= QPoint ( xw / 2 , xh * 11/16 )  ;
  QRect  XX ( CX , WX )                 ;
  PaintSymbol ( p , Symbol , XX )       ;
}

void N::StandardMatter::PaintName(QPainter & p,QString s,QRect region)
{
  QString t                                     ;
  QFont   f = p.font()                          ;
  f . setPixelSize ( region . height ( ) / 2 )  ;
  ///////////////////////////////////////////////
  if (s.contains(' '))                          {
    QStringList l = s.split(' ')                ;
    t = l.join("\n")                            ;
  } else t = s                                  ;
  ///////////////////////////////////////////////
  p . save     (                              ) ;
  p . setPen   ( QColor ( 0 , 0 , 0     )     ) ;
  p . setBrush ( QBrush ( QColor(0,0,0) )     ) ;
  p . setFont  ( f                            ) ;
  p . drawText ( region , Qt::AlignCenter , t ) ;
  p . restore  (                              ) ;
}

void N::StandardMatter::PaintSymbol(QPainter & p,QString s,QRect region)
{
  QPoint SP ( region.width()/4,region.width()/4 ) ;
  QRadialGradient ZR (region.center(),region.width()/2,SP + region.center() ) ;
  ZR.setColorAt   ( 0.0 , Border              ) ;
  ZR.setColorAt   ( 1.0 , QColor(248,248,248) ) ;
  ///////////////////////////////////////////////
  QBrush R        ( ZR     )              ;
  QColor B        ( 0,0,0  )              ;
  p . save        (        )              ;
  p . setPen      ( Border )              ;
  p . setBrush    ( R      )              ;
  p . drawEllipse ( region )              ;
  p . restore     (        )              ;
  /////////////////////////////////////////
  int w = region.width()                  ;
  w *=  7                                 ;
  w /= 10                                 ;
  /////////////////////////////////////////
  QPoint CX = region.center()             ;
  QSize  WX ( w , w )                     ;
  CX -= QPoint ( w / 2 , w / 2 )          ;
  /////////////////////////////////////////
  QRect T ( CX , WX )                     ;
  QFont f = p.font()                      ;
  f . setPixelSize( w      )              ;
  f . setBold     ( true   )              ;
  p . save        (        )              ;
  p . setPen      ( B      )              ;
  p . setBrush    ( B      )              ;
  p . setFont     ( f      )              ;
  if (s.contains("_") || s.contains("^")) {
    w *= 2                                ;
    w /= 3                                ;
    CX  = region.center()                 ;
    CX -= QPoint ( w * 3 / 4 , w / 2 )    ;
    WX  = QSize  ( w , w             )    ;
    T   = QRect  ( CX , WX           )    ;
    WX  = QSize  ( w / 2 , w / 2     )    ;
    if (s.contains("_"))                  {
      QStringList L = s.split("_")        ;
      CX  = T.bottomRight()               ;
      CX -= QPoint ( 0 , w / 3 )          ;
      f.setPixelSize(w)                   ;
      p.setFont (f)                       ;
      p.drawText(T,Qt::AlignCenter,L[0])  ;
      T = QRect ( CX , WX )               ;
      f.setPixelSize(WX.width())          ;
      p.setFont (f)                       ;
      p.drawText(T,Qt::AlignCenter,L[1])  ;
    } else
    if (s.contains("^"))                  {
      QStringList L = s.split("^")        ;
      CX  = T.topRight()                  ;
      f.setPixelSize(w)                   ;
      p.setFont (f)                       ;
      p.drawText(T,Qt::AlignCenter,L[0])  ;
      T = QRect ( CX , WX )               ;
      f.setPixelSize(WX.width())          ;
      p.setFont (f)                       ;
      p.drawText(T,Qt::AlignCenter,L[1])  ;
    }                                     ;
  } else                                  {
    p.drawText ( T,Qt::AlignCenter,s    ) ;
  }                                       ;
  p . restore     (        )              ;
}

bool N::StandardMatter::FocusIn(QWidget * widget)
{
  return true ;
}

bool N::StandardMatter::FocusOut(QWidget * widget)
{
  return true ;
}

QString N::StandardMatter::toolTip(void)
{
  return Fullname ;
}

void N::StandardMatter::setParticle(int id)
{
  switch ( id )                          {
    case  1                              :
      Border   = QColor(204,144,255)     ;
      Fullname = tr("up")                ;
      Symbol   = tr("u")                 ;
      Particles << EUUID + 12            ;
      Particles << EUUID + 13            ;
      Particles << EUUID + 14            ;
      Particles << EUUID + 15            ;
      Particles << EUUID + 16            ;
      Particles << EUUID + 17            ;
    break                                ;
    case  2                              :
      Border   = QColor(204,144,255)     ;
      Fullname = tr("charm")             ;
      Symbol   = tr("c")                 ;
      Particles << EUUID + 24            ;
      Particles << EUUID + 25            ;
      Particles << EUUID + 26            ;
      Particles << EUUID + 27            ;
      Particles << EUUID + 28            ;
      Particles << EUUID + 29            ;
    break                                ;
    case  3                              :
      Border   = QColor(204,144,255)     ;
      Fullname = tr("top")               ;
      Symbol   = tr("t")                 ;
      Particles << EUUID + 36            ;
      Particles << EUUID + 37            ;
      Particles << EUUID + 38            ;
      Particles << EUUID + 39            ;
      Particles << EUUID + 40            ;
      Particles << EUUID + 41            ;
    break                                ;
    case  4                              :
      Border   = QColor(204,144,255)     ;
      Fullname = tr("down")              ;
      Symbol   = tr("d")                 ;
      Particles << EUUID + 18            ;
      Particles << EUUID + 19            ;
      Particles << EUUID + 20            ;
      Particles << EUUID + 21            ;
      Particles << EUUID + 22            ;
      Particles << EUUID + 23            ;
    break                                ;
    case  5                              :
      Border   = QColor(204,144,255)     ;
      Fullname = tr("strange")           ;
      Symbol   = tr("s")                 ;
      Particles << EUUID + 30            ;
      Particles << EUUID + 31            ;
      Particles << EUUID + 32            ;
      Particles << EUUID + 33            ;
      Particles << EUUID + 34            ;
      Particles << EUUID + 35            ;
    break                                ;
    case  6                              :
      Border   = QColor(204,144,255)     ;
      Fullname = tr("bottom")            ;
      Symbol   = tr("b")                 ;
      Particles << EUUID + 42            ;
      Particles << EUUID + 43            ;
      Particles << EUUID + 44            ;
      Particles << EUUID + 45            ;
      Particles << EUUID + 46            ;
      Particles << EUUID + 47            ;
    break                                ;
    case  7                              :
      Border   = QColor( 85,204, 34)     ;
      Fullname = tr("electron")          ;
      Symbol   = tr("e")                 ;
      Particles << EUUID                 ;
      Particles << EUUID +  1            ;
    break                                ;
    case  8                              :
      Border   = QColor( 85,204, 34)     ;
      Fullname = tr("muon")              ;
      Symbol   = tr("Mu")                ;
      Particles << EUUID +  2            ;
      Particles << EUUID +  3            ;
    break                                ;
    case  9                              :
      Border   = QColor( 85,204, 34)     ;
      Fullname = tr("tau")               ;
      Symbol   = tr("Tau")               ;
      Particles << EUUID +  4            ;
      Particles << EUUID +  5            ;
    break                                ;
    case 10                              :
      Border   = QColor( 85,204, 34)     ;
      Fullname = tr("electron neutrino") ;
      Symbol   = tr("V_e")               ;
      Particles << EUUID +  6            ;
      Particles << EUUID +  7            ;
    break                                ;
    case 11                              :
      Border   = QColor( 85,204, 34)     ;
      Fullname = tr("muon neutrino")     ;
      Symbol   = tr("V_u")               ;
      Particles << EUUID +  8            ;
      Particles << EUUID +  9            ;
    break                                ;
    case 12                              :
      Border   = QColor( 85,204, 34)     ;
      Fullname = tr("tau neutrino")      ;
      Symbol   = tr("V_r")               ;
      Particles << EUUID + 10            ;
      Particles << EUUID + 11            ;
    break                                ;
    case 13                              :
      Border   = QColor(255, 96, 96)     ;
      Fullname = tr("gluon")             ;
      Symbol   = tr("g")                 ;
      Particles << EUUID + 48            ;
      Particles << EUUID + 49            ;
      Particles << EUUID + 50            ;
      Particles << EUUID + 51            ;
      Particles << EUUID + 52            ;
      Particles << EUUID + 53            ;
      Particles << EUUID + 54            ;
      Particles << EUUID + 55            ;
    break                                ;
    case 14                              :
      Border   = QColor(255, 96, 96)     ;
      Fullname = tr("photon")            ;
      Symbol   = tr("r")                 ;
      Particles << EUUID + 56            ;
    break                                ;
    case 15                              :
      Border   = QColor(255, 96, 96)     ;
      Fullname = tr("Z boson")           ;
      Symbol   = tr("Z")                 ;
      Particles << EUUID + 60            ;
    break                                ;
    case 16                              :
      Border   = QColor(255, 96, 96)     ;
      Fullname = tr("W boson")           ;
      Symbol   = tr("W")                 ;
      Particles << EUUID + 58            ;
      Particles << EUUID + 59            ;
    break                                ;
    case 17                              :
      Border   = QColor(248,224, 96)     ;
      Fullname = tr("Higgs boson")       ;
      Symbol   = tr("H")                 ;
      Particles << EUUID + 61            ;
    break                                ;
    case 18                              :
      Border   = QColor(248,224, 96)     ;
      Fullname = tr("Graviton")          ;
      Symbol   = tr("G")                 ;
      Particles << EUUID + 57            ;
    break                                ;
  }                                      ;
}

bool N::StandardMatter::Menu(QWidget * widget,QPoint pos)
{
  nScopedMenu ( mm , widget )      ;
  QAction * aa                     ;
  aa  = mm.add(101,Fullname)       ;
  aa -> setEnabled ( false )       ;
  mm . addSeparator ( )            ;
  mm . add (101,tr("Information")) ;
  if (NotNull(qPlan))              {
    mm . setFont ( AppPlan )       ;
  }                                ;
  aa = mm . exec ( )               ;
  if (IsNull(aa)) return true      ;
  return true                      ;
}
