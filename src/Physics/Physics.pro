NAME         = Physics
TARGET       = $${NAME}

QT           = core
QT          += gui
QT          += network
QT          += sql
QT          += script
QT          += positioning
QT          += widgets
QT          += opengl
QT          += printsupport
QT          += multimedia
QT          += multimediawidgets
QT          += opengl
QT          += QtCUDA
QT          += QtOpenCV
QT          += Essentials
QT          += QtCalendar
QT          += QtGMP
QT          += QtGSL
QT          += QtAlgebra
QT          += QtDiscrete
QT          += QtFFT
QT          += Mathematics
QT          += QtFuzzy
QT          += QtFLP
QT          += QtFoundation
QT          += QtGeometry
QT          += QtGadgets
QT          += QtComponents
QT          += QtManagers
QT          += Science

load(qt_build_config)
load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/physics.h

SOURCES     += $${PWD}/nParticleLists.cpp
SOURCES     += $${PWD}/nMatterBox.cpp
SOURCES     += $${PWD}/nMatterPainter.cpp
SOURCES     += $${PWD}/nStandardMatter.cpp
SOURCES     += $${PWD}/nMatterContainer.cpp
SOURCES     += $${PWD}/nStandardParticles.cpp
SOURCES     += $${PWD}/nMesonLists.cpp
SOURCES     += $${PWD}/nBaryonLists.cpp
SOURCES     += $${PWD}/nCompositeParticles.cpp
SOURCES     += $${PWD}/nNucleusEditor.cpp
SOURCES     += $${PWD}/nNucleusList.cpp
SOURCES     += $${PWD}/nIsotopeList.cpp
SOURCES     += $${PWD}/CelestialsEditor.cpp
SOURCES     += $${PWD}/StarsEditor.cpp

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}
include ($${PWD}/../../Translations.pri)
