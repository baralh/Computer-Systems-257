#ifndef CMSC257_A1SUPPORT_INCLUDED
#define CMSC257_A1SUPPORT_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File          : cmsc257-s17-assign1-support.h
//  Description   : This is a set of general-purpose utility functions we use
//                  for the 257 assignment #1.
//

//
// Functional Prototypes

int float_display_array(float *arr, int n );
	// This function prints out the array of floating point values

int integer_display_array(int *arr, int n );
	// This function prints out the array of integer values

int float_evens(float *arr, int n );
	// Return the number of even numbers in the array (float version)

int integer_evens(int *arr , int n);
	// Return the number of even numbers in the array (int version)

int countBits(int integer);
	// Count the number of '1' bits in the binary representation

int * most_values(int *arr, int n, int max);
	// Print out the values with the most occurences in array
	// Since it has to return " value(s)" I Changed the method too retun an arrya of numbers there is no other way to
	// return more than one value
unsigned short reverseBits(unsigned short in);
        // Reverse the binary bit string

void binaryString(char *str, int size, int number);
        // Print the binary equivalent of the number passed

void integerQuickSort(int *arr,int l, int r);
        // Sort the integer array using QuickSort algorithm
        
// int converFloat
#endif // CMSC257_A1SUPPORT_INCLUDED


