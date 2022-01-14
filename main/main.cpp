#include <iostream>
#include <config/config.h>
#include <gwo/gwo.h>

int *src = new int[3];
int *dest = new int[3];

int LENGTH , WIDTH , HEIGHT;

bool*** gridBarrier;
int*** gridWeight;

int GWO::times = 0;
int GWO::reconnectedTimes = 0;

int main(int, char**) {

    std::ifstream ifield("input/2020-12-21-field1.json");
    nlohmann::json fieldInf;
    ifield >> fieldInf;
    
    src = Utils::createNewPos(fieldInf["src"]["x"],fieldInf["src"]["y"],fieldInf["src"]["z"]);
    dest = Utils::createNewPos(fieldInf["dest"]["x"],fieldInf["dest"]["y"],fieldInf["dest"]["z"]);

    Utils::printNode(src);
    Utils::printNode(dest);

    LENGTH = fieldInf["length"];
    WIDTH = fieldInf["width"];
    HEIGHT = fieldInf["height"];

    gridWeight = Utils::createNewGrid<int>(LENGTH,WIDTH,HEIGHT,-100);
    gridBarrier = Utils::createNewGrid<bool>(LENGTH,WIDTH,HEIGHT,0);
    
    Utils::initGridByFieldData(fieldInf);

    Utils::writeGrid2CSV("./output/outputGWO_Weight.csv" , gridWeight);
    Utils::writeGrid2CSV("./output/outputGWO_Barrier.csv" , gridBarrier);
    
    std::vector<Wolf*> candicateWolfs;
    auto start_time = std::chrono::high_resolution_clock::now();

    double p1 = 1 , p2 = 0 , p3 = 0;
    GWO* pathPlanning = new GWO(30,p1,p2,p3,src,dest,Global);

    std::vector<Wolf*> wolfs = pathPlanning->execute();      
    
    std::vector<std::vector<int>> phaseOnePath = wolfs.at(0)->getPath();

    Utils::writePath2CSV("./output/outputGWO_Greeding+DE_0.csv" , phaseOnePath);
    
    if(p2 == 0){
        printf("finding shortest path...\n");
        phaseOnePath = Utils::removeRedunmentPath(phaseOnePath);
        printf("Global => Local => Find Path shortest Step : %d \n", phaseOnePath.size());
    }else
        printf("Global => Local => Find Path originStep : %d \n", phaseOnePath.size());
    
    GWO::clearCount();
    auto finish_time_phase_1 = std::chrono::high_resolution_clock::now();
    auto executionTime_phase_1 = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time_phase_1-start_time).count() * 1e-9;

    printf("Global path is found ,spend time is : %f\n",executionTime_phase_1 );

    printf("Prepared to Optimize the path...\n" );

    std::vector<std::vector<int>> oPath;

    Utils::writePath2CSV("./output/outputGWO_GLOBAL_0.csv" , phaseOnePath);
    Utils::writeCurve2CSV("./output/outputGWO_Origin_Curve_0.csv" , phaseOnePath);
    double avgOri = Utils::calcauteAverageCurvature(phaseOnePath);

    int step = 0 ;

    int prev[DIMENSION];

    unsigned long long t_iterations = 0;
    unsigned long long t_reconnectedTimes = 0;

    for(size_t i=0;i<phaseOnePath.size() -1 ;i++){

        for(int dim=0; dim < DIMENSION ; dim++){
            src[dim] = phaseOnePath.at(i).at(dim);
            dest[dim] = phaseOnePath.at(i+1).at(dim);
        }
        
        GWO* pathPlanning;
        GWO::reconnectedTimes = 0;

        if(i==0)
            pathPlanning = new GWO(30,p1,p2,p3,src,dest,Local);
        else
            pathPlanning = new GWO(30,p1,p2,p3,src,dest,Local , prev);

        std::vector<Wolf*> wolfs = pathPlanning->execute();

        while(wolfs.empty()){
                                
            GWO::reconnectedTimes++;

            if(VERBOSE){
                printf("Connected Failed => (%d,%d,%d)->(%d,%d,%d) => try to reconnected... reconnectedTimes = %d \n" ,
                    src[0],src[1],src[2],dest[0],dest[1],dest[2],
                    GWO::reconnectedTimes
                );
            }
            
            if(i==0)
                pathPlanning = new GWO(30,p1,p2,p3,src,dest,Local);
            else
                pathPlanning = new GWO(30,p1,p2,p3,src,dest,Local , prev);
            
            wolfs = pathPlanning->execute();
        }

        Wolf* wolf = wolfs.at(0);
        std::vector<std::vector<int>> path = wolf->getPath();
        
        int* _ = wolf->getPrevPosition();
        for(int dim=0;dim<DIMENSION;dim++)
            prev[dim] = _[dim];

        if(i == 0)
            oPath.insert(oPath.end(),path.begin(),path.end());
        else
            oPath.insert(oPath.end(),path.begin()+2,path.end());

        if(i == (phaseOnePath.size()-1))
            oPath.push_back(Utils::intArray2Vector(dest));
        
        step += wolf->getStep();
        
        t_iterations += pathPlanning->total_iteration;
        t_reconnectedTimes += GWO::reconnectedTimes; 

        printf("Connected Done => (%d,%d,%d)->(%d,%d,%d) => total_iteration = %d \n" ,
            src[0],src[1],src[2],dest[0],dest[1],dest[2],
            pathPlanning->total_iteration
        );

    }

    int i = oPath.size()-1;
    while(i != 1){
        int* curr = &oPath.at(i-1)[0];
        int* next = &oPath.at(i)[0];
        if(Utils::calcauteDistance(curr,next) == 0)
            oPath.erase(oPath.begin() + i);
        i--;
    }

    printf("Connected Done => total_point = %d , step = %d  , avg_iteration_times = %d , Reconnected times = %d  \n", phaseOnePath.size() ,step,(t_iterations / phaseOnePath.size()), t_reconnectedTimes );

    Utils::writeCurve2CSV("./output/outputGWO_Curve_0.csv" , oPath);

    double avg = Utils::calcauteAverageCurvature(oPath);

    Utils::writePath2CSV("./output/outputGWO_LOCAL_0.csv" , oPath);

    auto finish_time_phase_2 = std::chrono::high_resolution_clock::now();
    auto executionTime_phase_2 = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time_phase_2-finish_time_phase_1).count() * 1e-9;

    printf("Optimized path is found ,spend time is : %f\n",executionTime_phase_2 );

    auto executionTime_total = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time_phase_2-start_time).count() * 1e-9;

    printf("Complete ! ,spend time is : %f\n", executionTime_total );

}
