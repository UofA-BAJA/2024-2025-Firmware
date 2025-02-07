#include <Arduino.h>

enum DataTypes{
    RPM_FRONT_L = 0x000,
    RPM_FRONT_R = 0x001,
    RPM_BACK_L = 0x002,
    RPM_BACK_R = 0x003,
    MOTOR_RPM = 0x004,
    IMU_ROTATION_X = 0x005,
    IMU_ROTATION_Y = 0x006,
    IMU_ROTATION_Z = 0x007,
    //THE_FRIENDS_WE_MADE_ALONG_THE_WAY = 0x108,
    BRAKE_PRESSURE_REAR = 0x008,
    BRAKE_PRESSURE_FRONT = 0x009,
    CAR_SPEED = 0x00A,
    IMU_ACCELERATION_X = 0x00B,
    IMU_ACCELERATION_Y = 0x00C,
    IMU_ACCELERATION_Z = 0x00D,
    CVT_TEMPERATURE = 0x00E,
    // The reserves are here because we can have a total of 32 data types that can be transmitted live
    RESERVE_16,
    RESERVE_17,
    RESERVE_18,
    RESERVE_19,
    RESERVE_20,
    RESERVE_21,
    RESERVE_22,
    RESERVE_23,
    RESERVE_24,
    RESERVE_25,
    RESERVE_26,
    RESERVE_27,
    RESERVE_28,
    RESERVE_29,
    RESERVE_30,
    RESERVE_31,
    RESERVE_32
    // Do not add more data types! Replace one of the reserved slots with it!
};

int hz = 2;

int popcount64b(uint64_t x);

// The chance a packet has to be a null packet.
float nullPacketChance = .1;


void setup()
{

	randomSeed(analogRead(0));

	Serial.begin(115200);

}


void loop()
{


	// Extremely basic test:

	float packet[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	// First create the packet
	int id = 0;

	int bits = random(1, 6);

	for(int i = 0; i < bits; i++){
		int rand = random(0, 31);

		id |= (1 << rand);

	}

	memcpy(&packet[0], &id, sizeof(int));
	packet[1] = millis() / 1000.0;


	for(int i = 2; i < bits + 2; i++){

		float rand = (float) random(-10000, 10000) / 100.0;

		packet[i] = rand;

	}


    //   Serial.print(id, BIN);
    //   Serial.print(" ");

    //   for(int i = 1; i < 8; i++){

    //     Serial.print(packet[i]);

    //     Serial.print(" ");

    //   }


	char dataAsCharacters[32];

	memcpy(dataAsCharacters, packet, 8 * sizeof(float));

	for(int i = 0; i < 32; i++){
		Serial.print(dataAsCharacters[i]);
	}

	Serial.println();

	float delayTime = (1.0 / hz) * 1000;

	delay(delayTime);

}
