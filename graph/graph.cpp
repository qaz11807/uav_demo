#include "graph.h"

BinaryHeap::BinaryHeap(){              
    heap.resize(1);        
}

BinaryHeap::BinaryHeap(int n){
    heap.resize(n + 1);
}

bool BinaryHeap::IsHeapEmpty(){
    return (heap.size()<1);
};

int BinaryHeap::Minimum(){
    return heap[1].element;
}             

int BinaryHeap::ExtractMin(){

    if (IsHeapEmpty()) {
        std::cout << "error: heap is empty\n";
        exit(-1);
    }
    int min = heap[1].element;    
    heap[1] = heap[heap.size()-1];            // 把最後一個element放到第一個位置,
    heap.erase(heap.begin()+heap.size()-1);   // 再刪除最後一個element
    MinHeapify(1, (int)heap.size());          // 目前, heap[1]具有最大Key, 需要進行調整

    return min;      
}

void BinaryHeap::DecreaseKey(int node, int newKey){

    int index_node = FindPosition(node);      // 找到node所在的位置index

    if (newKey > heap[index_node].key) {      // 如果不是把node的Key下修, 便終止此函式
        //std::cout << "new key is larger than current key\n";
        return;
    }
    heap[index_node].key = newKey;            // 更新node之Key後, 需要檢查是否新的subtree滿足Min Heap
    while (index_node > 1 && heap[GetParentNode(index_node)].key > heap[index_node].key) {
        swap(heap[index_node], heap[GetParentNode(index_node)]);
        index_node = GetParentNode(index_node);
    }
}
void BinaryHeap::MinHeapify(int node, int length){

    int left = 2*node,          // 取得left child
        right = 2*node + 1,     // 取得right child
        smallest;               // smallest用來記錄包含root與child, 三者之中Key最小的node

    if (left <= length && heap[left].key < heap[node].key)
        smallest = left;
    else
        smallest = node;

    if (right <= length && heap[right].key < heap[smallest].key)
        smallest = right;

    if (smallest != node) {                 // 如果目前node的Key不是三者中的最小
        swap(heap[smallest], heap[node]);   // 就調換node與三者中Key最小的node之位置
        MinHeapify(smallest, length);       // 調整新的subtree成Min Heap
    }
}

void  BinaryHeap::BuildMinHeap(std::vector<int> array){

    // 將array[]的資料放進 heap之矩陣中, 並預留 heap[0] 不做使用
    for (int i = 0; i < array.size(); i++) {     
        heap[i + 1].element = i;                 // 把array[]的idx視為element
        heap[i + 1].key = array[i];              // 把array[]的數值視為key
    }
    for (int i = (int)heap.size()/2; i >= 1 ; i--) {
        MinHeapify(i, (int)heap.size()-1);     // length要減一, 因為heap從從1開始存放資料
    }
}

void BinaryHeap::MinHeapInsert(int node, int key){

    heap.push_back(HeapNode(node,key));    // 在heap[]尾巴新增一個node
    DecreaseKey(node, key);
}

void BinaryHeap::swap(struct HeapNode &p1, struct HeapNode &p2){
    struct HeapNode temp = p1;
    p1 = p2;
    p2 = temp;
}
int BinaryHeap::FindPosition(int node){

    int idx = 0;
    for (int i = 1; i < heap.size(); i++) {
        if (heap[i].element == node) {
            idx = i;
        }
    }
    return idx;
}
int BinaryHeap::GetParentNode(int node){
    return std::floor(node/2);
};

void Graph_SP::Dijkstra(int Start){

    InitializeSingleSource(Start);

    BinaryHeap minQueue(num_vertex);   
    minQueue.BuildMinHeap(distance); 

    visited.resize(num_vertex, false);  
    
    while (!minQueue.IsHeapEmpty()) {
        int u = minQueue.ExtractMin();
        for (std::vector<std::pair<int, int>>::iterator itr = AdjList[u].begin(); itr != AdjList[u].end(); itr++) {
            Relax(u, (*itr).first, (*itr).second);    
            minQueue.DecreaseKey((*itr).first, distance[(*itr).first]);
        }
    }
    // std::cout << "\nprint predecessor:\n";
    // PrintDataArray(predecessor);
    // std::cout << "\nprint distance:\n";
    // PrintDataArray(distance);
}

std::vector<std::vector<int>> Graph_SP::retrievePath(std::vector<std::vector<int>> originPath ,int dest){
    std::vector<std::vector<int>> path;
    int index = dest;
    while( index != -1){
        path.push_back(originPath.at(index));
        index = predecessor.at(index);
    }
    std::reverse(path.begin(),path.end());
    return path;
}

void Graph_SP::InitializeSingleSource(int Start){

    distance.resize(num_vertex);
    predecessor.resize(num_vertex);

    for (int i = 0; i < num_vertex; i++) {
        distance[i] = Max_Distance;
        predecessor[i] = -1;
    }
    distance[Start] = 0;
}

void Graph_SP::Relax(int from, int to, int weight){
    if (distance[to] > distance[from] + weight) {
        distance[to] = distance[from] + weight;
        predecessor[to] = from;
    }
}
void Graph_SP::AddEdge(int from, int to, int weight){
    AdjList[from].push_back(std::make_pair(to,weight));
}
void Graph_SP::PrintDataArray(std::vector<int> array){
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << i;
    std::cout << std::endl;
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << array[i];
    std::cout << std::endl;
}

void Graph_SP::PrintSelf(){
    std::cout << "\nprint predecessor:\n";
    PrintDataArray(predecessor);
    std::cout << "\nprint distance:\n";
    PrintDataArray(distance);
}