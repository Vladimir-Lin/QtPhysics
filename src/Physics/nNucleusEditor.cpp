#include <physics.h>

#define NucleusTable PlanTable(Nucleus)

typedef struct  {
  SUID uuid     ;
  int  quantity ;
} NucleusMember ;

N::NucleusEditor:: NucleusEditor(QWidget * parent,Plan * p)
                 : TreeWidget   (          parent,       p)
                 , Object       (0,Types::Nucleus         )
{
  Configure ( ) ;
}

N::NucleusEditor::~NucleusEditor(void)
{
}

QSize N::NucleusEditor::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 320 , 320 ) ) ;
}

void N::NucleusEditor::Configure(void)
{
  setWindowTitle               ( tr("Nucleon members") ) ;
  ////////////////////////////////////////////////////////
  NewTreeWidgetItem            ( head                  ) ;
  head -> setText              (  0 , tr("Particle")   ) ;
  head -> setText              (  1 , tr("Quantity")   ) ;
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
  setAlignment                 ( 1 , Qt::AlignRight      |
                                     Qt::AlignVCenter  ) ;
  MountClicked                 ( 2                     ) ;
}

bool N::NucleusEditor::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true                ) ;
  DisableAllActions (                                    ) ;
  AssignAction      ( Label        , windowTitle     ( ) ) ;
  LinkAction        ( Refresh      , startup         ( ) ) ;
  LinkAction        ( Save         , Save            ( ) ) ;
  LinkAction        ( Insert       , New             ( ) ) ;
  LinkAction        ( Delete       , Delete          ( ) ) ;
  LinkAction        ( Export       , Export          ( ) ) ;
  LinkAction        ( Font         , setFont         ( ) ) ;
  LinkAction        ( Copy         , CopyToClipboard ( ) ) ;
  LinkAction        ( SelectNone   , SelectNone      ( ) ) ;
  LinkAction        ( SelectAll    , SelectAll       ( ) ) ;
  return true                                              ;
}

void N::NucleusEditor::run(int Type,ThreadData * data)
{ Q_UNUSED ( data ) ;
  switch   ( Type ) {
    case 10001      :
      List ( )      ;
    break           ;
  }                 ;
}

bool N::NucleusEditor::startup(void)
{
  clear (       ) ;
  start ( 10001 ) ;
  return true     ;
}

void N::NucleusEditor::List(void)
{
  if (ObjectUuid()<=0) return                  ;
  SqlConnection SC ( plan -> sql )             ;
  if (SC.open("NucleusEditor","List"))         {
    QString         Q                          ;
    QByteArray      B                          ;
    Q = SC . sql . SelectFrom                  (
          "members"                            ,
          NucleusTable                         ,
          SC.WhereUuid(ObjectUuid())         ) ;
    if (SC.Fetch(Q)) B = SC.ByteArray(0)       ;
    if (B.size()>0)                            {
      NucleusMember * nm                       ;
      int             ns                       ;
      nm = (NucleusMember *)B.data()           ;
      ns = B.size() / sizeof(NucleusMember)    ;
      for (int i=0;i<ns;i++)                   {
        QString n                              ;
        int     q = nm[i].quantity             ;
        SUID    u = nm[i].uuid                 ;
        n = SC . getName                       (
              PlanTable(Names)                 ,
              "uuid"                           ,
              vLanguageId                      ,
              u                              ) ;
        NewTreeWidgetItem ( it )               ;
        it -> setData ( 0,Qt::UserRole,u     ) ;
        it -> setText ( 0,n                  ) ;
        it -> setText ( 1,QString::number(q) ) ;
        setAlignments   ( it )                 ;
        addTopLevelItem ( it )                 ;
      }                                        ;
    }                                          ;
    SC.close()                                 ;
  }                                            ;
  SC.remove()                                  ;
  Alert ( Done )                               ;
}

void N::NucleusEditor::New(void)
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

void N::NucleusEditor::Delete(void)
{
  QTreeWidgetItem * it = currentItem () ;
  nDropOut         ( IsNull(it) )       ;
  int index = indexOfTopLevelItem(it)   ;
  nDropOut         ( index < 0  )       ;
  takeTopLevelItem ( index      )       ;
  Save             (            )       ;
}

void N::NucleusEditor::Save(void)
{
  nDropOut ( ObjectUuid()<=0 )                             ;
  SqlConnection SC ( plan -> sql )                         ;
  if (SC.open("NucleusEditor","Save"))                     {
    QString    Q                                           ;
    QByteArray B                                           ;
    for (int i=0;i<topLevelItemCount();i++)                {
      QTreeWidgetItem * it                                 ;
      NucleusMember nm                                     ;
      it = topLevelItem ( i )                              ;
      nm . uuid     = nTreeUuid(it,0)                      ;
      nm . quantity = it->text(1).toInt()                  ;
      B  . append((const char *)&nm,sizeof(NucleusMember)) ;
    }                                                      ;
    Q = SC . sql . Update                                  (
          NucleusTable                                     ,
          SC.sql.Where(1,"uuid")                           ,
          1                                                ,
          "members"                                      ) ;
    SC . Prepare ( Q                        )              ;
    SC . Bind    ( "uuid"    , ObjectUuid() )              ;
    SC . Bind    ( "members" , B            )              ;
    SC . Exec    (                          )              ;
    SC . close   (                          )              ;
  }                                                        ;
  SC.remove()                                              ;
  Alert ( Done )                                           ;
}

bool N::NucleusEditor::Menu(QPoint pos)
{
  QAction   * aa                        ;
  MenuManager mm ( this )               ;
  QTreeWidgetItem * it = itemAt(pos)    ;
  ///////////////////////////////////////
  mm . add ( 101 , tr("Refresh")      ) ;
  if (topLevelItemCount()>0)            {
    mm . add ( 102 , tr("Save")       ) ;
    mm . add ( 103 , tr("Export")     ) ;
  }                                     ;
  mm . add ( 201 , tr("Add") )          ;
  if (NotNull(it))                      {
    mm . add ( 202 , tr("Delete") )     ;
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
      Save           (      )           ;
    break                               ;
    case 103                            :
      Export         (      )           ;
    break                               ;
    case 201                            :
      New            (      )           ;
    break                               ;
    case 202                            :
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

void N::NucleusEditor::doubleClicked(QTreeWidgetItem * item,int column)
{
  nDropOut ( column > 1 )                         ;
  if (column==0)                                  {
    QComboBox * cb                                ;
    cb = setComboBox                              (
           item                                   ,
           column                                 ,
           SIGNAL(activated   (int))              ,
           SLOT  (nameFinished(int))            ) ;
    cb -> blockSignals ( true )                   ;
    SqlConnection SC ( plan -> sql )              ;
    if (SC.open("NucleusEditor","setComboBox"))   {
      UUIDs   U                                   ;
      UUIDs   C                                   ;
      UUIDs   X                                   ;
      SUID    u                                   ;
      U = SC . Uuids                              (
            PlanTable(Particles)                  ,
            "uuid"                                ,
            SC.OrderByAsc("id")                 ) ;
      C = SC . Uuids                              (
            PlanTable(Composite)                  ,
            "uuid"                                ,
            SC.OrderByAsc("id")                 ) ;
      X << U[0]                                   ;
      X << C[0]                                   ;
      X << C[1]                                   ;
      U  . takeAt ( 0 )                           ;
      C  . takeAt ( 0 )                           ;
      C  . takeAt ( 1 )                           ;
      X << U                                      ;
      X << C                                      ;
      U  = X                                      ;
      foreach (u,U)                               {
        QString n = SC . getName                  (
                      PlanTable(Names)            ,
                      "uuid"                      ,
                      vLanguageId                 ,
                      u                         ) ;
        cb -> addItem ( n , u )                   ;
      }                                           ;
      SC.close()                                  ;
    }                                             ;
    SC.remove()                                   ;
    cb -> setInsertPolicy ( QComboBox::NoInsert ) ;
    cb -> setEditable     ( true                ) ;
    cb -> blockSignals    ( false               ) ;
    cb -> showPopup       (                     ) ;
  } else
  if (column==1)                                  {
    QSpinBox * sb                                 ;
    sb = setSpinBox                               (
          item                                    ,
          column                                  ,
          1                                       ,
          1000000                                 ,
          SIGNAL ( editingFinished  ()          ) ,
          SLOT   ( quantityFinished () )        ) ;
    sb -> blockSignals ( true                   ) ;
    sb -> setValue     ( item->text(1).toInt()  ) ;
    sb -> blockSignals ( false                  ) ;
  }                                               ;
}

void N::NucleusEditor::nameFinished(int index)
{
  if (IsNull(ItemEditing)) return                 ;
  QComboBox * cb = Casting(QComboBox,ItemWidget)  ;
  if (IsNull(cb)) return                          ;
  SUID    u = nTreeUuid(ItemEditing,0)            ;
  SUID    z = cb->itemData(index).toULongLong()   ;
  QString n = cb->currentText()                   ;
  ItemEditing -> setData ( 0 , Qt::UserRole , z ) ;
  ItemEditing -> setText ( 0 , n                ) ;
  if (u<=0) ItemEditing->setText(1,"1")           ;
  setAlignments ( ItemEditing )                   ;
  removeOldItem ( true , 0    )                   ;
  Save          (             )                   ;
}

void N::NucleusEditor::quantityFinished(void)
{
  if (IsNull(ItemEditing)) return                             ;
  QSpinBox * sb = Casting(QSpinBox,ItemWidget)                ;
  if (IsNull(sb)) return                                      ;
  ItemEditing -> setText ( 1 , QString::number(sb->value()) ) ;
  removeOldItem ( true , 0 )                                  ;
  Save          (          )                                  ;
}

QString N::NucleusEditor::toHtml(void)
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

void N::NucleusEditor::Export(void)
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
