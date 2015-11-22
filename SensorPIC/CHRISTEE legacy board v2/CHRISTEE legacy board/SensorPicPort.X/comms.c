#include <xc.h>
#include "constants.h"
#include "comms.h"


void UpdateData(void) {
//    toAuto.Front = datBufs_get(&BufLongFront);
//    toAuto.Back = datBufs_get(&BufLongBack);
//    toAuto.Left = datBufs_get(&BufLongLeft);
//    toAuto.Rignt = datBufs_get(&BufLongRight);


    toAuto.Front = pastBufs_get(&Front);
    toAuto.Left = pastBufs_get(&Left);
    toAuto.Rignt = pastBufs_get(&Right);
    toAuto.Back = pastBufs_get(&Back);
    toAuto.EncoderRight = EncoderRight;
    EncoderRight = 0;
    toAuto.EncoderLeft = EncoderLeft;
    EncoderLeft = 0;
    toAuto.ShortLeft = datBufs_ave(&BufShortLeft);
    toAuto.ShortRight = datBufs_ave(&BufShortRight);
}

void UpdateComms(void) {
    StateReceived = fromAuto.state;
}