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

QuarkStates QuarkState [ 36 ] =                 {
  {  0 , QKID +  0 , 'u' , 1 , 0 , 0 ,  2 , 1 } ,
  {  1 , QKID +  1 , 'u' , 0 , 1 , 1 , -2 , 1 } ,
  {  0 , QKID +  2 , 'u' , 0 , 1 , 0 ,  2 , 1 } ,
  {  1 , QKID +  3 , 'u' , 1 , 0 , 1 , -2 , 1 } ,
  {  0 , QKID +  4 , 'u' , 0 , 0 , 1 ,  2 , 1 } ,
  {  1 , QKID +  5 , 'u' , 1 , 1 , 0 , -2 , 1 } ,

  {  2 , QKID +  6 , 'd' , 1 , 0 , 0 , -1 , 1 } ,
  {  3 , QKID +  7 , 'd' , 0 , 1 , 1 ,  1 , 1 } ,
  {  2 , QKID +  8 , 'd' , 0 , 1 , 0 , -1 , 1 } ,
  {  3 , QKID +  9 , 'd' , 1 , 0 , 1 ,  1 , 1 } ,
  {  2 , QKID + 10 , 'd' , 0 , 0 , 1 , -1 , 1 } ,
  {  3 , QKID + 11 , 'd' , 1 , 1 , 0 ,  1 , 1 } ,

  {  4 , QKID + 12 , 'c' , 1 , 0 , 0 ,  2 , 1 } ,
  {  5 , QKID + 13 , 'c' , 0 , 1 , 1 , -2 , 1 } ,
  {  4 , QKID + 14 , 'c' , 0 , 1 , 0 ,  2 , 1 } ,
  {  5 , QKID + 15 , 'c' , 1 , 0 , 1 , -2 , 1 } ,
  {  4 , QKID + 16 , 'c' , 0 , 0 , 1 ,  2 , 1 } ,
  {  5 , QKID + 17 , 'c' , 1 , 1 , 0 , -2 , 1 } ,

  {  6 , QKID + 18 , 's' , 1 , 0 , 0 , -1 , 1 } ,
  {  7 , QKID + 19 , 's' , 0 , 1 , 1 ,  1 , 1 } ,
  {  6 , QKID + 20 , 's' , 0 , 1 , 0 , -1 , 1 } ,
  {  7 , QKID + 21 , 's' , 1 , 0 , 1 ,  1 , 1 } ,
  {  6 , QKID + 22 , 's' , 0 , 0 , 1 , -1 , 1 } ,
  {  7 , QKID + 23 , 's' , 1 , 1 , 0 ,  1 , 1 } ,

  {  8 , QKID + 24 , 't' , 1 , 0 , 0 ,  2 , 1 } ,
  {  9 , QKID + 25 , 't' , 0 , 1 , 1 , -2 , 1 } ,
  {  8 , QKID + 26 , 't' , 0 , 1 , 0 ,  2 , 1 } ,
  {  9 , QKID + 27 , 't' , 1 , 0 , 1 , -2 , 1 } ,
  {  8 , QKID + 28 , 't' , 0 , 0 , 1 ,  2 , 1 } ,
  {  9 , QKID + 29 , 't' , 1 , 1 , 0 , -2 , 1 } ,

  { 10 , QKID + 30 , 'b' , 1 , 0 , 0 , -1 , 1 } ,
  { 11 , QKID + 31 , 'b' , 0 , 1 , 1 ,  1 , 1 } ,
  { 10 , QKID + 32 , 'b' , 0 , 1 , 0 , -1 , 1 } ,
  { 11 , QKID + 33 , 'b' , 1 , 0 , 1 ,  1 , 1 } ,
  { 10 , QKID + 34 , 'b' , 0 , 0 , 1 , -1 , 1 } ,
  { 11 , QKID + 35 , 'b' , 1 , 1 , 0 ,  1 , 1 }
}                                               ;

QuarkFlavors QuarkFlavor [ 6 ] = {
  { 100 , 0 , 'R' }              ,
  {  10 , 0 , 'G' }              ,
  {   1 , 0 , 'B' }              ,
  {  11 , 1 , 'R' }              ,
  { 101 , 1 , 'G' }              ,
  { 110 , 1 , 'B' }
}                                ;

N::MesonLists:: MesonLists (QWidget * parent,Plan * p)
              : TreeWidget (          parent,       p)
{
  Configure ( ) ;
}

N::MesonLists::~MesonLists(void)
{
}

void N::MesonLists::Configure(void)
{
  setWindowTitle               ( tr("Possible mesons")      ) ;
  /////////////////////////////////////////////////////////////
  QFont F = plan -> fonts [ Fonts::ListView ]                 ;
  QFont O = F                                                 ;
  O . setOverline              ( true                       ) ;
  /////////////////////////////////////////////////////////////
  NewTreeWidgetItem            ( head                       ) ;
  head -> setText              ( 0 , tr("q"           )     ) ;
  head -> setText              ( 1 , tr("Color"       )     ) ;
  head -> setText              ( 2 , tr("q"           )     ) ;
  head -> setText              ( 3 , tr("Color"       )     ) ;
  head -> setText              ( 4 , tr("Charge"      )     ) ;
  head -> setText              ( 5 , tr("Color charge")     ) ;
  head -> setText              ( 6 , tr("Confinement" )     ) ;
  head -> setText              ( 7 , tr("Quarks"      )     ) ;
  head -> setText              ( 8 , tr("Meson"       )     ) ;
  /////////////////////////////////////////////////////////////
  setDragDropMode              ( NoDragDrop                 ) ;
  setRootIsDecorated           ( false                      ) ;
  setAlternatingRowColors      ( true                       ) ;
  setSelectionMode             ( SingleSelection            ) ;
  setColumnCount               ( 9                          ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded      ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded      ) ;
  plan -> setFont              ( this                       ) ;
  /////////////////////////////////////////////////////////////
  setAllAlignments             ( head , Qt::AlignCenter     ) ;
  nFullLoop ( i , columnCount() )                             {
    if (i==2) head -> setFont  ( i , O                      ) ;
         else head -> setFont  ( i , F                      ) ;
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
  setAlignment ( 4 , Qt::AlignRight|Qt::AlignVCenter )        ;
  setAlignment ( 5 , Qt::AlignRight|Qt::AlignVCenter )        ;
  setAlignment ( 6 , Qt::AlignRight|Qt::AlignVCenter )        ;
  setAlignment ( 7 , Qt::AlignRight|Qt::AlignVCenter )        ;
  Stable       (                                     )        ;
  /////////////////////////////////////////////////////////////
  emit AutoFit                 (                            ) ;
}

bool N::MesonLists::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true            ) ;
  DisableAllActions (                                ) ;
  AssignAction      ( Label        , windowTitle ( ) ) ;
  LinkAction        ( Export       , Export      ( ) ) ;
  return true                                          ;
}

int N::MesonLists::ColorCharge (int id)
{
  int v                          ;
  v  = QuarkState [ id ] . red   ;
  v *= 10                        ;
  v += QuarkState [ id ] . green ;
  v *= 10                        ;
  v += QuarkState [ id ] . blue  ;
  return v                       ;
}

void N::MesonLists::add(int positive,int anti,bool inclusive)
{
  int     pcc   = ColorCharge ( positive )             ;
  int     acc   = ColorCharge ( anti     )             ;
  int     tcc   = pcc + acc                            ;
  SUID    pid   = QuarkState  [ positive ] . uuid      ;
  SUID    aid   = QuarkState  [ anti     ] . uuid      ;
  int     pec   = QuarkState  [ positive ] . charge    ;
  int     aec   = QuarkState  [ anti     ] . charge    ;
  int     tec   = pec + aec                            ;
  int     xec   = ( tec > 0 ) ? tec : -tec             ;
  bool    white = ( ( tcc % 111 ) == 0 )               ;
  bool    eic   = ( ( xec % 3   ) == 0 )               ;
  bool    confi = white && eic                         ;
  bool    cap   = AntiColors  [ pcc      ]             ;
  bool    caa   = AntiColors  [ acc      ]             ;
  QString smp   = QString(QuarkState[positive].symbol) ;
  QString sma   = QString(QuarkState[anti    ].symbol) ;
  QString cnp   = FullColors  [ pcc      ]             ;
  QString cna   = FullColors  [ acc      ]             ;
  QFont   F     = plan->fonts [ Fonts::TreeView ]      ;
  QFont   O     = F                                    ;
  O . setOverline ( true )                             ;
  //////////////////////////////////////////////////////
  BoundState BS                                        ;
  BS << pid                                            ;
  BS << aid                                            ;
  if (Bounds.contains(BS)) return                      ;
  Bounds << BS                                         ;
  //////////////////////////////////////////////////////
  NewTreeWidgetItem ( item )                           ;
  if ( ( positive % 2 ) == 0 )                         {
    item -> setFont ( 0 , F )                          ;
  } else                                               {
    item -> setFont ( 0 , O )                          ;
  }                                                    ;
  item -> setText ( 0 , smp )                          ;
  item -> setData ( 0 , Qt::UserRole , pid )           ;
  //////////////////////////////////////////////////////
  if ( ! cap )                                         {
    item -> setFont ( 1 , F )                          ;
  } else                                               {
    item -> setFont ( 1 , O )                          ;
  }                                                    ;
  item -> setText ( 1 , cnp )                          ;
  item -> setData ( 1 , Qt::UserRole , pcc )           ;
  //////////////////////////////////////////////////////
  if ( ( anti % 2 ) == 0 )                             {
    item -> setFont ( 2 , F )                          ;
  } else                                               {
    item -> setFont ( 2 , O )                          ;
  }                                                    ;
  item -> setText ( 2 , sma )                          ;
  item -> setData ( 2 , Qt::UserRole , aid )           ;
  //////////////////////////////////////////////////////
  if ( ! caa )                                         {
    item -> setFont ( 3 , F )                          ;
  } else                                               {
    item -> setFont ( 3 , O )                          ;
  }                                                    ;
  item -> setText ( 3 , cna )                          ;
  item -> setData ( 3 , Qt::UserRole , acc )           ;
  //////////////////////////////////////////////////////
  if ( eic )                                           {
    item -> setText ( 4 , QString::number(tec/3)     ) ;
  } else                                               {
    item -> setText ( 4 , QString("%1 / 3").arg(tec) ) ;
  }                                                    ;
  //////////////////////////////////////////////////////
  if ( white )                                         {
    item ->setText  ( 5 , tr("Bounded")              ) ;
  } else                                               {
    item ->setText  ( 5 , QString::number(tcc)       ) ;
  }                                                    ;
  //////////////////////////////////////////////////////
  if ( confi )                                         {
    item ->setText  ( 6 , tr("Bounded")              ) ;
  } else                                               {
    item ->setText  ( 6 , tr("~"      )              ) ;
  }                                                    ;
  //////////////////////////////////////////////////////
  QString quarks                                       ;
  if (cap) quarks . append ( "~" )                     ;
  quarks . append ( smp )                              ;
  if (caa) quarks . append ( "~" )                     ;
  quarks . append ( sma )                              ;
  item -> setText   ( 7 , quarks                     ) ;
  //////////////////////////////////////////////////////
  if ( inclusive || confi )                            {
    setAlignments   ( item )                           ;
    addTopLevelItem ( item )                           ;
  } else                                               {
    delete item                                        ;
  }                                                    ;
}

void N::MesonLists::Export(void)
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
      m += "<td><center>"                   ;
      m += it->text(0)                      ;
      m += "</center></td>"                 ;
      m += "\n"                             ;
      m += "<td><center>"                   ;
      m += it->text(1)                      ;
      m += "</center></td>"                 ;
      m += "\n"                             ;
      m += "<td><center>"                   ;
      m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(2)                      ;
      m += "</span>"                        ;
      m += "</center></td>"                 ;
      m += "\n"                             ;
      for (int j=3;j<=8;j++)                {
        m += "<td><center>"                 ;
        m += it->text(j)                    ;
        m += "</center></td>"               ;
        m += "\n"                           ;
      }                                     ;
    } else                                  {
      SUID pid = nTreeUuid(it,0)            ;
      SUID aid = nTreeUuid(it,2)            ;
      int  pcc = nTreeInt (it,1)            ;
      int  acc = nTreeInt (it,3)            ;
      bool pia = AntiQuarks  [ pid ]        ;
      bool aia = AntiQuarks  [ aid ]        ;
      bool cap = AntiColors  [ pcc ]        ;
      bool caa = AntiColors  [ acc ]        ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (pia) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(0)                      ;
      if (pia) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (cap) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(1)                      ;
      if (cap) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (aia) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(2)                      ;
      if (aia) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      m += "<td><center>"                   ;
      if (caa) m += "<span style=\"text-decoration:overline\">" ;
      m += it->text(3)                      ;
      if (caa) m += "</span>"               ;
      m += "</td>"                          ;
      m += "\n"                             ;
      ///////////////////////////////////////
      for (int j=4;j<=6;j++)                {
        m += "<td>"                         ;
        m += it->text(j)                    ;
        m += "</td>"                        ;
        m += "\n"                           ;
      }                                     ;
      ///////////////////////////////////////
      QString ps   = it->text(7)            ;
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
      m += it->text(8)                      ;
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

void N::MesonLists::Stable(void)
{
  clear          ( )                            ;
  Bounds . clear ( )                            ;
  for (int p=0;p<18;p++) for (int a=0;a<18;a++) {
    add ( ( p * 2 ) , ( a * 2 ) + 1 , false )   ;
  }                                             ;
  reportItems ( )                               ;
  Alert ( Done )                                ;
}

void N::MesonLists::Mesons(void)
{
  clear          ( )                            ;
  Bounds . clear ( )                            ;
  for (int p=0;p<18;p++) for (int a=0;a<18;a++) {
    add ( ( p * 2 ) , ( a * 2 ) + 1 , true )    ;
  }                                             ;
  reportItems ( )                               ;
  Alert ( Done )                                ;
}

void N::MesonLists::Possible(void)
{
  clear          ( )                            ;
  Bounds . clear ( )                            ;
  for (int p=0;p<36;p++) for (int a=0;a<36;a++) {
    add ( p , a , true )                        ;
  }                                             ;
  reportItems ( )                               ;
  Alert ( Done )                                ;
}

void N::MesonLists::doubleClicked(QTreeWidgetItem * item,int column)
{
  if ( column != 8 ) return                  ;
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
    QString    Q                              ;
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

void N::MesonLists::boundChanged(int id)
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
      SUID       bud = 0                                    ;
      BoundState BS                                         ;
      BS << aid                                             ;
      BS << bid                                             ;
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
        ItemEditing->setText(8,combo->currentText())        ;
      }                                                     ;
      SC.close()                                            ;
    }                                                       ;
    SC.remove()                                             ;
  }                                                         ;
  removeOldItem ()                                          ;
}

void N::MesonLists::Synchronize(void)
{
  SqlConnection SC ( plan->sql )              ;
  if (SC.open("MesonLists","Synchronize"))    {
    QString Q                                 ;
    for (int i=0;i<topLevelItemCount();i++)   {
      QTreeWidgetItem * it  = topLevelItem(i) ;
      SUID              pid = nTreeUuid(it,0) ;
      SUID              aid = nTreeUuid(it,2) ;
      BoundState        BS                    ;
      BS << pid                               ;
      BS << aid                               ;
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
            it->setText(8,l                 ) ;
          }                                   ;
        }                                     ;
      }                                       ;
    }                                         ;
    SC.close()                                ;
  }                                           ;
  SC.remove()                                 ;
  Alert ( Done )                              ;
}

bool N::MesonLists::Menu(QPoint pos)
{
  nScopedMenu ( mm , this )                            ;
  QAction * aa                                         ;
  mm . add ( 101 , tr ( "Export"                   ) ) ;
  mm . add ( 102 , tr ( "Synchronize database"     ) ) ;
  mm . addSeparator (                                ) ;
  mm . add ( 201 , tr ( "Bounded states"           ) ) ;
  mm . add ( 202 , tr ( "Mesons"                   ) ) ;
  mm . add ( 203 , tr ( "All possible combinations") ) ;
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
      Stable      ( )                                  ;
    break                                              ;
    case 202                                           :
      Mesons      ( )                                  ;
    break                                              ;
    case 203                                           :
      Possible    ( )                                  ;
    break                                              ;
  }                                                    ;
  return true                                          ;
}
