#include <physics.h>

#define NucleusTable PlanTable(Nucleus)

#define ELECTRON     94242282865164288ull
#define PROTON       7441200272769865044ull
#define NEUTRON      7441200249358104353ull

typedef struct  {
  SUID uuid     ;
  int  quantity ;
} NucleusMember ;

N::NucleusList:: NucleusList ( QWidget * parent , Plan * p )
               : TreeDock    (           parent ,        p )
{
  Configure ( ) ;
}

N::NucleusList::~NucleusList(void)
{
}

QSize N::NucleusList::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 480 , 480 ) ) ;
}

void N::NucleusList::Configure(void)
{
  setWindowTitle               ( tr("Nucleus lists")   ) ;
  ////////////////////////////////////////////////////////
  NewTreeWidgetItem            ( head                  ) ;
  head -> setText              (  0 , tr("Nucleus"   ) ) ;
  head -> setText              (  1 , tr("Symbol"    ) ) ;
  head -> setText              (  2 , tr("Electrons" ) ) ;
  head -> setText              (  3 , tr("Neutrons"  ) ) ;
  head -> setText              (  4 , tr("Protons"   ) ) ;
  head -> setText              (  5 , tr("Others"    ) ) ;
  head -> setText              (  6 , ""               ) ;
  ////////////////////////////////////////////////////////
  setDragDropMode              ( NoDragDrop            ) ;
  setRootIsDecorated           ( false                 ) ;
  setAlternatingRowColors      ( true                  ) ;
  setSelectionMode             ( SingleSelection       ) ;
  setColumnCount               ( 7                     ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded ) ;
  plan -> setFont              ( this                  ) ;
  assignHeaderItems            ( head                  ) ;
  ////////////////////////////////////////////////////////
  setAlignment                 ( 2 , Qt::AlignRight      |
                                     Qt::AlignVCenter  ) ;
  setAlignment                 ( 3 , Qt::AlignRight      |
                                     Qt::AlignVCenter  ) ;
  setAlignment                 ( 4 , Qt::AlignRight      |
                                     Qt::AlignVCenter  ) ;
  setAlignment                 ( 5 , Qt::AlignRight      |
                                     Qt::AlignVCenter  ) ;
  ////////////////////////////////////////////////////////
  MountClicked                 ( 2                     ) ;
}

bool N::NucleusList::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true                ) ;
  DisableAllActions (                                    ) ;
  AssignAction      ( Label        , windowTitle     ( ) ) ;
  LinkAction        ( Refresh      , startup         ( ) ) ;
  LinkAction        ( Insert       , New             ( ) ) ;
  LinkAction        ( Export       , Export          ( ) ) ;
  LinkAction        ( Font         , setFont         ( ) ) ;
  LinkAction        ( Copy         , CopyToClipboard ( ) ) ;
  LinkAction        ( SelectNone   , SelectNone      ( ) ) ;
  LinkAction        ( SelectAll    , SelectAll       ( ) ) ;
  return true                                              ;
}

void N::NucleusList::run(int Type,ThreadData * data)
{ Q_UNUSED ( data ) ;
  switch   ( Type ) {
    case 10001      :
      List ( )      ;
    break           ;
  }                 ;
}

bool N::NucleusList::startup(void)
{
  clear (       ) ;
  start ( 10001 ) ;
  return true     ;
}

QString N::NucleusList::Count(SUID u,QByteArray & B)
{
  NucleusMember * nm = (NucleusMember *)B.data()         ;
  int             ts = B.size() / sizeof (NucleusMember) ;
  int             tt = 0                                 ;
  if (u<=0)                                              {
    for (int i=0;i<ts;i++)                               {
      if ( nm[i].uuid == ELECTRON ) {           ; } else
      if ( nm[i].uuid == PROTON   ) {           ; } else
      if ( nm[i].uuid == NEUTRON  ) {           ; } else {
        tt += nm[i].quantity                             ;
      }                                                  ;
    }                                                    ;
  } else                                                 {
    for (int i=0;i<ts;i++)                               {
      if ( nm[i].uuid == u ) tt = nm[i].quantity         ;
    }                                                    ;
  }                                                      ;
  return QString::number(tt)                             ;
}

void N::NucleusList::List(void)
{
  SqlConnection SC ( plan -> sql )             ;
  if (SC.open("NucleusList","List"))           {
    QString Q                                  ;
    UUIDs   U                                  ;
    SUID    u                                  ;
    U = SC . Uuids                             (
          NucleusTable                         ,
          "uuid"                               ,
          SC.OrderByAsc("id")                ) ;
    foreach (u,U)                              {
      NewTreeWidgetItem ( it )                 ;
      QString    n                             ;
      QString    s                             ;
      QByteArray b                             ;
      n = SC . getName                         (
            PlanTable(Names)                   ,
            "uuid"                             ,
            vLanguageId                        ,
            u                                ) ;
      Q = SC . sql . SelectFrom                (
            "symbol,members"                   ,
            NucleusTable                       ,
            SC.WhereUuid(u)                  ) ;
      if (SC.Fetch(Q))                         {
        s = SC . String    ( 0 )               ;
        b = SC . ByteArray ( 1 )               ;
      }                                        ;
      it -> setData   ( 0 , Qt::UserRole , u ) ;
      it -> setText   ( 0 , n                ) ;
      it -> setText   ( 1 , s                ) ;
      if (b.size()>0)                          {
        it -> setText ( 2,Count (ELECTRON,b) ) ;
        it -> setText ( 3,Count (NEUTRON ,b) ) ;
        it -> setText ( 4,Count (PROTON  ,b) ) ;
        it -> setText ( 5,Count (0       ,b) ) ;
      }                                        ;
      setAlignments   ( it                   ) ;
      addTopLevelItem ( it                   ) ;
    }                                          ;
    SC.close()                                 ;
  }                                            ;
  SC.remove()                                  ;
  emit AutoFit (      )                        ;
  Alert        ( Done )                        ;
}

void N::NucleusList::New(void)
{
  removeOldItem     ( true , 0             ) ;
  NewTreeWidgetItem ( it                   ) ;
  it -> setData     ( 0 , Qt::UserRole , 0 ) ;
  setAlignments     ( it                   ) ;
  addTopLevelItem   ( it                   ) ;
  DoProcessEvents                            ;
  scrollToItem      ( it                   ) ;
  setCurrentItem    ( it                   ) ;
  doubleClicked     ( it , 0               ) ;
}

bool N::NucleusList::Menu(QPoint pos)
{
  QAction   * aa                                         ;
  MenuManager mm ( this )                                ;
  QTreeWidgetItem * it = itemAt(pos)                     ;
  ////////////////////////////////////////////////////////
  mm . add ( 101 , tr("Refresh" ) )                      ;
  mm . add ( 102 , tr("Export"  ) )                      ;
  mm . add ( 103 , tr("Add"     ) )                      ;
  if (NotNull(it))                                       {
    mm . add ( 201 , tr("Edit"    ) )                    ;
    mm . add ( 202 , tr("Isotopes") )                    ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  DockingMenu    ( mm   )                                ;
  mm . add ( 501 , tr("set font"    ) )                  ;
  mm . add ( 502 , tr("set language") )                  ;
  if (topLevelItemCount()>0)                             {
    mm . add   ( 503 , tr("Multilingual translations") ) ;
  }                                                      ;
  mm . setFont   ( plan )                                ;
  aa = mm . exec (      )                                ;
  if (IsNull(aa)) return true                            ;
  if (RunDocking(mm,aa)) return true                     ;
  ////////////////////////////////////////////////////////
  switch (mm[aa])                                        {
    case 101                                             :
      startup        (                               )   ;
    break                                                ;
    case 102                                             :
      Export         (                               )   ;
    break                                                ;
    case 103                                             :
      New            (                               )   ;
    break                                                ;
    case 201                                             :
      emit Edit      ( it->text(0) , nTreeUuid(it,0) )   ;
    break                                                ;
    case 202                                             :
      emit Isotopes  ( it->text(0) , nTreeUuid(it,0) )   ;
    break                                                ;
    case 501                                             :
      setFont        (                               )   ;
    break                                                ;
    case 502                                             :
      assignLanguage ( this                          )   ;
      startup        (                               )   ;
    break                                                ;
    case 503                                             :
      if (topLevelItemCount()>0)                         {
        UUIDs U = itemUuids ( 0 )                        ;
        emit Translations ( windowTitle() , U )          ;
      }                                                  ;
    break                                                ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  return true                                            ;
}

void N::NucleusList::doubleClicked(QTreeWidgetItem * item,int column)
{
  nDropOut ( column > 1 )                     ;
  QLineEdit * line                            ;
  switch (column)                             {
    case 0                                    :
      line = setLineEdit                      (
               item,column                    ,
               SIGNAL ( returnPressed  () )   ,
               SLOT   ( nameFinished   () ) ) ;
      line -> setFocus ( Qt::TabFocusReason ) ;
    break                                     ;
    case 1                                    :
      line = setLineEdit                      (
               item,column                    ,
               SIGNAL ( returnPressed  () )   ,
               SLOT   ( symbolFinished () ) ) ;
      line -> setFocus ( Qt::TabFocusReason ) ;
    break                                     ;
  }                                           ;
}

void N::NucleusList::nameFinished(void)
{
  if (IsNull(ItemEditing)) return                  ;
  QLineEdit * line = Casting(QLineEdit,ItemWidget) ;
  if (IsNull(line)) return                         ;
  SUID    u = nTreeUuid(ItemEditing,0)             ;
  QString n = line->text()                         ;
  EnterSQL         ( SC , plan->sql          )     ;
    if ( n . length ( ) <= 0 && u <= 0 )           {
    } else                                         {
      if ( u <= 0)                                 {
        u = SC . Unique                            (
              PlanTable(MajorUuid)                 ,
              "uuid"                               ,
              74413001                           ) ;
        SC . assureUuid                            (
          PlanTable(MajorUuid)                     ,
          u                                        ,
          Types::Nucleus                         ) ;
        SC . insertUuid ( NucleusTable,u,"uuid"  ) ;
      }                                            ;
      if (u>0)                                     {
        SC . assureName                            (
          PlanTable(Names)                         ,
          u                                        ,
          vLanguageId                              ,
          n                                      ) ;
        ItemEditing -> setData (0,Qt::UserRole,u ) ;
        ItemEditing -> setText (0,n              ) ;
      }                                            ;
    }                                              ;
  LeaveSQL         ( SC , plan->sql          )     ;
  removeOldItem    ( true , 0                )     ;
  Alert            ( Done                    )     ;
}

void N::NucleusList::symbolFinished(void)
{
  if (IsNull(ItemEditing)) return                  ;
  QLineEdit * line = Casting(QLineEdit,ItemWidget) ;
  if (IsNull(line)) return                         ;
  SUID    u = nTreeUuid(ItemEditing,0)             ;
  QString n = line->text()                         ;
  EnterSQL         ( SC , plan->sql        )       ;
    if ( u > 0 )                                   {
      QString Q                                    ;
      Q = SC . sql . Update                        (
            NucleusTable                           ,
            SC.sql.Where(1,"uuid")                 ,
            1                                      ,
            "symbol"                       )       ;
      SC . Prepare ( Q                     )       ;
      SC . Bind    ( "uuid"   , u          )       ;
      SC . Bind    ( "symbol" , n.toUtf8() )       ;
      SC . Exec    (                       )       ;
      ItemEditing -> setText ( 1 , n )             ;
    }                                              ;
  LeaveSQL         ( SC , plan->sql        )       ;
  removeOldItem    ( true , 0              )       ;
  Alert            ( Done                  )       ;
}

QString N::NucleusList::toHtml(void)
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
    for (int j=0;j<(columnCount()-1);j++)               {
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

void N::NucleusList::Export(void)
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
