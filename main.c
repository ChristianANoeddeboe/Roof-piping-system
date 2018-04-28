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

/**
 * Uses the equiation for the line, to check the y value of a particular point of a segment.
 * @param x - The point on our segment we want to know the y value for.
 * @param current - The segment we will be checking.
 * @return - Returns the y value for the segment.
 */
int getY(int x, segment current) {
    int a = (abs(current.endy - current.starty)/abs(current.startx - current.endx));
    return (current.endy + a*(x - current.endx));
}

/**
 * Checks how much a segment is blocked by another segment.
 * @param current - Segment we want to check for how much it is blocked.
 * @param compare - Segment which maybe blocks our current segment.
 * @return The amount our current segment is blocked by the compared segment.
 */
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

/**
 * Checks every segment, if another segment blocks it.
 * @param segments - All roof segments.
 * @param length - Amount of segments in the array.
 */
void findBlocked(segment * segments, int length) {
    for (int i = 0 ; i < length ; i++) {
        int original = segments[i].capacity;
        for (int j = 0 ; j < length ; j++) {
            if (j != i) {
                int n = amountBlocked(segments[i], segments[i]);
                if ((original - n) < segments[i].capacity) {
                    segments[i].capacity = (original - n);
                }
            }
        }
    }
}

/**
 * Returns weather or not the current segment falls on the segment it compares to.
 * @param current - The segment we're checking if it falls on another.
 * @param compare - The segment we would like to know if another falls on.
 * @return 1 if it falls on it, 0 if it does not.
 */
int checkValidFall(segment current, segment compare) {
    if (current.falldir && compare.falldir) {                                       //Both segments fall left.
        if (current.startx <= compare.endx && current.startx >= compare.startx) {
            int comparey = getY(current.startx, compare);
            if (comparey < current.starty) {
                return 1;
            }
        }
    } else if (current.falldir && !compare.falldir) {                               //Current falls left, compare falls right.
        if (current.startx <= compare.endx && current.startx >= compare.startx) {
            int comparey = getY(current.startx, compare);
            if (comparey < current.starty) {
                return 1;
            }
        }
    } else if (!current.falldir && compare.falldir) {                               //Current falls right, compare falls left.
        if (current.endx <= compare.endx && current.endx >= compare.startx) {
            int comparey = getY(current.endx, compare);
            if (comparey < current.endy) {
                return 1;
            }
        }
    } else { //Both fall right.
        if (current.endx <= compare.endx && current.endx >= compare.startx) {       //End point x value is between compare start and end point.
            int comparey = getY(current.endx, compare);
            if (comparey < current.endy) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Cycles through all roof segments, to check if any of them delivers water to another piece. Also checks if the segment to compare to is itself.
 * @param segments - The segments for the roof.
 * @param length - The total amount of segments.
 */
void findFallsOn(segment * segments, int length) {
    for (int i = 0 ; i < length ; i++) {
        for (int j = 0 ; j < length ; j++) {
            if (j != i) {
                if (checkValidFall(segments[i], segments[j])) {
                    segments[i].fallsOn = &segments[j];
                    break;
                }
            }
        }
    }
}

segment initSegment() {
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

int main() {

    //Variable for number of segments.
    int length = getInput();
    segment segments[length];
    //Loads each segment, one at a time.
    for (int i = 0 ; i < length ; i++) {
        segments[i] = initSegment();
    }

    /* Used to test with. Value each segment should return is right of every line.
    int length = 6;
    segment segments[] = {
            {13, 7, 15, 6, 0, abs(13-15)},  //2
            {3, 8, 7, 7, 0, abs(3-7)},      //4
            {1, 7, 5, 6, 0, abs(1-5)},      //2
            {5, 5, 9, 3, 0, abs(5-9)},      //11
            {6, 3, 8, 2, 0, abs(6-8)},      //0
            {9, 6, 12, 8, 1, abs(9-12)}     //3
    };
    */

    //First checks if any of the segments deliver water to another segment.
    findFallsOn(segments, length);

    //Second, checks if any segments receive less water, because it is blocked by another segment.
    findBlocked(segments, length);

    //Third, update each segments capacity, with regards to what falls on it from other segments.
    for (int i = 0 ; i < length ; i++) {
        if (segments[i].fallsOn) {
            segments[i].fallsOn->capacity += segments[i].capacity;
        }
    }

    //Last, print the capacity of each segment in order they were entered.
    for (int i = 0 ; i < length ; i++) {
        printf("%d\n", segments[i].capacity);
    }

    return 0;
}