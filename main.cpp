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
//vector<string> words;


struct MinHeapNode {
	string line;
	int i;
};





void merge(vector<string>& arr, int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	vector<string> L(n1);
	vector<string> R(n2);
	for (i = 0; i < n1; i++) {
		L[i] = arr[l + i];
	}
	for (j = 0; j < n2; j++) {
		R[j] = arr[m + 1 + j];
	}



	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2) {
		if (L[i].compare(R[j]) < 0) {
			arr[k++] = L[i++];
		}
		else {
			arr[k++] = R[j++];
		}
	}

	while (i < n1) {
		arr[k++] = L[i++];
	}

	while (j < n2) {
		arr[k++] = R[j++];
	}
	L.clear();
	R.clear();


}


void mergeSort(vector<string>& arr, int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);
		merge(arr, l, m, r);
	}
}



void mergeArr(vector<MinHeapNode>& arr, int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	vector<MinHeapNode> L(n1);
	vector<MinHeapNode> R(n2);

	for (i = 0; i < n1; i++) {
		L[i] = arr[l + i];
	}
	for (j = 0; j < n2; j++) {
		R[j] = arr[m + 1 + j];
	}

	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2) {
		if (L[i].line.compare(R[j].line) < 0) {
			arr[k++] = L[i++];
		}
		else {
			arr[k++] = R[j++];
		}
	}

	while (i < n1) {
		arr[k++] = L[i++];
	}

	while (j < n2) {
		arr[k++] = R[j++];
	}
	L.clear();
	R.clear();


}

void mergeSortArr(vector<MinHeapNode>& arr, int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortArr(arr, l, m);
		mergeSortArr(arr, m + 1, r);
		mergeArr(arr, l, m, r);
	}
}


void mergeFile(long numfile) {

	vector<ifstream> list(numfile);

	ofstream outfile("output.txt");

	vector<MinHeapNode> arr;

	for (long i = 0; i < numfile; i++) {
		string path = "";
		ifstream f;
		list[i].open(path.append(to_string(i)).append(".txt"));

	}


	for (long i = 0; i < numfile; i++) {
		string line;
		getline(list[i], line);
		MinHeapNode minHeapNode;
		minHeapNode.line = line;
		minHeapNode.i = i;
		arr.push_back(minHeapNode);
	}

	int k = 0;
	while (arr.size() > 0) {
		mergeSortArr(arr, 0, arr.size() - 1);
		outfile << arr[0].line;


		outfile << "\n";
		if (!list[arr[0].i].eof()) {

			getline(list[arr[0].i], arr[0].line);
		}
		else {
			arr.erase(arr.begin());
		}
	}







	outfile.close();

	for (long i = 0; i < numfile; i++) {
		list[i].close();
	}



}







long createInitialRuns(long long int max_size) {

	string path;
	cout << "input path to file:\n";
	cin >> path;

	long file_id = -1;
	long long int file_size;
	ifstream myfile(path, std::ios::binary | std::ios::ate);
	if (myfile.is_open()) {
		file_size = myfile.tellg();
	}
	else {
		cout << "doesn't exist file\n";
		return -1;
	}


	myfile.close();
	long long int max_size_file = 2.5 * pow(2, 30) - 100;/////////////////////////////////////////////////////////////////////////

	if (file_size == 0) {
		cout << "file is empty\n";
		return -1;
		//exit(EXIT_FAILURE);
	}
	else if (file_size > max_size) {
		cout << "size of file > " << max_size << "\n";

	}
	else if (file_size < max_size_file) {




		myfile.open(path);
		file_id = -2;
		if (myfile.is_open()) {
			vector<string> w;
			while (!myfile.eof()) {
				string line;
				getline(myfile, line);
				w.push_back(line);

			}

			mergeSort(w, 0, w.size() - 1);
			ofstream file;
			file.open("output.txt");

			for (int i = 0; i < w.size(); i++) {
				file << w[i];
				if (i != w.size() - 1) {
					file << "\n";
				}
			}
			w.clear();
			file.close();
		}
		myfile.close();
	}

	else {

		myfile.open(path);
		file_id = 0;

		if (myfile.is_open()) {
			long long int cur_size = 0;


			vector<string> w;

			while (!myfile.eof()) {

				string line;
				getline(myfile, line);
				if (cur_size + line.size() < max_size_file) {
					cur_size += line.size();
					w.push_back(line);
				}
				else {

					mergeSort(w, 0, w.size() - 1);
					string fname = "";
					ofstream file;
					file.open(fname.append(to_string(file_id)).append(".txt"));
					for (int i = 0; i < w.size(); i++) {
						file << w[i];
						if (i != w.size() - 1) {
							file << "\n";
						}
					}
					file.close();
					file_id++;
					w.clear();
					cur_size = line.size();
					w.push_back(line);
				}
			}
			if (w.size() != 0) {
				mergeSort(w, 0, w.size() - 1);
				string fname = "./file/";
				ofstream file;
				file.open(fname.append(to_string(file_id)).append(".txt"));
				for (int i = 0; i < w.size(); i++) {
					file << w[i];
					if (i != w.size() - 1) {
						file << "\n";
					}
				}
				file.close();
				file_id++;
				w.clear();
			}


		}



		myfile.close();

	}




	return file_id;
}





int main() {

	cout << "Enter command:\n";
	cout << "input\n";
	cout << "output\n";
	cout << "maxsize\n";
	cout << "break\n";


	int max_size = 0;
	int id = -1;
	while (1) {
		cout << "Enter command:\n";
		string command;
		cin >> command;
		if (command == "maxsize") {
			cout << "enter max size(gb):\n";
			cin >> max_size;
		}
		else if (command == "input") {
			if (max_size == 0) {
				cout << "enter max size(gb) before excute input command:\n";
				cin >> max_size;

			}
			auto start = high_resolution_clock::now();

			id = createInitialRuns(max_size * pow(2, 30));
			if (id > -1)
				mergeFile(id);


			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Time taken by function: "
				<< duration.count() << " microseconds" << endl;


		}
		else if (command == "output") {
			if (id == -1) {
				cout << "input file is not suitable\n";

			}
			else {

				ifstream myfile("output.txt");
				while (!myfile.eof()) {
					string line;
					getline(myfile, line);
					cout << line << "\n";
				}
				myfile.close();

			}

		}
		else if (command == "break") {
			break;
		}
	}





}

