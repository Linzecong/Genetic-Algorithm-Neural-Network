#ifndef NEURALNET
#define NEURALNET
#include<vector>
#include <stdlib.h>
#include <math.h>
#include"Alg.h"

//神经元
struct Neuron{
    int NumInputs;//每个神经元的输入数量
    std::vector<double> vecWeight;//每个输入的权重
    Neuron(int Inputs):NumInputs(Inputs){
        for(int i=0;i<Inputs+1;i++)
            vecWeight.push_back(RandomClamped());//随机生成权重
    }
};

//神经层
struct NeuronLayer{
    int NumNeurons;//每层拥有的神经元数量
    std::vector<Neuron> vecNeurons;//储存神经元
    NeuronLayer(int NeuronNum,int NeuronInputNum):NumNeurons(NeuronNum){
        for(int i=0;i<NeuronNum;i++)
            vecNeurons.push_back(Neuron(NeuronInputNum));
    }
};

//神经网络
class NeuralNet{
    unsigned int NumInputs;//输入数量
    int NumOutputs;//输出数量
    int NumHiddenLayers;//隐藏层数量
    int NeuronsPerHiddenLayer;//每个隐藏层拥有的神经元数量
    std::vector<NeuronLayer> vecLayers;//层数
public:
    NeuralNet();
    void CreateNet();
    std::vector<double> GetWeights()const;//获取神经网络中所有神经元的权重
    int GetNumberOfWeights()const;//获取神经网络中所有权重的数量
    void PutWeights(std::vector<double> &weights);//更新权重
    inline double Sigmoid(double activation,double response);//激活函数
    std::vector<double> Update(std::vector<double> &inputs);//通过输入获得输出
};

NeuralNet::NeuralNet(){
    NumInputs=NUM_INPUT;
    NumOutputs=NUM_OUTPUT;
    NumHiddenLayers=NUM_HIDDENLAYER;
    NeuronsPerHiddenLayer=NUM_NEURONSPERHIDDENLAYER;
    CreateNet();

}

void NeuralNet::CreateNet(){
    //初始化隐藏层
    if(NumHiddenLayers>0){
        vecLayers.push_back(NeuronLayer(NeuronsPerHiddenLayer,NumInputs));
        for(int i=0;i<NumHiddenLayers-1;i++)
            vecLayers.push_back(NeuronLayer(NeuronsPerHiddenLayer,NeuronsPerHiddenLayer));
        //初始化输出层
        vecLayers.push_back(NeuronLayer(NumOutputs,NeuronsPerHiddenLayer));
    }
    else{
        //如果没有隐藏层则只初始化一个输出层
        vecLayers.push_back(NeuronLayer(NumOutputs,NumInputs));
    }
}

std::vector<double> NeuralNet::GetWeights() const{    
    std::vector<double> weights;
    for(int i=0;i<NumHiddenLayers+1;i++)
        for(int j=0;j<vecLayers[i].NumNeurons;j++)
            for(int k=0;k<vecLayers[i].vecNeurons[j].NumInputs;k++)
                weights.push_back(vecLayers[i].vecNeurons[j].vecWeight[k]);
    return weights;
}

int NeuralNet::GetNumberOfWeights() const{
    int weights=0;
    for (int i=0;i<NumHiddenLayers + 1; ++i)
        for (int j=0;j<vecLayers[i].NumNeurons; ++j)
            for (int k=0;k<vecLayers[i].vecNeurons[j].NumInputs; ++k)
                weights++;
    return weights;
}

void NeuralNet::PutWeights(std::vector<double> &weights){
    int Weight=0;
    for(int i=0;i<NumHiddenLayers+1;i++)
        for(int j=0;j<vecLayers[i].NumNeurons;j++)
            for(int k=0;k<vecLayers[i].vecNeurons[j].NumInputs;k++)
                vecLayers[i].vecNeurons[j].vecWeight[k]=weights[Weight++];


}

double NeuralNet::Sigmoid(double activation, double response){
    return ( 1 / ( 1 + exp(-activation / response)));
}

std::vector<double> NeuralNet::Update(std::vector<double> &inputs){
    std::vector<double> outputs;
    int Weight=0;
    if(inputs.size()!=NumInputs)//防止出错
        return outputs;
    for(int i=0;i<NumHiddenLayers+1;i++){
        if(i>0)
            inputs=outputs;
        outputs.clear();
        Weight=0;
        for(int j=0;j<vecLayers[i].NumNeurons;j++){
            double netinput=0;
            int tempNumInputs=vecLayers[i].vecNeurons[j].NumInputs;
            for(int k=0;k<tempNumInputs-1;k++)
                netinput+=vecLayers[i].vecNeurons[j].vecWeight[k]*inputs[Weight++];
            netinput+=vecLayers[i].vecNeurons[j].vecWeight[tempNumInputs-1]*(-1);//调整参数
            outputs.push_back(Sigmoid(netinput,1));//获得一个输出，可以用各种不同函数
            Weight=0;
        }
    }
    return outputs;
}

#endif // NEURALNET
