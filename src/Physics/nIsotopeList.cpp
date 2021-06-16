#include <physics.h>

#define NucleusTable PlanTable(Nucleus)

N::IsotopeList:: IsotopeList ( QWidget * parent , Plan * p )
               : TreeWidget  (           parent ,        p )
               , Object      ( 0 , Types::Nucleus          )
{
  Configure ( ) ;
}

N::IsotopeList::~IsotopeList(void)
{
}

QSize N::IsotopeList::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 320 , 320 ) ) ;
}

void N::IsotopeList::Configure(void)
{
  setWindowTitle               ( tr("Isotope lists")   ) ;
  ////////////////////////////////////////////////////////
  NewTreeWidgetItem            ( head                  ) ;
  head -> setText              (  0 , tr("Isotope")    ) ;
  head -> setText              (  1 , tr("Nucleon")    ) ;
  ////////////////////////////////////////////////////////
  setDragDropMode              ( NoDragDrop            ) ;
  setRootIsDecorated           ( false                 ) ;
  setAlternatingRowColors      ( true                  ) ;
  setSelectionMode             ( SingleSelection       ) ;
  setColumnCount               ( 2                     ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded ) ;
  plan -> setFont              ( this                  ) ;
  assignHeaderItems            ( head                  ) ;
  ////////////////////////////////////////////////////////
  MountClicked                 ( 2                     ) ;
}

bool N::IsotopeList::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true                ) ;
  DisableAllActions (                                    ) ;
  AssignAction      ( Label        , windowTitle     ( ) ) ;
  LinkAction        ( Refresh      , startup         ( ) ) ;
  LinkAction        ( Insert       , New             ( ) ) ;
  LinkAction        ( Delete       , Delete          ( ) ) ;
  LinkAction        ( Export       , Export          ( ) ) ;
  LinkAction        ( Font         , setFont         ( ) ) ;
  LinkAction        ( Copy         , CopyToClipboard ( ) ) ;
  LinkAction        ( SelectNone   , SelectNone      ( ) ) ;
  LinkAction        ( SelectAll    , SelectAll       ( ) ) ;
  return true                                              ;
}

void N::IsotopeList::run(int Type,ThreadData * data)
{ Q_UNUSED ( data ) ;
  switch   ( Type ) {
    case 10001      :
      List ( )      ;
    break           ;
  }                 ;
}

bool N::IsotopeList::startup(void)
{
  clear (       ) ;
  start ( 10001 ) ;
  return true     ;
}

void N::IsotopeList::List(void)
{
  GroupItems    GI ( plan        )                 ;
  SqlConnection SC ( plan -> sql )                 ;
  if (SC.open("NucleusList","List"))               {
    QString Q                                      ;
    UUIDs   U                                      ;
    SUID    u                                      ;
    SUID    z                                      ;
    Nucleons = SC . Uuids                          (
                 NucleusTable                      ,
                 "uuid"                            ,
                 SC.OrderByAsc("id")             ) ;
    foreach (u,Nucleons)                           {
      Q = SC . getName                             (
            PlanTable(Names)                       ,
            "uuid"                                 ,
            vLanguageId                            ,
            u                                    ) ;
      Nuclecus [ u ] = Q                           ;
    }                                              ;
    if (ObjectUuid()>0)                            {
      u = ObjectUuid()                             ;
      U = GI . Subordination                       (
            SC                                     ,
            u                                      ,
            Types  :: Nucleus                      ,
            Types  :: Nucleus                      ,
            Groups :: Contains                     ,
            SC.OrderByAsc("position")            ) ;
      foreach (z,U)                                {
        NewTreeWidgetItem ( it )                   ;
        it -> setData   ( 0 , Qt::UserRole , z )   ;
        it -> setData   ( 1 , Qt::UserRole , u )   ;
        it -> setText   ( 0 , Nuclecus[z]      )   ;
        it -> setText   ( 1 , Nuclecus[u]      )   ;
        addTopLevelItem ( it                   )   ;
      }                                            ;
    } else                                         {
      foreach (u,Nucleons)                         {
        U = GI . Subordination                     (
              SC                                   ,
              u                                    ,
              Types  :: Nucleus                    ,
              Types  :: Nucleus                    ,
              Groups :: Contains                   ,
              SC.OrderByAsc("position")          ) ;
        foreach (z,U)                              {
          NewTreeWidgetItem ( it )                 ;
          it -> setData   ( 0 , Qt::UserRole , z ) ;
          it -> setData   ( 1 , Qt::UserRole , u ) ;
          it -> setText   ( 0 , Nuclecus[z]      ) ;
          it -> setText   ( 1 , Nuclecus[u]      ) ;
          addTopLevelItem ( it                   ) ;
        }                                          ;
      }                                            ;
    }                                              ;
    SC.close()                                     ;
  }                                                ;
  SC.remove()                                      ;
  Alert ( Done )                                   ;
}

void N::IsotopeList::New(void)
{
  removeOldItem     ( true , 0             ) ;
  NewTreeWidgetItem ( it                   ) ;
  it -> setData     ( 0 , Qt::UserRole , 0 ) ;
  addTopLevelItem   ( it                   ) ;
  DoProcessEvents                            ;
  scrollToItem      ( it                   ) ;
  setCurrentItem    ( it                   ) ;
  doubleClicked     ( it , 0               ) ;
}

void N::IsotopeList::Delete(void)
{
  QTreeWidgetItem * it = currentItem () ;
  nDropOut         ( IsNull(it) )       ;
  int index = indexOfTopLevelItem(it)   ;
  nDropOut         ( index < 0  )       ;
  ///////////////////////////////////////
  SUID u = nTreeUuid ( it , 0 )         ;
  SUID z = nTreeUuid ( it , 1 )         ;
  if (u>0 && z>0)                       {
    GroupItems GI ( plan              ) ;
    EnterSQL      ( SC , plan->sql    ) ;
      UUIDs   U                         ;
      U << u                            ;
      GI . Detach                       (
        SC                              ,
        z                               ,
        Types  :: Nucleus               ,
        Types  :: Nucleus               ,
        Groups :: Contains              ,
        U                             ) ;
      U  . clear ( )                    ;
      U << z                            ;
      GI . Detach                       (
        SC                              ,
        u                               ,
        Types::Nucleus                  ,
        Types::Nucleus                  ,
        Groups::Originate               ,
        U                             ) ;
    LeaveSQL      ( SC , plan->sql    ) ;
  }                                     ;
  ///////////////////////////////////////
  takeTopLevelItem ( index      )       ;
}

bool N::IsotopeList::Menu(QPoint pos)
{
  QAction   * aa                        ;
  MenuManager mm ( this )               ;
  QTreeWidgetItem * it = itemAt(pos)    ;
  ///////////////////////////////////////
  mm . add ( 101 , tr("Refresh" ) )     ;
  mm . add ( 102 , tr("Export"  ) )     ;
  mm . addSeparator ( )                 ;
  mm . add ( 103 , tr("Add"     ) )     ;
  if (NotNull(it))                      {
    mm . add ( 104 , tr("Delete") )     ;
  }                                     ;
  ///////////////////////////////////////
  mm . add ( 501 , tr("set font"    ) ) ;
  mm . add ( 502 , tr("set language") ) ;
  mm . setFont   ( plan )               ;
  aa = mm . exec (      )               ;
  if (IsNull(aa)) return true           ;
  ///////////////////////////////////////
  switch (mm[aa])                       {
    case 101                            :
      startup        (      )           ;
    break                               ;
    case 102                            :
      Export         (      )           ;
    break                               ;
    case 103                            :
      New            (      )           ;
    break                               ;
    case 104                            :
      Delete         (      )           ;
    break                               ;
    case 501                            :
      setFont        (      )           ;
    break                               ;
    case 502                            :
      assignLanguage ( this )           ;
      startup        (      )           ;
    break                               ;
  }                                     ;
  ///////////////////////////////////////
  return true                           ;
}

void N::IsotopeList::doubleClicked(QTreeWidgetItem * item,int column)
{
  nDropOut ( column > 1 )                       ;
  QComboBox * cb                                ;
  switch (column)                               {
    case 0                                      :
      cb = setComboBox                          (
             item                               ,
             column                             ,
             SIGNAL(activated      (int))       ,
             SLOT  (isotopeFinished(int))     ) ;
    break                                       ;
    case 1                                      :
      cb = setComboBox                          (
             item                               ,
             column                             ,
             SIGNAL(activated      (int))       ,
             SLOT  (nucleonFinished(int))     ) ;
    break                                       ;
  }                                             ;
  ///////////////////////////////////////////////
  cb -> blockSignals ( true  )                  ;
  SUID u                                        ;
  foreach (u,Nucleons)                          {
    cb -> addItem ( Nuclecus [ u ] , u )        ;
  }                                             ;
  cb -> setInsertPolicy ( QComboBox::NoInsert ) ;
  cb -> setEditable  ( true  )                  ;
  cb -> blockSignals ( false )                  ;
  ///////////////////////////////////////////////
  cb -> showPopup    (       )                  ;
}

void N::IsotopeList::isotopeFinished(int index)
{
  if (IsNull(ItemEditing)) return                   ;
  QComboBox * cb = Casting(QComboBox,ItemWidget)    ;
  if (IsNull(cb)) return                            ;
  SUID    u = cb->itemData(index).toULongLong()     ;
  QString n = cb->currentText()                     ;
  SUID    z = nTreeUuid(ItemEditing,1)              ;
  ItemEditing -> setData ( 0 , Qt::UserRole , u   ) ;
  ItemEditing -> setText ( 0 , n                  ) ;
  if (ObjectUuid()>0 && z<=0)                       {
    z = ObjectUuid()                                ;
    ItemEditing -> setData ( 1 , Qt::UserRole , z ) ;
    ItemEditing -> setText ( 1 , Nuclecus [ z ]   ) ;
  }                                                 ;
  Save                   ( ItemEditing            ) ;
  removeOldItem          ( true , 0               ) ;
}

void N::IsotopeList::nucleonFinished(int index)
{
  if (IsNull(ItemEditing)) return                 ;
  QComboBox * cb = Casting(QComboBox,ItemWidget)  ;
  if (IsNull(cb)) return                          ;
  SUID    u = cb->itemData(index).toULongLong()   ;
  QString n = cb->currentText()                   ;
  ItemEditing -> setData ( 1 , Qt::UserRole , u ) ;
  ItemEditing -> setText ( 1 , n                ) ;
  Save                   ( ItemEditing          ) ;
  removeOldItem          ( true , 0             ) ;
}

void N::IsotopeList::Save(QTreeWidgetItem * item)
{
  SUID u = nTreeUuid ( item , 0 )  ;
  SUID z = nTreeUuid ( item , 1 )  ;
  if (u<=0) return                 ;
  if (z<=0) return                 ;
  GroupItems GI ( plan           ) ;
  EnterSQL      ( SC , plan->sql ) ;
    UUIDs U                        ;
    U << u                         ;
    GI . Join                      (
      SC                           ,
      z                            ,
      Types  :: Nucleus            ,
      Types  :: Nucleus            ,
      Groups :: Contains           ,
      0                            ,
      U                          ) ;
    U  . clear ( )                 ;
    U << z                         ;
    GI . Join                      (
      SC                           ,
      u                            ,
      Types  :: Nucleus            ,
      Types  :: Nucleus            ,
      Groups :: Originate          ,
      0                            ,
      U                          ) ;
  LeaveSQL      ( SC , plan->sql ) ;
  Alert         ( Done           ) ;
}

QString N::IsotopeList::toHtml(void)
{
  QString m = ""                                        ;
  TreeWidgetItems Items                                 ;
  Items << headerItem ( )                               ;
  for (int i=0;i<topLevelItemCount();i++)               {
    Items << topLevelItem ( i )                         ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  m  = "<table cellpadding=1 cellspacing=1 border=1>\n" ;
  m += "<tbody>\n"                                      ;
  ///////////////////////////////////////////////////////
  for (int i=0;i<Items.count();i++)                     {
    QTreeWidgetItem * it = Items [ i ]                  ;
    m += "<tr>\n"                                       ;
    for (int j=0;j<columnCount();j++)                   {
      m += "<td>"                                       ;
      m += it->text(j)                                  ;
      m += "</td>\n"                                    ;
    }                                                   ;
    m += "</tr>\n"                                      ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  m += "</tbody>\n"                                     ;
  m += "</table>\n"                                     ;
  ///////////////////////////////////////////////////////
  return m                                              ;
}

void N::IsotopeList::Export(void)
{
  QString filename                          ;
  filename = QFileDialog::getSaveFileName   (
                this                        ,
                tr("Export to HTML")        ,
                plan->Temporary("")         ,
                tr("HTML (*.html *.htm)") ) ;
  if (filename.length()<=0) return          ;
  ///////////////////////////////////////////
  QFile F(filename)                         ;
  if (!F.open(QIODevice::WriteOnly)) return ;
  ///////////////////////////////////////////
  QString m = toHtml()                      ;
  m . prepend ( "<html>\n"  )               ;
  m . prepend ( "<head>\n"  )               ;
  m . prepend ( "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n") ;
  m . prepend ( tr("<title>%1</title>\n").arg(windowTitle())) ;
  m . prepend ( "</head>\n" )               ;
  m . prepend ( "<body>\n"  )               ;
  m . append  ( "</body>\n" )               ;
  m . append  ( "</html>\n" )               ;
  F . write   ( m.toUtf8()  )               ;
  ///////////////////////////////////////////
  F . close   (             )               ;
  Alert       ( Done        )               ;
}
