#include "chessboard.h"

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent), isBlackTurn(true), gameFinished(false), gameMode(PVP), isDarkMode(false)
{
    initBoard();
    setAttribute(Qt::WA_AcceptTouchEvents);
}

void ChessBoard::initBoard()
{
    for(int i = 0; i < BOARD_SIZE; ++i)
        for(int j = 0; j < BOARD_SIZE; ++j)
            board[i][j] = 0;
    isBlackTurn = true;
    gameFinished = false;
    steps.clear();
    update();
    emit turnChanged(true);
}

void ChessBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width(), h = height();
    int side = qMin(w, h);
    int offset = side / (BOARD_SIZE + 1);
    int grid = (side - 2 * offset) / (BOARD_SIZE - 1);

    painter.translate((w - side)/2, (h - side)/2);
    painter.fillRect(offset-5, offset-5, (BOARD_SIZE-1)*grid+10, (BOARD_SIZE-1)*grid+10, isDarkMode ? QColor(40,35,30) : QColor(210,180,140));

    QPen pen(isDarkMode ? QColor(180,160,140) : QColor(101,67,33), 1);
    painter.setPen(pen);
    for(int i=0;i<BOARD_SIZE;i++){
        painter.drawLine(offset, offset+i*grid, offset+(BOARD_SIZE-1)*grid, offset+i*grid);
        painter.drawLine(offset+i*grid, offset, offset+i*grid, offset+(BOARD_SIZE-1)*grid);
    }

    painter.setBrush(isDarkMode ? QColor(220,200,180) : QColor(80,50,20));
    int star[3]={3,7,11};
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            painter.drawEllipse(offset+star[i]*grid-4, offset+star[j]*grid-4,8,8);

    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++){
            if(board[i][j]==0) continue;
            int x=offset+j*grid, y=offset+i*grid;
            int r=grid/2-1;
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(0,0,0,80));
            painter.drawEllipse(x+2,y+2,r*2,r*2);

            QRadialGradient g(x-3,y-3,r);
            g.setColorAt(0, board[i][j]==1 ? QColor(80,80,80) : Qt::white);
            g.setColorAt(1, board[i][j]==1 ? Qt::black : QColor(220,220,220));
            painter.setBrush(g);
            painter.drawEllipse(QPoint(x,y),r,r);
        }
    }
}

void ChessBoard::mousePressEvent(QMouseEvent *e)
{
    if(gameFinished || e->button()!=Qt::LeftButton) return;
    int r,c;
    if(!getGridPos(e->pos(),r,c)) return;
    board[r][c] = isBlackTurn?1:2;
    steps.append({r,c});
    handleMove(r,c);
}

void ChessBoard::touchEvent(QTouchEvent *e)
{
    if(gameFinished) return;
    auto p = e->touchPoints().first().pos().toPoint();
    int r,c;
    if(!getGridPos(p,r,c)) return;
    board[r][c] = isBlackTurn?1:2;
    steps.append({r,c});
    handleMove(r,c);
}

bool ChessBoard::getGridPos(const QPoint &p, int &row, int &col)
{
    int w=width(),h=height();
    int side=qMin(w,h);
    int off=side/(BOARD_SIZE+1);
    int grid=(side-2*off)/(BOARD_SIZE-1);
    QPoint real=p-QPoint((w-side)/2,(h-side)/2);
    col=(real.x()-off+grid/2)/grid;
    row=(real.y()-off+grid/2)/grid;
    return row>=0&&row<BOARD_SIZE&&col>=0&&col<BOARD_SIZE&&board[row][col]==0;
}

void ChessBoard::handleMove(int r, int c)
{
    if(checkWin(r,c,board[r][c])){
        gameFinished=true;
        emit gameOver(board[r][c]==1?"黑棋":"白棋");
    }else{
        isBlackTurn=!isBlackTurn;
        emit turnChanged(isBlackTurn);
        if(gameMode==PVE&&!isBlackTurn&&!gameFinished)
            QTimer::singleShot(300,this,&ChessBoard::aiPlay);
    }
    update();
}

void ChessBoard::aiPlay()
{
    auto p=findBestPos();
    int r=p.x(),c=p.y();
    board[r][c]=2;
    steps.append({r,c});
    handleMove(r,c);
}

int ChessBoard::getScore(int x,int y,int color)
{
    int s=0;
    int dx[4]={1,0,1,-1}, dy[4]={0,1,1,1};
    for(int d=0;d<4;d++){
        int cnt=1;
        for(int i=1;i<5;i++){
            int nx=x+dx[d]*i, ny=y+dy[d]*i;
            if(nx>=0&&nx<15&&ny>=0&&ny<15&&board[nx][ny]==color) cnt++;
            else break;
        }
        for(int i=1;i<5;i++){
            int nx=x-dx[d]*i, ny=y-dy[d]*i;
            if(nx>=0&&nx<15&&ny>=0&&ny<15&&board[nx][ny]==color) cnt++;
            else break;
        }
        if(cnt==5)s+=100000;
        else if(cnt==4)s+=10000;
        else if(cnt==3)s+=1000;
        else if(cnt==2)s+=100;
    }
    return s;
}

QPoint ChessBoard::findBestPos()
{
    int max=-1; QPoint best(7,7);
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++){
            if(board[i][j])continue;
            int s1=getScore(i,j,2),s2=getScore(i,j,1);
            if(s1+s2>max){max=s1+s2;best={i,j};}
        }
    return best;
}

bool ChessBoard::checkWin(int x,int y,int c)
{
    int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    for(int d=0;d<4;d++){
        int cnt=1;
        for(int i=1;i<5;i++){
            int nx=x+dx[d]*i, ny=y+dy[d]*i;
            if(nx>=0&&nx<15&&ny>=0&&ny<15&&board[nx][ny]==c)cnt++;
            else break;
        }
        for(int i=1;i<5;i++){
            int nx=x-dx[d]*i, ny=y-dy[d]*i;
            if(nx>=0&&nx<15&&ny>=0&&ny<15&&board[nx][ny]==c)cnt++;
            else break;
        }
        if(cnt>=5)return true;
    }
    return false;
}

void ChessBoard::restartGame(){initBoard();}
void ChessBoard::undoMove()
{
    if(steps.isEmpty()||gameFinished)return;
    auto last=steps.last(); steps.pop_back();
    board[last.x][last.y]=0;
    isBlackTurn=!isBlackTurn;
    emit turnChanged(isBlackTurn);
    update();
}
void ChessBoard::setGameMode(GameMode m){gameMode=m;initBoard();}
void ChessBoard::toggleDarkMode(bool b){isDarkMode=b;update();}
