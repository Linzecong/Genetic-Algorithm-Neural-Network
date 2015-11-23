#ifndef NEURALNET
#define NEURALNET
#include<vector>
#include <stdlib.h>
#include <math.h>
#include"Alg.h"

//��Ԫ
struct Neuron{
    int NumInputs;//ÿ����Ԫ����������
    std::vector<double> vecWeight;//ÿ�������Ȩ��
    Neuron(int Inputs):NumInputs(Inputs){
        for(int i=0;i<Inputs+1;i++)
            vecWeight.push_back(RandomClamped());//�������Ȩ��
    }
};

//�񾭲�
struct NeuronLayer{
    int NumNeurons;//ÿ��ӵ�е���Ԫ����
    std::vector<Neuron> vecNeurons;//������Ԫ
    NeuronLayer(int NeuronNum,int NeuronInputNum):NumNeurons(NeuronNum){
        for(int i=0;i<NeuronNum;i++)
            vecNeurons.push_back(Neuron(NeuronInputNum));
    }
};

//������
class NeuralNet{
    unsigned int NumInputs;//��������
    int NumOutputs;//�������
    int NumHiddenLayers;//���ز�����
    int NeuronsPerHiddenLayer;//ÿ�����ز�ӵ�е���Ԫ����
    std::vector<NeuronLayer> vecLayers;//����
public:
    NeuralNet();
    void CreateNet();
    std::vector<double> GetWeights()const;//��ȡ��������������Ԫ��Ȩ��
    int GetNumberOfWeights()const;//��ȡ������������Ȩ�ص�����
    void PutWeights(std::vector<double> &weights);//����Ȩ��
    inline double Sigmoid(double activation,double response);//�����
    std::vector<double> Update(std::vector<double> &inputs);//ͨ�����������
};

NeuralNet::NeuralNet(){
    NumInputs=NUM_INPUT;
    NumOutputs=NUM_OUTPUT;
    NumHiddenLayers=NUM_HIDDENLAYER;
    NeuronsPerHiddenLayer=NUM_NEURONSPERHIDDENLAYER;
    CreateNet();

}

void NeuralNet::CreateNet(){
    //��ʼ�����ز�
    if(NumHiddenLayers>0){
        vecLayers.push_back(NeuronLayer(NeuronsPerHiddenLayer,NumInputs));
        for(int i=0;i<NumHiddenLayers-1;i++)
            vecLayers.push_back(NeuronLayer(NeuronsPerHiddenLayer,NeuronsPerHiddenLayer));
        //��ʼ�������
        vecLayers.push_back(NeuronLayer(NumOutputs,NeuronsPerHiddenLayer));
    }
    else{
        //���û�����ز���ֻ��ʼ��һ�������
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
    if(inputs.size()!=NumInputs)//��ֹ����
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
            netinput+=vecLayers[i].vecNeurons[j].vecWeight[tempNumInputs-1]*(-1);//��������
            outputs.push_back(Sigmoid(netinput,1));//���һ������������ø��ֲ�ͬ����
            Weight=0;
        }
    }
    return outputs;
}

#endif // NEURALNET
