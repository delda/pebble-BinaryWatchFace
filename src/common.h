#pragma once
#include <pebble.h>

// Variabili per definire il tempo
extern char bufferTime[20];
extern unsigned int hour, minute;

// Variabili per definire i layer dell'orologio
extern int s_bulletsNumber[2];
extern int s_bufferTime[2][6];

// Opzioni di visualizzazione dei componenti dell'orologio
extern int shape;
extern int color;
extern int number;
extern int bluetooth;
extern int battery;
extern int date;
extern int help_num;
extern int snow;
extern int show_heart_rate;
extern int show_steps;

// Bluetooth
extern int bluetooth_status;
#define BT_NEVER 0
#define BT_ON_DISCONNECT 1
#define BT_ALWAYS 2

// Battery
extern int battery_level;
extern int battery_modality;
#define BA_NEVER 0
#define BA_UNDER_20_PERC 1
#define BA_ALWAYS 2
#define BA_PERCENT_WARNING 30

// Date string
#define DEBUG 0

// Struttura che contiene la paletta dei colori per ogni singola modalità
typedef struct{
  GColor background;
  GColor text;
  GColor strokeDot;
  GColor fillDot;
  GColor time;
} Color;

char *logTime();
