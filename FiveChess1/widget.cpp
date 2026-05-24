#include "widget.h"
#include "ui_widget.h"   // 这行必须在第二行！！！

// 下面所有代码不动，只保证上面两行顺序！
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setStyleSheet(R"(
        QWidget#Widget {
            background-color: #f8f2e4;
        }
        QLabel#statusLabel {
            font-family: "微软雅黑";
            font-size: 20px;
            font-weight: bold;
            color: #6b3e26;
            padding: 10px 20px;
            background-color: rgba(255,255,255,0.8);
            border-radius: 15px;
            border: 1px solid #d4b89a;
            margin: 8px;
        }
        QPushButton {
            font-family: "微软雅黑";
            font-size: 16px;
            font-weight: bold;
            color: white;
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #b87333, stop:1 #804000);
            border: none;
            border-radius: 20px;
            padding: 12px 24px;
            margin: 6px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #d48b4a, stop:1 #995010);
        }
        QPushButton:pressed {
            background: #804000;
            padding: 10px 22px;
        }
        QPushButton#darkBtn {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #444, stop:1 #222);
        }
        QPushButton#darkBtn:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #555, stop:1 #333);
        }
    )");


    // 重点：必须连接棋盘信号！！！
    connect(ui->restartBtn, &QPushButton::clicked, ui->boardWidget, &ChessBoard::restartGame);
    connect(ui->undoBtn, &QPushButton::clicked, ui->boardWidget, &ChessBoard::undoMove);
    connect(ui->btnPvp, &QPushButton::clicked, this, [=](){ui->boardWidget->setGameMode(PVP);});
    connect(ui->btnPve, &QPushButton::clicked, this, [=](){ui->boardWidget->setGameMode(PVE);});
    connect(ui->darkBtn, &QPushButton::toggled, ui->boardWidget, &ChessBoard::toggleDarkMode);

    // 棋盘信号
    connect(ui->boardWidget, &ChessBoard::turnChanged, this, [=](bool isBlack){
        ui->statusLabel->setText(isBlack ? "当前轮到：黑棋" : "当前轮到：白棋");
    });
    connect(ui->boardWidget, &ChessBoard::gameOver, this, [=](QString winner){
        QMessageBox::information(this, "游戏结束", winner + "获胜！");
    });
}

Widget::~Widget() { delete ui; }
void Widget::onRestartBtnClicked() { ui->boardWidget->restartGame(); }
void Widget::onUndoBtnClicked() { ui->boardWidget->undoMove(); }
void Widget::onGameFinished(QString winner) { QMessageBox::information(this, "游戏结束", winner + " 获胜！"); }
void Widget::onTurnChanged(bool isBlack) { ui->statusLabel->setText("当前轮到：" + QString(isBlack ? "黑棋" : "白棋")); }
void Widget::onModePvpClicked() { ui->boardWidget->setGameMode(PVP); ui->statusLabel->setText("双人对战 | 轮到：黑棋"); }
void Widget::onModePveClicked() { ui->boardWidget->setGameMode(PVE); ui->statusLabel->setText("人机对战 | 你是黑棋"); }
void Widget::onDarkModeToggled(bool checked) { ui->boardWidget->toggleDarkMode(checked); }
