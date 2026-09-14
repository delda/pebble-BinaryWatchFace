#include "common.h"

char bufferTime[20];
unsigned int hour, minute;
int s_bulletsNumber[2];
int s_bufferTime[2][6];
int shape;
int color;
int number;
int bluetooth;
int battery;
int date;
int help_num;
int snow;
int show_heart_rate;
int show_steps;
int bluetooth_status;
int battery_level;
int battery_modality;

char *logTime(){
  time_t now = time(NULL);
  strftime(bufferTime, 20, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
  return bufferTime;
}
