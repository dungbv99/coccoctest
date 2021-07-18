#include "bits/stdc++.h"
#include <sys/stat.h>
#include <unistd.h>


using namespace std;

uint64_t limitMemory;

//when program run, will incur extra memory like stack to store recursive when sort array
//so don't read use limitMemory to read from input file to sort
uint64_t calculateSize(){
    return limitMemory/1.25;
}

/*
 * receive a vector<string>
 * sort it and write to file with index of file is file name
 */
void writeSortToFile(vector<string>& listLine, string outFilePath){
    ofstream outFile(outFilePath);
    sort(listLine.begin(), listLine.end());
    ostream_iterator<string> output_iterator(outFile, "\n");
    /*
     * write n-1 line with "\n" in the end of line
     * and write nth line without "\n"
     * to guarantee when merge file does not read "\n" in the last line of file
     */
    copy(listLine.begin(), listLine.begin()+listLine.size()-1, output_iterator);
    outFile << listLine[listLine.size()-1];
    outFile.close();
    return;
}

/*
 * depend on limited memory to read input file
 * when reading an appropriate memory size, go writeSortToFile function to sort and write to file
 * return number of file to be split and sorted
 */
int divideSortFile(string inputPath){
    ifstream inFile(inputPath);
    string line;
    vector<string> listLine;
    uint64_t size = 0;
    int n = 0;
    mkdir("file", 0777);
    while(getline(inFile, line)){
        listLine.push_back(line);
        size += line.size();
        if(size > limitMemory){
            writeSortToFile(listLine, "./file/" + to_string(n) + ".txt");
            size = 0;
            listLine.clear();
            listLine.shrink_to_fit();
            n++;
        }
    }
    //the redundancy of input file
    if(!listLine.empty()){
        writeSortToFile(listLine, "./file/" + to_string(n) + ".txt");
        listLine.clear();
        n++;
    }
    return n;
}

/*
 * define pi type with:
 *  -   pi.first is the line value in priority queue
 *  -   pi.second is the index of file
 */
typedef pair<string, int> pi;

/*
 * merge n sorted file to output file
 */
void mergeFile(int n, string outPath){
    vector<ifstream> listSortedFile(n);
    /*
     * use priority queue with greater comparator for the first element is always the smallest of the elements it contains
     * pi.first is string which will be used to compare
     */
    priority_queue<pi, vector<pi>, greater<pi>> pq;
    string line;
    for(int i=0; i < n; i++){
        listSortedFile[i].open("./file/" + to_string(i) + ".txt");
        getline(listSortedFile[i], line);
        pq.push({line, i});
    }
    ofstream outFile(outPath);
    while (n>0){ // while exist file is open
        pi minNode = pq.top();
        pq.pop();
        outFile << minNode.first << "\n";
        if(listSortedFile[minNode.second].eof()){ //if the file is EOF, close file and decrease n
            n--;
            listSortedFile[minNode.second].close();
        }else{ //push the next line of file to pq
            getline(listSortedFile[minNode.second], line);
            pq.push({line, minNode.second});
        }
    }
}
//remove temporary folder in step 1 : delete all file before delete folder
void rmTemporaryFolder(int n){
    for(int i = 0; i < n; i++){
        string path = "./file/"+ to_string(i)+".txt";
        remove(path.c_str());
    }
    rmdir("./file/");
}

/*
 * execute external sort function
 * divide 3 step
 * step 1: divide input file to n file depend on limit memory and sort them
 * step 2: merge n sorted file
 * step 3: remove temporary folder which create to store temporary sorted file in step 1
 */
void externalSort(string inputPath, string outputPath){
    int n = divideSortFile(inputPath);
    mergeFile(n, outputPath);
    rmTemporaryFolder(n);
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
    cout << "input path of file to sort: \n";
    cin >> inputPath;
    cout << "output file path: \n";
    cin >> outputPath;
    cout << "limitMemory(byte): \n";
    cin >> limitMemory;
    time_t start, finish;
    time(&start);
    externalSort(inputPath, outputPath);
    time(&finish);
    cout << "sort time = " << difftime(finish, start) << "seceonds \n" ;
//    checkOutputSortFile(outputPath);
}






