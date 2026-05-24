#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QVector>
#include <QTouchEvent>
#include <QPoint>
#include <QRandomGenerator>
#include <QFont>

enum GameMode {
    PVP,
    PVE,
    NETWORK
};

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);

signals:
    void gameOver(QString winner);
    void turnChanged(bool isBlack);
    void aiMoveReady();

public slots:
    void restartGame();
    void undoMove();
    void setGameMode(GameMode mode);
    void aiPlay();
    void toggleDarkMode(bool isDark);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void touchEvent(QTouchEvent *event);

private:
    static const int BOARD_SIZE = 15;
    int board[BOARD_SIZE][BOARD_SIZE];
    bool isBlackTurn;
    bool gameFinished;
    GameMode gameMode;
    bool isDarkMode;

    struct Step { int x, y; };
    QVector<Step> steps;

    void initBoard();
    bool checkWin(int x, int y, int color);
    bool getGridPos(const QPoint &pos, int &row, int &col);
    void handleMove(int row, int col);

    int getScore(int x, int y, int color);
    QPoint findBestPos();
};

#endif // CHESSBOARD_H
