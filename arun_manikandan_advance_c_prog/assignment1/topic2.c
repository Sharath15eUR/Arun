// This is the solution for the TOPIC 2 Question of the assignment Submitted by Arun Manikandan

// TODO

// Below is the sample flow of the code:

//     1. Ask the user about the size of the array and take the array elemetns as input
//     2. Sort the even and odd elements while maintianng thier positions

//     Constraints:

//         - No indexing is allwoed
//         - IMust be inplace sorting
//         - Can use only pointer arithmetic

#include<stdio.h>

void sortEvenOdd(int *arr, int size) {

    int *evenPtr = arr; //pointer to hold the even values
    int *currentPtr = arr; // pointer for iteration

    while (currentPtr < arr + size) { // iterate starting from first element till the last
        if (*currentPtr % 2 == 0) { // CHeck is the current pointer pointing value is even
            if (currentPtr != evenPtr) { // If it is even and is not the same as the evenPtr then
                int temp = *currentPtr; // We have found an even element that needs to be moved to the first so swap the remaining elemetns to the back while mainitning order
                
                // Shift elements between evenPtr and currentPtr to the right
                for (int *ptr = currentPtr; ptr > evenPtr; ptr--) {
                    *ptr = *(ptr - 1); //Moving the elements to teh right by 1
                }
                // Reset the even ptr to the temp ptr so that we have pointing to the next                 
                *evenPtr = temp;
            }
            evenPtr++; // Move the even pointer one step to the right
        }
        currentPtr++; // Move the iterating pointer
    }
}


int main(){

    int n;
    printf("Enter the number of elements : ");
    scanf("%d" , &n);
    int arr[n];

    printf("Enter your elements : \n");

    for(int i=0;i<n;i++) scanf("%d" , &arr[i]);
    printf("\n\n");

    printf("======== Before sorting the array ======== \n");

    for(int i=0;i<n;i++) printf("%d\t" , arr[i]);
    
    printf("\n\n");

    printf("======== After sorting the array ======== \n");

    sortEvenOdd(arr , n);

    for(int i=0;i<n;i++) printf("%d\t" , arr[i]);
    printf("\n\n");

    return 1;
}