#include "bits/stdc++.h"
#include <sys/stat.h>
using namespace std;

uint64_t limitMemory=1000000;
//when program run, will incur extra memory like stack to store recursive when sort array
//so don't read use limitMemory to read from input file to sort
uint64_t calculateSize(){
    return limitMemory/2.5;
}

struct MyPair{
    int id;
    string value;
};

void writeSortToFile(vector<string> listLine, string outFilePath){
    ofstream outFile(outFilePath);
    sort(listLine.begin(), listLine.end());
    ostream_iterator<string> output_iterator(outFile, "\n");
    copy(listLine.begin(), listLine.begin()+listLine.size()-1, output_iterator);
    outFile << listLine[listLine.size()-1];
    outFile.close();
    return;
}
//split input file to n file with n
//size of 1 file is approximately calculateSize()
//n = size of input file / calculateSize()
int divideSortFile(string inputPath){
    ifstream inFile(inputPath);
    string line;
    vector<string> listLine;
    uint64_t size = 0;
    int i = 0;
    mkdir("file", 0777);
    while(getline(inFile, line)){
        listLine.push_back(line);
        size += line.size();
        if(size > calculateSize()){
            writeSortToFile(listLine, "./file/"+to_string(i)+".txt");
            size = 0;
            listLine.clear();
            listLine.shrink_to_fit();
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

//after first element in list is writen in out put file, heapify list to get the smallest element
void heapify(vector<MyPair> &list, int n, int i){
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
//build the first heap in the first time
void buildHeap(vector<MyPair> &list, int n){
    for(int i = n/2-1; i >=0; i--){
        heapify(list,n,i);
    }
}

void mergeFile(int n, string outPath){
    cout << "merge \n";
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
            buildHeap(list, n);
        }else{
            getline(listFile[list[0].id],list[0].value);
        }
    }
}

//compare all 2 adjacent line in output file
void checkOutputSortFile(string outputPath){
    ifstream out;
    string l1;
    string l2;
    out.open(outputPath);
    getline(out, l1);
    int l = 1;
    while (!out.eof()){
        getline(out, l2);
        l++;
        if(l1.compare(l2) > 0){
            cout << l1 << "  " << l2 << " " << l << "\n";
        }
        l1 = l2;
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
//    checkOutputSortFile(outputPath);
}






