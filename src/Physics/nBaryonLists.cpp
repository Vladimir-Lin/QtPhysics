#include <physics.h>

#define QKID 94242282865164288ull + 12

typedef struct {
  int  Id      ;
  SUID uuid    ;
  char symbol  ;
  int  red     ;
  int  green   ;
  int  blue    ;
  int  charge  ;
  int  spin    ;
} QuarkStates  ;

typedef struct {
  int  Value   ;
  int  Anti    ;
  char Color   ;
} QuarkFlavors ;

extern QuarkStates  QuarkState  [ 36 ] ;
extern QuarkFlavors QuarkFlavor [  6 ] ;

N::BaryonLists:: BaryonLists (QWidget * parent,Plan * p)
               : TreeWidget  (          parent,       p)
{
  Configure ( ) ;
}

N::BaryonLists::~BaryonLists(void)
{
}

QSize N::BaryonLists::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 480 , 320 ) ) ;
}

void N::BaryonLists::Configure(void)
{
  setWindowTitle               ( tr("Possible baryons")     ) ;
  /////////////////////////////////////////////////////////////
  QFont F = plan -> fonts [ Fonts::ListView ]                 ;
  /////////////////////////////////////////////////////////////
  NewTreeWidgetItem            ( head                       ) ;
  head -> setText              (  0 , tr("Q"           )    ) ;
  head -> setText              (  1 , tr("Color"       )    ) ;
  head -> setText              (  2 , tr("Q"           )    ) ;
  head -> setText              (  3 , tr("Color"       )    ) ;
  head -> setText              (  4 , tr("Q"           )    ) ;
  head -> setText              (  5 , tr("Color"       )    ) ;
  head -> setText              (  6 , tr("Charge"      )    ) ;
  head -> setText              (  7 , tr("Color charge")    ) ;
  head -> setText              (  8 , tr("Spin"        )    ) ;
  head -> setText              (  9 , tr("Confinement" )    ) ;
  head -> setText              ( 10 , tr("Quarks"      )    ) ;
  head -> setText              ( 11 , tr("Baryon"      )    ) ;
  /////////////////////////////////////////////////////////////
  setDragDropMode              ( NoDragDrop                 ) ;
  setRootIsDecorated           ( false                      ) ;
  setAlternatingRowColors      ( true                       ) ;
  setSelectionMode             ( SingleSelection            ) ;
  setColumnCount               ( 12                         ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded      ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded      ) ;
  plan -> setFont              ( this                       ) ;
  /////////////////////////////////////////////////////////////
  setAllAlignments             ( head , Qt::AlignCenter     ) ;
  nFullLoop ( i , columnCount() )                             {
    head -> setFont ( i , F )                                 ;
  }                                                           ;
  setHeaderItem                ( head                       ) ;
  /////////////////////////////////////////////////////////////
  MountClicked                 ( 2                          ) ;
  /////////////////////////////////////////////////////////////
  for (int i=0;i<6;i++)                                       {
    bool antic = ( QuarkFlavor[i].Anti == 1 )                 ;
    switch ( QuarkFlavor[i].Color )                           {
      case 'R'                                                :
        FullColors [ QuarkFlavor[i].Value ] = tr ( "Red"   )  ;
      break                                                   ;
      case 'G'                                                :
        FullColors [ QuarkFlavor[i].Value ] = tr ( "Green" )  ;
      break                                                   ;
      case 'B'                                                :
        FullColors [ QuarkFlavor[i].Value ] = tr ( "Blue"  )  ;
      break                                                   ;
    }                                                         ;
    AntiColors [ QuarkFlavor[i].Value ] = antic               ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  for (int i=0;i<36;i++)                                      {
    bool anti = ( ( i % 2 ) == 1 )                            ;
    AntiQuarks [ QuarkState[i].uuid ] = anti                  ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  setAlignment (  6 , Qt::AlignRight|Qt::AlignVCenter )       ;
  setAlignment (  7 , Qt::AlignRight|Qt::AlignVCenter )       ;
  setAlignment (  8 , Qt::AlignRight|Qt::AlignVCenter )       ;
  setAlignment (  9 , Qt::AlignRight|Qt::AlignVCenter )       ;
  setAlignment ( 10 , Qt::AlignRight|Qt::AlignVCenter )       ;
  Baryons      (                                      )       ;
  /////////////////////////////////////////////////////////////
  emit AutoFit (                                      )       ;
}

bool N::BaryonLists::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true            ) ;
  DisableAllActions (                                ) ;
  AssignAction      ( Label        , windowTitle ( ) ) ;
  LinkAction        ( Export       , Export      ( ) ) ;
  return true                                          ;
}

int N::BaryonLists::ColorCharge (int id)
{
  int v                          ;
  v  = QuarkState [ id ] . red   ;
  v *= 10                        ;
  v += QuarkState [ id ] . green ;
  v *= 10                        ;
  v += QuarkState [ id ] . blue  ;
  return v                       ;
}

void N::BaryonLists::add(int a,int b,int c,bool inclusive)
{
  RMAPs SPINs                                          ;
  for (int i=0;i<36;i++)                               {
    int spin = QuarkState[i].spin                      ;
    SPINs [ QuarkState[i].uuid ] = spin                ;
  }                                                    ;
  //////////////////////////////////////////////////////
  int     acc   = ColorCharge ( a )                    ;
  int     bcc   = ColorCharge ( b )                    ;
  int     ccc   = ColorCharge ( c )                    ;
  int     tcc   = acc + bcc + ccc                      ;
  SUID    aid   = QuarkState  [ a ] . uuid             ;
  SUID    bid   = QuarkState  [ b ] . uuid             ;
  SUID    cid   = QuarkState  [ c ] . uuid             ;
  int     aec   = QuarkState  [ a ] . charge           ;
  int     bec   = QuarkState  [ b ] . charge           ;
  int     cec   = QuarkState  [ c ] . charge           ;
  int     tec   = aec + bec + cec                      ;
  int     xec   = ( tec > 0 ) ? tec : -tec             ;
  int     asn   = SPINs       [ aid ]                  ;
  int     bsn   = SPINs       [ bid ]                  ;
  int     csn   = SPINs       [ cid ]                  ;
  int     tsn   = asn + bsn + csn                      ;
  bool    white = ( ( tcc % 111 ) == 0 )               ;
  bool    eic   = ( ( xec % 3   ) == 0 )               ;
  bool    confi = white && eic                         ;
  bool    caa   = AntiColors  [ acc      ]             ;
  bool    cab   = AntiColors  [ bcc      ]             ;
  bool    cac   = AntiColors  [ ccc      ]             ;
  QString sma   = QString(QuarkState[ a ] . symbol )   ;
  QString smb   = QString(QuarkState[ b ] . symbol )   ;
  QString smc   = QString(QuarkState[ c ] . symbol )   ;
  QString cna   = FullColors  [ acc      ]             ;
  QString cnb   = FullColors  [ bcc      ]             ;
  QString cnc   = FullColors  [ ccc      ]             ;
  QFont   F     = plan->fonts [ Fonts::TreeView ]      ;
  QFont   O     = F                                    ;
  O . setOverline ( true )                             ;
  //////////////////////////////////////////////////////
  BoundState BS                                        ;
  BS << aid                                            ;
  BS << bid                                            ;
  BS << cid                                            ;
  if (Bounds.contains(BS)) return                      ;
  Bounds << BS                                         ;
  //////////////////////////////////////////////////////
  NewTreeWidgetItem ( item )                           ;
  if ( ( a % 2 ) == 0 )                                {
    item -> setFont ( 0 , F )                          ;
  } else                                               {
    item -> setFont ( 0 , O )                          ;
  }                                                    ;
  item -> setText ( 0 , sma )                          ;
  item -> setData ( 0 , Qt::UserRole , aid )           ;
  //////////////////////////////////////////////////////
  if ( ! caa )                                         {
    item -> setFont ( 1 , F )                          ;
  } else                                               {
    item -> setFont ( 1 , O )                          ;
  }                                                    ;
  item -> setText ( 1 , cna )                          ;
  item -> setData ( 1 , Qt::UserRole , acc )           ;
  //////////////////////////////////////////////////////
  if ( ( b % 2 ) == 0 )                                {
    item -> setFont ( 2 , F )                          ;
  } else                                               {
    item -> setFont ( 2 , O )                          ;
  }                                                    ;
  item -> setText ( 2 , smb )                          ;
  item -> setData ( 2 , Qt::UserRole , bid )           ;
  //////////////////////////////////////////////////////
  if ( ! cab )                                         {
    item -> setFont ( 3 , F )                          ;
  } else                                               {
    item -> setFont ( 3 , O )                          ;
  }                                                    ;
  item -> setText ( 3 , cnb )                          ;
  item -> setData ( 3 , Qt::UserRole , bcc )           ;
  //////////////////////////////////////////////////////
  if ( ( c % 2 ) == 0 )                                {
    item -> setFont ( 4 , F )                          ;
  } else                                               {
    item -> setFont ( 4 , O )                          ;
  }                                                    ;
  item -> setText ( 4 , smc )                          ;
  item -> setData ( 4 , Qt::UserRole , cid )           ;
  //////////////////////////////////////////////////////
  if ( ! cac )                                         {
    item -> setFont ( 5 , F )                          ;
  } else                                               {
    item -> setFont ( 5 , O )                          ;
  }                                                    ;
  item -> setText ( 5 , cnc )                          ;
  item -> setData ( 5 , Qt::UserRole , ccc )           ;
  //////////////////////////////////////////////////////
  if ( eic )                                           {
    item -> setText ( 6 , QString::number(tec/3)     ) ;
  } else                                               {
    item -> setText ( 6 , QString("%1 / 3").arg(tec) ) ;
  }                                                    ;
  //////////////////////////////////////////////////////
  if ( white )                                         {
    item ->setText  ( 7 , tr("Bounded")              ) ;
  } else                                               {
    item ->setText  ( 7 , QString::number(tcc)       ) ;
  }                                                    ;
  //////////////////////////////////////////////////////
  item -> setText   ( 8 , "-?-"                      ) ;
  if ( ( tsn % 2 ) == 0 )                              {
//    item -> setText ( 8 , QString::number(tsn/2)     ) ;
  } else                                               {
//    item -> setText ( 8 , QString("%1 / 2").arg(tsn) ) ;
  }                                                    ;
  //////////////////////////////////////////////////////
  if ( confi )                                         {
    item ->setText  ( 9 , tr("Bounded")              ) ;
  } else                                               {
    item ->setText  ( 9 , tr("~"      )              ) ;
  }                                                    ;
  //////////////////////////////////////////////////////
  QString quarks                                       ;
  if (caa) quarks . append ( "~" )                     ;
  quarks . append ( sma )                              ;
  if (cab) quarks . append ( "~" )                     ;
  quarks . append ( smb )                              ;
  if (cac) quarks . append ( "~" )                     ;
  quarks . append ( smc )                              ;
  item -> setText   ( 10 , quarks                    ) ;
  //////////////////////////////////////////////////////
  if ( inclusive || confi )                            {
    setAlignments   ( item )                           ;
    addTopLevelItem ( item )                           ;
  } else                                               {
    delete item                                        ;
  }                                                    ;
}

void N::BaryonLists::Export(void)
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
  TreeWidgetItems Items                     ;
  Items << headerItem ( )                   ;
  for (int i=0;i<topLevelItemCount();i++)   {
    Items << topLevelItem ( i )             ;
  }                                         ;
  ///////////////////////////////////////////
  QString m                                 ;
  m  = "<table cellpadding=1 cellspacing=1 border=1>" ;
  m += "\n"                                 ;
  m += "<tbody>"                            ;
  m += "\n"                                 ;
  F . write ( m.toUtf8() )                  ;
  ///////////////////////////////////////////
  for (int i=0;i<Items.count();i++)         {
    QTreeWidgetItem * it = Items [ i ]      ;
    m  = "<tr>"                             ;
    m += "\n"                               ;
    if ( i == 0 )                           {
      for (int j=0;j<=11;j++)               {
        m += "<td><center>"                 ;
        m += it->text(j)                    ;
        m += "</center></td>"               ;
        m += "\n"                           ;
      }                                     ;
    } else                                  {
      SUID aid = nTreeUuid(it,0)            ;
      SUID bid = nTreeUuid(it,2)            ;
      SUID cid = nTreeUuid(it,4)            ;
      int  acc = nTreeInt (it,1)            ;
      int  bcc = nTreeInt (it,3)            ;
      int  ccc = nTreeInt (it,5)            ;
      bool aia = AntiQuarks  [ aid ]        ;
      bool bia = AntiQuarks  [ bid ]        ;
      bool cia = AntiQuarks  [ cid ]        ;
      bool caa = AntiColors  [ acc ]        ;
      bool cab = AntiColors  [ bcc ]        ;
      bool cac = AntiColors  [ ccc ]        ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (aia) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(0)                      ;
      if (aia) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (caa) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(1)                      ;
      if (caa) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (bia) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(2)                      ;
      if (bia) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (cab) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(3)                      ;
      if (cab) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (cia) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(4)                      ;
      if (cia) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (cac) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(5)                      ;
      if (cac) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      for (int j=6;j<=9;j++)                {
        m += "<td>"                         ;
        m += it->text(j)                    ;
        m += "</td>"                        ;
        m += "\n"                           ;
      }                                     ;
      ///////////////////////////////////////
      QString ps   = it->text(10)           ;
      bool    ANTI = false                  ;
      int     AI   = 0                      ;
      m += "<td>"                           ;
      while (AI<ps.length())                {
        QChar c = ps.at(AI)                 ;
        if ( c == '~' )                     {
          m += "<span style=\"text-decoration:overline\">" ;
          ANTI = true                       ;
        } else                              {
          m += c                            ;
          if (ANTI) m += "</span>"          ;
          ANTI = false                      ;
        }                                   ;
        AI++                                ;
      }                                     ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      m += "<td>"                           ;
      m += it->text(11)                     ;
      m += "</td>"                          ;
      m += "\n"                             ;
    }                                       ;
    m += "</tr>"                            ;
    m += "\n"                               ;
    F . write ( m.toUtf8() )                ;
  }                                         ;
  ///////////////////////////////////////////
  m += "</tbody>"                           ;
  m += "\n"                                 ;
  m += "</table>"                           ;
  m += "\n"                                 ;
  F . write ( m.toUtf8() )                  ;
  ///////////////////////////////////////////
  F.close()                                 ;
}

void N::BaryonLists::Baryons(void)
{
  clear          ( )              ;
  plan -> processEvents ( )       ;
  Bounds . clear ( )              ;
  for (int a=0;a<36;a++)          {
    for (int b=0;b<36;b++)        {
      for (int c=0;c<36;c++)      {
        add ( a , b , c , false ) ;
      }                           ;
    }                             ;
  }                               ;
  reportItems (      )            ;
  Alert       ( Done )            ;
}

void N::BaryonLists::Possible(void)
{
  clear          ( )             ;
  plan -> processEvents ( )      ;
  Bounds . clear ( )             ;
  for (int a=0;a<36;a++)         {
    for (int b=0;b<36;b++)       {
      for (int c=0;c<36;c++)     {
        add ( a , b , c , true ) ;
      }                          ;
    }                            ;
  }                              ;
  reportItems ( )                ;
  Alert ( Done )                 ;
}

void N::BaryonLists::doubleClicked(QTreeWidgetItem * item,int column)
{
  if ( column != 11 ) return                  ;
  QComboBox * cb                              ;
  cb = setComboBox                            (
        item                                  ,
        column                                ,
        SIGNAL(activated    (int))            ,
        SLOT  (boundChanged (int))       )    ;
  cb->setInsertPolicy(QComboBox::NoInsert)    ;
  cb->setEditable(true)                       ;
  cb->setMaxVisibleItems(30)                  ;
  cb->blockSignals(true)                      ;
  SqlConnection SC ( plan->sql )              ;
  if (SC.open("BaryonLists","doubleClicked")) {
    UUIDs      U                              ;
    SUID       u                              ;
    U = SC . Uuids                            (
          PlanTable(Composite)                ,
          "uuid"                              ,
          SC.OrderByAsc("id")               ) ;
    foreach (u,U)                             {
      QString n                               ;
      n = SC . getName                        (
            PlanTable(Names)                  ,
            "uuid"                            ,
            vLanguageId                       ,
            u                               ) ;
      cb -> addItem ( n , u )                 ;
    }                                         ;
    SC.close()                                ;
  }                                           ;
  SC.remove()                                 ;
  cb->blockSignals(false)                     ;
  cb->showPopup()                             ;
}

void N::BaryonLists::boundChanged(int id)
{
  QComboBox * combo = qobject_cast<QComboBox *>(ItemWidget) ;
  if (IsNull(combo)) return                                 ;
  SUID u = combo->itemData(id,Qt::UserRole).toULongLong()   ;
  if (u>0)                                                  {
    SqlConnection SC ( plan->sql )                          ;
    GroupItems    GI ( plan      )                          ;
    if (SC.open("BaryonLists","boundChanged"))              {
      QString    Q                                          ;
      SUID       aid = nTreeUuid ( ItemEditing , 0 )        ;
      SUID       bid = nTreeUuid ( ItemEditing , 2 )        ;
      SUID       cid = nTreeUuid ( ItemEditing , 4 )        ;
      SUID       bud = 0                                    ;
      BoundState BS                                         ;
      BS << aid                                             ;
      BS << bid                                             ;
      BS << cid                                             ;
      Q = SC . sql . SelectFrom                             (
            "uuid"                                          ,
            PlanTable(BoundStates)                          ,
            SC.sql.Where(1,"bounds")                      ) ;
      SC . Prepare ( Q             )                        ;
      SC . Bind    ( "bounds" , BS )                        ;
      if (SC.Exec() && SC.Next()) bud = SC.Uuid(0)          ;
      if (bud>0)                                            {
        UUIDs U                                             ;
        U << bud                                            ;
        GI . Join                                           (
          SC                                                ,
          u                                                 ,
          Types::Composite                                  ,
          Types::Bound                                      ,
          Groups::Subordination                             ,
          0                                                 ,
          U                                               ) ;
        ItemEditing->setText(11,combo->currentText())       ;
      }                                                     ;
      SC.close()                                            ;
    }                                                       ;
    SC.remove()                                             ;
  }                                                         ;
  removeOldItem ()                                          ;
}

void N::BaryonLists::Synchronize(void)
{
  SqlConnection SC ( plan->sql )              ;
  if (SC.open("MesonLists","Synchronize"))    {
    QString Q                                 ;
    for (int i=0;i<topLevelItemCount();i++)   {
      QTreeWidgetItem * it  = topLevelItem(i) ;
      SUID              aid = nTreeUuid(it,0) ;
      SUID              bid = nTreeUuid(it,2) ;
      SUID              cid = nTreeUuid(it,4) ;
      BoundState        BS                    ;
      BS << aid                               ;
      BS << bid                               ;
      BS << cid                               ;
      Q = SC.sql.SelectFrom                   (
            "uuid"                            ,
            PlanTable(BoundStates)            ,
            SC.sql.Where                      (
              2                               ,
              "particles"                     ,
              "bounds"                    ) ) ;
      SC . Prepare ( Q                      ) ;
      SC . Bind  ("particles",BS.particles()) ;
      SC . Bind  ("bounds"   ,BS            ) ;
      if (SC.Exec() && SC.Next())             {
        BS.uuid = SC.Uuid(0)                  ;
      } else                                  {
        BS.uuid = SC.Unique                   (
                    PlanTable(MajorUuid)      ,
                    "uuid"                    ,
                    74412001                ) ;
        SC.assureUuid                         (
          PlanTable(MajorUuid)                ,
          BS.uuid                             ,
          Types::Bound                      ) ;
        Q = SC.sql.InsertInto                 (
              PlanTable(BoundStates)          ,
              3                               ,
              "uuid"                          ,
              "particles"                     ,
              "bounds"                      ) ;
        SC . Prepare ( Q )                    ;
        SC . Bind("uuid"     ,BS.uuid       ) ;
        SC . Bind("particles",BS.particles()) ;
        SC . Bind("bounds"   ,BS            ) ;
        SC . Exec(                          ) ;
      }                                       ;
      if (BS.uuid>0)                          {
        UUIDs ZUIDs                           ;
        Q = SC.sql.SelectFrom                 (
              "first"                         ,
              PlanTable(Groups)               ,
              QString("where second = %1 and t1 = %2 and t2 = %3 and relation = %4 order by position asc")
              .arg(BS.uuid                    )
              .arg(Types::Composite           )
              .arg(Types::Bound               )
              .arg(Groups::Subordination)   ) ;
        SqlLoopNow ( SC , Q )                 ;
          ZUIDs << SC.Uuid(0)                 ;
        SqlLoopErr ( SC , Q )                 ;
        SqlLoopEnd ( SC , Q )                 ;
        if (ZUIDs.count()>0)                  {
          QStringList L                       ;
          SUID zuid                           ;
          foreach (zuid,ZUIDs)                {
            QString n = SC.getName            (
                          PlanTable(Names)    ,
                          "uuid"              ,
                          vLanguageId         ,
                          zuid              ) ;
            if (n.length()>0) L << n          ;
          }                                   ;
          if (L.count()>0)                    {
            QString l = L.join(", ")          ;
            it->setText(11,l                ) ;
          }                                   ;
        }                                     ;
      }                                       ;
    }                                         ;
    SC.close()                                ;
  }                                           ;
  SC.remove()                                 ;
  Alert ( Done )                              ;
}

bool N::BaryonLists::Menu(QPoint pos)
{
  nScopedMenu ( mm , this )                            ;
  QAction * aa                                         ;
  mm . add ( 101 , tr ( "Export"                   ) ) ;
  mm . add ( 102 , tr ( "Synchronize database"     ) ) ;
  mm . addSeparator (                                ) ;
  mm . add ( 201 , tr ( "Bounded states"           ) ) ;
  mm . add ( 202 , tr ( "All possible combinations") ) ;
  mm . setFont ( plan                                ) ;
  aa = mm . exec ( )                                   ;
  if ( IsNull ( aa ) ) return true                     ;
  switch ( mm[aa] )                                    {
    case 101                                           :
      Export      ( )                                  ;
    break                                              ;
    case 102                                           :
      Synchronize ( )                                  ;
    break                                              ;
    case 201                                           :
      Baryons     ( )                                  ;
    break                                              ;
    case 202                                           :
      Possible    ( )                                  ;
    break                                              ;
  }                                                    ;
  return true                                          ;
}
