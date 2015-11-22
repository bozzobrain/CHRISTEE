#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include "constants.h"
#include "calculations.h"

void MainCalculations(void) {

//    pastBufs_put(&Front, SensorSelectFront());
//    pastBufs_put(&Left, SensorSelectLeft());
//    pastBufs_put(&Right, SensorSelectRight());
//    pastBufs_put(&Back, SensorSelectRight());


    int holderFront = SensorSelectFront();
    int holderLeft = SensorSelectLeft();
    int holderRight = SensorSelectRight();
    int holderBack = SensorSelectBack();

    if (varIsAbout(pastBufs_get(&Front), holderFront, RANGE) == true) {
        pastBufs_put(&Front, holderFront);
    }
    else {
        pastBufs_put(&Front, validRead(pastBufs_get(&Front), holderFront));
    }

    if (varIsAbout(pastBufs_get(&Left), holderLeft, RANGE) == true) {
        pastBufs_put(&Left, holderLeft);
    }
    else {
        pastBufs_put(&Left, validRead(pastBufs_get(&Left), holderLeft));
    }

    if (varIsAbout(pastBufs_get(&Right), holderRight, RANGE) == true) {
        pastBufs_put(&Right, holderRight);
    }
    else {
        pastBufs_put(&Right, validRead(pastBufs_get(&Right), holderRight));
    }

    if (varIsAbout(pastBufs_get(&Back), holderBack, RANGE) == true) {
        pastBufs_put(&Back, holderBack);
    }
    else {
        pastBufs_put(&Back, validRead(pastBufs_get(&Back), holderBack));
    }

}

void PreCalculations(void) {

    //filter for the front
    if (datBufs_get(&BufLongFront) < MAX_LONG) {
        if (((datBufs_get(&BufMediumFrontRight) + datBufs_get(&BufMediumFrontLeft)) / 2) > MAX_MEDIUM) {
            pastBufs_put(&Front, datBufs_get(&BufLongFront));
        } else if (((datBufs_get(&BufMediumFrontRight) + datBufs_get(&BufMediumFrontLeft)) / 2) < MIN_LONG) {
            pastBufs_put(&Front, (datBufs_get(&BufMediumFrontRight) + datBufs_get(&BufMediumFrontLeft)) / 2);
        } else {
            pastBufs_put(&Front, (datBufs_get(&BufMediumFrontRight) + datBufs_get(&BufMediumFrontLeft) + datBufs_get(&BufLongFront)) / 3);
        }
    } else {
        pastBufs_put(&Front, MAX_LONG);
    }


    //filter for the right
    if (datBufs_get(&BufLongRight) < MAX_LONG) {
        if (datBufs_get(&BufMediumRight) > MAX_MEDIUM) {
            pastBufs_put(&Right, datBufs_get(&BufLongRight));
        } else if (datBufs_get(&BufMediumRight) < MIN_LONG) {
            pastBufs_put(&Right, datBufs_get(&BufMediumRight));
        } else {
            pastBufs_put(&Right, (datBufs_get(&BufMediumRight) + datBufs_get(&BufLongRight)) / 2);
        }

    } else {
        pastBufs_put(&Right, MAX_LONG);
    }


    //filter for the left
    if (datBufs_get(&BufLongLeft) < MAX_LONG) {
        if (datBufs_get(&BufMediumLeft) > MAX_MEDIUM) {
            pastBufs_put(&Left, datBufs_get(&BufLongLeft));
        } else if (datBufs_get(&BufMediumLeft) < MIN_LONG) {
            pastBufs_put(&Left, datBufs_get(&BufMediumLeft));
        } else {
            pastBufs_put(&Left, (datBufs_get(&BufMediumLeft) + datBufs_get(&BufLongLeft)) / 2);
        }

    } else {
        pastBufs_put(&Left, MAX_LONG);
    }

    //filter for the back
    if (datBufs_get(&BufLongBack) < MAX_LONG) {
        if (datBufs_get(&BufMediumBack) > MAX_MEDIUM) {
            pastBufs_put(&Back, datBufs_get(&BufLongBack));
        } else if (datBufs_get(&BufMediumBack) < MIN_LONG) {
            pastBufs_put(&Back, datBufs_get(&BufMediumBack));
        } else {
            pastBufs_put(&Back, (datBufs_get(&BufMediumBack) + datBufs_get(&BufLongBack)) / 2);
        }

    } else {
        pastBufs_put(&Back, MAX_LONG);
    }

}

void CalculationsLong(void) {

    insertionSort(LongBack, sizeof (LongBack));
    insertionSort(LongRight, sizeof (LongRight));
    insertionSort(LongFront, sizeof (LongFront));
    insertionSort(LongLeft, sizeof (LongLeft));

    int a;
    for (a = 0; a < 8; a++) {
        SortLongBack[a] = LongBack[(a + 4)];
        SortLongRight[a] = LongRight[(a + 4)];
        SortLongFront[a] = LongFront[(a + 4)];
        SortLongLeft[a] = LongLeft[(a + 4)];
    }


    datBufs_put(&BufLongBack, LongConvertion(Average(&SortLongBack, sizeof (SortLongBack))));
    datBufs_put(&BufLongRight, LongConvertion(Average(&SortLongRight, sizeof (SortLongRight))));
    datBufs_put(&BufLongFront, LongConvertion(Average(&SortLongFront, sizeof (SortLongFront))));
    datBufs_put(&BufLongLeft, LongConvertion(Average(&SortLongLeft, sizeof (SortLongLeft))));


}

void CalculationsMedium(void) {
    insertionSort(MediumBack, sizeof (MediumBack));
    insertionSort(MediumLeft, sizeof (MediumLeft));
    insertionSort(MediumRight, sizeof (MediumRight));
    insertionSort(MediumFrontRight, sizeof (MediumFrontRight));
    insertionSort(MediumFrontLeft, sizeof (MediumFrontLeft));
    insertionSort(ShortRight, sizeof (ShortRight));
    insertionSort(ShortLeft, sizeof (ShortLeft));

    int r;
    for (r = 0; r < 8; r++) {
        SortMediumBack[r] = MediumBack[(r + 4)];
        SortMediumLeft[r] = MediumLeft[(r + 4)];
        SortMediumRight[r] = MediumRight[(r + 4)];
        SortMediumFrontRight[r] = MediumFrontRight[(r + 4)];
        SortMediumFrontLeft[r] = MediumFrontLeft[(r + 4)];
        SortShortRight[r] = ShortRight[(r + 4)];
        SortShortLeft[r] = ShortLeft[(r + 4)];
    }

    datBufs_put(&BufMediumBack, MediumConvertion(Average(&SortMediumBack, sizeof (SortMediumBack))));
    datBufs_put(&BufMediumLeft, MediumConvertion(Average(&SortMediumLeft, sizeof (SortMediumLeft))));
    datBufs_put(&BufMediumRight, MediumConvertion(Average(&SortMediumRight, sizeof (SortMediumRight))));
    datBufs_put(&BufMediumFrontRight, MediumConvertion(Average(&SortMediumFrontRight, sizeof (SortMediumFrontRight))));
    datBufs_put(&BufMediumFrontLeft, MediumConvertion(Average(&SortMediumFrontLeft, sizeof (SortMediumFrontLeft))));
    datBufs_put(&BufShortRight, ShortConvertion(Average(&SortShortRight, sizeof (SortShortRight))));
    datBufs_put(&BufShortLeft, ShortConvertion(Average(&SortShortLeft, sizeof (SortShortLeft))));
}

unsigned int Average(unsigned int * Array, unsigned int size) {
    size = size / 2;
    unsigned int sum = 0;
    int x;
    for (x = 0; x < size; x++) {
        sum = sum + Array[x];
    }
    return (sum / size);
}

unsigned int LongConvertion(unsigned int counts) {
    //place the convertion here for the longs
    if (counts < 1730){

        return (550);
    } else {
int cm = 3300000000*pow(counts, -2.156);
        return (cm);
    }
}

unsigned int MediumConvertion(unsigned int counts) {
    //place the convertion here for the mediums
int cm = 184139*pow(counts, -1.124);
        return (cm);

}

unsigned int ShortConvertion(unsigned int counts) {
    //place the convertion here for the shorts
    int cm = 24075*pow(counts, -1.062);
    return (cm);
}

void MoveADCSamples(__eds__ unsigned int * Get, unsigned int * Store) {
    int h;
    for (h = 0; h < SAMP_BUF_SIZE; h++) {
        Store[h] = Get[h];
    }
}

int SensorSelectRight(void) {
    if (datBufs_get(&BufLongRight) < MAX_LONG) {
        if (datBufs_get(&BufMediumRight) > MAX_MEDIUM) {
            return (datBufs_get(&BufLongRight));
        } else if (datBufs_get(&BufMediumRight) < MIN_LONG) {
            return (datBufs_get(&BufMediumRight));
        } else {
            return ((datBufs_get(&BufMediumRight) + datBufs_get(&BufLongRight)) / 2);
        }

    } else {
        return (MAX_LONG);
    }
}

int SensorSelectLeft(void) {
    if (datBufs_get(&BufLongLeft) < MAX_LONG) {
        if (datBufs_get(&BufMediumLeft) > MAX_MEDIUM) {
            return (datBufs_get(&BufLongLeft));
        } else if (datBufs_get(&BufMediumLeft) < MIN_LONG) {
            return (datBufs_get(&BufMediumLeft));
        } else {
            return ((datBufs_get(&BufMediumLeft) + datBufs_get(&BufLongLeft)) / 2);
        }

    } else {
        return (MAX_LONG);
    }
}

int SensorSelectFront(void) {
    if (datBufs_get(&BufLongFront) < MAX_LONG) {
        if (((datBufs_get(&BufMediumFrontRight) + datBufs_get(&BufMediumFrontLeft)) / 2) > MAX_MEDIUM) {
            return (datBufs_get(&BufLongFront));
        } else if (((datBufs_get(&BufMediumFrontRight) + datBufs_get(&BufMediumFrontLeft)) / 2) < MIN_LONG) {
            return ((datBufs_get(&BufMediumFrontRight) + datBufs_get(&BufMediumFrontLeft)) / 2);
        } else {
            return ((datBufs_get(&BufMediumFrontRight) + datBufs_get(&BufMediumFrontLeft) + datBufs_get(&BufLongFront)) / 3);
        }
    } else {
        return (MAX_LONG);
    }




}

int SensorSelectBack(void) {
    if (datBufs_get(&BufLongBack) < MAX_LONG) {
        if (datBufs_get(&BufMediumBack) > MAX_MEDIUM) {
            return (datBufs_get(&BufLongBack));
        } else if (datBufs_get(&BufMediumBack) < MIN_LONG) {
            return (datBufs_get(&BufMediumBack));
        } else {
            return ((datBufs_get(&BufMediumBack) + datBufs_get(&BufLongBack)) / 2);
        }

    } else {
        return (MAX_LONG);
    }
}

bool varIsAbout(int past, int current, int plusMinus) {

    if ((current <(past + plusMinus)) && (current > (past - plusMinus))){
        return true;
    } else {return false;}

   
}

int validRead(int past, int current) {
    int signalDifference = current - past;
    return (past + (signalDifference * WEIGHTED_AVE));
   
}