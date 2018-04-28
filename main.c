#include <stdio.h>
#include <stdlib.h>

typedef struct segment {
    int startx;
    int starty;
    int endx;
    int endy;
    int falldir;
    int capacity;
    struct segment * fallsOn;
} segment;

int getInput() {
    int n;
    scanf("%d",&n);
    return n;
}

int getY(int x, segment current) {
    int a = (abs(current.endy - current.starty)/abs(current.startx - current.endx));
    return (current.endy + a*(x - current.endx));
}

int amountBlocked(segment current, segment compare) {
    if (current.starty < compare.starty || current.starty < compare.endy || current.endy < compare.starty || current.endy < compare.endy) {
        if (current.startx >= compare.startx && current.endx <= compare.endx) {
            int comparey = getY(current.endx, compare);
            if (comparey > current.endy) {
                return (current.endx - current.startx);
            }
        } else if (current.startx >= compare.startx && current.startx <= compare.endx) {
            int comparey = getY(current.startx, compare);
            if (comparey > current.endy) {
                return (compare.endx - current.startx);
            }
        } else if (current.startx <= compare.startx && current.endx >= compare.endx) {
            int comparey = getY(current.endx, compare);
            if (comparey > current.endy) {
                return (compare.endx - compare.startx);
            }
        } else if (current.endx > compare.startx && current.endx <= compare.endx) {
            int comparey = getY(current.endx, compare);
            if (comparey > current.endy) {
                return (current.endx - compare.startx);
            }
        }
    }
    return 0;
}

void findBlocked(segment * segments, int length, int current) {
    int original = segments[current].capacity;
    for (int i = 0 ; i < length ; i++) {
        if (i != current) {
            int n = amountBlocked(segments[current], segments[i]);
            if ((original - n) < segments[current].capacity) {
                segments[current].capacity = (original - n);
            }
        }
    }
}

int checkValidFall(segment current, segment compare) { //Returns weather or not the current segment falls on the segment it compares to.
    if (current.falldir && compare.falldir) { //Both segments fall left.
        if (current.startx <= compare.endx && current.startx >= compare.startx) {
            int comparey = getY(current.startx, compare);
            if (comparey < current.starty) {
                return 1;
            }
        }
    } else if (current.falldir && !compare.falldir) { //Current falls left, compare falls right.
        if (current.startx <= compare.endx && current.startx >= compare.startx) {
            int comparey = getY(current.startx, compare);
            if (comparey < current.starty) {
                return 1;
            }
        }
    } else if (!current.falldir && compare.falldir) { //Current falls right, compare falls left.
        if (current.endx <= compare.endx && current.endx >= compare.startx) {
            int comparey = getY(current.endx, compare);
            if (comparey < current.endy) {
                return 1;
            }
        }
    } else { //Both fall right.
        if (current.endx <= compare.endx && current.endx >= compare.startx) { //End point x value is between compare start and end point.
            int comparey = getY(current.endx, compare);
            if (comparey < current.endy) {
                return 1;
            }
        }
    }
    return 0;
}

void findFallsOn(segment * segments, int length, int current) { //Cycles through all combinations of two segments, and finds out which fall on which.
    for (int i = 0 ; i < length ; i++) {
        if (i != current) { //Doesn't compare a segment with itself.
            if (checkValidFall(segments[current], segments[i])) {
                segments[current].fallsOn = &segments[i];
                break;
            }
        }
    }
}

segment initSegment() { //Initializes each segment, one at a time.
    int input[4];
    scanf("%d %d %d %d", &input[0], &input[1], &input[2], &input[3]);
    struct segment segment;
    segment.startx = input[0];
    segment.starty = input[1];
    segment.endx = input[2];
    segment.endy = input[3];
    segment.capacity = abs(segment.startx-segment.endx); //Calculates the capacity of each segment by how long it reaches horizontally.
    if(segment.starty < segment.endy) { //Specifies what way the segment falls. Left or right.
        segment.falldir = 1;
    } else {
        segment.falldir = 0;
    }
    segment.fallsOn = NULL;
    return segment;
}

void printSegments(segment * segments, int length) {
    for (int i = 0 ; i < length ; i++) {
        printf("%p %d %d %d %d %d %d %p\n", &segments[i], segments[i].startx, segments[i].starty, segments[i].endx, segments[i].endy, segments[i].falldir, segments[i].capacity, segments[i].fallsOn);
    }
}

int main(void) {
    /*
    int length = getInput(); //Variable for number of segments.
    segment segments[length];
    for (int i = 0 ; i < length ; i++) { //Loads each segment, one at a time.
        segments[i] = initSegment();
    }
    */

    int length = 6;
    segment segments[] = {
            {13, 7, 15, 6, 0, abs(13-15)},  //2
            {3, 8, 7, 7, 0, abs(3-7)},      //4
            {1, 7, 5, 6, 0, abs(1-5)},      //2
            {5, 5, 9, 3, 0, abs(5-9)},      //11
            {6, 3, 8, 2, 0, abs(6-8)},      //0
            {9, 6, 12, 8, 1, abs(9-12)}     //3
    };
    /*
    int length = 2;
    segment segments[] = {
            {8, 9, 10, 7, 0, abs(8-10)},  //0
            {4, 6, 11, 13, 0, abs(4-11)}     //7
    };
     */
    for (int i = 0 ; i < length ; i++) { //First checks if any of the segments contribute to another segments total capacity.
        findFallsOn(segments, length, i);
    }
    for (int i = 0 ; i < length ; i++) { //Checks if a segment is blocked by another segment.
        findBlocked(segments, length, i);
    }
    for (int i = 0 ; i < length ; i++) { //Updates each segments capacity, with regards to what falls on it from other segments.
        if (segments[i].fallsOn) {
            segments[i].fallsOn->capacity += segments[i].capacity;
        }
    }
    for (int i = 0 ; i < length ; i++) { //Prints the capacity of each segment in order they were inputted.
        printf("%d\n", segments[i].capacity);
    }
    return 0;
}