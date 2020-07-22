#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
	string fileName = "weight.dat";
	int numFeatures = (1 << (5 * 4));	// 32 * 32 * 32 * 32
	int *featureWeight_4TupleInner;
	int *featureWeight_4TupleOuter;
	int featureWeight_4Tuple[4];

	featureWeight_4TupleInner = new int[numFeatures];
	featureWeight_4TupleOuter = new int[numFeatures];

	ifstream fin(fileName.c_str(), ifstream::in);
	fin.read(reinterpret_cast<char *>(featureWeight_4TupleInner), numFeatures * sizeof(int));
	fin.read(reinterpret_cast<char *>(featureWeight_4TupleOuter), numFeatures * sizeof(int));

	// Inner
	cout << "Inner 4-Tuple" << endl;
	for(int i = 0; i < numFeatures; i++) {
		for(int j = 0; j < 4; j++) {
			featureWeight_4Tuple[j] = (i >> (j * 5)) & 0x1f;
		}

		cout << "|";
		for(int j = 3; j >= 0; j--) {
			cout << setw(2) << featureWeight_4Tuple[j] << "|";
		}
		cout << setw(8) << featureWeight_4TupleInner[i] << endl;
	}

	// Outer
	cout << "Outer 4-Tuple" << endl;
	for(int i = 0; i < numFeatures; i++) {
		for(int j = 0; j < 4; j++) {
			featureWeight_4Tuple[j] = (i >> (j * 5)) & 0x1f;
		}

		cout << "|";
		for(int j = 3; j >= 0; j--) {
			cout << setw(2) << featureWeight_4Tuple[j] << "|";
		}
		cout << setw(8) << featureWeight_4TupleOuter[i] << endl;
	}


	delete [] featureWeight_4TupleInner;
	delete [] featureWeight_4TupleOuter;

	return 0;
}