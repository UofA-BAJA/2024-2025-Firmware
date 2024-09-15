#ifndef IMU_H
#define IMU_H

class IMU{

    public:
        IMU(int can_socket_fd, int can_id);

        double getRotationX();
        double getRotationY();
        double getRotationZ();

        double getAccelerationX();
        double getAccelerationY();
        double getAccelerationZ();
    
    private:
        int can_socket_fd;
        int can_id;
};


#endif