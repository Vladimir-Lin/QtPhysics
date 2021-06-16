#include <physics.h>

N::CelestialsEditor:: CelestialsEditor ( QWidget * parent , Plan * p )
                    : ItemEditor       (           parent ,        p )
{
  Configure ( ) ;
}

N::CelestialsEditor::~CelestialsEditor(void)
{
}

void N::CelestialsEditor::Configure(void)
{
  dockingOrientation = Qt::Vertical               ;
  ItemType           = Types::Celestial           ;
  ItemTable          = PlanTable(Celestials)      ;
  NewTreeWidgetItem ( head                      ) ;
  head->setText     ( 0 , tr("Celestial")       ) ;
  setWindowTitle    ( tr("Types of celestials") ) ;
  assignHeaderItems ( head                      ) ;
  setDragDropMode   ( DropOnly                  ) ;
  setDropFlag       ( DropStar , true           ) ;
  plan -> setFont   ( this                      ) ;
}

bool N::CelestialsEditor::dropStars(QWidget * widget,QPointF pos,const UUIDs & Uuids)
{
  if ( widget == this ) return false ;
  QTreeWidgetItem * it               ;
  it = itemAt ( pos.toPoint() )      ;
  if ( IsNull(it) ) return false     ;
  SUID t = nTreeUuid ( it , 0 )      ;
  if ( t <= 0 ) return false         ;
  EnterSQL ( SC , plan->sql )        ;
    QString Q                        ;
    SUID    u                        ;
    foreach (u,Uuids)                {
      Q = SC . sql . Update          (
            PlanTable(Stars)         ,
            SC.WhereUuid(u)          ,
            1                        ,
            "type"                 ) ;
      SC . Prepare ( Q             ) ;
      SC . Bind    ( "type" , t    ) ;
      SC . Exec    (               ) ;
    }                                ;
  LeaveSQL ( SC , plan->sql )        ;
  Alert    ( Done           )        ;
  return true                        ;
}

bool N::CelestialsEditor::Menu(QPoint pos)
{
  MenuManager       mm ( this )                       ;
  QTreeWidgetItem * item = itemAt ( pos )             ;
  QAction         * aa                                ;
  UUIDs             Uuids                             ;
  if (NotNull(item))                                  {
    mm . add ( 101 , tr("Edit")                     ) ;
  }                                                   ;
  mm . add ( 201 , tr("Insert" )                    ) ;
  mm . addSeparator()                                 ;
  mm . add ( 501 , tr("Refresh")                    ) ;
  mm . addSeparator()                                 ;
  mm . add ( 601 , tr("Translations")               ) ;
  DockingMenu  ( mm   )                               ;
  mm . setFont ( plan )                               ;
  aa = mm.exec()                                      ;
  if (IsNull(aa)) return true                         ;
  if (RunDocking(mm,aa)) return true                  ;
  switch (mm[aa])                                     {
    case 101                                          :
      emit Edit ( item->text(0) , nTreeUuid(item,0) ) ;
    break                                             ;
    case 201                                          :
      New  ( )                                        ;
    break                                             ;
    case 501                                          :
      List ( )                                        ;
    break                                             ;
    case 601                                          :
      Uuids = itemUuids ( 0                     )     ;
      emit Translations ( windowTitle() , Uuids )     ;
    break                                             ;
    default                                           :
    break                                             ;
  }                                                   ;
  return true                                         ;
}
