QT       += core gui\
            webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG -= qtquickcompiler

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    components/pageview.cpp \
    components/usercenterbox.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    components/aboutsoftwave.cpp \
    components/closeconfirm.cpp \
    components/controller.cpp \
    components/firstmenu.cpp \
    components/inbrowser.cpp \
    components/message.cpp \
    components/secondmenu.cpp \
    components/settingwindow.cpp \
    components/mssa_update.cpp \
    pages/analysis/list.cpp \
    pages/analysis/sales.cpp \
    pages/analysis/summary.cpp \
    pages/analysis/view.cpp \
    pages/categoryinform/categoryinform.cpp \
    pages/dashboard/dashboard.cpp \
    pages/goodslist/editgooditem.cpp \
    pages/goodslist/goodslist.cpp \
    pages/home/homepage.cpp \
    pages/import/import.cpp \
    pages/order/orderlist.cpp

HEADERS += \
    bridge.h \
    components/pageview.h \
    components/usercenterbox.h \
    loginwindow.h \
    mainwindow.h\
    framelesswindow.h \
    components/aboutsoftwave.h \
    components/closeconfirm.h \
    components/controller.h \
    components/firstmenu.h \
    components/inbrowser.h \
    components/message.h \
    components/mssa_update.h \
    components/secondmenu.h \
    components/settingwindow.h \
    pages/analysis/list.h \
    pages/analysis/sales.h \
    pages/analysis/summary.h \
    pages/analysis/view.h \
    pages/categoryinform/categoryinform.h \
    pages/dashboard/dashboard.h \
    pages/goodslist/editgooditem.h \
    pages/goodslist/goodslist.h \
    pages/home/homepage.h \
    pages/import/import.h \
    pages/order/orderlist.h

FORMS += \
    components/usercenterbox.ui \
    loginwindow.ui \
    mainwindow.ui \
    components/aboutsoftwave.ui \
    components/closeconfirm.ui \
    components/controller.ui \
    components/firstmenu.ui \
    components/inbrowser.ui \
    components/message.ui \
    components/mssa_update.ui \
    components/secondmenu.ui \
    components/settingwindow.ui \
    pages/analysis/list.ui \
    pages/analysis/sales.ui \
    pages/analysis/summary.ui \
    pages/analysis/view.ui \
    pages/categoryinform/categoryinform.ui \
    pages/dashboard/dashboard.ui \
    pages/goodslist/editgooditem.ui \
    pages/goodslist/goodslist.ui \
    pages/home/homepage.ui \
    pages/import/import.ui \
    pages/order/orderlist.ui

win32{
    LIBS += -luser32
    SOURCES += framelesswindow.cpp
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourse.qrc

RC_ICONS += favicon.ico

DISTFILES +=
