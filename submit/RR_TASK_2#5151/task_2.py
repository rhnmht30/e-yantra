
'''
*****************************************************************************************
*
*        		===============================================
*           		Rapid Rescuer (RR) Theme (eYRC 2019-20)
*        		===============================================
*
*  This script is to implement Task 2 of Rapid Rescuer (RR) Theme (eYRC 2019-20).
*
*  This software is made available on an "AS IS WHERE IS BASIS".
*  Licensee/end user indemnifies and will keep e-Yantra indemnified from
*  any and all claim(s) that emanate from the use of the Software or
*  breach of the terms of this agreement.
*
*  e-Yantra - An MHRD project under National Mission on Education using ICT (NMEICT)
*
*****************************************************************************************
'''


# Team ID:			[ 5151 ]
# Author List:		[ Arnav Saha, Rohan Mehta ]
# Filename:			task_2.py
# Functions:		findCombination
# 					[ Comma separated list of functions in this file ]
# Global variables:	[ List of global variables defined in this file ]


# Import necessary modules
# Do not import any other modules
import cv2
import numpy as np
import os


def findCombination(Digits_list, Sum_integer):
    """
    Purpose:
    ---
    the function takes list of Digits and integer of Sum and returns the list of combination of digits

    Input Arguments:
    ---
    `Digits_list` :		[ list ]
        list of Digits on the first line of text file
    `Sum_integer` :     [ int ]
        integer of Sum on the second line of text file

    Returns:
    ---
    `combination_of_digits` :	[ list ]
        list of digits whose total is equal to Sum_integer

    Example call:
    ---
    combination_of_digits = findCombination(Digits_list, Sum_integer)

    """

    combination_of_digits = []

    #############	Add your Code here	###############
    length = len(Digits_list)
    Digits_list_copy = []
    # copying the digits list
    for i in Digits_list:
        Digits_list_copy.append(i)

    # sorting the copy in descending order
    Digits_list_copy.sort(reverse=True)
    # list of the indexes of the numbers which are equal to sum
    minIndexes = []
    # taking first sum to be max for computation of the min index sum
    lastIndexSum = 9223372036854775807
    # computing the list of digits temporarily
    tempMinIndexes = []
    # total numbers used: should be max in first case
    numbersUsed = 9223372036854775807
    for i in range(0, length):
        # delete any stray values
        del tempMinIndexes[:]
        numbersUsedInLoop = 0
        Sum_left = Sum_integer
        # sum of the indexes of the numbers
        indexSum = 0
        # initialize the index with first ith digit
        indexSum = Digits_list.index(Digits_list_copy[i])
        tempMinIndexes.append(Digits_list.index(Digits_list_copy[i]))
        Sum_left = Sum_integer - Digits_list_copy[i]
        # starting just one next to current digit
        for j in range(i+1, length):
            if (Digits_list_copy[j] <= Sum_left and Digits_list_copy[j] != 0):
                Sum_left = Sum_left - Digits_list_copy[j]
                indexSum = indexSum + Digits_list.index(Digits_list_copy[j])
                tempMinIndexes.append(Digits_list.index(Digits_list_copy[j]))
                numbersUsedInLoop += 1

            if (Sum_left == 0):
                if (indexSum < lastIndexSum and numbersUsedInLoop <= numbersUsed):
                    del minIndexes[:]
                    for k in tempMinIndexes:
                        minIndexes.append(k)
                    del tempMinIndexes[:]
                    lastIndexSum = indexSum
                    numbersUsed = numbersUsedInLoop
                break
            else:
                continue
    minIndexes.sort()
    for i in minIndexes:
        combination_of_digits.append(Digits_list[i])
    ###################################################

    return combination_of_digits


#############	You can add other helper functions here		#############


#########################################################################


# NOTE:	YOU ARE NOT ALLOWED TO MAKE ANY CHANGE TO THIS FUNCTION
#
# Function Name:	main
# Inputs:			None
# Outputs: 			None
# Purpose: 			the function first takes 'Test_input0.txt' as input, finds the combination
# 					of digits and selects one of them using findCombination function, it then asks
#                   the user whether to repeat the same on other Test input txt files
#                   present in 'Test_inputs' folder or not


if __name__ == '__main__':

    curr_dir_path = os.getcwd()
    # path to directory of 'Test_inputs'
    txt_input_dir_path = curr_dir_path + '/Test_inputs/'

    file_num = 0
    txt_file_path = txt_input_dir_path + 'Test_input' + \
        str(file_num) + '.txt'      # path to 'Test_input0.txt' text file

    txt_input_obj = open(txt_file_path, 'r')

    print('\n============================================')

    print('\nFor Test_input' + str(file_num) + '.txt')

    Digits_list = [int(digit) for digit in txt_input_obj.readline().split()]
    Sum_integer = [int(integer)
                   for integer in txt_input_obj.readline().split()][0]

    print('\nGiven List of Digits = %s \n\nGiven Integer of Sum = %d' %
          (Digits_list, Sum_integer))

    try:

        combination_of_digits = findCombination(Digits_list, Sum_integer)

    except Exception as e:

        print(
            '\n[ERROR] findCombination function ran into an error, check your code !\n')
        raise(e)
        exit()

    print('\nCombination of digits chosen = %s \n\nNumber of digits chosen = %d' % (
        combination_of_digits, len(combination_of_digits)))

    print('\n============================================')

    choice = input(
        '\nWant to run your script on other text input files ? ==>> "y" or "n": ')

    if choice == 'y':

        file_count = len(os.listdir(txt_input_dir_path))

        for file_num in range(1, file_count):

            txt_file_path = txt_input_dir_path + 'Test_input' + \
                str(file_num) + '.txt'      # path to 'Test_input0.txt' text file

            txt_input_obj = open(txt_file_path, 'r')

            print('\n============================================')

            print('\nFor Test_input' + str(file_num) + '.txt')

            Digits_list = [int(digit)
                           for digit in txt_input_obj.readline().split()]
            Sum_integer = [int(integer)
                           for integer in txt_input_obj.readline().split()][0]

            print('\nGiven List of Digits = %s \n\nGiven Integer of Sum = %d' %
                  (Digits_list, Sum_integer))

            try:

                combination_of_digits = findCombination(
                    Digits_list, Sum_integer)

            except Exception as e:

                print(
                    '\n[ERROR] findCombination function ran into an error, check your code !\n')
                raise(e)
                exit()

            print('\nCombination of digits chosen = %s \n\nNumber of digits chosen = %d' % (
                combination_of_digits, len(combination_of_digits)))

            print('\n============================================')

    else:

        print('')
