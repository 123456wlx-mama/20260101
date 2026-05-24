QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    widget.cpp \
    chessboard.cpp  # 新增

HEADERS += \
    widget.h \
    chessboard.h    # 新增

FORMS += \
    widget.ui

android {
    QT += androidextras
}
