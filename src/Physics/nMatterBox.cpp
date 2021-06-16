#include <physics.h>

N::MatterBox:: MatterBox (QWidget * parent,Plan * p)
             : Widget    (          parent,       p)
             , matter    (NULL                     )
{
  Configure ( ) ;
}

N::MatterBox::~MatterBox(void)
{
}

N::MatterPainter * N::MatterBox::Painter(void) const
{
  return matter ;
}

void N::MatterBox::setPainter(MatterPainter * painter)
{
  matter = painter                    ;
  if (IsNull(matter)) return          ;
  setToolTip ( painter -> toolTip() ) ;
}

void N::MatterBox::paintEvent(QPaintEvent * event)
{ Q_UNUSED ( event )            ;
  if (IsNull(matter)) return    ;
  QPainter p ( this )           ;
  QRect W(0,0,width(),height()) ;
  matter -> Paint ( p , W )     ;
}

void N::MatterBox::contextMenuEvent(QContextMenuEvent * event)
{
  if (Menu(event->pos())) event->accept(); else
  QWidget::contextMenuEvent(event);
}

void N::MatterBox::focusInEvent(QFocusEvent * event)
{
  if (!focusIn (event)) QWidget::focusInEvent (event) ;
}

void N::MatterBox::focusOutEvent(QFocusEvent * event)
{
  if (!focusOut(event)) QWidget::focusOutEvent(event) ;
}

void N::MatterBox::Configure(void)
{
}

bool N::MatterBox::FocusIn(void)
{
  if (IsNull(matter)) return false  ;
  return matter -> FocusIn ( this ) ;
}

bool N::MatterBox::FocusOut(void)
{
  if (IsNull(matter)) return false   ;
  return matter -> FocusOut ( this ) ;
}

bool N::MatterBox::Menu(QPoint pos)
{
  if (IsNull(matter)) return false ;
  return matter->Menu(this,pos)    ;
}
