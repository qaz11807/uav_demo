#ifndef UTILS
#define UTILS

#include <bits/stdc++.h>
#include <config/config.h>
#include <graph/graph.h> 
#include <json/single_include/nlohmann/json.hpp> 

extern int* src;
extern int* dest;
extern bool*** gridBarrier;
extern int*** gridWeight;
extern int LENGTH , WIDTH , HEIGHT;

namespace Utils{

    double GenerateRandomNumber();
    std::vector<int> GenerateRandomNeighbor(int length); 
    int GenerateRandomLength(int min , int max);

    template <typename T>
    T*** createNewGrid(int length,int width, int height , T val){
        T*** grid = new T**[length];
        for (int i = 0; i < length; i++) {
            grid[i] = new T*[width];
            for (int j = 0; j < width; j++){
                grid[i][j] = new T[height];
                for (int k = 0; k < height; k++)
                    grid[i][j][k] = val;
            }
        }

        return grid;
    }
    int* createNewPos(int x,int y,int z);

    void initGridByFieldData(nlohmann::json inf);

    void printNode(int position[]);

    int angleCalcaute(int currentVector[],int nextVector[]);
    double calcauteDistance(int currentPosition[] , int nextPosition[]);
    double calcauteVectorDistance(int vector[]);
    double calcauteCurvature(int prev[] , int current[], int next[]);
    double calcauteAverageCurvature(std::vector<std::vector<int>> path);
    double calculateFitness(double p1, double p2,double p3 ,int current[] , int rssi);
    
    bool isValid(int row, int col ,int z);
    bool isDestination (int currentPosition[]);
    bool isHaveSignal (int currentPosition[]);
    bool isBarrier3D (int currentPosition[]);
    bool isPassBarrier3D (int currentPosition[] , int nextPosition[]);
    bool isNearBarrier(int scanLength , int currentPosition[]);
    bool isValidDirection(int currentPosition[] , int nextPosition[],bool strict);
    bool isValidDirectionAlpha(int currentPosition[] , int nextPosition[]);
    bool isZeroVector(int vector[]);
    
    int getNearestBarrierDistance(int*** barrierList,int scanLength , int currentPosition[],int size);
    
    std::vector<int> intArray2Vector(int arr[]);
    std::vector<std::vector<int>> removeRedunmentPath(std::vector<std::vector<int>> );
    
    void writePath2CSV(std::string fileName , std::vector<std::vector<int>> path);
    template <typename T>
    void writeGrid2CSV(std::string fileName , T*** grid){
        std::ofstream file(fileName);
        for(int l=0;l< LENGTH ;l++)
            for(int h=0;h< WIDTH ;h++)
                for(int d=0;d< HEIGHT ;d++)
                    file << std::to_string(l)+ "," << std::to_string(h)+ "," << std::to_string(d) + ","  +std::to_string(grid[l][h][d])+"\n"; 
        file.close(); 
    }
    void writeCurve2CSV(std::string fileName ,std::vector<std::vector<int>> path);

    std::vector<std::vector<int>> readCsv2Path(std::string fileName);

}

#endif  
