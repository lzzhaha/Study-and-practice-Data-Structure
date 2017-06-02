#include "MinHeap.h"
using std::cout;
using std::endl;
using std::ostream;
 		
template<typename T>
ostream& operator<<(ostream& os , const MinHeap<T>& heap){
	for(auto it = heap.data.begin(); it!=heap.data.send();it++ ){
		os<<*it<" ";
	}
	return os;
}



int main(){
	
	double arr1[] = {11.1,-1,-2.3,50,70, 150, -33};
	int arr2[] = {15, 0, 70, -45,-80, 60};
	MinHeap<double> doubleHeap(arr1,7);
	cout<<"doubleHeap: "<<doubleHeap<<endl;
	MinHeap<int> intHeap;
	for(int element : arr2){
		intHeap.insert(element);
	}
	cout<<"intHeap: "<<intHeap<<endl;
	return 0;
}

