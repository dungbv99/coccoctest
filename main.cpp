#define _CRT_SECURE_NO_WARNINGS
#include <chrono> 

#include <string>
#include <iostream>
#include<vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>     // EXIT_FAILURE
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace std::chrono;

void heapify(vector<string> &arr,int n, int i){
	int largest = i;
	int l = 2*i+1;
	int r = 2*i+2;
	if(l<n && arr[l]>arr[largest]){
		largest = l;
	}
	if(r<n && arr[r] > arr[largest]){
		largest = r;
	}
	if(largest != i){
		arr[i].swap(arr[largest]);
		heapify(arr,n,largest);
	}
}

void heapSort(vector<string> &arr, int n){
	for(int i = n/2-1; i >=0; i-- ){
		heapify(arr,n,i);
	}
	for(int i = n-1; i >=0; i--){
		arr[0].swap(arr[i]);
		heapify(arr,i,0);
	}
}

long createInitialRuns(long long int max_ram){
	string path;
	cout << "input path to file:\n";
	cin >> path;
	//path = "input.txt";
	ifstream f;
	f.open(path);
	if(!f.is_open()){
		cout << "file does n't exist\n";
		return 0;
	}
	vector<string> w;
	long long int ram = 0;
	int file_id = -1;
	if(f.is_open()){
		file_id = 0;
		while(!f.eof()){
			string line;
			if(ram < max_ram/1.2){
				
				getline(f,line);
				ram += line.size();
				
				w.push_back(line);

			}else{
				heapSort(w,w.size());
				string fname = "./file/";
				fname.append(to_string(file_id)).append(".txt");
				ofstream file(fname);
				for(int i = 0; i < w.size(); i++){
					file << w[i];
					if(i != w.size()-1){
						file << "\n";
					}
				}
				file.close();
				w.clear();
				ram = line.size();
				file_id++;
	
				w.push_back(line);
			}
		}

		if(w.size() > 0){
				heapSort(w,w.size());
				string fname = "./file/";
				fname.append(to_string(file_id)).append(".txt");
				ofstream file(fname);
				for(int i = 0; i < w.size(); i++){
					file << w[i];
					if(i != w.size()-1){
						file << "\n";
					}
				}
				file.close();


				file_id++;


		}

	}


	
	f.close();


	return file_id;
}

typedef struct MinHeapNode{
	string line;
	int id;
} MinHeapNode;

void heapifyForMinHeapNode(vector<MinHeapNode> &arr, int n, int i){
	int largest = i;
	int l = 2*i+1;
	int r = 2*i+2;
	if(l < n and arr[largest].line.compare(arr[l].line)<0){
		largest = l;
	}
	if(r < n and arr[largest].line.compare(arr[r].line)<0 ){
		largest = r;
	}
	if(largest != i){
		MinHeapNode tmp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = tmp;
		heapifyForMinHeapNode(arr,n,largest);
	}
}





void mergeFile(int n){
	vector<ifstream> listfile(n);
	vector<MinHeapNode> arr;

	for(int i = 0; i < n; i++){

		string filename = "./file/";
		filename.append(to_string(i)).append(".txt");
		listfile[i].open(filename);


		string line;
		getline(listfile[i],line);
		MinHeapNode minHeapNode;
		minHeapNode.id = i;
		minHeapNode.line = line;
		arr.push_back(minHeapNode);
	}
	//build heap

	for(int i = (n-1)/2; i >=0; i--){
		heapifyForMinHeapNode(arr,n,i);
	}
	
	ofstream output;
	output.open("output.txt");
	int cnt = 0;

	while(cnt < n){

		output << arr[0].line;
		output << "\n";
		string l;
		if(!listfile[arr[0].id].eof()){
			
			getline(listfile[arr[0].id],arr[0].line);
			
			heapifyForMinHeapNode(arr,n-cnt,0);
		}else{

			cnt++;
			arr.erase(arr.begin());

		}
	}
	



	for(int i = 0; i < n; i++){
		listfile[i].close();
	}
	
	output.close();
}







void testmemorywithquicksort(){
	long long int max_ram = 5*pow(2,20);
	string path;
	//getline(cin,path);
	path = "input.txt";
	ifstream f;
	f.open(path);
	vector<string> w;
	long long int ram = 0;
	while(!f.eof()){
	
		string line;
		getline(f,line);
		ram += line.size();
		if(line != "\0")
			w.push_back(line);
		//cout << ram << "\n";
		if (ram > max_ram)
			break;
		
	}


	f.close();
	cout << "quicksort";
	//quickSort(w,0,w.size()-1);
	heapSort(w,w.size());

	ofstream output;
	output.open("out.txt");
	for(int i = 0; i < w.size(); i++){
		output<<w[i];
		output << "\n";
	}
	output.close();
}


void cmp(){

	ifstream f2;
	f2.open("output.txt");
	int i = 0;
	string l1;
	getline(f2,l1);
	while(!f2.eof()){
		i++;
		string l2;

		getline(f2,l2);
		if(l1.compare(l2) > 0){
			cout << i << "\n";
			cout << "\n";
		}
		l1 = l2;
	}
}

int main(){
	

	double max_ram = 0;
	while (1) {
		cout << "Enter command:\n";
		string command;
		cin >> command;
		if (command == "setmaxmemory") {
			cout << "enter max memory(gb):\n";
			cin >> max_ram;
		}
		else if (command == "input") {
			if (max_ram == 0) {
				cout << "enter max memory(gb) before excute input command:\n";
				cin >> max_ram;

			}
			auto start = high_resolution_clock::now();

			int numoffile = createInitialRuns(max_ram * pow(2, 30));
			mergeFile(numoffile);


			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Time taken by function: "
				<< duration.count() << " microseconds" << endl;


		}
		else if (command == "break") {
			break;
		}
	}





}






















