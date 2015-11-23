#ifndef ALG
#define ALG
#include<vector>
#include<algorithm>

int RandInt(int a,int b){
    int num=0;
    num=1+rand()%10;
    if(num>5)
        return a;
    else
        return b;
}

double RandomClamped(){
    return -1+2*(rand()/((double)RAND_MAX+1));
}

float RandFloat() {
    return rand()/((float)RAND_MAX+1);
}

void Clamp(double &arg, double min, double max){
    if (arg < min){
        arg = min;
    }
    if (arg > max){
        arg = max;
    }
}

struct Genome{
    std::vector<double> vecWeights;//������������Ԫ��Ȩ��
    double Fitness;//�˻������Ӧ��
    Genome():Fitness(0){}
    Genome(std::vector<double> w,double f):vecWeights(w),Fitness(f){}
    //������
    friend bool operator<(const Genome& lhs,const Genome& rhs){
        return (lhs.Fitness<rhs.Fitness);
    }
};

class Alg{
    std::vector<Genome> vecPop;//��Ⱥ
    unsigned int PopSize;//Ⱥ��С
    int ChromoLength;//Ⱦɫ�峤�ȣ�����ֱ�ӵ��ڻ��򳤶ȣ�
    double TotalFitness;//����Ӧ�ԣ������㷨��
    double BestFitness;//�����Ӧ��
    int FittestGenome;//�����Ӧ�Ի�������
    double MutationRate;//ͻ�伸��
    double CrossoverRate;//�ӽ�����
    int Generation;//����
    void Crossover(const std::vector<double> &mum,const std::vector<double> &dad,std::vector<double> &baby1,std::vector<double> &baby2);//���滥���������鼸��
    void Mutate(std::vector<double> &chromo);//ͻ�伸��
    void GrabNBest(int NBest,const int NumCopies,std::vector<Genome> &myvecPop);//������Ӧ����ǿ��Ⱦɫ��
    void Reset();
    Genome GetChromoRulette();//ͨ�������㷨��ѡ��һ��Ⱦɫ��
public:
    Alg(int popsize,double MutRat,double CrossRat,int numweights);
    std::vector<Genome>	Epoch(std::vector<Genome> &old_pop);
    std::vector<Genome>	GetChromos()const{return vecPop;}
};

void Alg::Crossover(const std::vector<double> &mum, const std::vector<double> &dad, std::vector<double> &baby1, std::vector<double> &baby2){
    //û�з����������飬��ôֱ�ӷ���˫��
    if((RandFloat()>CrossoverRate)||(mum==dad)){
        baby1 = mum;
        baby2 = dad;
        return;
    }
    //���ѡȡһ�ν��л���
    int cp=RandInt(0,ChromoLength-1);
    for(int i=0;i<cp;i++){
        baby1.push_back(mum[i]);
        baby2.push_back(dad[i]);
    }
    for(unsigned int i=cp;i<mum.size();i++){
        baby1.push_back(dad[i]);
        baby2.push_back(mum[i]);
    }
}

void Alg::Mutate(std::vector<double> &chromo){
    //��ÿһ���������һ�����ʵ�ͻ�䣬�ı�Ȩ��
    for(unsigned int i=0;i<chromo.size();i++)
        if(RandFloat()<0.1)
            chromo[i]+=(RandomClamped()*0.3);
}

Genome Alg::GetChromoRulette(){
    double Slice=(double)(RandFloat()*TotalFitness);
    Genome TheChosenOne;
    double FitnessSoFar=0;
    for(unsigned int i=0;i<PopSize;i++){
        FitnessSoFar+=vecPop[i].Fitness;
        if(FitnessSoFar>=Slice){
            TheChosenOne=vecPop[i];
            break;
        }
    }
    return TheChosenOne;
}

void Alg::GrabNBest(int NBest, const int NumCopies, std::vector<Genome> &myvecPop){
    //NBestΪ��Ӣ����
    //CopiesΪΪ��Ӣ���Ƶ�����
    while(NBest--)
        for(int i=0;i<NumCopies;i++)
            myvecPop.push_back(vecPop[(PopSize-1)-NBest]);
}

void Alg::Reset(){
    TotalFitness=0;
    BestFitness=0;

}

Alg::Alg(int popsize, double MutRat, double CrossRat, int numweights){
    PopSize=popsize;
    MutationRate=MutRat;
    CrossoverRate=CrossRat;
    ChromoLength=numweights;
    TotalFitness=0;
    Generation=0;
    FittestGenome=0;
    BestFitness=0;
    for(unsigned int i=0;i<PopSize;i++){
        vecPop.push_back(Genome());
        for(int j=0;j<ChromoLength;j++)
            vecPop[i].vecWeights.push_back(RandomClamped());//�������Ȩ��
    }
}

std::vector<Genome> Alg::Epoch(std::vector<Genome> &old_pop){
    vecPop=old_pop;
    Reset();
    std::sort(vecPop.begin(),vecPop.end());//����ѡ��Ӣ
    std::vector<Genome> vecNewPop;
    GrabNBest(NUM_ELITE,NUM_ELITE_COPY,vecNewPop);
    while(vecNewPop.size()<PopSize){
        Genome mum = GetChromoRulette();
        Genome dad = GetChromoRulette();
        std::vector<double>	baby1,baby2;
        Crossover(mum.vecWeights,dad.vecWeights, baby1, baby2);
        Mutate(baby1);
        Mutate(baby2);
        vecNewPop.push_back(Genome(baby1, 0));
        vecNewPop.push_back(Genome(baby2, 0));
    }
    vecPop = vecNewPop;
    return vecPop;
}

#endif // ALG
