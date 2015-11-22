#include <xc.h>
#include "constants.h"
#include "comms.h"

void UpdateData(void)
{
    toAuto.Front = pastBufs_get(&Front);
    toAuto.Left = pastBufs_get(&Left);
    toAuto.Rignt = pastBufs_get(&Right);
    toAuto.Back = pastBufs_get(&Back);
    toAuto.ShortLeft = datBufs_ave(&BufShortLeft);
    toAuto.ShortRight = datBufs_ave(&BufShortRight);

    EncoderRightStorage = EncoderRight;
    EncoderRight = 0;
    EncoderLeftStorage = EncoderLeft;
    EncoderLeft = 0;

    toAuto.DistanceRight = Distance(EncoderRightStorage);
    toAuto.DistanceLeft = Distance(EncoderLeftStorage);

    static int LeftCounter = 0;
    static int RightCounter = 0;

    if (EncoderLeftStorage == 0)
    {
        if (LeftCounter > 10)
        {
            SpeedLeft = 0;
        } else
        {
            LeftCounter++;
        }
    } else
    {
        LeftCounter = 0;
    }

    if (EncoderRightStorage == 0)
    {
        if (RightCounter > 10)
        {
            SpeedRight = 0;
        } else
        {
            RightCounter++;
        }
    } else
    {
        RightCounter = 0;
    }

    toAuto.SpeedRight = SpeedRight;
    toAuto.SpeedLeft = SpeedLeft;


}

void UpdateComms(void)
{
    StateReceived = fromAuto.state;
}

unsigned int Distance(unsigned int pulses)
{
    return pulses * 418;
}

unsigned int Speed(unsigned int time)
{
    return time;
}