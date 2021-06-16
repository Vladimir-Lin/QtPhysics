#include <physics.h>

N::ParticleLists:: ParticleLists ( QWidget * parent , Plan * p )
                 : TreeDock      (           parent ,        p )
{
  Configure ( ) ;
}

N::ParticleLists::~ParticleLists(void)
{
}

void N::ParticleLists::HeaderMenu(QHeaderView * view,QPoint pos)
{
}

void N::ParticleLists::Configure(void)
{
  installHeader                (                            ) ;
  NewTreeWidgetItem            ( head                       ) ;
  head -> setText              ( 0 , tr("Name"  )           ) ;
  head -> setText              ( 1 , tr("Symbol")           ) ;
  for (int i=0;i<2;i++)                                       {
    head -> setTextAlignment   ( i , Qt::AlignCenter        ) ;
  }                                                           ;
  setWindowTitle               ( tr("Elementary particles") ) ;
  setDragDropMode              ( NoDragDrop                 ) ;
  setRootIsDecorated           ( false                      ) ;
  setAlternatingRowColors      ( true                       ) ;
  setSelectionMode             ( SingleSelection            ) ;
  setColumnCount               ( 2                          ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded      ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded      ) ;
  plan -> setFont              ( this                       ) ;
  assignHeaderItems            ( head                       ) ;
  /////////////////////////////////////////////////////////////
  MountClicked                 ( 2                          ) ;
}

bool N::ParticleLists::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true                ) ;
  DisableAllActions (                                    ) ;
  AssignAction      ( Label        , windowTitle     ( ) ) ;
  LinkAction        ( Refresh      , startup         ( ) ) ;
  LinkAction        ( Copy         , CopyToClipboard ( ) ) ;
  LinkAction        ( Font         , modifyFont      ( ) ) ;
  LinkAction        ( Export       , Export          ( ) ) ;
  return true                                              ;
}

void N::ParticleLists::run(int Type,ThreadData * Data)
{
  switch (Type) {
    case 1001   :
      List ( )  ;
    break       ;
  }             ;
}

bool N::ParticleLists::startup(void)
{
  start ( 1001 ) ;
  return true    ;
}

void N::ParticleLists::List(void)
{
  clear              (                          ) ;
  DocumentManager DM ( plan                     ) ;
  EnterSQL           ( SC , plan->sql           ) ;
    QString Q                                     ;
    UUIDs Uuids = SC.Uuids(PlanTable(Particles)   ,
                    "uuid","order by id asc"  )   ;
    SUID  uuid                                    ;
    foreach (uuid,Uuids)                          {
      NewTreeWidgetItem(item)                     ;
      QString name = DM.Name(SC,uuid,vLanguageId) ;
      QString symbol                              ;
      Q = SC.sql.SelectFrom                       (
            "symbol"                              ,
            PlanTable(Particles)                  ,
            SC.WhereUuid(uuid)                  ) ;
      if (SC.Fetch(Q)) symbol = SC.String(0)      ;
      item->setData(0,Qt::UserRole,uuid)          ;
      item->setText(0,name             )          ;
      item->setText(1,symbol           )          ;
      addTopLevelItem(item)                       ;
    }                                             ;
  LeaveSQL         ( SC , plan->sql           )   ;
  Alert            ( Done                     )   ;
  reportItems      (                          )   ;
  emit AutoFit     (                          )   ;
}

void N::ParticleLists::modifyFont(void)
{
  bool  okay = false                         ;
  QFont mf   = font()                        ;
  mf = QFontDialog::getFont(&okay,mf)        ;
  if (!okay) return                          ;
  for (int i=0;i<topLevelItemCount();i++)    {
    QTreeWidgetItem * item = topLevelItem(i) ;
    item->setFont(1,mf)                      ;
  }                                          ;
}

void N::ParticleLists::Export(void)
{
  QString filename = QFileDialog::getSaveFileName (
                       this                       ,
                       tr("Save particles")       ,
                       plan->Temporary("")        ,
                       tr("Text file (*.txt)")  ) ;
  if (filename.length()<=0) return                ;
  QFile F(filename)                               ;
  if (!F.open(QIODevice::WriteOnly)) return       ;
  for (int i=0;i<topLevelItemCount();i++)         {
    QTreeWidgetItem * item  = topLevelItem(i)     ;
    QString name   = item->text(0)                ;
    QString symbol = item->text(1)                ;
    QString report                                ;
    report = tr("%1 %2\n"                         )
            .arg(name                             )
            .arg(symbol                         ) ;
    F.write(report.toUtf8())                      ;
  }                                               ;
  F.close()                                       ;
  Alert ( Done )                                  ;
}

bool N::ParticleLists::Menu(QPoint pos)
{ Q_UNUSED       ( pos  )            ;
  QAction   * aa                     ;
  MenuManager mm ( this )            ;
  DockingMenu    ( mm   )            ;
  mm . setFont   ( plan )            ;
  aa = mm . exec (      )            ;
  if (IsNull(aa)) return true        ;
  if (RunDocking(mm,aa)) return true ;
  return true                        ;
}

void N::ParticleLists::doubleClicked(QTreeWidgetItem * item,int column)
{
  QLineEdit * line                         ;
  switch (column)                          {
    case 0                                 :
      line = setLineEdit                   (
               item,column                 ,
               SIGNAL(returnPressed  ())   ,
               SLOT  (nameFinished   ()) ) ;
      line->setFocus( Qt::TabFocusReason ) ;
    break                                  ;
    case 1                                 :
      line = setLineEdit                   (
               item,column                 ,
               SIGNAL(returnPressed  ())   ,
               SLOT  (symbolFinished ()) ) ;
      line->setFocus( Qt::TabFocusReason ) ;
    break                                  ;
  }                                        ;
}

void N::ParticleLists::nameFinished(void)
{
  if (IsNull(ItemEditing)) return                  ;
  QLineEdit * line = Casting(QLineEdit,ItemWidget) ;
  if (IsNull(line)) return                         ;
  SUID    uuid = nTreeUuid(ItemEditing,0)          ;
  QString name = line->text()                      ;
  DocumentManager DM ( plan )                      ;
  EnterSQL         ( SC , plan->sql          )     ;
    DM.assureName  ( SC,uuid,vLanguageId,name)     ;
    ItemEditing->setText(0,name)                   ;
  LeaveSQL         ( SC , plan->sql          )     ;
  removeOldItem    ( true , 0                )     ;
  Alert            ( Done                        ) ;
}

void N::ParticleLists::symbolFinished(void)
{
  if (IsNull(ItemEditing)) return                  ;
  QLineEdit * line = Casting(QLineEdit,ItemWidget) ;
  if (IsNull(line)) return                         ;
  SUID    uuid   = nTreeUuid(ItemEditing,0)        ;
  QString symbol = line->text()                    ;
  EnterSQL         ( SC , plan->sql          )     ;
    QString Q                                      ;
    Q = SC.sql.Update(PlanTable(Particles)         ,
                      "where uuid = :UUID"         ,
                      1,"symbol"                 ) ;
    SC . Prepare ( Q                             ) ;
    SC . Bind    ( "uuid"   , uuid               ) ;
    SC . Bind    ( "symbol" , symbol.toUtf8()    ) ;
    SC . Exec    (                               ) ;
    ItemEditing->setText(1,symbol)                 ;
  LeaveSQL         ( SC , plan->sql              ) ;
  removeOldItem    ( true , 0                    ) ;
  Alert            ( Done                        ) ;
}
