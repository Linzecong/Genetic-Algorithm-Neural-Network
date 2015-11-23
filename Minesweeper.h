#ifndef MINESWEEPER
#define MINESWEEPER
#include<QVector2D>
#include "Neuralnet.h"
#include<math.h>

class Minesweeper{
    NeuralNet Brain;//神经网络，即扫雷机的大脑
    QVector2D LookAt;//面朝哪
    double Rotation;//车的各种参数
    double Speed;
    double LTrack;
    double RTrack;
    int ClosestMine;//离它最近的地雷
public:
    Minesweeper();
    double Fitness;
    QVector2D Position;//位置
    int CheckForMine(std::vector<QVector2D> mines);
    bool Update(std::vector<QVector2D> mines);
    QVector2D GetClosestMine(std::vector<QVector2D> object);//获得最近的地雷位置，并用向量表示
    void Reset();
    void IncrementFitness(double val){Fitness+=val;}
    void PutWeights(std::vector<double> &w){Brain.PutWeights(w);}
    int GetNumberOfWeights()const{return Brain.GetNumberOfWeights();}
};

int Minesweeper::CheckForMine(std::vector<QVector2D> mines){
    QVector2D len=Position-mines[ClosestMine];
    if(len.length()<(SWEEPER_SIZE/2+MINE_SIZE/2))
        return ClosestMine;
    return -1;
}

Minesweeper::Minesweeper(){
    Rotation=(RandFloat()*3.141592*2);
    LTrack=0.16;
    RTrack=0.16;
    Fitness=0;
    Position=QVector2D(RandFloat()*WIDTH,RandFloat()*HEIGHT);
}

bool Minesweeper::Update(std::vector<QVector2D> mines){
    std::vector<double> inputs;
    QVector2D ClostestMine=GetClosestMine(mines);
    ClostestMine.normalize();
    //向神经网络输入四个参数
    inputs.push_back(ClostestMine.x());
    inputs.push_back(ClostestMine.y());
    inputs.push_back(LookAt.x());
    inputs.push_back(LookAt.y());
    //获得输出，并复值到车子的参数
    std::vector<double> output=Brain.Update(inputs);
    LTrack=output[0];
    RTrack=output[1];
    double RotForce=LTrack-RTrack;
    Clamp(RotForce,-0.3,0.3);
    Speed=LTrack+RTrack;
    Rotation+=RotForce;
    LookAt.setX(-sin(Rotation));
    LookAt.setY(cos(Rotation));
    Position+=(LookAt*Speed);
    if(Position.x()>WIDTH-SWEEPER_SIZE/2)Position.setX(SWEEPER_SIZE/2);
    if(Position.x()<SWEEPER_SIZE/2)Position.setX(WIDTH-SWEEPER_SIZE/2);
    if(Position.y()>HEIGHT-SWEEPER_SIZE/2)Position.setY(SWEEPER_SIZE/2);
    if(Position.y()<SWEEPER_SIZE/2)Position.setY(HEIGHT-SWEEPER_SIZE/2);
    return true;
}

QVector2D Minesweeper::GetClosestMine(std::vector<QVector2D> object){
    double closest_so_far=99999;
    QVector2D ClosesrObject(0,0);
    for(unsigned int i=0;i<object.size();i++){
        QVector2D temp=object[i]-Position;
        double len_to_object=temp.length();
        if(len_to_object<closest_so_far){
            closest_so_far=len_to_object;
            ClosesrObject=Position-object[i];
            ClosestMine=i;
        }
    }
    return ClosesrObject;
}

void Minesweeper::Reset(){
    Position=QVector2D(RandFloat()*WIDTH,RandFloat()*HEIGHT);
    Fitness=0;
    Rotation=(RandFloat()*3.141592*2);
}

#endif // MINESWEEPER
