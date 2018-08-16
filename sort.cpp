#include <cstdlib>
#include <cstdio>
#include <vector>
#include <climits>

using namespace std;

#define FAILED_IF_NON_ZERO(x, test) do {\
	if(x < 0) { \
		printf(" %s failed at  %s, %s:%d \n",test,  __FUNCTION__, __FILE__, __LINE__ ); \
		return -1; \
	}} while(0)

#define FAILED_IF_NOT_EQUAL(x, y, arr, len, test) do {\
	print_arr(arr,len, test);\
	if(x !=  y) {\
		printf(" %s failed at: %s,%s:%d \n",test,  __FUNCTION__, __FILE__,__LINE__ ); \
	} else{ \
		printf(" %s passed at:%s,%s:%d \n", test, __FUNCTION__,__FILE__, __LINE__); \
	}} while(0)


int is_sorted(int* arr, int len){
	for(int i = 1; i < len; i++){
		if(arr[i] < arr[i-1])
			return -1;
	}
	return 0;
}

void print_arr(int *arr, int len, const char* text) {

	printf(" %s :", text);
	for(int i = 0; i <len; i++){
		printf(" %d", arr[i]);
	}
	printf("\n");

}

typedef int (*sorting_fun_ptr)(int* arr, int len);

int run_sorter(sorting_fun_ptr sorter, int* arr, int len, const char* test) {
	int ret;
	ret = sorter(arr, len);
	FAILED_IF_NON_ZERO(ret, test);
	ret = is_sorted(arr, len);
	FAILED_IF_NOT_EQUAL(ret, 0, arr, len, test);
}


int test_sorting(sorting_fun_ptr sorter){

		int ret;

		int arr1[1] = {1};
		run_sorter(sorter, arr1, 1,"1.single_input_test");

		int arr2[4] = {1,2,3,4};
		run_sorter(sorter, arr2, 4, "2.sorted_input_test");

		int arr3[4] = {1,1,1,1};
		run_sorter(sorter, arr3, 4, "3.equal_input_test ");

		int arr4[4] = {91,81,71,61};
		run_sorter(sorter, arr4, 4, "4.rev_sorted_test  ");

		int arr5[4] = {91,-81,71,-61};
		run_sorter(sorter, arr5, 4, "5.negt_test        ");
#if 0
		int arr6[1000];
		for(int i =0; i <1000; i++){
			arr6[i] = rand()/1000;
		}
		run_sorter(sorter, arr6, 1000, "6.random_test      ");
#endif
}

void swap(int* x, int* y)
{
	int temp = *x;
	*x = *y;
	*y = temp;

}

int bubble_sort(int* arr, int len){

	for(int i=0; i < len; i++) {
		// Note: starts from 1 and end till N-len
		for(int j=1; j<len-i; j++) {
			if(arr[j-1] > arr[j]) {
				swap(&arr[j-1], &arr[j]);
			}
		}
	}

	return 0;
}

int selection_sort(int* arr, int len){

	for(int i = 0; i < len; i++) {
		int max = INT_MIN, max_idx = -1;
		int j = 0;
		for(j = 0; j <len-i; j++) {
			if(arr[j] > max) {
				max = arr[j];
				max_idx = j;
			}
		}
		swap(&arr[max_idx], &arr[j-1]);
	}

	return 0;
}

int insertion_sort(int* arr, int len){
	for(int i = 1; i <len; i++) {
		int curr = arr[i];
		int j = 0;
		// move in reverse direction
		for( j=i-1; j >= 0; j--) {
			if(arr[j] >= curr)
				arr[j+1] = arr[j];
			else
				break;
		}
		arr[j+1] = curr;
	}

	return 0;
}

int merge_sorted_array(int* arr, int start, int mid, int end){
	for(int i = mid; i <= end; i++) {
			int curr = arr[i];
			int j = 0;
			// move in reverse direction
			for( j=i-1; j >= start; j--) {
				if(arr[j] >= curr)
					arr[j+1] = arr[j];
				else
					break;
			}
			arr[j+1] = curr;
		}
}

int merge_sort1(int* arr, int start, int end){
	if(start>=end) return 0;

	int mid = (start+end)/2;

	merge_sort1(arr, start, mid);
	merge_sort1(arr, mid+1, end);

	merge_sorted_array(arr, start, mid, end);
}
int merge_sort(int* arr, int len){

	merge_sort1(arr, 0, len-1);
	return 0;
}

int partion(int* arr, int start, int end){

	int pivot = arr[end];

	//printf("%d -%d %d-",pivot, start, end);
	while(start < end) {

		while(start < end && arr[start] <= pivot)
			start++;

		while(end > start && arr[end] > pivot)
			end--;

		if(start < end) {
			swap(&arr[start], &arr[end]);
			start++;
			end--;
		}

		//printf(" -%d %d-", start, end);
	}
	//print_arr(arr, 10, "partion");

	return start;
}
int quick_sort1(int* arr, int start, int end) {
	if(start >= end) return 0;

	int part_idx = partion(arr, start, end);
	//printf("%d ", part_idx );

	quick_sort1(arr, start, part_idx-1);
	quick_sort1(arr, part_idx, end);

	return 0;

}

int quick_sort(int* arr, int len){
	quick_sort1(arr, 0, len-1);
	return 0;
}

int main(){

#if 0
	int arr[10];
	for(int i= 0; i < 10; i++){
		arr[i] = rand()%10;
	}
	print_arr(arr, 10, "raw");
	//bubble_sort(arr, 10);
	//print_arr(arr, 10, "buble_sort");

	//selection_sort(arr, 10);
	//print_arr(arr, 10, "selction_sort");
	quick_sort(arr, 10);
	print_arr(arr, 10, "quick_sort");
#else

	printf("------------Testing Bubble Sort------------------ \n");
	test_sorting(bubble_sort);

	printf("------------Testing Insertion Sort------------------ \n");
	test_sorting(insertion_sort);

	printf("------------Testing selection Sort------------------ \n");
	test_sorting(selection_sort);

	printf("------------Testing merge Sort------------------ \n");
	test_sorting(merge_sort);

	printf("------------Testing quick Sort------------------ \n");
	test_sorting(quick_sort);

#endif
	printf("Hello, I am What I am");
}
