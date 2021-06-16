#include <physics.h>

N::MatterContainer:: MatterContainer (QWidget * parent,Plan * p)
                   : Widget          (          parent,       p)
{
  Configure ( ) ;
}

N::MatterContainer::~MatterContainer(void)
{
}

void N::MatterContainer::contextMenuEvent(QContextMenuEvent * event)
{
  if (Menu(event->pos())) event->accept(); else
  QWidget::contextMenuEvent(event);
}

void N::MatterContainer::focusInEvent(QFocusEvent * event)
{
  if (!focusIn (event)) QWidget::focusInEvent (event) ;
}

void N::MatterContainer::focusOutEvent(QFocusEvent * event)
{
  if (!focusOut(event)) QWidget::focusOutEvent(event) ;
}

void N::MatterContainer::resizeEvent(QResizeEvent * event)
{
  if (Relocation()) event->accept();
  else QWidget::resizeEvent(event);
}

void N::MatterContainer::showEvent(QShowEvent * event)
{
  QWidget::showEvent(event) ;
  Relocation ( )            ;
}

void N::MatterContainer::Configure(void)
{
}

bool N::MatterContainer::FocusIn(void)
{
  return true ;
}

bool N::MatterContainer::FocusOut(void)
{
  return false ;
}

bool N::MatterContainer::Relocation(void)
{
  return true ;
}

void N::MatterContainer::addMatter(SUID uuid,MatterBox * box,QRect rect)
{
  MatterUuid [ uuid ] = box  ;
  MatterRect [ box  ] = rect ;
}

bool N::MatterContainer::Menu(QPoint pos)
{
  return true ;
}
