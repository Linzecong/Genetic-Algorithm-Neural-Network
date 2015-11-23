#ifndef CONTROLLER
#define CONTROLLER
#include<vector>
#include "Alg.h"
#include "Minesweeper.h"

class Controller{
    std::vector<Genome> vecThePopulation;//种群
    int NumSweepers;
    int NumWeightsInNN;
    Alg* MyAlg;
    int MovedTime;
public:
    Controller();
    std::vector<Minesweeper> vecSweepers;
    std::vector<std::vector<QVector2D> > vecMinesList;
    bool Update();//下一代
    int GetFitest();//获取最优个体
    int Generations;
    int NumMines;
};

Controller::Controller(){
    //初始化
    MovedTime=0;
    NumSweepers=POPSIZE;
    NumMines=NUM_MINE;
    Generations=0;
    //新建扫雷机
    for(int i=0;i<NumSweepers;i++)
        vecSweepers.push_back(Minesweeper());
    //神经元总数
    NumWeightsInNN=vecSweepers[0].GetNumberOfWeights();
    //把权重当成基因，传递给遗传算法
    MyAlg=new Alg(NumSweepers,MUTRAT,CROSSRAT,NumWeightsInNN);
    //获取种群
    vecThePopulation=MyAlg->GetChromos();
    //把遗传算法生成的权重复制到扫雷机的神经元上
    for(int i=0;i<NumSweepers;i++)
        vecSweepers[i].PutWeights(vecThePopulation[i].vecWeights);
    //初始化地雷表
    for(int i=0;i<NumSweepers;i++){
        std::vector<QVector2D> temp;
        for(int i=0;i<NumMines;i++){
            temp.push_back(QVector2D(RandFloat()*(WIDTH-MINE_SIZE)+MINE_SIZE,RandFloat()*(WIDTH-MINE_SIZE)+MINE_SIZE));
        }
        vecMinesList.push_back(temp);
    }
}

bool Controller::Update(){
    //先行动GENERATION_STEP次判断哪一个是最优，后生成下一代
    if(MovedTime<GENERATION_STEP){
        for(int i=0;i<NumSweepers;i++){
            vecSweepers[i].Update(vecMinesList[i]);//通过神经网络更新位置
            int GrabHit=vecSweepers[i].CheckForMine(vecMinesList[i]);//如果吃到地雷，则更新适应性
            if(GrabHit>=0){
                vecSweepers[i].IncrementFitness(1);
                vecMinesList[i][GrabHit]=QVector2D(RandFloat()*(WIDTH-10)+10,RandFloat()*(WIDTH-10)+10);
            }
            vecThePopulation[i].Fitness = vecSweepers[i].Fitness;
        }
        MovedTime++;
    }
    else{
        MovedTime=0;
        Generations++;
        vecThePopulation=MyAlg->Epoch(vecThePopulation);//生成下一代
        for(int i=0;i<NumSweepers;i++)//更新扫雷机的脑袋
            vecSweepers[i].PutWeights(vecThePopulation[i].vecWeights);
    }
    return true;
}

int Controller::GetFitest(){
    double max=0;
    unsigned int temp=0;
    for(unsigned int i=0;i<vecSweepers.size();i++)
        if(max<vecSweepers[i].Fitness){
            temp=i;
            max=vecSweepers[i].Fitness;
        }
    return temp;
}

#endif // CONTROLLER
