#include "BinaryHeap.h"

using std::cout;


int main(){
       	
	int arr[] = {12, 11, 13, 5, 6,7};
        int size = sizeof(arr)/sizeof(arr[0]);
        heapSort(arr,size);

      	for(auto& element : arr){
                cout<<element<<" ";
        }
	cout<<std::endl;
        return 0;
}


