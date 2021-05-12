#include "bits/stdc++.h"
#include <sys/stat.h>
using namespace std;

uint64_t limitMemory=1000000;

uint64_t caculateSize(){
    return limitMemory/1.2;
}

struct MyPair{
    int id;
    string value;
};

void writeSortToFile(vector<string> listLine, string outFilePath){
    ofstream outFile(outFilePath);
    sort(listLine.begin(), listLine.end());
    ostream_iterator<string> output_iterator(outFile, "\n");
    copy(listLine.begin(), listLine.end(), output_iterator);
    return;
}

int divideSortFile(string inputPath){
    ifstream inFile(inputPath);
    string line;
    vector<string> listLine;
    uint64_t size = 0;
    int i = 0;
    mkdir("file", 0777);
    while(getline(inFile, line)){
        listLine.push_back(line);
        size += sizeof(listLine);
        if(size > caculateSize()){
            writeSortToFile(listLine, "./file/"+to_string(i)+".txt");
            size = 0;
            listLine.clear();
            i++;
        }
    }
    if(!listLine.empty()){
        writeSortToFile(listLine, "./file/"+to_string(i)+".txt");
        listLine.clear();
        i++;
    }
    return i;
}


void heapify(vector<MyPair> list, int n, int i){
    int min = i;
    int l = 2*i+1;
    int r = 2*i+2;
    if(l < n && list[l].value < list[min].value ){
        min = l;
    }
    if(r < n && list[r].value < list[min].value){
        min = r;
    }
    if(min != i){
        swap(list[i], list[min]);
        heapify(list, n, min);
    }
}

void buildHeap(vector<MyPair> list, int n){
    for(int i = n/2-1; i >=0; i--){
        heapify(list,n,i);
    }
}

void mergeFile(int n, string outPath){
    vector<ifstream> listFile(n);
    vector<MyPair> list(n);
    for(int i=0; i < n; i++){
        listFile[i].open("./file/"+to_string(i)+".txt");
        getline(listFile[i], list[i].value);
        list[i].id = i;
    }

    buildHeap(list, n);
    ofstream outFile(outPath);

    while (n>0){
        heapify(list, n, 0);
        outFile << list[0].value << "\n";
        if(listFile[list[0].id].eof()){
            n--;
            listFile[list[0].id].close();
            list.erase(list.begin());
        }else{
            getline(listFile[list[0].id],list[0].value);
        }
    }
}

int main(){
    string inputPath;
    string outputPath;
    cout << "input file path: \n";
    cin >> inputPath;
    cout << "output file path: \n";
    cin >> outputPath;
    cout << "limitMemory(kb): \n";
    cin >> limitMemory;
    int n = divideSortFile(inputPath);
    mergeFile(n, outputPath);
}






