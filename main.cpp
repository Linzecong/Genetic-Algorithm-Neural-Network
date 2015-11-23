#define POPSIZE 30
#define MUTRAT 0.1
#define CROSSRAT 0.7
#define NUM_MINE 100
#define WIDTH 800
#define HEIGHT 800
#define GENERATION_STEP 1000
#define NUM_ELITE 4
#define NUM_ELITE_COPY 1
#define SWEEPER_SIZE 20
#define MINE_SIZE 5
#define NUM_INPUT 4
#define NUM_OUTPUT 2

int NUM_HIDDENLAYER;
int NUM_NEURONSPERHIDDENLAYER;

#include "Widget.h"
#include <QApplication>
#include <QInputDialog>
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    NUM_HIDDENLAYER=QInputDialog::getInt(NULL,"Initialize...","Please enter the number of hidden layer:",1,0,10);
    NUM_NEURONSPERHIDDENLAYER=QInputDialog::getInt(NULL,"Initialize...","Please enter the number of neurons per hidden layer:",10,1,1000);
    Widget w;
    w.setFixedSize(825,825);
    w.show();
    return a.exec();
}
