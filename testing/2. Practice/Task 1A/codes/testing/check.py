import numpy as np
import cv2
import sys

n = input('Enter maze number: ')
img = cv2.imread('maze0' + n + '.jpg')

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
maze_size = int(gray.shape[0]/20)
initial_point = (0, 0)
final_point = (maze_size-1, maze_size-1)
visited = np.zeros([maze_size, maze_size], dtype=int)


def buildGraph(gray):
    graph = {}
    for i in range(0, maze_size):
        for j in range(0, maze_size):
            graph[(i, j)] = findNeighbours(gray, i, j)
    return graph


def findNeighbours(gray, row, column):
    x_start = column * 20
    y_start = row * 20
    neighbours = []
    top = True
    bottom = True
    left = True
    right = True

    for x in range(x_start, x_start + 20):
        # print(i, y_start)
        # gray[y_start, i] = 122
        if(gray[y_start, x] == 255 and top):
            neighbours.append((row-1, column))
            top = False
        if(gray[y_start + 19, x] == 255 and bottom):
            neighbours.append((row+1, column))
            bottom = False

    for y in range(y_start, y_start + 20):
        if(gray[y, x_start] == 255 and left):
            neighbours.append((row, column-1))
            left = False
        if(gray[y, x_start + 19] == 255 and right):
            neighbours.append((row, column+1))
            right = False

    return neighbours


def isSafe(graph, visited, i, j, x, y):
    if((x, y) in graph[(i, j)] and visited[x][y] == 0):
        return True

    return False


def findPath(graph, visited, i, j, x, y, dist):
    queue = []
    queue.insert(0, (i, j, dist))
    # q.enqueue((i, j, dist))
    visited[i][j] = 1
    prev = {}

    while(len(queue)):
        node = queue[-1]  # node = (i,j,dist)
        # node = q.front()  # node = (i,j,dist)
        queue.pop()
        # q.dequeue()

        if(node[0] == x and node[1] == y):
            # prev[(x, y)] = (node[0], node[1])
            return prev

        # up
        if(isSafe(graph, visited, node[0], node[1], node[0]-1, node[1])):
            queue.insert(0, ((node[0] - 1, node[1], node[2]+1)))
            prev[(node[0]-1, node[1])] = (node[0], node[1])
            visited[node[0]][node[1]] = 1

        # down
        if(isSafe(graph, visited, node[0], node[1], node[0]+1, node[1])):
            prev[(node[0]+1, node[1])] = (node[0], node[1])
            queue.insert(0, ((node[0] + 1, node[1], node[2]+1)))
            visited[node[0]][node[1]] = 1

        # left
        if(isSafe(graph, visited, node[0], node[1], node[0], node[1]-1)):
            prev[(node[0], node[1]-1)] = (node[0], node[1])
            queue.insert(0, ((node[0], node[1]-1, node[2]+1)))
            visited[node[0]][node[1]] = 1

        # right
        if(isSafe(graph, visited, node[0], node[1], node[0], node[1] + 1)):
            prev[(node[0], node[1]+1)] = (node[0], node[1])
            queue.insert(0, ((node[0], node[1] + 1, node[2]+1)))
            visited[node[0]][node[1]] = 1


def constructPath(parent):
    children = []
    shortest = []

    for key in parent.keys():
        children.insert(0, key)

    # print(children)
    final = (maze_size-1, maze_size-1)
    while(1):
        shortest.insert(0, final)
        if(parent[final] == (0, 0)):
            shortest.insert(0, (0, 0))
            break
        final = parent[final]

    return shortest


graph = buildGraph(gray)
path = findPath(graph, visited,
                initial_point[0], initial_point[1], final_point[0], final_point[1], 0)
shortest = constructPath(path)
print(shortest, len(shortest))
# findPath(graph, visited, 0, 0, 9, 9, min_dist, 1)
# cv2.imshow('canvas', gray)
# cv2.waitKey(0)
# cv2.destroyAllWindows()
