#ifndef WOLF
#define WOLF

#include <bits/stdc++.h> 
#include <config/config.h>
#include <utils/utils.h>

class Wolf{
    private:
        double fitness;
        double bestFitness;
        int position[DIMENSION];
        int bestPosition[DIMENSION];
        int step;
        int prevAngle;  
        int prevVector[DIMENSION];  
        double prevX[DIMENSION];
        int vector[DIMENSION];

        bool*** visted; 
        std::vector<std::vector<int>> route;

    public:
        bool isFinish;

        Wolf(); 
        Wolf(int src[DIMENSION]);
        Wolf(int src[DIMENSION] , int prevPosition[3]);
        Wolf& operator= (const Wolf& other);
        
        void setAngle(int prevAngle);
        int getAngle();

        int getStep();
        
        void setX (double newX[DIMENSION]);
        int getXByDimension(int dim);
        
        void setVector (int newV[DIMENSION]);
        int* getVector();
        int* getPrevVector();
        int getVectorByDimension(int dim);
        
        std::vector<std::vector<int>> getPathDebug();
        std::vector<std::vector<int>> getPath();
        void appendDebugPath();
        
        void setPosition(int pos[DIMENSION]); 
        int* getPosition();
        int* getPrevPosition();
        
        int getBestPositionByDimension(int dim);
        int getPositionByDimension(int dim);
        
        void calculateFitness(double p1 , double p2 ,double p3,int dest[DIMENSION]);
        double getFitness();

        bool isVisted(int pos[DIMENSION]);

};

#endif  