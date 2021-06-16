/****************************************************************************
 *                                                                          *
 * Copyright (C) 2015 Neutrino International Inc.                           *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_PHYSICS_H
#define QT_PHYSICS_H

#include <QtManagers>
#include <Science>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_PHYSICS_LIB)
#      define Q_PHYSICS_EXPORT Q_DECL_EXPORT
#    else
#      define Q_PHYSICS_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_PHYSICS_EXPORT
#endif


namespace N
{

class Q_PHYSICS_EXPORT MatterPainter      ;
class Q_PHYSICS_EXPORT MatterBox          ;
class Q_PHYSICS_EXPORT MatterContainer    ;
class Q_PHYSICS_EXPORT ParticleLists      ;
class Q_PHYSICS_EXPORT MesonLists         ;
class Q_PHYSICS_EXPORT BaryonLists        ;
class Q_PHYSICS_EXPORT CompositeParticles ;
class Q_PHYSICS_EXPORT NucleusList        ;
class Q_PHYSICS_EXPORT NucleusEditor      ;
class Q_PHYSICS_EXPORT IsotopeList        ;
class Q_PHYSICS_EXPORT CelestialsEditor   ;
class Q_PHYSICS_EXPORT StarsEditor        ;

class Q_PHYSICS_EXPORT MatterPainter
{
  public:

    explicit        MatterPainter (void) ;
    virtual        ~MatterPainter (void) ;

    virtual void    Paint    (QPainter & painter,QRect region) = 0 ;
    virtual bool    Menu     (QWidget * widget,QPoint pos) = 0 ;
    virtual bool    FocusIn  (QWidget * widget) = 0 ;
    virtual bool    FocusOut (QWidget * widget) = 0 ;
    virtual QString toolTip  (void) = 0 ;

  protected:

  private:

};

class Q_PHYSICS_EXPORT MatterBox : public Widget
{
  Q_OBJECT
  public:

    explicit     MatterBox        (StandardConstructor);
    virtual     ~MatterBox        (void);

    MatterPainter * Painter       (void) const ;
    void            setPainter    (MatterPainter * painter) ;

  protected:

    MatterPainter * matter ;

    virtual void paintEvent       (QPaintEvent * event) ;

    virtual void focusInEvent     (QFocusEvent       * event) ;
    virtual void focusOutEvent    (QFocusEvent       * event) ;

    virtual void contextMenuEvent (QContextMenuEvent * event) ;

    virtual void Configure        (void) ;
    virtual bool FocusIn          (void) ;
    virtual bool FocusOut         (void) ;

  private:

  public slots:

  protected slots:

    virtual bool Menu             (QPoint pos) ;

  private slots:

  signals:

};

class Q_PHYSICS_EXPORT MatterContainer : public Widget
{
  Q_OBJECT
  public:

    explicit     MatterContainer  (StandardConstructor);
    virtual     ~MatterContainer  (void);

  protected:

    QMap<SUID       ,MatterBox *> MatterUuid ;
    QMap<MatterBox *,QRect      > MatterRect ;

    virtual void focusInEvent     (QFocusEvent       * event) ;
    virtual void focusOutEvent    (QFocusEvent       * event) ;

    virtual void contextMenuEvent (QContextMenuEvent * event) ;

    virtual void resizeEvent       (QResizeEvent      * event) ;
    virtual void showEvent         (QShowEvent        * event) ;

    virtual void Configure        (void) ;
    virtual bool FocusIn          (void) ;
    virtual bool FocusOut         (void) ;

  private:

  public slots:

    virtual bool Relocation       (void) ;

    virtual void addMatter        (SUID uuid,MatterBox * box,QRect rect) ;

  protected slots:

    virtual bool Menu             (QPoint pos) ;

  private slots:

  signals:

};

class Q_PHYSICS_EXPORT StandardMatter : public QObject
                                      , public MatterPainter
{
  Q_OBJECT
  public:

    int     Type      ;
    QColor  Border    ;
    QString Fullname  ;
    QString Symbol    ;
    UUIDs   Particles ;

    explicit StandardMatter   (QObject * parent,int id) ;
    virtual ~StandardMatter   (void) ;

    virtual void    Paint     (QPainter & painter,QRect region) ;
    virtual bool    Menu      (QWidget * widget,QPoint pos) ;
    virtual bool    FocusIn   (QWidget * widget) ;
    virtual bool    FocusOut  (QWidget * widget) ;
    virtual QString toolTip   (void) ;

    virtual  void setParticle (int id) ;

  protected:

    void PaintName            (QPainter & painter,QString s,QRect region) ;
    void PaintSymbol          (QPainter & painter,QString s,QRect region) ;

  private:

  public slots:

  protected slots:

  private slots:

  signals:

};

class Q_PHYSICS_EXPORT StandardParticles : public MatterContainer
{
  Q_OBJECT
  public:

    explicit      StandardParticles   (StandardConstructor);
    virtual      ~StandardParticles   (void);

    virtual QSize sizeHint            (void) const ;

  protected:

    SMAPs PositionMaps ;

    virtual void  paintEvent          (QPaintEvent * event) ;

    virtual void  Configure           (void) ;
    virtual bool  FocusIn             (void) ;
    virtual bool  FocusOut            (void) ;

    virtual StandardMatter * standard (int i) ;

  private:

  public slots:

    virtual bool  Relocation          (void) ;

  protected slots:

    virtual bool  Menu                (QPoint pos) ;

  private slots:

  signals:

};

class Q_PHYSICS_EXPORT ParticleLists : public TreeDock
{
  Q_OBJECT
  public:

    explicit     ParticleLists  (StandardConstructor);
    virtual     ~ParticleLists  (void);

  protected:

    virtual void Configure      (void) ;
    virtual bool FocusIn        (void) ;
    virtual void run            (int Type,ThreadData * Data) ;

  private:

  public slots:

    virtual bool startup        (void) ;
    virtual void List           (void) ;
    virtual void Export         (void) ;
    virtual void modifyFont     (void) ;

    virtual void HeaderMenu     (QHeaderView * view,QPoint pos) ;

  protected slots:

    virtual bool Menu           (QPoint pos) ;
    virtual void doubleClicked  (QTreeWidgetItem * item,int column) ;
    virtual void nameFinished   (void) ;
    virtual void symbolFinished (void) ;

  private slots:

  signals:

};

class Q_PHYSICS_EXPORT MesonLists : public TreeWidget
{
  Q_OBJECT
  public:

    explicit     MesonLists    (StandardConstructor) ;
    virtual     ~MesonLists    (void) ;

  protected:

    EMAPs       FullColors ;
    BMAPs       AntiColors ;
    XMAPs       AntiQuarks ;
    BoundStates Bounds     ;

    virtual void Configure     (void) ;
    virtual bool FocusIn       (void) ;

    int          ColorCharge   (int id) ;

  private:

  public slots:

    virtual void add           (int positive,int anti,bool inclusive) ;

    virtual void Export        (void) ;
    virtual void Synchronize   (void) ;
    virtual void Stable        (void) ;
    virtual void Mesons        (void) ;
    virtual void Possible      (void) ;

  protected slots:

    virtual bool Menu          (QPoint pos) ;
    virtual void doubleClicked (QTreeWidgetItem * item,int column) ;
    virtual void boundChanged  (int id) ;

  private slots:

  signals:

};

class Q_PHYSICS_EXPORT BaryonLists : public TreeWidget
{
  Q_OBJECT
  public:

    explicit     BaryonLists   (StandardConstructor) ;
    virtual     ~BaryonLists   (void) ;

    virtual QSize sizeHint     (void) const ;

  protected:

    EMAPs       FullColors ;
    BMAPs       AntiColors ;
    XMAPs       AntiQuarks ;
    BoundStates Bounds     ;

    virtual void Configure     (void) ;
    virtual bool FocusIn       (void) ;

    int          ColorCharge   (int id) ;

  private:

  public slots:

    virtual void add           (int a,int b,int c,bool inclusive) ;

    virtual void Export        (void) ;
    virtual void Synchronize   (void) ;
    virtual void Baryons       (void) ;
    virtual void Possible      (void) ;

  protected slots:

    virtual bool Menu          (QPoint pos) ;
    virtual void doubleClicked (QTreeWidgetItem * item,int column) ;
    virtual void boundChanged  (int id) ;

  private slots:

  signals:

};

class Q_PHYSICS_EXPORT CompositeParticles : public TreeWidget
{
  Q_OBJECT
  public:

    explicit CompositeParticles (StandardConstructor) ;
    virtual ~CompositeParticles (void) ;

    virtual QSize sizeHint      (void) const ;

  protected:

    virtual void Configure      (void) ;
    virtual bool FocusIn        (void) ;

    virtual void run            (int Type,ThreadData * data) ;

  private:

  public slots:

    virtual bool startup        (void) ;
    virtual void New            (void) ;
    virtual void List           (void) ;

  protected slots:

    virtual bool Menu           (QPoint pos) ;
    virtual void doubleClicked  (QTreeWidgetItem * item,int column) ;

    virtual void nameFinished   (void) ;
    virtual void symbolFinished (void) ;

  private slots:

  signals:

    void Bounded      (QString name,SUID uuid) ;
    void Properties   (QString name,SUID uuid) ;
    void Symbols      (QString name,SUID uuid) ;
    void Translations (QString title,UUIDs & Uuids);

};

class Q_PHYSICS_EXPORT NucleusList : public TreeDock
{
  Q_OBJECT
  public:

    explicit     NucleusList    (StandardConstructor) ;
    virtual     ~NucleusList    (void) ;

    virtual QSize sizeHint      (void) const ;

    QString       toHtml        (void) ;

  protected:

    virtual void Configure      (void) ;
    virtual bool FocusIn        (void) ;

    virtual void run            (int Type,ThreadData * data) ;

  private:

    QString      Count          (SUID uuid,QByteArray & B) ;

  public slots:

    virtual bool startup        (void) ;
    virtual void List           (void) ;
    virtual void New            (void) ;
    virtual void Export         (void) ;

  protected slots:

    virtual bool Menu           (QPoint pos) ;
    virtual void doubleClicked  (QTreeWidgetItem * item,int column) ;
    virtual void nameFinished   (void) ;
    virtual void symbolFinished (void) ;

  private slots:

  signals:

    void Edit                   (QString name,SUID uuid) ;
    void Isotopes               (QString name,SUID uuid) ;
    void Translations           (QString title,UUIDs & Uuids) ;

};

class Q_PHYSICS_EXPORT NucleusEditor : public TreeWidget
                                     , public Object
{
  Q_OBJECT
  public:

    explicit     NucleusEditor    (StandardConstructor) ;
    virtual     ~NucleusEditor    (void) ;

    virtual QSize sizeHint        (void) const ;

    QString       toHtml          (void) ;

  protected:

    virtual void Configure        (void) ;
    virtual bool FocusIn          (void) ;

    virtual void run              (int Type,ThreadData * data) ;

  private:

  public slots:

    virtual bool startup          (void) ;
    virtual void List             (void) ;
    virtual void New              (void) ;
    virtual void Delete           (void) ;
    virtual void Save             (void) ;
    virtual void Export           (void) ;

  protected slots:

    virtual bool Menu             (QPoint pos) ;
    virtual void doubleClicked    (QTreeWidgetItem * item,int column) ;
    virtual void nameFinished     (int index) ;
    virtual void quantityFinished (void) ;

  private slots:

  signals:

};

class Q_PHYSICS_EXPORT IsotopeList : public TreeWidget
                                   , public Object
{
  Q_OBJECT
  public:

    explicit     IsotopeList    (StandardConstructor) ;
    virtual     ~IsotopeList    (void) ;

    virtual QSize sizeHint      (void) const ;

    QString       toHtml        (void) ;

  protected:

    UUIDs Nucleons ;
    NAMEs Nuclecus ;

    virtual void Configure      (void) ;
    virtual bool FocusIn        (void) ;

    virtual void run            (int Type,ThreadData * data) ;

  private:

    void         Save           (QTreeWidgetItem * item) ;

  public slots:

    virtual bool startup        (void) ;
    virtual void List           (void) ;
    virtual void New            (void) ;
    virtual void Delete         (void) ;
    virtual void Export         (void) ;

  protected slots:

    virtual bool Menu           (QPoint pos) ;
    virtual void doubleClicked  (QTreeWidgetItem * item,int column) ;
    virtual void isotopeFinished (int index) ;
    virtual void nucleonFinished (int index) ;

  private slots:

  signals:

};

class Q_PHYSICS_EXPORT CelestialsEditor : public ItemEditor
{
  Q_OBJECT
  public:

    explicit CelestialsEditor (StandardConstructor) ;
    virtual ~CelestialsEditor (void);

  protected:

    virtual void Configure    (void) ;

    virtual bool dropStars    (QWidget * widget,QPointF pos,const UUIDs & Uuids) ;

  private:

  public slots:

  protected slots:

    virtual bool Menu         (QPoint pos) ;

  private slots:

  signals:

    void Edit                 (QString name,SUID uuid) ;
    void Translations         (QString title,UUIDs & Uuids);

};

class Q_PHYSICS_EXPORT StarsEditor : public TreeDock
                                   , public Group
                                   , public GroupItems
{
  Q_OBJECT
  public:

    explicit StarsEditor             (StandardConstructor) ;
    virtual ~StarsEditor             (void);

  protected:

    UUIDs  TypeUuids  ;
    NAMEs  StarTypes  ;
    QPoint dragPoint  ;
    bool   dropAction ;

    virtual bool FocusIn             (void) ;
    virtual void Configure           (void) ;

    virtual bool        hasItem      (void) ;
    virtual bool        startDrag    (QMouseEvent * event) ;
    virtual bool        fetchDrag    (QMouseEvent * event) ;
    virtual QMimeData * dragMime     (void) ;
    virtual void        dragDone     (Qt::DropAction dropIt,QMimeData * mime) ;
    virtual bool        finishDrag   (QMouseEvent * event) ;

    virtual bool        acceptDrop   (QWidget * source,const QMimeData * mime);
    virtual bool        dropNew      (QWidget * source,const QMimeData * mime,QPoint pos) ;

    virtual bool        dropStars    (QWidget * widget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool        dropPictures (QWidget * widget,QPointF pos,const UUIDs & Uuids) ;

    virtual void        run          (int Type,ThreadData * data) ;

  private:

  public slots:

    virtual bool startup             (void) ;
    virtual void List                (void) ;
    virtual void Insert              (void) ;
    virtual void Delete              (void) ;
    virtual void Paste               (void) ;

    virtual bool RunCommand          (VarArgs & arguments) ;
    virtual void Append              (SUID type,QStringList & Names) ;

  protected slots:

    virtual bool Menu                (QPoint pos) ;
    virtual void doubleClicked       (QTreeWidgetItem * item,int column) ;
    virtual void starFinished        (void) ;
    virtual void typeFinished        (int index) ;
    virtual void Finalize            (QTreeWidgetItem * item) ;

  private slots:

  signals:

    void Edit                        (QString name,SUID uuid) ;
    void Gallery                     (QString name,SUID uuid,int type) ;
    void Translations                (QString title,UUIDs & Uuids) ;

};

}

QT_END_NAMESPACE

#endif
