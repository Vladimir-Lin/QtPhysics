#include <physics.h>

N::CompositeParticles:: CompositeParticles (QWidget * parent,Plan * p)
                      : TreeWidget         (          parent,       p)
{
  Configure ( ) ;
}

N::CompositeParticles::~CompositeParticles(void)
{
}

QSize N::CompositeParticles::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 240 , 320 ) ) ;
}

void N::CompositeParticles::Configure(void)
{
  setWindowTitle               ( tr("Composite particles") ) ;
  ////////////////////////////////////////////////////////////
  NewTreeWidgetItem            ( head                      ) ;
  head -> setText              (  0 , tr("Name"  )         ) ;
  head -> setText              (  1 , tr("Symbol")         ) ;
  ////////////////////////////////////////////////////////////
  setDragDropMode              ( NoDragDrop                ) ;
  setRootIsDecorated           ( false                     ) ;
  setAlternatingRowColors      ( true                      ) ;
  setSelectionMode             ( SingleSelection           ) ;
  setColumnCount               ( 2                         ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded     ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded     ) ;
  plan -> setFont              ( this                      ) ;
  assignHeaderItems            ( head                      ) ;
  ////////////////////////////////////////////////////////////
  MountClicked                 ( 2                         ) ;
}

bool N::CompositeParticles::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true            ) ;
  DisableAllActions (                                ) ;
  AssignAction      ( Label        , windowTitle ( ) ) ;
  LinkAction        ( Insert       , New         ( ) ) ;
  return true                                          ;
}

void N::CompositeParticles::run(int Type,ThreadData * data)
{ Q_UNUSED ( data ) ;
  switch ( Type )   {
    case 10001      :
      List ( )      ;
    break           ;
  }                 ;
}

bool N::CompositeParticles::startup(void)
{
  clear (       ) ;
  start ( 10001 ) ;
  return true     ;
}

void N::CompositeParticles::List(void)
{
  SqlConnection SC ( plan->sql )            ;
  if (SC.open("CompositeParticles","List")) {
    QString Q                               ;
    UUIDs   U                               ;
    SUID    u                               ;
    U = SC . Uuids                          (
          PlanTable(Composite)              ,
          "uuid"                            ,
          SC.OrderByAsc("id")             ) ;
    foreach (u,U)                           {
      QString n = SC.getName                (
                    PlanTable(Names)        ,
                    "uuid"                  ,
                    vLanguageId             ,
                    u                     ) ;
      QString s                             ;
      Q = SC.sql.SelectFrom                 (
            "symbol"                        ,
            PlanTable(Composite)            ,
            SC.WhereUuid(u)               ) ;
      if (SC.Fetch(Q)) s = SC.String(0)     ;
      NewTreeWidgetItem ( it )              ;
      it -> setData ( 0 , Qt::UserRole,u  ) ;
      it -> setText ( 0 , n               ) ;
      it -> setText ( 1 , s               ) ;
      addTopLevelItem ( it )                ;
    }                                       ;
    SC.close()                              ;
  }                                         ;
  SC.remove()                               ;
  Alert ( Done )                            ;
}

void N::CompositeParticles::New(void)
{
  NewTreeWidgetItem ( IT )      ;
  IT->setData(0,Qt::UserRole,0) ;
  setAlignments   ( IT     )    ;
  addTopLevelItem ( IT     )    ;
  scrollToItem    ( IT     )    ;
  doubleClicked   ( IT , 0 )    ;
}

void N::CompositeParticles::doubleClicked(QTreeWidgetItem * item,int column)
{
  nDropOut ( column > 1 )                 ;
  QLineEdit * line                        ;
  removeOldItem()                         ;
  switch (column)                         {
    case 0                                :
      line = setLineEdit                  (
               item                       ,
               column                     ,
               SIGNAL(returnPressed())    ,
               SLOT  (nameFinished ()) )  ;
      line->setFocus(Qt::TabFocusReason)  ;
    break                                 ;
    case 1                                :
      line = setLineEdit                  (
               item                       ,
               column                     ,
               SIGNAL(returnPressed ())   ,
               SLOT  (symbolFinished()) ) ;
      line->setFocus(Qt::TabFocusReason)  ;
    break                                 ;
  }                                       ;
}

void N::CompositeParticles::nameFinished(void)
{
  nDropOut ( IsNull(ItemEditing) )                     ;
  QLineEdit * line = Casting (QLineEdit  ,ItemWidget)  ;
  if (IsNull(line)) return                             ;
  ItemEditing -> setText ( ItemColumn , line->text() ) ;
  //////////////////////////////////////////////////////
  SUID    u      = nTreeUuid(ItemEditing,0)            ;
  QString name   = line->text()                        ;
  int     column = ItemColumn                          ;
  EnterSQL ( SC , plan->sql )                          ;
    if ( column == 0 )                                 {
      QString Q                                        ;
      if ( u <=0 )                                     {
        u = SC.Unique                                  (
              PlanTable(Composite)                     ,
              "uuid"                                   ,
              74412002                               ) ;
        SC . assureUuid                                (
          PlanTable(MajorUuid)                         ,
          u                                            ,
          Types::Composite                           ) ;
        SC . insertUuid                                (
          PlanTable(Composite)                         ,
          u                                            ,
          "uuid"                                     ) ;
        ItemEditing->setData(0,Qt::UserRole,u)         ;
      }                                                ;
      if (u>0)                                         {
        SC . assureName                                (
          PlanTable(Names)                             ,
          u                                            ,
          vLanguageId                                  ,
          name                                       ) ;
        ItemEditing->setText(0,name)                   ;
      }                                                ;
    }                                                  ;
  LeaveSQL ( SC , plan->sql )                          ;
  removeOldItem ( )                                    ;
  Alert    ( Done           )                          ;
}

void N::CompositeParticles::symbolFinished(void)
{
  nDropOut ( IsNull(ItemEditing) )                      ;
  QLineEdit * line = Casting  (QLineEdit  ,ItemWidget)  ;
  if (IsNull(line)) return                              ;
  ItemEditing -> setText ( ItemColumn , line->text() )  ;
  ///////////////////////////////////////////////////////
  SUID    u      = nTreeUuid(ItemEditing,0)             ;
  QString name   = line->text()                         ;
  int     column = ItemColumn                           ;
  EnterSQL ( SC , plan->sql )                           ;
    if ( column == 1 && u > 0 )                         {
      QString Q                                         ;
      Q = SC.sql.Update                                 (
            PlanTable(Composite)                        ,
            SC.WhereUuid(u)                             ,
            1                                           ,
            "symbol"                                  ) ;
      SC . Prepare ( Q                        )         ;
      SC . Bind    ( "symbol" , name.toUtf8() )         ;
      if ( SC . Exec (  ) )                             {
        ItemEditing->setText(1,name)                    ;
      }                                                 ;
    }                                                   ;
  LeaveSQL ( SC , plan->sql )                           ;
  removeOldItem ( )                                     ;
  Alert    ( Done           )                           ;
}

bool N::CompositeParticles::Menu(QPoint pos)
{
  nScopedMenu ( mm , this )                              ;
  QAction * aa                                           ;
  QTreeWidgetItem * it = itemAt(pos)                     ;
  mm . add ( 101 , tr ( "New"            ) )             ;
  if (NotNull(it))                                       {
    mm . add ( 201 , tr ( "Bound states" ) )             ;
    mm . add ( 202 , tr ( "Properties"   ) )             ;
    mm . add ( 203 , tr ( "Symbols"      ) )             ;
  }                                                      ;
  mm . addSeparator ( )                                  ;
  if (topLevelItemCount()>0)                             {
    mm . add   ( 501 , tr("Multilingual translations") ) ;
  }                                                      ;
  mm . setFont ( plan )                                  ;
  aa = mm . exec ( )                                     ;
  if ( IsNull ( aa ) ) return true                       ;
  switch ( mm[aa] )                                      {
    case 101                                             :
      New ( )                                            ;
    break                                                ;
    case 201                                             :
      emit Bounded    ( it->text(0) , nTreeUuid(it,0)  ) ;
    break                                                ;
    case 202                                             :
      emit Properties ( it->text(0) , nTreeUuid(it,0)  ) ;
    break                                                ;
    case 203                                             :
      emit Symbols    ( it->text(0) , nTreeUuid(it,0)  ) ;
    break                                                ;
    case 501                                             :
      if (topLevelItemCount()>0)                         {
        UUIDs U = itemUuids ( 0 )                        ;
        emit Translations ( windowTitle() , U )          ;
      }                                                  ;
    break                                                ;
  }                                                      ;
  return true                                            ;
}
