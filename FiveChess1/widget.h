#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "chessboard.h"

namespace Ui {
class Widget;  // 只前向声明！！！
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onRestartBtnClicked();
    void onUndoBtnClicked();
    void onGameFinished(QString winner);
    void onTurnChanged(bool isBlack);
    void onModePvpClicked();
    void onModePveClicked();
    void onDarkModeToggled(bool checked);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
