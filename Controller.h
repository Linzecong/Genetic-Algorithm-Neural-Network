#ifndef CONTROLLER
#define CONTROLLER
#include<vector>
#include "Alg.h"
#include "Minesweeper.h"

class Controller{
    std::vector<Genome> vecThePopulation;//��Ⱥ
    int NumSweepers;
    int NumWeightsInNN;
    Alg* MyAlg;
    int MovedTime;
public:
    Controller();
    std::vector<Minesweeper> vecSweepers;
    std::vector<std::vector<QVector2D> > vecMinesList;
    bool Update();//��һ��
    int GetFitest();//��ȡ���Ÿ���
    int Generations;
    int NumMines;
};

Controller::Controller(){
    //��ʼ��
    MovedTime=0;
    NumSweepers=POPSIZE;
    NumMines=NUM_MINE;
    Generations=0;
    //�½�ɨ�׻�
    for(int i=0;i<NumSweepers;i++)
        vecSweepers.push_back(Minesweeper());
    //��Ԫ����
    NumWeightsInNN=vecSweepers[0].GetNumberOfWeights();
    //��Ȩ�ص��ɻ��򣬴��ݸ��Ŵ��㷨
    MyAlg=new Alg(NumSweepers,MUTRAT,CROSSRAT,NumWeightsInNN);
    //��ȡ��Ⱥ
    vecThePopulation=MyAlg->GetChromos();
    //���Ŵ��㷨���ɵ�Ȩ�ظ��Ƶ�ɨ�׻�����Ԫ��
    for(int i=0;i<NumSweepers;i++)
        vecSweepers[i].PutWeights(vecThePopulation[i].vecWeights);
    //��ʼ�����ױ�
    for(int i=0;i<NumSweepers;i++){
        std::vector<QVector2D> temp;
        for(int i=0;i<NumMines;i++){
            temp.push_back(QVector2D(RandFloat()*(WIDTH-MINE_SIZE)+MINE_SIZE,RandFloat()*(WIDTH-MINE_SIZE)+MINE_SIZE));
        }
        vecMinesList.push_back(temp);
    }
}

bool Controller::Update(){
    //���ж�GENERATION_STEP���ж���һ�������ţ���������һ��
    if(MovedTime<GENERATION_STEP){
        for(int i=0;i<NumSweepers;i++){
            vecSweepers[i].Update(vecMinesList[i]);//ͨ�����������λ��
            int GrabHit=vecSweepers[i].CheckForMine(vecMinesList[i]);//����Ե����ף��������Ӧ��
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
        vecThePopulation=MyAlg->Epoch(vecThePopulation);//������һ��
        for(int i=0;i<NumSweepers;i++)//����ɨ�׻����Դ�
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
