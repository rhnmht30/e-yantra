
'''
*****************************************************************************************
*
*        		===============================================
*           		Rapid Rescuer (RR) Theme (eYRC 2019-20)
*        		===============================================
*
*  This script is to implement Task 1A of Rapid Rescuer (RR) Theme (eYRC 2019-20).
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
# Author List:		[ Rohan Mehta, Arnav Saha ]
# Filename:			task_1a.py
# Functions:		readImage, solveMaze, buildGraph, findNeighbours, isSafe, findPath,constructPath
# 					[ Comma separated list of functions in this file ]
# Global variables:	CELL_SIZE
# 					[ List of global variables defined in this file ]


# Import necessary modules
# Do not import any other modules
import cv2
import numpy as np
import os


# To enhance the maze image
import image_enhancer


# Maze images in task_1a_images folder have cell size of 20 pixels
CELL_SIZE = 20


def readImage(img_file_path):
    """
    Purpose:
    ---
    the function takes file path of original image as argument and returns it's binary form

    Input Arguments:
    ---
    `img_file_path` :		[ str ]
            file path of image

    Returns:
    ---
    `original_binary_img` :	[ numpy array ]
            binary form of the original image at img_file_path

    Example call:
    ---
    original_binary_img = readImage(img_file_path)

    """

    binary_img = None

    #############	Add your Code here	###############
    # read image from given path
    binary_img = cv2.imread(img_file_path)
    # convert image to grayscale
    binary_img = cv2.cvtColor(binary_img, cv2.COLOR_BGR2GRAY)
    ###################################################

    return binary_img


def solveMaze(original_binary_img, initial_point, final_point, no_cells_height, no_cells_width):
    """
    Purpose:
    ---
    the function takes binary form of original image, start and end point coordinates and solves the maze
    to return the list of coordinates of shortest path from initial_point to final_point

    Input Arguments:
    ---
    `original_binary_img` :	[ numpy array ]
            binary form of the original image at img_file_path
    `initial_point` :		[ tuple ]
            start point coordinates
    `final_point` :			[ tuple ]
            end point coordinates
    `no_cells_height` :		[ int ]
            number of cells in height of maze image
    `no_cells_width` :		[ int ]
            number of cells in width of maze image

    Returns:
    ---
    `shortestPath` :		[ list ]
            list of coordinates of shortest path from initial_point to final_point

    Example call:
    ---
    shortestPath = solveMaze(
        original_binary_img, initial_point, final_point, no_cells_height, no_cells_width)

    """

    shortestPath = []

    #############	Add your Code here	###############
    lengthOfPath = 0
    # initialise visited cell array with 0
    visited = np.zeros([no_cells_width, no_cells_height], dtype=int)
    # build all neighbours graph for each cell
    graph = buildGraph(original_binary_img, no_cells_width, no_cells_height)
    # build a child:parent relationship to recontruct the shortest path
    parentDict = findPath(graph, visited,
                          initial_point[0], initial_point[1], final_point[0], final_point[1], lengthOfPath)
    # reconstruct the shortest path
    shortestPath = constructPath(parentDict, initial_point, final_point)
    ###################################################

    return shortestPath


#############	You can add other helper functions here		#############
def buildGraph(original_binary_img, no_cells_width, no_cells_height):
    graph = {}
    for i in range(0, no_cells_width):
        for j in range(0, no_cells_height):
            graph[(i, j)] = findNeighbours(original_binary_img, i, j)
    return graph


def findNeighbours(original_binary_img, row, column):
    x_start = column * CELL_SIZE
    y_start = row * CELL_SIZE
    neighbours = []
    top = True
    bottom = True
    left = True
    right = True
    for x in range(x_start, x_start + CELL_SIZE):
        if(original_binary_img[y_start, x] == 255 and top):
            # when no black bars are found on top of any cell
            neighbours.append((row-1, column))
            top = False
        if(original_binary_img[y_start + CELL_SIZE - 1, x] == 255 and bottom):
            # when no black bars are found on bottom of any cell
            neighbours.append((row+1, column))
            bottom = False

    for y in range(y_start, y_start + CELL_SIZE):
        if(original_binary_img[y, x_start] == 255 and left):
            # when no black bars are found on left of any cell
            neighbours.append((row, column-1))
            left = False
        if(original_binary_img[y, x_start + CELL_SIZE - 1] == 255 and right):
            # when no black bars are found on right of any cell
            neighbours.append((row, column+1))
            right = False

    return neighbours


def isSafe(graph, visited, i, j, x, y):
    # a cell is safe to travel only if it is a neighbour and
    # it has not been visited
    if((x, y) in graph[(i, j)] and visited[x][y] == 0):
        return True

    return False


def findPath(graph, visited, i, j, x, y, dist):
    # Implementing BFS using queue as list
    queue = []
    queue.insert(0, (i, j, dist))
    visited[i][j] = 1
    parentDict = {}

    while(len(queue)):
        node = queue[-1]  # node = (i,j,dist)
        queue.pop()

        if(node[0] == x and node[1] == y):
            return parentDict

        # up
        if(isSafe(graph, visited, node[0], node[1], node[0]-1, node[1])):
            queue.insert(0, ((node[0] - 1, node[1], node[2]+1)))
            parentDict[(node[0]-1, node[1])] = (node[0], node[1])
            visited[node[0]][node[1]] = 1

        # left
        if(isSafe(graph, visited, node[0], node[1], node[0], node[1]-1)):
            parentDict[(node[0], node[1]-1)] = (node[0], node[1])
            queue.insert(0, ((node[0], node[1]-1, node[2]+1)))
            visited[node[0]][node[1]] = 1

        # right
        if(isSafe(graph, visited, node[0], node[1], node[0], node[1] + 1)):
            parentDict[(node[0], node[1]+1)] = (node[0], node[1])
            queue.insert(0, ((node[0], node[1] + 1, node[2]+1)))
            visited[node[0]][node[1]] = 1

        # down
        if(isSafe(graph, visited, node[0], node[1], node[0]+1, node[1])):
            parentDict[(node[0]+1, node[1])] = (node[0], node[1])
            queue.insert(0, ((node[0] + 1, node[1], node[2]+1)))
            visited[node[0]][node[1]] = 1


def constructPath(parent, initial_point, final_point):
    children = []
    shortest = []

    for key in parent.keys():
        children.insert(0, key)
    while(1):
        shortest.insert(0, final_point)
        if(parent[final_point] == initial_point):
            shortest.insert(0, initial_point)
            break
        final_point = parent[final_point]
    return shortest
#########################################################################


# NOTE:	YOU ARE NOT ALLOWED TO MAKE ANY CHANGE TO THIS FUNCTION
#
# Function Name:	main
# Inputs:			None
# Outputs: 			None
# Purpose: 			the function first takes 'maze00.jpg' as input and solves the maze by calling readImage
# 					and solveMaze functions, it then asks the user whether to repeat the same on all maze images
# 					present in 'task_1a_images' folder or not


if __name__ == '__main__':

    curr_dir_path = os.getcwd()
    # path to directory of 'task_1a_images'
    img_dir_path = curr_dir_path + '/../task_1a_images/'

    file_num = 0
    img_file_path = img_dir_path + 'maze0' + \
        str(file_num) + '.jpg'		# path to 'maze00.jpg' image file

    print('\n============================================')

    print('\nFor maze0' + str(file_num) + '.jpg')

    try:

        original_binary_img = readImage(img_file_path)
        height, width = original_binary_img.shape

    except AttributeError as attr_error:

        print('\n[ERROR] readImage function is not returning binary form of original image in expected format !\n')
        exit()

    # number of cells in height of maze image
    no_cells_height = int(height/CELL_SIZE)
    # number of cells in width of maze image
    no_cells_width = int(width/CELL_SIZE)
    initial_point = (0, 0)											# start point coordinates of maze
    # end point coordinates of maze
    final_point = ((no_cells_height-1), (no_cells_width-1))

    try:

        shortestPath = solveMaze(
            original_binary_img, initial_point, final_point, no_cells_height, no_cells_width)

        if len(shortestPath) > 2:

            img = image_enhancer.highlightPath(
                original_binary_img, initial_point, final_point, shortestPath)

        else:

            print(
                '\n[ERROR] shortestPath returned by solveMaze function is not complete !\n')
            exit()

    except TypeError as type_err:

        print('\n[ERROR] solveMaze function is not returning shortest path in maze image in expected format !\n')
        exit()

    print('\nShortest Path = %s \n\nLength of Path = %d' %
          (shortestPath, len(shortestPath)))

    print('\n============================================')

    cv2.imshow('canvas0' + str(file_num), img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    choice = input(
        '\nWant to run your script on all maze images ? ==>> "y" or "n": ')

    if choice == 'y':

        file_count = len(os.listdir(img_dir_path))

        for file_num in range(file_count):

            img_file_path = img_dir_path + 'maze0' + str(file_num) + '.jpg'

            print('\n============================================')

            print('\nFor maze0' + str(file_num) + '.jpg')

            try:

                original_binary_img = readImage(img_file_path)
                height, width = original_binary_img.shape

            except AttributeError as attr_error:

                print(
                    '\n[ERROR] readImage function is not returning binary form of original image in expected format !\n')
                exit()

            # number of cells in height of maze image
            no_cells_height = int(height/CELL_SIZE)
            # number of cells in width of maze image
            no_cells_width = int(width/CELL_SIZE)
            initial_point = (0, 0)											# start point coordinates of maze
            # end point coordinates of maze
            final_point = ((no_cells_height-1), (no_cells_width-1))

            try:

                shortestPath = solveMaze(
                    original_binary_img, initial_point, final_point, no_cells_height, no_cells_width)

                if len(shortestPath) > 2:

                    img = image_enhancer.highlightPath(
                        original_binary_img, initial_point, final_point, shortestPath)

                else:

                    print(
                        '\n[ERROR] shortestPath returned by solveMaze function is not complete !\n')
                    exit()

            except TypeError as type_err:

                print(
                    '\n[ERROR] solveMaze function is not returning shortest path in maze image in expected format !\n')
                exit()

            print('\nShortest Path = %s \n\nLength of Path = %d' %
                  (shortestPath, len(shortestPath)))

            print('\n============================================')

            cv2.imshow('canvas0' + str(file_num), img)
            cv2.waitKey(0)
            cv2.destroyAllWindows()

    else:

        print('')
