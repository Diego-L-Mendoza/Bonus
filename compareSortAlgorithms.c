#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Diego Mendoza

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated

void heapify(int arr[], int n, int i)
{
	int largest = i; 
	int left = 2*i + 1; 
	int right = 2*i + 2; 

	// If left child is larger
	if (left < n && arr[left] > arr[largest])
		largest = left;

	// If right child is larger
	if (right < n && arr[right] > arr[largest])
		largest = right;

	
	if (largest != i)
	{
		// Allocate memory for temp storage
		int* temp = (int*)Alloc(sizeof(int));

		// swap
		*temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = *temp;

		// Free temp
		DeAlloc(temp);

		// Recursively heapify the affected sub-tree
		heapify(arr, n, largest);
	}
}

void heapSort(int arr[], int n)
{
	// Rearrange array
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// One by one extract an element from heap
	for (int i=n-1; i>=0; i--)
	{
		
		int* temp = (int*)Alloc(sizeof(int));
		
		*temp = arr[0];
		arr[0] = arr[i];
		arr[i] = *temp;

		//free temp
		DeAlloc(temp);

		heapify(arr, i, 0);
	}
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated

void merge(int pData[], int l, int m, int r)
{
	
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	
	int *L = (int *)Alloc(sizeof(int) * n1);
	int *R = (int *)Alloc(sizeof(int) * n2);
	
	for (i = 0; i < n1; i++)
		L[i] = pData[l + i];
	for (j = 0; j < n2; j++)
		R[j] = pData[m + 1 + j];
	
	i = 0;
	j = 0;
	k = l;
	
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			pData[k] = L[i];
			i++;
		}
		else
		{
			pData[k] = R[j];
			j++;
		}
		k++;
	}
	
	while (i < n1)
	{
		pData[k] = L[i];
		i++;
		k++;
	}
	
	while (j < n2)
	{
		pData[k] = R[j];
		j++;
		k++;
	}
	
	DeAlloc(L);
	DeAlloc(R);

	
}

void mergeSort(int pData[], int l, int r)
{
	
	if(l < r)
	{
		int m = l + (r - l) / 2;
		mergeSort(pData, l, m);
		mergeSort(pData, m + 1, r);

		merge(pData, l, m, r);
	
	}
	
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, key, j;
	for (i = 1; i < n; i++)
	{
		key = pData[i];
		j = i - 1;

		while (j >= 0 && pData[j] > key)
		{
			pData[j + 1] = pData[j];
			j = j - 1;
		}
		pData[j + 1] = key;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	int i, j;
	for (i = 0; i < n-1; i++)       
		for (j = 0; j < n-i-1; j++)  
			if (pData[j] > pData[j+1])
			{
				int temp = pData[j];
				pData[j] = pData[j+1];
				pData[j+1] = temp;
			}
}


// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i, j, min_idx;

	for (i = 0; i < n-1; i++)
	{
		min_idx = i;
		for (j = i+1; j < n; j++)
			if (pData[j] < pData[min_idx])
				min_idx = j;
		int temp = pData[min_idx];
		pData[min_idx] = pData[i];
		pData[i] = temp;
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		
	}
	
}
