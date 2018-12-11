#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <vector>
#include <bits/stdc++.h>

const int n = 20;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void clearButtonPressed();
    void goButtonPressed();

private:
    int xStart = 0, yStart = 19, xFinish = 19, yFinish = 0;
    int horizontalMoves[4] = {0, 0, 1, -1};
    int verticalMoves[4] = {1, -1, 0, 0};
    bool graph[n][n];
    int used[n][n];
    int distances[n][n];
    bool inRectangle(int,int);
    int round(double);
    std::queue <std::pair<int,int>> queue;
    std::vector <std::pair<int,int>> answer;
    Ui::MainWindow *ui;
    bool needToDrawRoad;
    void deleteVertex(double,double);
    bool checkVertex(int, int);
    void fillFullGraf();
};

#endif // MAINWINDOW_H
