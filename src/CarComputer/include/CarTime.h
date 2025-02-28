#ifndef CARTIME_H
#define CARTIME_H

namespace BajaWildcatRacing
{

    class CarTime{

        public:

            static float getCurrentTimeSeconds();

            static void setCurrentTimeSeconds(float time);

        private:

            static float currTimeSeconds;

        
    };
}


#endif