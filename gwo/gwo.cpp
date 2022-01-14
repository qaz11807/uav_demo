#include "./gwo.h"

GWO::GWO(){
    populations = 0;

    p1 = 0;
    p2 = 0;
    p3 = 0;

    total_iteration = 0;
    doneCount = 0;

    for(int wi=0;wi< populations ;wi++)
        wolfAgents[wi] = new Wolf(src); 
    
    alphaWolf =  wolfAgents[0] ;
    betaWolf =  wolfAgents[1] ;
    deltaWolf =  wolfAgents[2] ;

    mode = Global;
}

GWO::GWO(int _populations,double _p1 , double _p2 ,double _p3 , int _src[DIMENSION],int _dest[DIMENSION] , Mode _mode){
    populations = _populations;
    p1 = _p1;
    p2 = _p2;
    p3 = _p3;

    total_iteration = 0;
    doneCount = 0;

    for(int wi=0;wi< populations ;wi++)
        wolfAgents[wi] = new Wolf(src); 
 
    alphaWolf =  wolfAgents[0] ;
    betaWolf =  wolfAgents[1] ;
    deltaWolf = wolfAgents[2] ;
    
    mode = _mode;
}

GWO::GWO(int _populations,double _p1 , double _p2 ,double _p3 , int _src[DIMENSION],int _dest[DIMENSION] , Mode _mode , int prevPosition[DIMENSION]){
    populations = _populations;
    p1 = _p1;
    p2 = _p2;
    p3 = _p3;

    total_iteration = 0;
    doneCount = 0;

    for(int wi=0;wi< populations ;wi++)
        wolfAgents[wi] = new Wolf(src,prevPosition);  

    alphaWolf =  wolfAgents[0] ;
    betaWolf =  wolfAgents[1] ;
    deltaWolf = wolfAgents[2] ;
    
    mode = _mode;
}

double GWO::calculateD (double C , int leader , int agent){
    return fabs(C * leader - agent);
}

double GWO::calculateX (double A , int leader , int D){
    return leader - A * D;
}

void GWO::hierarchy() {

    double alphaScore = alphaWolf->getFitness();
    double betaScore = betaWolf->getFitness();
    double deltaScore = deltaWolf->getFitness();

    for(register unsigned int agentIndex=0; agentIndex < populations; agentIndex++) {

        if( wolfAgents[agentIndex]->isFinish)
            continue;

        wolfAgents[agentIndex]->calculateFitness(p1,p2,p3,dest);

        double fitness = wolfAgents[agentIndex]->getFitness();
         
        if( fitness <  alphaScore ) {
            alphaWolf = wolfAgents[agentIndex]; 
        }

        if( (fitness >= alphaScore) && (fitness < betaScore) ) {
            betaWolf = wolfAgents[agentIndex];  
        }

        if( (fitness >= alphaScore) && (fitness >= betaScore) && (fitness < deltaScore) ) {
            deltaWolf = wolfAgents[agentIndex];
        }
    }
    
}

bool GWO::updateWolves(double a) {

    double totalWeight = alphaWolf->getFitness() + betaWolf->getFitness() + deltaWolf->getFitness();
    double weightAlpha = (( alphaWolf->getFitness())*1.0) / totalWeight; 
    double weightBeta = (( betaWolf->getFitness())*1.0) / totalWeight; 
    double weightDelta = (( deltaWolf->getFitness())*1.0 )/ totalWeight;

    int origin[3] = {0,0,0};

    for(int agentIndex=0; agentIndex<populations; agentIndex++) {

        Wolf* agent = wolfAgents[agentIndex] ;
        int *current = agent->getPosition();
        int* prev = agent->getPrevPosition();

        double Lipschitz = K + reconnectedTimes * INC_PER_FAILED;
        double maxAngle = MAX_ANGLE + reconnectedTimes ;
        
        if(agent->isFinish)
            continue;
        
        if(mode == Global && ((Utils::calcauteDistance(agent->getPosition(),dest)) <= MIN_FITNESS || Utils::isDestination(agent->getPosition()))){
            if(doneCount == 0)
                printf("Find First Path Iteration_times = %d \n",total_iteration );
            doneCount++;
            agent->isFinish = true;
            if(VERBOSE)
                printf("Find Path agentIndex : %d , Iteration_times = %d , doneCount = %d \n",agentIndex,total_iteration , doneCount);
            agent->setPosition(dest);
        }

        if(mode == Local && (Utils::calcauteDistance(current,dest) <= 3 )){

            int v1[DIMENSION],v2[DIMENSION];

            for(int dim = 0; dim<DIMENSION; dim++){
                if(prev != NULL )
                    v1[dim] = current[dim] - prev[dim];
                else
                    v1[dim] = 0;
                v2[dim] = dest[dim] - current[dim];
            }
            
            int angle = Utils::angleCalcaute(v1,v2);

            double xArr[DIMENSION];         
            for(int dim = 0; dim<DIMENSION; dim++)
               xArr[dim] = dest[dim];
            
            if(prev != NULL ){
                double r = Utils::calcauteCurvature(prev,current,dest);
                double randD = Utils::GenerateRandomNumber();

                if( r <= K && angle <= maxAngle ){
                    agent->setPosition(dest);
                    agent->setX(xArr); 
                    agent->setVector(v2);
                    agent->setAngle(angle);

                    agent->isFinish = true;

                    return true;
                }
                            
            }

        }
            
        int nextPos[DIMENSION];
        double xArr[DIMENSION];

        for(int dim = 0; dim<DIMENSION; dim++){
            int pos = current[dim];

            double r1 = Utils::GenerateRandomNumber(); 
            double r2 = Utils::GenerateRandomNumber(); 

            double A1 = 2.0 * a * r1 - a; 
            double C1 = 2.0 * r2;

            double D_alpha = calculateD(C1,alphaWolf->getPositionByDimension(dim),pos) ;
            double X1 = calculateX(A1,alphaWolf->getPositionByDimension(dim),D_alpha) ;

            r1 = Utils::GenerateRandomNumber(); 
            r2 = Utils::GenerateRandomNumber(); 

            double A2 = 2.0 * a * r1 - a; 
            double C2 = 2.0 * r2; 

            double D_beta = calculateD(C2,betaWolf->getPositionByDimension(dim),pos) ;
            double X2 = calculateX(A2,betaWolf->getPositionByDimension(dim),D_beta) ;

            r1 = Utils::GenerateRandomNumber();
            r2 = Utils::GenerateRandomNumber(); 

            double A3 = 2.0 * a * r1 - a; 
            double C3 = 2.0 * r2; 

            double D_delta = calculateD(C3,deltaWolf->getPositionByDimension(dim),pos) ;
            double X3 = calculateX(A3,deltaWolf->getPositionByDimension(dim),D_delta) ;

            double b1 = 1, b2 = 0.5;
            double rand = Utils::GenerateRandomNumber();

            int X = ceil(weightAlpha * X1 + weightBeta * X2 + weightDelta *X3);

            double formulaB1 = b1 * X ;
            double formulaB2 = b2 * rand * ( agent->getBestPositionByDimension(dim) - agent->getXByDimension(dim));
            
            int PSO_X = formulaB1 + formulaB2;

            int nX = PSO_X;

            xArr[dim] = X;

            if(mode == Global){
                if((nX - pos) > GLOBAL_STEP)
                    nextPos[dim] =  pos+GLOBAL_STEP; 
                else if((nX - pos) < -GLOBAL_STEP)
                    nextPos[dim] =  pos-GLOBAL_STEP;
                else nextPos[dim] = nX;
            }
            else {
                if((nX - pos) > LOCAL_STEP)
                    nextPos[dim] =  pos + LOCAL_STEP; 
                else if((nX - pos) < -LOCAL_STEP)
                    nextPos[dim] =  pos-LOCAL_STEP;
                else nextPos[dim] = nX;
            }

        }

        if( 
            nextPos[0] == src[0] &&         
            nextPos[1] == src[1] &&     
            nextPos[2] == src[2] 
        ){
            int length = mode == Global ? 1 : LOCAL_STEP ;
            std::vector<int> alter = Utils::GenerateRandomNeighbor(length);

            for(int dim = 0; dim < DIMENSION; dim++){
                if(mode == Global){
                    for(int dim=0;dim<DIMENSION;dim++){
                        if(dest[dim] > current[dim])
                            nextPos[dim] = current[dim] + 1;
                        else if(dest[dim] < current[dim])
                            nextPos[dim] = current[dim] - 1;
                        else
                            nextPos[dim] = current[dim] ;
                    }
                }else
                    nextPos[dim] = current[dim] + alter[dim]; 
                
                xArr[dim] = nextPos[dim];            
            }

        }

        double randP = Utils::GenerateRandomNumber();
        double p = 0.9;
        
        if(Utils::isValid(nextPos[0],nextPos[1],nextPos[2])){
             
            int v1[DIMENSION],v2[DIMENSION];         
            for(int dim = 0; dim<DIMENSION; dim++){
                if(prev != NULL )
                    v1[dim] = current[dim] - prev[dim];
                else
                    v1[dim] = current[dim] - 0;
                v2[dim] = nextPos[dim] - current[dim];
            }
               
            int angle = Utils::angleCalcaute(v1,v2);

            double newFitness = Utils::calculateFitness(p1,p2,p3,nextPos,gridWeight[nextPos[0]][nextPos[1]][nextPos[2]]);
        
            bool isValidDirection = true;

            double r = 0;
            double randV = Utils::GenerateRandomNumber();

            switch (mode){
                case Global:
                    //isValidDirection = Utils::isValidDirectionAlpha(gridBarrier,current,nextPos,src,dest,block,barrierSize) ;
                    isValidDirection = Utils::isValidDirection(current,nextPos,true) ;

                    if( ( 180 - angle ) <= 90 && prev != NULL)
                        continue;

                    // if(!isValidDirection && randV > 0.3)
                    //     continue;

                    break;

                case Local:
                    if(reconnectedTimes < 20)
                        isValidDirection = Utils::isValidDirection(current,nextPos,true);   
                    else
                        isValidDirection = Utils::isValidDirectionAlpha(current,nextPos);                  
                    r = 0;

                    bool isMatch ;
                    if( prev != NULL ){
                        r = Utils::calcauteCurvature(prev,current,nextPos);     

                        if( angle > maxAngle || r > Lipschitz)
                            continue;          
                    }
 
                    if(Utils::isDestination(nextPos)){
                        agent->isFinish = true;
                        agent->setPosition(nextPos);
                        agent->setX(xArr); 
                        agent->setVector(v2);
                        agent->setAngle(angle);

                        return true; 
                    }
                    
                    if(!isValidDirection){
                        if( (r > Lipschitz) || angle > maxAngle)
                            continue;
                        else{
                            if ( K < 10 ){
                                if(randV > 0)
                                    continue;
                            }     
                            else 
                                continue;
                        }
                    }

                    break;
                
            }

            if(!isValidDirection)
                newFitness += 10000;

            double randB = Utils::GenerateRandomNumber();
            if( Utils::isNearBarrier(5,nextPos) && randB > 0.3)
                newFitness += 10000;
    
            if( !Utils::isBarrier3D(nextPos) && !agent->isVisted(nextPos) ){
                
                if( (newFitness < agent->getFitness() || randP > p  )){
                    if(Utils::isPassBarrier3D(current,nextPos) )
                        continue;
                    
                    agent->setAngle(angle);
                    agent->setX(xArr);
                    agent->setPosition(nextPos);
                    agent->setVector(v2);
                         
                }

            }
                
        }
                      
    }

    return false;
}

bool cmp(Wolf* w1,Wolf* w2){
    return w1->getStep() < w2->getStep();
} 

bool cmpFitness(Wolf* w1,Wolf* w2){
    return w1->getFitness() < w2->getFitness();
} 

bool cmpPathSize(Wolf* w1,Wolf* w2){
    return w1->getPath().size() < w2->getPath().size();
} 

void GWO::clearCount(){
    times = 0;
}

std::vector<Wolf*> GWO::execute(){
    double a;
        
    bool finish = false;

    times++;

    if(mode == Global)
        printf("Global => %d times => starting find path ..." , times );

    std::vector<std::vector<int>> dPath;
    while(!finish){
        
        for(register unsigned int iteration=0; iteration<MAX_ITERATION; iteration++) {

            total_iteration++;

            if ( ( mode == Global && total_iteration >= MAX_ITERATION_LIMIT_GLOBAL ) || doneCount >= 1){
                finish = true;
                break;
            }

            if ( mode == Local && total_iteration >= MAX_ITERATION_LIMIT_LOCAL ){
                finish = true;
                break;
            }
    
            hierarchy();
                    
            a = 2.0 - ( 2.0 * iteration) / MAX_ITERATION; 
             
            if(updateWolves(a)){
                finish = true;
                break;
            }
    
        }

    }

    std::vector<Wolf*> latestStepWolfs;

    if(mode == Global){
        Utils::writePath2CSV("./output/outputGWO_TEST_BEST_"+std::to_string(times)+".csv" , dPath);
        printf("Done! , total_iteration_times = %d , doneCount = %d \n" , total_iteration , doneCount);

        printf("Global => %d times => Sorting All Path ..." , times);
        std::sort(wolfAgents,wolfAgents+populations,cmp);
        printf("Done!\n");

        printf("Global => %d times => Select 5 least step path to candicate ..." ,times);

        int cnt = 0;
        for(int index = 0; index<populations; index++){
            if(!wolfAgents[index]->isFinish)
                continue;
            
            if(VERBOSE)
                printf("Find Path agentIndex : %d , step is %d \n",index,wolfAgents[index]->getStep());
            latestStepWolfs.push_back(wolfAgents[index]);
            
            cnt++;
            if(cnt>=5 )
                break;
        }

        if(cnt == 0){
            Utils::writePath2CSV("./output/outputGWO_Greeding.csv" , wolfAgents[0]->getPath());
        }

    }

    if(mode == Local){
        std::vector<Wolf*> ds;
        for(int index = 0; index<populations; index++){
            ds.push_back(wolfAgents[index]);
            if(!wolfAgents[index]->isFinish)
                continue;
            latestStepWolfs.push_back(wolfAgents[index]);
        }
        
        if(latestStepWolfs.size() == 0){
            Wolf*  w = ds[0];
            std::vector<std::vector<int>> p = w->getPath();
        }

    }

    return latestStepWolfs;
        
}