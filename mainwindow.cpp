#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    needToDrawRoad = false;
    ui->setupUi(this);
    fillFullGraf();
    connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(clearButtonPressed()));
    connect(ui->goButton, SIGNAL(pressed()), this, SLOT(goButtonPressed()));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::fillFullGraf(){
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            graph[i][j]= 1;
            distances[i][j] = -1,
            used[i][j] = 0;
        }
    }
}

int MainWindow::round(double x){
    return ((x-(int)x)<0.5?(int)x:(int)(x+1));
}

void MainWindow::deleteVertex(double x, double y){
    graph[round(x/20-1)][round(y/20-1)]=0;
}

bool MainWindow::checkVertex(int x, int y){
    return graph[round(x/20-1)][round(y/20-1)];
}

bool MainWindow::inRectangle(int x, int y) {
    return (0 <= x && x < n && 0 <= y && y < n);
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    for (int x=20; x<420; x+=20){
        for (int y=20; y<420; y+=20)
            if(checkVertex(x,y)){
                painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
                painter.drawEllipse(x-2,y-2,4,4);}
            else {
                painter.setBrush(QBrush(Qt::black));
                painter.drawEllipse(x-2,y-2,4,4);
            }
    }
    painter.setBrush(QBrush(Qt::white));
    painter.drawEllipse(20-3, 400-3, 6, 6);
    painter.drawEllipse(400-3, 20-3, 6, 6);

    if(needToDrawRoad) {
        int x1,y1,x2,y2;
        for (int i = 0; i < answer.size()-1; ++i){
            x1 = 20*(answer[i].first+1);
            y1 = 20*(answer[i].second+1);
            x2 = 20*(answer[i+1].first+1);
            y2 = 20*(answer[i+1].second+1);
            painter.drawLine(x1,y1,x2,y2);
        }
        needToDrawRoad=false;
        answer.clear();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    deleteVertex(event->x(),event->y());
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            distances[i][j] = -1,
            used[i][j] = 0;
        }
    }
    answer.clear();
    repaint();
}

void MainWindow::clearButtonPressed(){
    fillFullGraf();
    answer.clear();
    repaint();
}

void MainWindow::goButtonPressed(){
    needToDrawRoad = true;
    queue.push({xStart, yStart});
    used[xStart][yStart] = 1;
    distances[xStart][yStart] = 0;
    while (!queue.empty()) {
        std::pair<int,int> temp = queue.front();
        queue.pop();
        for (int i = 0; i < 4; ++i) {
            int newx = temp.first + horizontalMoves[i], newy = temp.second + verticalMoves[i];
            if (inRectangle(newx, newy)&&!used[newx][newy] && graph[newx][newy]) {
                distances[newx][newy] = distances[temp.first][temp.second] + 1;
                used[newx][newy] = 1;
                queue.push({newx, newy});
            }
        }
    }
    if (distances[xFinish][yFinish] == -1) {
        needToDrawRoad=false;
        QMessageBox::information(0,"Robot in deadlock!", "Road will be clear.");
        clearButtonPressed();
        return;
    }
    std::pair<int,int> current = {xFinish, yFinish};
    while (current.first != xStart || current.second != yStart) {
        answer.push_back(current);
        for (int i = 0; i < 4; ++i) {
            int newx = current.first + horizontalMoves[i], newy = current.second + verticalMoves[i];
            int oldx = current.first, oldy = current.second;
            if (distances[oldx][oldy] == distances[newx][newy] + 1) {
                current = {newx, newy};
                break;
            }
        }
    }
    answer.push_back(current);
    repaint();
}
