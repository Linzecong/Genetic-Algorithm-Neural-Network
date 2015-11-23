#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QHBoxLayout>
#include<vector>
#include<QVector2D>
#include<QTimer>
#include"Minesweeper.h"
#include"Controller.h"
class Widget : public QWidget{
public:
    Widget();
    QTimer* Timer;
    QGraphicsScene Scene;
    QGraphicsView View;
    QHBoxLayout* Layout;
    Controller* System;
    void Render(Minesweeper Sweepers, std::vector<QVector2D> Mines);
    void Round();
    QGraphicsEllipseItem* Sweeper;
    std::vector<QGraphicsRectItem*> MinesList;
    QGraphicsTextItem* TestItem;
};

Widget::Widget(){
    Layout=new QHBoxLayout;
    Layout->addWidget(&View);
    View.setScene(&Scene);
    this->setLayout(Layout);
    View.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    View.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    Scene.addLine(0,0,800,0);
    Scene.addLine(0,0,0,800);
    Scene.addLine(800,0,800,800);
    Scene.addLine(800,800,0,800);
    System=new Controller;
    Timer=new QTimer;
    Timer->setInterval(5);
    connect(Timer,&QTimer::timeout,this,&Widget::Round);
    Timer->start();
    Sweeper=Scene.addEllipse(0,0,20,20,QPen(Qt::blue),QBrush(Qt::blue));
    for(int i=0;i<System->NumMines;i++)
      MinesList.push_back(Scene.addRect(0,0,5,5,QPen(Qt::black),QBrush(Qt::black)));
    TestItem=Scene.addText(QString::number(System->Generations)+" ");
}

void Widget::Round(){
     System->Update();
     Render(System->vecSweepers[System->GetFitest()],System->vecMinesList[System->GetFitest()]);
}

void Widget::Render(Minesweeper Sweepers, std::vector<QVector2D> Mines){
    Sweeper->setPos(Sweepers.Position.toPointF()-QPointF(10,10));
    for(unsigned int i=0;i<Mines.size();i++)
       MinesList[i]->setPos(Mines[i].toPointF()-QPointF(2.5,2.5));
     TestItem->setPlainText(QString::number(System->Generations)+" "+QString::number(Sweepers.Fitness));
}

#endif // WIDGET_H
