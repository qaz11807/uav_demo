#include "./wolf.h"

Wolf::Wolf(){
    fitness = 0;
    step = 0;
    bestFitness = 0;
    isFinish = false;
    prevAngle = 0;

    for(int dim = 0; dim < DIMENSION; dim++){
        position[dim] = 0;
        bestPosition[dim] = 0;
        prevX[dim] = 0;
        vector[dim] = 0;
        prevVector[dim] = 0;
    }

    visted = Utils::createNewGrid(LENGTH,WIDTH,HEIGHT,false);

}

Wolf::Wolf(int src[DIMENSION]){
    fitness = 0;
    step = 0;
    bestFitness = 0;
    isFinish = false;
    prevAngle = 0;

    for(int dim = 0; dim < DIMENSION; dim++){
        position[dim] = src[dim];
        bestPosition[dim] = src[dim];
        prevX[dim] = src[dim];
        vector[dim] = 0;
        prevVector[dim] = 0;
    }

    visted = Utils::createNewGrid(LENGTH,WIDTH,HEIGHT,false);

    route.clear();
    route.push_back(Utils::intArray2Vector(src));

}

Wolf::Wolf(int src[DIMENSION] , int prevPosition[DIMENSION]){
    fitness = 0;
    step = 0;
    bestFitness = 0;
    isFinish = false;
    prevAngle = 0;

    for(int dim = 0; dim < DIMENSION; dim++){
        position[dim] = src[dim];
        bestPosition[dim] = src[dim];
        prevX[dim] = src[dim];
        prevVector[dim] = 0;
        vector[dim] = 0;
    }

    visted = Utils::createNewGrid(LENGTH,WIDTH,HEIGHT,false);

    route.clear();
    route.push_back(Utils::intArray2Vector(prevPosition));
    route.push_back(Utils::intArray2Vector(position)); 
}

Wolf& Wolf::operator= (const Wolf& other){
    if(this != &other){
        fitness = other.fitness;
        bestFitness = other.bestFitness;
        step = other.step;
        isFinish = other.isFinish;
        prevAngle = other.prevAngle ;

        for(int dim = 0; dim < DIMENSION; dim++){
            position[dim] = other.position[dim];
            bestPosition[dim] = other.bestPosition[dim];
            prevX[dim] = other.prevX[dim];
            prevVector[dim] = other.prevVector[dim];
            vector[dim] = other.vector[dim];
        }

        for(int l=0;l<LENGTH;l++)
            for(int h=0;h< WIDTH ;h++)
                for(int d=0;d< HEIGHT ;d++)
                        visted[l][h][d] = other.visted[l][h][d];
    }
    return *this;
}

void Wolf::setAngle (int angle){
    prevAngle = angle;
}

int Wolf::getAngle (){
    return prevAngle;
}

int Wolf::getStep (){
    return step;
}

void Wolf::setX (double newX[DIMENSION]){
    for(int dim = 0; dim<DIMENSION; dim++){
        prevX[dim] = newX[dim];
    } 
}

int Wolf::getXByDimension(int dim){
    return prevX[dim];
}

void Wolf::setVector (int newV[DIMENSION]){
    for(int dim = 0; dim<DIMENSION; dim++){
        prevVector[dim] = vector[dim];
        vector[dim] = newV[dim];
    } 
}

int* Wolf::getVector(){
    return vector;
}

int* Wolf::getPrevVector(){
    return prevVector;
}

int Wolf::getVectorByDimension(int dim){
    return vector[dim];
}

std::vector<std::vector<int>> Wolf::getPath(){
    return route;
}

void Wolf::setPosition(int pos[DIMENSION]){
    for(int dim = 0; dim<DIMENSION; dim++){
        position[dim] = pos[dim];
    }

    visted[pos[0]][pos[1]][pos[2]] = true;

    step += 1;
    if( prevAngle==0 && route.size() > 1){
        route.pop_back(); 
    }
    route.push_back(Utils::intArray2Vector(pos)); 
}

int* Wolf::getPosition(){
    return position;
}

int Wolf::getPositionByDimension(int dim){
    return position[dim];
}

int* Wolf::getPrevPosition(){
    if(route.size() >=2)
        return &route.at(route.size()-2)[0];
    else
        return NULL;
}
 
int Wolf::getBestPositionByDimension(int dim){
    return bestPosition[dim];
}

void Wolf::calculateFitness(double p1 , double p2 ,double p3 , int dest[DIMENSION]){
    fitness = Utils::calculateFitness(p1,p2,p3,position,gridWeight[position[0]][position[1]][position[2]]);
    if(fitness < bestFitness || bestFitness == 0){
        bestFitness = fitness;
        for(int dim = 0; dim<DIMENSION; dim++)
            bestPosition[dim] = position[dim];
    }
}

double Wolf::getFitness(){
    return fitness;
}

bool Wolf::isVisted(int pos[DIMENSION]){
    return visted[pos[0]][pos[1]][pos[2]];
}
