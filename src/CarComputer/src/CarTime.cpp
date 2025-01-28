#include "CarTime.h"

namespace BajaWildcatRacing
{

    float CarTime::currTimeSeconds = 0.0f;

    float CarTime::getCurrentTimeSeconds(){
        return CarTime::currTimeSeconds;
    }

    void CarTime::setCurrentTimeSeconds(float time){
        CarTime::currTimeSeconds = time;
    }

}
