#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>

double sigmoid(double x)
{
	return 1.0 / (1.0 + exp(-x));
}
double sigmoidDerivative(double x)
{
	return sigmoid(x) * (1 - sigmoid(x));
}
class NeuralNetwork
{
	const double BIAS = -1;
	const double ALPHA = 0.5;
	const double MAX_ERROR = 1e-9;
	struct neuron
	{
		double output;
		std::vector<double> prevLayerConnectionsWeights;
	};
	std::vector<std::vector<neuron>> layers;
	void calcOutput(std::vector<int>& input)
	{
		for(int i = 0; i < (int)input.size(); i++)
		{
			layers[0][i].output = input[i];
		}
		for(int i = 1; i < (int)layers.size(); i++)
		{
			for(int j = 0; j < (int)layers[i].size(); j++)
			{
				if(i < (int)layers.size() - 1 && j == (int)layers[i].size() - 1)
				{
					continue;
				}
				double sum = 0;
				for(int k = 0; k < (int)layers[i - 1].size(); k++)
				{
					sum += layers[i - 1][k].output * layers[i][j].prevLayerConnectionsWeights[k];
				}
				layers[i][j].output = sigmoid(sum);
			}
		}
	}
	void backpropagate(std::vector<double>& errors)
	{
		std::vector<std::vector<double>> deltas;
		for(int i = 0; i < (int)layers.size(); i++)
		{
			deltas.push_back(std::vector<double>(layers[i].size(), 0));
		}
		for(int i = 0; i < (int)layers[layers.size() - 1].size(); i++)
		{
			deltas[layers.size() - 1][i] = errors[i] * sigmoidDerivative(layers[layers.size() - 1][i].output);
		}

		for(int i = layers.size() - 1; i > 0; i--)
		{
			for(int j = 0; j < (int)layers[i].size(); j++)
			{
				for(int k = 0; k < (int)layers[i - 1].size(); k++)
				{
					layers[i][j].prevLayerConnectionsWeights[k] += ALPHA * layers[i - 1][k].output * deltas[i][j];
					deltas[i - 1][k] += layers[i][j].prevLayerConnectionsWeights[k] * deltas[i][j];
				}

			}
			for(int k = 0; k < (int)layers[i - 1].size(); k++)
			{
				deltas[i - 1][k] *= sigmoidDerivative(layers[i - 1][k].output);
			}
		}

	}

public:
	NeuralNetwork(std::vector<int> layerSizes)
	{
		for(int i = 0; i < (int)layerSizes.size(); i++)
		{
			int size = layerSizes[i];
			if(i < (int)layerSizes.size() - 1)
			{
				size++;
			}
			std::vector<neuron> layer;
			for(int j = 0; j < size; j++)
			{
				neuron n;
				if(i > 0)
				{
					for(int k = 0; k < layerSizes[i - 1] + 1; k++)
					{
						n.prevLayerConnectionsWeights.push_back(std::rand() % 100);
					}
				}
				if(j == layerSizes[i])
				{
					n.output = BIAS;
				}
				layer.push_back(n);
			}
			layers.push_back(layer);

		}


	}
	void train(int epochs, std::vector<std::vector<int>> &inputs, std::vector<std::vector<int>> &outputs)
	{
		for(int i = 0; i < epochs; i++)
		{
			double error = 0;
			for(int j = 0; j < (int)inputs.size(); j++)
			{
				calcOutput(inputs[j]);
				std::vector<double> errors;
				for(int k = 0; k < (int)outputs[j].size(); k++)
				{
					error += (outputs[j][k] - layers[layers.size() - 1][k].output) * (outputs[j][k] - layers[layers.size() - 1][k].output);
					errors.push_back(outputs[j][k] - layers[layers.size() - 1][k].output);
				}
				backpropagate(errors);
			}
			if(error < MAX_ERROR)
			{
				return;
			}
		}
	}

	void test(std::vector<int>& input)
	{
		calcOutput(input);
		for(int i = 0; i < (int)layers[layers.size() - 1].size(); i++)
		{
			std::cout << layers[layers.size() - 1][i].output << " ";
		}
		std::cout << "\n";
	}
};


int main()
{
	std::srand(std::time(nullptr));
	std::vector<std::vector<int>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	std::vector<std::vector<int>> outputs = {{1}, {0}, {0}, {1}};
	NeuralNetwork neuralNetwork({2, 100, 1});
	neuralNetwork.train(10000, inputs, outputs);

	for(int i = 0; i < (int)inputs.size(); i++)
	{
		neuralNetwork.test(inputs[i]);
	}

	return 0;
}
