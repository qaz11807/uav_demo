#include "./utils.h"

std::vector<int> Utils::GenerateRandomNeighbor(int length) {
    static std::vector<std::vector<int>> neighbors;
    if(neighbors.size() == 0) {
        for(int l= -1 * length ; l<= 1 * length ;l++)
            for(int h= -1 * length ;h<= 1 * length ;h++)
                for(int d= -1 * length;d<= 1 * length;d++){
                    std::vector<int> alter = {l,h,d};
                    neighbors.push_back(alter);
                }
    }
    int index = rand() % (neighbors.size()) ;
    return neighbors.at(index);
}

double Utils::GenerateRandomNumber() {
    static bool init=false;
    if(!init) {
        init = true;
        srand(time(NULL));
    }
    return (double)rand() / RAND_MAX;
}

int Utils::GenerateRandomLength(int min , int max) {
    static bool init=false;
    if(!init) {
        init = true;
        srand(time(NULL));
    }
    return rand() % (max - min + 1) + min;
}

double safe_acos(double value) {
    if (value<=-1.0) {
        return PI;
    } else if (value>=1.0) {
        return 0;
    } else {
        return acos(value);
    }
}


int Utils::angleCalcaute(int currentVector[DIMENSION],int nextVector[DIMENSION]){

    if(isZeroVector(currentVector) || isZeroVector(nextVector))
        return 0;

    double AB = 0;
    double DA = 0;
    double DB = 0;
    for(int dim = 0; dim<DIMENSION; dim++){
        AB += currentVector[dim] * nextVector[dim];
        DA += pow(currentVector[dim] ,2.0);
        DB += pow(nextVector[dim] ,2.0);
    }

    DA = sqrt(DA);
    DB = sqrt(DB);
    
    double val = 180.0 / PI;
    double cosTheta = AB / (DA * DB) ;
    return fabs(safe_acos(cosTheta) * val);
}

double Utils::calcauteDistance(int currentPosition[DIMENSION] , int nextPosition[DIMENSION]){
    double sum = 0;
    for(int dim = 0; dim<DIMENSION; dim++)
        sum += pow( nextPosition[dim] - currentPosition[dim] ,2.0) ;
    return sqrt(sum);
}

double Utils::calcauteVectorDistance(int vector[DIMENSION] ){
    double sum = 0;
    for(int dim = 0; dim<DIMENSION; dim++)
        sum += pow(vector[dim] ,2.0) ;
    return sqrt(sum);
}

double CurvatureModel1(int prev[DIMENSION] , int current[DIMENSION], int next[DIMENSION]){
    double t1 =  Utils::calcauteDistance(prev,current);
    double prevMx = (current[0] - prev[0]) / t1;
    double prevMy = (current[1] - prev[1]) / t1;
    double prevMz = (current[2] - prev[2]) / t1;

    double t2 =  Utils::calcauteDistance(current,next);
    double xDiff1 = (next[0] - current[0]) / t2;
    double yDiff1 = (next[1] - current[1]) / t2;
    double zDiff1 = (next[2] - current[2]) / t2;

    double xDiff2 =  xDiff1 - prevMx  ;
    double yDiff2 =  yDiff1 - prevMy  ;
    double zDiff2 =  zDiff1 - prevMz  ;

    double rzy = zDiff2*yDiff1 - yDiff2*zDiff1;
    double rxz = xDiff2*zDiff1 - zDiff2*xDiff1;
    double rtx = yDiff2*xDiff1 - xDiff2*yDiff1;

    double f1 = sqrt(pow(rzy,2) + pow( rxz,2) +pow( rtx,2) );
    double f2 = pow( pow( xDiff1 ,2) + pow( yDiff1 ,2) + pow( zDiff1 ,2) , 1.5 );

    double inf = std::numeric_limits<double>::infinity();
    double rCurve = (f2 == 0) ? inf : f1  / f2 ;

    return rCurve;
}

double Utils::calcauteCurvature(int prev[DIMENSION] , int current[DIMENSION], int next[DIMENSION]){
    return CurvatureModel1(prev,current,next) ;
}

double Utils::calcauteAverageCurvature(std::vector<std::vector<int>> path){
    std::vector<double> curves;
    double sum = 0;
    for(size_t i = 0;i<path.size()-2;i++){
        int v1[3],v2[3];
        int* prev = &path.at(i)[0];
        int* current = &path.at(i+1)[0];
        int* next = &path.at(i+2)[0];
        
        for(int dim=0;dim<DIMENSION;dim++){
            v1[dim] = current[dim] - prev[dim];
            v2[dim] = next[dim] - current[dim];
        }
        double r = Utils::calcauteCurvature(prev,current,next);
        if(r != 0){
            sum+=r;
            curves.push_back(r);
        }
    }
    printf("Avg = %f \n", sum / curves.size());
    return sum / curves.size();
}

double Utils::calculateFitness(double p1, double p2,double p3 , int current[DIMENSION] , int rssi){
    return p1 * Utils::calcauteDistance(current, dest) + p2 * ( - rssi ) ;
}

bool Utils::isValid(int row, int col ,int z) { 
    return (row >= 0) && (row < LENGTH) && 
           (col >= 0) && (col < WIDTH) &&
           (z >= 0) && (z < HEIGHT); 
}

bool Utils::isDestination (int currentPosition[DIMENSION]){
    bool isFinish = true;
    for(int dim = 0; dim<DIMENSION; dim++)
        isFinish &= ( dest[dim] == currentPosition[dim]); 
    return isFinish;
}

bool Utils::isBarrier3D (int currentPosition[DIMENSION]){
    return gridBarrier[currentPosition[0]][currentPosition[1]][currentPosition[2]] == true;
}

bool Utils::isPassBarrier3D (int currentPosition[DIMENSION] , int nextPosition[DIMENSION]){

    double xDiff = nextPosition[0] - currentPosition[0];
    double yDiff = nextPosition[1] - currentPosition[1];
    double zDiff = nextPosition[2] - currentPosition[2];

    if( xDiff == 0 && yDiff == 0 ){
        int x = currentPosition[0];
        int y = currentPosition[1];
        for(int z = std::min(currentPosition[2],nextPosition[2]) + 1; z < std::max(currentPosition[2],nextPosition[2]) ;z++){
            if(!isValid(x,y,z))
                continue;
            if(gridBarrier[x][y][z] == 1)
                return true;
        }
    }else{
        if( xDiff == 0 ){
            int x = currentPosition[0];
            int iter = 0;

            int *startPoint,*endPoint;

            if(nextPosition[1] > currentPosition[1]){
                startPoint = currentPosition;
                endPoint = nextPosition;
            }else{
                startPoint = nextPosition;
                endPoint = currentPosition;
            }
            float step = ( (endPoint[2] - startPoint[2])*1.0 / (endPoint[1] - startPoint[1])) ;
            for(int y= startPoint[1] + 1 ; y < endPoint[1] ;y++){
                int z = startPoint[2] + step * iter;
                iter ++ ;
                if(!isValid(x,y,z))
                    continue;
                if(gridBarrier[x][y][z] == 1)
                    return true;
            }
        }else{
            
            if(yDiff == 0){
                int y = currentPosition[0];
                int iter = 0;
                int *startPoint,*endPoint;

                if(nextPosition[0] > currentPosition[0]){
                    startPoint = currentPosition;
                    endPoint = nextPosition;
                }else{
                    startPoint = nextPosition;
                    endPoint = currentPosition;
                }

                float step = ( (endPoint[2] - startPoint[2])*1.0 / (endPoint[0] - startPoint[0])) ;
                for(int x= startPoint[0] + 1 ; x < endPoint[0] ;x++){
                    int z = startPoint[2] +step * iter;
                    iter ++ ;
                    if(!isValid(x,y,z))
                        continue;
                    if(gridBarrier[x][y][z] == 1)
                        return true;
                }
            }else{
                int iter = 0;
                double m = yDiff/xDiff;

                int *startPoint,*endPoint;

                if(nextPosition[0] > currentPosition[0]){
                    startPoint = currentPosition;
                    endPoint = nextPosition;
                }else{
                    startPoint = nextPosition;
                    endPoint = currentPosition;
                }

                float step = ( (endPoint[2] - startPoint[2])*1.0 / (endPoint[0] - startPoint[0])) ;
                for(int x = startPoint[0] + 1 ; x < endPoint[0] ;x++){
                    int y = (x - currentPosition[0]) * m + currentPosition[1];
                    int z = startPoint[2] + step* iter;
                    iter ++ ;
                    if(!isValid(x,y,z))
                        continue;
                    if(gridBarrier[x][y][z] == 1)
                        return true;
                }
                iter = 0;
                m = xDiff/yDiff;

                if(nextPosition[1] > currentPosition[1]){
                    startPoint = currentPosition;
                    endPoint = nextPosition;
                }else{
                    startPoint = nextPosition;
                    endPoint = currentPosition;
                }

                step = ( (endPoint[2] - startPoint[2])*1.0 / (endPoint[1] - startPoint[1])) ;
                for(int y= startPoint[1] + 1 ; y < endPoint[1] ;y++){
                    int x = (y - currentPosition[1]) * m + currentPosition[0];
                    int z = startPoint[2] + step * iter;
                    iter ++ ;

                    if(!isValid(x,y,z))
                        continue;
                    if(gridBarrier[x][y][z] == 1)
                        return true;
                }
            }
        }
    }

    return false;
}

bool Utils::isNearBarrier(int scanLength , int currentPosition[DIMENSION]){
    for(int l= -1  ; l<= 1  ;l++)
        for(int h= -1  ;h <= 1  ;h++)
            for(int d= -1 ;d<= 1 ;d++){
                if(isValid(l*scanLength,h*scanLength,d*scanLength)){
                    int alter[3] = {l*scanLength,h*scanLength,d*scanLength};
                    if(isBarrier3D(alter))
                        return true;
                }
            }
    return false ;
}

int* Utils::createNewPos(int x,int y,int z){
    int* p = new int[3];
    p[0] = x;
    p[1] = y;
    p[2] = z;
    return p;
}

void Utils::printNode(int position[]){
    printf("(%d,%d,%d)\n" , position[0] , position[1] , position[2]);
}

void Utils::initGridByFieldData(nlohmann::json inf){

    for(size_t  i=0; i< inf["basestations"].size(); i++) {
        int x_cent = inf["basestations"][i]["x"];
        int y_cent = inf["basestations"][i]["y"];
        int r = inf["basestations"][i]["r"];
        printf("[Basestation Weight Init] x_cent = %d , y_cent = %d \n",x_cent,y_cent);
        
        double theta = 0.0;                 
        while(theta <= 360){
            double Cos = cos(theta);
            double Sin = sin(theta);
            int radius = 0; 
            while(radius <= r){
                double x = radius * Cos,
                       y = radius * Sin;

                int x_circle = x_cent + (int) x,
                    y_circle = y_cent + (int) y;

                int Rssi = radius == 0 ? -60 : -60 - 10 * 2 * log(radius/3);
                Rssi = Rssi < MIN_ACCEPT_RSSI ? MIN_ACCEPT_RSSI : Rssi;
                 
                for(int d=0;d< HEIGHT ;d++){
                    if(Utils::isValid(x_circle,y_circle,d)){
                        if( Rssi > gridWeight[x_circle][y_circle][d] || gridWeight[x_circle][y_circle][d] == 0)
                            gridWeight[x_circle][y_circle][d] = Rssi;
                    }
                } 
                       
                radius+=1;
            }
            theta+=0.01;  
        }
    }

    for(size_t  i=0; i<  inf["barriers"].size(); i++) {
        int x =  inf["barriers"][i]["start"]["x"];
        int y =  inf["barriers"][i]["start"]["y"];
        int z =  inf["barriers"][i]["start"]["z"];
        int length =  inf["barriers"][i]["length"];
        int width =  inf["barriers"][i]["width"];
        int height =  inf["barriers"][i]["height"];
        int rotated =  inf["barriers"][i]["rotated"];
        printf("[Barrier Init] start = (%d,%d,%d) , params = (%d,%d,%d) , rotated = %d\n",x,y,z,length,width,height,rotated);

        if(rotated == 0){
            for(int l=x;l<= x+length ;l++)
                for(int h=y;h<= y+width ;h++)
                    for(int d=z;d<= z+height ;d++)
                        if(Utils::isValid(l,h,d))
                            gridBarrier[l][h][d] = true;
        }else{

            int h = length ;
            int l = width ;
            int d = height ;

            for(int x1=x;x1<= (x + l) ;x1++)
                for(int y1=(y -h);y1<=  y;y1++)
                    for(int z=z;z<= (z + d) ;z++){
                        int x2 = (x1 - x) * cos(rotated) - (y1 - y) * sin(rotated) + x;
                        int y2 = (y1 - y) * cos(rotated) + (x1 - x) * sin(rotated) + y;
                        if(Utils::isValid(x2,y2,z))
                            gridBarrier[x2][y2][z] = true;
                    }
                        
        }
                
    }
}

int Utils::getNearestBarrierDistance(int*** barrierList,int scanLength , int currentPosition[],int size){
    return 0;
};

bool Utils::isValidDirection(int currentPosition[DIMENSION] , int nextPosition[DIMENSION],bool strict){
    bool flag = false;

    if(!strict && isPassBarrier3D(src , dest) && isPassBarrier3D(currentPosition , dest))
        return true;

    for(int dim = 0; dim<DIMENSION; dim++){
        if(dest[dim] > currentPosition[dim] ){
            if(nextPosition[dim] >= currentPosition[dim] && nextPosition[dim] <= dest[dim])
                flag =  true;
            else 
                return false;   
        }
        else if(dest[dim] < currentPosition[dim] ){
            if(nextPosition[dim] <= currentPosition[dim] && nextPosition[dim] >= dest[dim])
                flag = true;
            else 
                return false;
        } 
        else if(dest[dim] == currentPosition[dim] ){
            if(nextPosition[dim] == currentPosition[dim] )
                flag =  true;
            else 
                return false;
        }
    }
    return true ;
}

bool Utils::isValidDirectionAlpha(int currentPosition[DIMENSION] , int nextPosition[DIMENSION]){
    bool flag = false;

    for(int dim = 0; dim<DIMENSION; dim++){
        if(dest[dim] > currentPosition[dim] ){
            if(nextPosition[dim] >= currentPosition[dim] && nextPosition[dim] <= dest[dim])
                flag =  true;
            else 
                return false;   
        }
        else if(dest[dim] < currentPosition[dim] ){
            if(nextPosition[dim] <= currentPosition[dim] && nextPosition[dim] >= dest[dim])
                flag = true;
            else 
                return false;
        } 
        else if(dest[dim] == currentPosition[dim] ){
            if(nextPosition[dim] == currentPosition[dim] )
                flag =  true;
            else 
                return false;
        }
    }
    return true ;
}

bool Utils::isZeroVector(int vector[DIMENSION]){
    bool flag = true;
    for(int dim = 0; dim<DIMENSION; dim++)
        flag &= vector[dim] == 0 ;
    return flag;
}

std::vector<int> Utils::intArray2Vector(int arr[DIMENSION]){
    std::vector<int> v;
    for(int dim = 0; dim<DIMENSION; dim++)
        v.push_back(arr[dim]);
    return v;
}

std::vector<std::vector<int>> Utils::removeRedunmentPath(std::vector<std::vector<int>> clearPath ){
    int vexNum = static_cast<int>(clearPath.size());
    Graph_SP G(clearPath.size());
    for(int i = 0; i< vexNum ; i++){
        std::vector<int> node1 = clearPath.at(i);
        for(size_t j = 0 ; j< vexNum ; j++){
            std::vector<int> node2 = clearPath.at(j);
            if(!isPassBarrier3D(&node1[0],&node2[0]) && i != j){
                int dist = Utils::calcauteDistance(&node1[0],&node2[0]);  
                G.AddEdge( i , j , dist);
            }
        }
    }

    G.Dijkstra(0);
    std::vector<std::vector<int>> shortPath = G.retrievePath(clearPath,vexNum-1) ;
    for(size_t i = 0; i< shortPath.size() - 1 ; i++){
        std::vector<int> node = shortPath.at(i);          
    }
    
    return shortPath;
}

void Utils::writePath2CSV(std::string fileName , std::vector<std::vector<int>> path){
    std::ofstream file(fileName);
    for(size_t  i=0; i< path.size(); i++) {
        std::vector<int> position = path.at(i);
        for(int dim=0; dim<DIMENSION ;dim++ ){
            file << std::to_string(position[dim]);
            if(dim != DIMENSION - 1)
                file << "," ; 
            else file << "\n" ;
        }
    }
    file.close(); 
}

std::vector<std::vector<int>> Utils::readCsv2Path(std::string fileName){
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::vector<int> > parsedCsv;
    while(std::getline(file,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<int> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(std::stoi(cell));
        }

        parsedCsv.push_back(parsedRow);
    }
    file.close();
    return  parsedCsv ;
}

void Utils::writeCurve2CSV(std::string fileName , std::vector<std::vector<int>> path){

    std::ofstream file(fileName);
    
    double distance = 0;
    int origin[3] = {0,0,0};

    std::vector<std::vector<double>> slopes;
    std::vector<double> distances;

    file << std::to_string(0)+ "," + std::to_string(0)+"\n";  

    for(size_t i=0;i<path.size() - 2 ;i++){
        int* node1 = &path.at(i)[0] ;
        int* node2 = &path.at(i+1)[0] ;
        int* node3 = &path.at(i+2)[0] ;

        int v1[DIMENSION] , v2[DIMENSION];
        for(int dim=0; dim <3; dim++){
            v1[dim] = node2[dim]-node1[dim];
            v2[dim] = node3[dim]-node2[dim];
        }

        int angle = angleCalcaute(v1,v2);
            
        distance += Utils::calcauteDistance(node1,node2); 

        double r = calcauteCurvature(node1,node2,node3);

        printf("(%d,%d,%d)->(%d,%d,%d)->(%d,%d,%d) => r = %f , angle = %d , dis = %f \n" , 
            node1[0],node1[1],node1[2],
            node2[0],node2[1],node2[2],
            node3[0],node3[1],node3[2],
            r,angle,distance
        );

        file << std::to_string(r)+ "," + std::to_string(distance)+"\n";  

    }

    file.close(); 

}
