#include <bits/stdc++.h> 

struct HeapNode{
    int element, key;
    HeapNode():element(0),key(0){};
    HeapNode(int node, int key):element(node), key(key){};
};

class BinaryHeap{
    private:
        std::vector<HeapNode> heap;        
        void swap(struct HeapNode &p1, struct HeapNode &p2);
        int FindPosition(int node);
        int GetParentNode(int node);
    public:
        BinaryHeap();
        BinaryHeap(int n);
        
        bool IsHeapEmpty();

        void MinHeapify(int node, int length);
        void BuildMinHeap(std::vector<int> array);
        void DecreaseKey(int node, int newKey);
        void MinHeapInsert(int node, int key);
        int Minimum();                  
        int ExtractMin();               
};

const int Max_Distance = 32678;

class Graph_SP{             // SP serves as Shortest Path
private:
    int num_vertex;
    std::vector<std::vector<std::pair<int,int>>> AdjList;
    std::vector<int> predecessor, distance;
    std::vector<bool> visited;
public:
    Graph_SP():num_vertex(0){};
    Graph_SP(int n):num_vertex(n){
        AdjList.resize(num_vertex);
    }
    void AddEdge(int from, int to, int weight);
    void PrintDataArray(std::vector<int> array);
    void PrintIntArray(int *array);
    void PrintSelf();

    void InitializeSingleSource(int Start);     // 以Start作為起點
    void Relax(int X, int Y, int weight);       // edge方向：from X to Y

    void Dijkstra(int Start = 0); 
    std::vector<std::vector<int>> retrievePath(std::vector<std::vector<int>> ,int dest);       
    friend class BinaryHeap;             // 以Binary Heap實現Min-Priority Queue
};





