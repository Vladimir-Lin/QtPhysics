#include <physics.h>

N::StarsEditor:: StarsEditor ( QWidget * parent , Plan * p                  )
               : TreeDock    (           parent ,        p                  )
               , Group       ( 0 , Types::Celestial , Groups::Subordination )
               , GroupItems  ( p                                            )
{
  Configure() ;
}

N::StarsEditor::~StarsEditor(void)
{
}

QMimeData * N::StarsEditor::dragMime (void)
{
  QMimeData * mime = standardMime("star")                    ;
  nKickOut ( IsNull(mime) , NULL )                           ;
  QImage image = windowIcon().pixmap(QSize(32,32)).toImage() ;
  mime -> setImageData(image)                                ;
  return mime                                                ;
}

bool N::StarsEditor::hasItem(void)
{
  UUIDs Uuids = selectedUuids(0) ;
  return ( Uuids.count() > 0 )   ;
}

bool N::StarsEditor::startDrag(QMouseEvent * event)
{
  QTreeWidgetItem * atItem = itemAt(event->pos())           ;
  nKickOut ( IsNull(atItem) , false )                       ;
  nKickOut (!IsMask(event->buttons(),Qt::LeftButton),false) ;
  dragPoint = event->pos()                                  ;
  nKickOut (!atItem->isSelected(),false)                    ;
  nKickOut (!PassDragDrop,true)                             ;
  return true                                               ;
}

bool N::StarsEditor::fetchDrag(QMouseEvent * event)
{
  nKickOut ( !IsMask(event->buttons(),Qt::LeftButton) , false) ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

void N::StarsEditor::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::StarsEditor::finishDrag(QMouseEvent * event)
{
  return true ;
}

bool N::StarsEditor::acceptDrop(QWidget * source,const QMimeData * mime)
{
  nKickOut ( nEqual(source,this) , false ) ;
  return dropHandler(mime)                 ;
}

bool N::StarsEditor::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  Alert ( Action ) ;
  return true      ;
}

void N::StarsEditor::Configure(void)
{
  t2                 = Types::Star                                    ;
  dockingOrientation = Qt::Vertical                                   ;
  /////////////////////////////////////////////////////////////////////
  LimitValues [  0 ] =  0                                             ;
  LimitValues [  1 ] = 42                                             ;
  LimitValues [  2 ] =  0                                             ;
  LimitValues [ 11 ] = Qt::AscendingOrder                             ;
  /////////////////////////////////////////////////////////////////////
  NewTreeWidgetItem            ( head                               ) ;
  head -> setText              ( 0 , tr("Name")                     ) ;
  head -> setText              ( 1 , tr("Type")                     ) ;
  setWindowTitle               ( tr("Star database")                ) ;
  setWindowIcon                ( QIcon(":/images/astrophysics.png") ) ;
  setDragDropMode              ( DragDrop                           ) ;
  setRootIsDecorated           ( false                              ) ;
  setAlternatingRowColors      ( true                               ) ;
  setSelectionMode             ( ExtendedSelection                  ) ;
  setColumnCount               ( 2                                  ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded              ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded              ) ;
  assignHeaderItems            ( head                               ) ;
  plan -> setFont              ( this                               ) ;
  MountClicked                 ( 2                                  ) ;
  setDropFlag                  ( DropStar , true                    ) ;
}

bool N::StarsEditor::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true                ) ;
  DisableAllActions (                                    ) ;
  AssignAction      ( Label        , windowTitle     ( ) ) ;
  LinkAction        ( Refresh      , startup         ( ) ) ;
  LinkAction        ( Copy         , CopyToClipboard ( ) ) ;
  LinkAction        ( Paste        , Paste           ( ) ) ;
  LinkAction        ( Insert       , Insert          ( ) ) ;
  if ( isFirst() || isSecond() )                           {
    LinkAction      ( Delete       , Delete          ( ) ) ;
  }                                                        ;
  return true                                              ;
}

bool N::StarsEditor::startup(void)
{
  clear (       ) ;
  start ( 10001 ) ;
  return true     ;
}

void N::StarsEditor::List(void)
{
  EnterSQL ( SC , plan->sql )                              ;
    QString Q                                              ;
    UUIDs Uuids                                            ;
    SUID  uu                                               ;
    TypeUuids = SC . Uuids                                 (
                  PlanTable(Celestials)                    ,
                  "uuid"                                   ,
                  SC.OrderByAsc("id")                    ) ;
    ObtainNames ( SC                                       ,
                  vLanguageId                              ,
                  TypeUuids                                ,
                  StarTypes                              ) ;
    if (isFirst ())                                        {
      if ( Types::Celestial == t1 )                        {
        Uuids = SC . Uuids                                 (
                  PlanTable(Stars)                         ,
                  "uuid"                                   ,
                  QString ( "where `type` = %1 %2"         )
                 . arg ( first                             )
                 . arg ( SC . OrderBy                      (
                         "id"                              ,
                         (Qt::SortOrder)LimitValues[11]))) ;
      } else                                               {
        Uuids = Subordination                              (
                  SC                                       ,
                  first                                    ,
                  t1                                       ,
                  t2                                       ,
                  relation                                 ,
                  SC . OrderBy                             (
                    "position"                             ,
                    (Qt::SortOrder)LimitValues[11]     ) ) ;
      }                                                    ;
    } else
    if (isSecond())                                        {
      Uuids = GetOwners                                    (
                SC                                         ,
                second                                     ,
                Types::Star                                ,
                t2                                         ,
                relation                                   ,
                SC . OrderBy                               (
                  "id"                                     ,
                  (Qt::SortOrder)LimitValues[11]       ) ) ;
    } else                                                 {
      Uuids = SC . Uuids                                   (
                PlanTable(Stars)                           ,
                "uuid"                                     ,
                SC . OrderBy                               (
                  "id"                                     ,
                  (Qt::SortOrder)LimitValues[11] )       ) ;
    }                                                      ;
    ////////////////////////////////////////////////////////
    FixUuids ( Uuids )                                     ;
    ////////////////////////////////////////////////////////
    foreach (uu,Uuids)                                     {
      SUID tt = 0                                          ;
      Q = SC . sql . SelectFrom                            (
            "type"                                         ,
            PlanTable(Stars)                               ,
            SC.WhereUuid(uu)                             ) ;
      if (SC.Fetch(Q))                                     {
        QString n                                          ;
        tt = SC . Uuid(0)                                  ;
        n  = SC . getName                                  (
               PlanTable(Names)                            ,
               "uuid"                                      ,
               vLanguageId                                 ,
               uu                                 )        ;
        NewTreeWidgetItem ( it                    )        ;
        it -> setData     ( 0 , Qt::UserRole , uu )        ;
        it -> setData     ( 1 , Qt::UserRole , tt )        ;
        it -> setText     ( 0 , n                 )        ;
        it -> setText     ( 1 , StarTypes [ tt ]  )        ;
        addTopLevelItem   ( it                    )        ;
      }                                                    ;
    }                                                      ;
  LeaveSQL     ( SC , plan->sql )                          ;
  emit AutoFit (                )                          ;
  Alert        ( Done           )                          ;
}

void N::StarsEditor::Append(SUID type,QStringList & Names)
{
  EnterSQL ( SC , plan->sql )                                     ;
    QString Q                                                     ;
    QString S                                                     ;
    SUID    star                                                  ;
    foreach ( S , Names )                                         {
      star  = SC . Unique                                         (
                PlanTable(MajorUuid)                              ,
                "uuid"                                            ,
                9004                                            ) ;
      SC . assureUuid                                             (
        PlanTable(MajorUuid)                                      ,
        star                                                      ,
        Types::Star                                             ) ;
      Q = SC . sql . InsertInto                                   (
            PlanTable(Stars)                                      ,
            2                                                     ,
            "uuid"                                                ,
            "type"                                              ) ;
      SC . Prepare ( Q                 )                          ;
      SC . Bind    ( "uuid" , star     )                          ;
      SC . Bind    ( "type" , type     )                          ;
      SC . Exec    (                   )                          ;
      if ( isFirst ( )  && Types::Celestial != t1  )              {
        UUIDs U                                                   ;
        U << star                                                 ;
        Join ( SC , first , t1 , Types::Star , relation , 0 , U ) ;
      } else
      if ( isSecond ( ) )                                         {
        UUIDs U                                                   ;
        U << second                                               ;
        Join ( SC , star  , Types::Star , t2 , relation , 0 , U ) ;
      }                                                           ;
      /////////////////////////////////////////////////////////////
      SC . assureName                                            (
        PlanTable(Names)                                         ,
        star                                                     ,
        vLanguageId                                              ,
        S                                                      ) ;
      ////////////////////////////////////////////////////////////
      NewTreeWidgetItem ( it                      )              ;
      it -> setData     ( 0 , Qt::UserRole , star )              ;
      it -> setData     ( 1 , Qt::UserRole , type )              ;
      it -> setText     ( 0 , S                   )              ;
      it -> setText     ( 1 , StarTypes [ type ]  )              ;
      addTopLevelItem   ( it                      )              ;
      plan -> processEvents ( )                                  ;
    }                                                            ;
  LeaveSQL     ( SC , plan->sql )                                ;
  emit AutoFit (                )                                ;
  Alert        ( Done           )                                ;
}

void N::StarsEditor::Paste(void)
{
  QString text = nClipboardText                   ;
  if (text.length()<=0) return                    ;
  QStringList S = text.split("\n")                ;
  S = File :: PurifyLines ( S )                   ;
  if (S.count()<=0) return                        ;
  /////////////////////////////////////////////////
  UuidSelection * US                              ;
  US  = new UuidSelection ( this , plan )         ;
  US -> setNames       ( TypeUuids , StarTypes  ) ;
  US -> setWindowTitle ( tr("Select star type") ) ;
  if ( QDialog::Accepted == US->exec() )          {
    SUID u = US->currentUuid()                    ;
    US   -> deleteLater   ( )                     ;
    plan -> processEvents ( )                     ;
    if (u>0)                                      {
      Append ( u , S )                            ;
    }                                             ;
    return                                        ;
  }                                               ;
  /////////////////////////////////////////////////
  QMessageBox::StandardButton sb                  ;
  QString T                                       ;
  QString M                                       ;
  T = windowTitle()                               ;
  M = tr("Append star names without type?")       ;
  sb = QMessageBox::question(this,T,M)            ;
  if ( QMessageBox::Yes != sb ) return            ;
  /////////////////////////////////////////////////
  foreach ( T , S ) if ( T.length() > 0 )         {
    NewTreeWidgetItem ( it )                      ;
    it -> setText ( 0 , T )                       ;
    addTopLevelItem ( it )                        ;
    plan -> processEvents ( )                     ;
  }                                               ;
  Alert ( Done )                                  ;
}

void N::StarsEditor::Insert(void)
{
  NewTreeWidgetItem(item)         ;
  item->setData(0,Qt::UserRole,0) ;
  item->setData(1,Qt::UserRole,0) ;
  setAlignments   ( item     )    ;
  addTopLevelItem ( item     )    ;
  scrollToItem    ( item     )    ;
  doubleClicked   ( item , 0 )    ;
}

void N::StarsEditor::Delete(void)
{
}

void N::StarsEditor::doubleClicked(QTreeWidgetItem * item,int column)
{
  QLineEdit * line                         ;
  ComboBox  * cc                           ;
  SUID        t  = nTreeUuid ( item , 1 )  ;
  switch (column)                          {
    case 0                                 :
      line = setLineEdit                   (
               item,column                 ,
               SIGNAL(returnPressed  ())   ,
               SLOT  (starFinished   ()) ) ;
      line->setFocus( Qt::TabFocusReason ) ;
    break                                  ;
    case 1                                 :
      cc   = (ComboBox *)setComboBox       (
               item,column                 ,
               SIGNAL(activated   (int))   ,
               SLOT  (typeFinished(int)) ) ;
      cc->appendNames(TypeUuids,StarTypes) ;
      if (t>=0) (*cc) <= t                 ;
      cc->setMaxVisibleItems(30)           ;
      cc->setEditable(true)                ;
      cc->showPopup()                      ;
    break                                  ;
  }                                        ;
}

void N::StarsEditor::typeFinished(int)
{
  if (IsNull(ItemEditing)) return                  ;
  QComboBox * cc = Casting(QComboBox,ItemWidget)   ;
  if (IsNull(cc)) return                           ;
  SUID au = N::GetUuid(cc)                         ;
  ItemEditing -> setText ( 1 , cc->currentText() ) ;
  ItemEditing -> setData ( 1 , Qt::UserRole , au ) ;
  Finalize               ( ItemEditing           ) ;
  removeOldItem          ( true , 1              ) ;
}

void N::StarsEditor::starFinished(void)
{
  if (IsNull(ItemEditing)) return                  ;
  QLineEdit * line = Casting(QLineEdit,ItemWidget) ;
  if (IsNull(line)) return                         ;
  ItemEditing -> setText ( 0 , line->text() )      ;
  Finalize      ( ItemEditing )                    ;
  removeOldItem ( false , 0   )                    ;
}

void N::StarsEditor::Finalize(QTreeWidgetItem * item)
{
  QString name = item->text(0)                                    ;
  SUID    star = nTreeUuid ( item , 0 )                           ;
  SUID    type = nTreeUuid ( item , 1 )                           ;
  if (name.length()<=0) return                                    ;
  if (type         <=0) return                                    ;
  EnterSQL ( SC , plan->sql )                                     ;
    QString Q                                                     ;
    if ( star <= 0 )                                              {
      star  = SC . Unique                                         (
                PlanTable(MajorUuid)                              ,
                "uuid"                                            ,
                9004                                            ) ;
      SC . assureUuid                                             (
        PlanTable(MajorUuid)                                      ,
        star                                                      ,
        Types::Star                                             ) ;
      Q = SC . sql . InsertInto                                   (
            PlanTable(Stars)                                      ,
            2                                                     ,
            "uuid"                                                ,
            "type"                                              ) ;
      item -> setData ( 0                                         ,
                        Qt::UserRole                              ,
                        star           )                          ;
      SC . Prepare ( Q                 )                          ;
      SC . Bind    ( "uuid" , star     )                          ;
      SC . Bind    ( "type" , type     )                          ;
      SC . Exec    (                   )                          ;
      if ( isFirst ( ) && Types::Celestial != t1 )                {
        UUIDs U                                                   ;
        U << star                                                 ;
        Join ( SC , first , t1 , Types::Star , relation , 0 , U ) ;
      } else
      if ( isSecond ( ) )                                         {
        UUIDs U                                                   ;
        U << second                                               ;
        Join ( SC , star  , Types::Star , t2 , relation , 0 , U ) ;
      }                                                           ;
    } else                                                        {
      Q = SC . sql . Update                                       (
            PlanTable(Stars)                                      ,
            SC.sql.Where(1,"uuid")                                ,
            1                                                     ,
            "type"                                              ) ;
    }                                                             ;
    ///////////////////////////////////////////////////////////////
    SC . Prepare ( Q                   )                          ;
    SC . Bind    ( "uuid" , star       )                          ;
    SC . Bind    ( "type" , type       )                          ;
    SC . Exec    (                     )                          ;
    ///////////////////////////////////////////////////////////////
    SC . assureName                                               (
      PlanTable(Names)                                            ,
      star                                                        ,
      vLanguageId                                                 ,
      name                                                      ) ;
  LeaveSQL ( SC , plan->sql )                                     ;
  Alert    ( Done           )                                     ;
}

bool N::StarsEditor::dropStars(QWidget * widget,QPointF psf,const UUIDs & Uuids)
{
  nKickOut ( widget == this , true )   ;
  //////////////////////////////////////
  QPoint pos = psf.toPoint()           ;
  QTreeWidgetItem * item = itemAt(pos) ;
  if (IsNull(item))                    {
    Alert ( Error )                    ;
    return false                       ;
  }                                    ;
  //////////////////////////////////////
  dropAction = true                    ;
  //////////////////////////////////////
  EnterSQL ( SC , plan->sql )          ;
    SUID  star = nTreeUuid(item,0)     ;
    UUIDs U    = Uuids                 ;
    Join ( SC                          ,
           star                        ,
           Types::Star                 ,
           Types::Star                 ,
           Groups::Contains            ,
           0                           ,
           U                         ) ;
  LeaveSQL ( SC , plan->sql )          ;
  //////////////////////////////////////
  Alert    ( Done           )          ;
  dropAction = false                   ;
  return true                          ;
}

bool N::StarsEditor::dropPictures (QWidget * widget,QPointF psf,const UUIDs & Uuids)
{
  nKickOut ( widget == this , true )   ;
  //////////////////////////////////////
  QPoint pos = psf.toPoint()           ;
  QTreeWidgetItem * item = itemAt(pos) ;
  if (IsNull(item))                    {
    Alert ( Error )                    ;
    return false                       ;
  }                                    ;
  //////////////////////////////////////
  dropAction = true                    ;
  //////////////////////////////////////
  EnterSQL ( SC , plan->sql )          ;
    SUID  star = nTreeUuid(item,0)     ;
    UUIDs U    = Uuids                 ;
    Join ( SC                          ,
           star                        ,
           Types  :: Star              ,
           Types  :: Picture           ,
           Groups :: Subordination     ,
           0                           ,
           U                         ) ;
  LeaveSQL ( SC , plan->sql )          ;
  //////////////////////////////////////
  Alert    ( Done           )          ;
  dropAction = false                   ;
  return true                          ;
}

void N::StarsEditor::run(int Type,ThreadData * data)
{ Q_UNUSED ( data ) ;
  switch   ( Type ) {
    case 10001      :
      List ( )      ;
    break           ;
  }                 ;
}

bool N::StarsEditor::RunCommand(VarArgs & arguments)
{
  return true ;
}

bool N::StarsEditor::Menu(QPoint pos)
{
  QTreeWidgetItem * item = itemAt ( pos )                       ;
  MenuManager       mm ( this )                                 ;
  QAction         * aa                                          ;
  UUIDs             Uuids                                       ;
  int               StartId  = 0                                ;
  int               PageSize = 0                                ;
  ///////////////////////////////////////////////////////////////
  PageMenu                                                      (
    mm                                                          ,
    tr("Total %1 stars")                                        ,
    StartId                                                     ,
    PageSize                                                  ) ;
  ///////////////////////////////////////////////////////////////
  if (NotNull(item))                                            {
    mm.add(101,tr("Edit"))                                      ;
  }                                                             ;
  mm.add(201,tr("Insert" ))                                     ;
  mm.addSeparator()                                             ;
  mm.add(501,tr("Refresh"))                                     ;
  mm.addSeparator()                                             ;
  if (NotNull(item))                                            {
    mm.add(401,tr("Star gallery"))                              ;
  }                                                             ;
  mm . add ( 601 , tr("Translations")                         ) ;
  DockingMenu  ( mm   )                                         ;
  mm . setFont ( plan )                                         ;
  aa = mm.exec()                                                ;
  if (IsNull(aa))                                               {
    PageChanged(StartId,PageSize)                               ;
    return false                                                ;
  }                                                             ;
  if (RunDocking(mm,aa)) return true                            ;
  switch (mm[aa])                                               {
    case 101                                                    :
      emit Edit ( item->text(0) , nTreeUuid(item,0) )           ;
    break                                                       ;
    case 201                                                    :
      Insert  ( )                                               ;
    break                                                       ;
    case 401                                                    :
      if (NotNull(item))                                        {
        SUID u = nTreeUuid ( item , 0 )                         ;
        if (u>0)                                                {
          emit Gallery ( item->text(0) , u , Types::Star )      ;
        }                                                       ;
      }                                                         ;
    break                                                       ;
    case 501                                                    :
      startup ( )                                               ;
    break                                                       ;
    case 601                                                    :
      Uuids = itemUuids ( 0                     )               ;
      emit Translations ( windowTitle() , Uuids )               ;
    break                                                       ;
    default                                                     :
    break                                                       ;
  }                                                             ;
  RunPageMenu ( mm , aa , StartId , PageSize )                  ;
  PageChanged ( StartId , PageSize           )                  ;
  return true                                                   ;
}
