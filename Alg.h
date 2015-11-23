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
    std::vector<double> vecWeights;//基因，这里是神经元的权重
    double Fitness;//此基因的适应性
    Genome():Fitness(0){}
    Genome(std::vector<double> w,double f):vecWeights(w),Fitness(f){}
    //排序用
    friend bool operator<(const Genome& lhs,const Genome& rhs){
        return (lhs.Fitness<rhs.Fitness);
    }
};

class Alg{
    std::vector<Genome> vecPop;//种群
    unsigned int PopSize;//群大小
    int ChromoLength;//染色体长度（这里直接等于基因长度）
    double TotalFitness;//总适应性，轮盘算法用
    double BestFitness;//最高适应性
    int FittestGenome;//最高适应性基因的序号
    double MutationRate;//突变几率
    double CrossoverRate;//杂交几率
    int Generation;//代数
    void Crossover(const std::vector<double> &mum,const std::vector<double> &dad,std::vector<double> &baby1,std::vector<double> &baby2);//交叉互换，即重组几率
    void Mutate(std::vector<double> &chromo);//突变几率
    void GrabNBest(int NBest,const int NumCopies,std::vector<Genome> &myvecPop);//保留适应性最强的染色体
    void Reset();
    Genome GetChromoRulette();//通过轮盘算法，选出一条染色体
public:
    Alg(int popsize,double MutRat,double CrossRat,int numweights);
    std::vector<Genome>	Epoch(std::vector<Genome> &old_pop);
    std::vector<Genome>	GetChromos()const{return vecPop;}
};

void Alg::Crossover(const std::vector<double> &mum, const std::vector<double> &dad, std::vector<double> &baby1, std::vector<double> &baby2){
    //没有发生基因重组，那么直接返回双亲
    if((RandFloat()>CrossoverRate)||(mum==dad)){
        baby1 = mum;
        baby2 = dad;
        return;
    }
    //随机选取一段进行互换
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
    //对每一个基因进行一定概率的突变，改编权重
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
    //NBest为精英数量
    //Copies为为精英复制的数量
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
            vecPop[i].vecWeights.push_back(RandomClamped());//赋予随机权重
    }
}

std::vector<Genome> Alg::Epoch(std::vector<Genome> &old_pop){
    vecPop=old_pop;
    Reset();
    std::sort(vecPop.begin(),vecPop.end());//排序，选精英
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
