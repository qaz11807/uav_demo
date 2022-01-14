#include <gwo/wolf/wolf.h> 
#include <config/config.h> 

enum Mode { Global, Local };

bool cmp(Wolf* w1,Wolf* w2);

class GWO{
    private :
        int populations;
        
        Wolf* wolfAgents[30];

        Wolf* alphaWolf;
        Wolf* betaWolf;
        Wolf* deltaWolf;

        double p1,p2,p3;

        Mode mode;
        
        double calculateD (double C , int leader , int agent);
        double calculateX (double A , int leader , int D);

        friend Wolf;

        void hierarchy();
        bool updateWolves(double a);

        int doneCount = 0;

    public:
        unsigned long long total_iteration = 0;
        static int reconnectedTimes;
        static int times;
        
        GWO();
        GWO(int _populations,double _p1 , double _p2 ,double _p3 , int _src[],int _dest[] , Mode _mode);
        GWO(int _populations,double _p1 , double _p2 ,double _p3 , int _src[],int _dest[] , Mode _mode , int prevPosition[]);

        std::vector<Wolf*> execute();
        bool updateWolvesForTest(double a , int mode) ;
        static void clearCount();
};
