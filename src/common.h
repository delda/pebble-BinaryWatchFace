#pragma once
#include <pebble.h>

// Variabili per definire il tempo
char bufferTime[20];
unsigned int hour, minute;
static char *s_textBase[]={"1","2","4","8","16","32"};

// Variabili per definire i layer dell'orologio
int s_bulletsNumber[2];
int s_bufferTime[2][6];

// Opzioni di visualizzazione dei componenti dell'orologio
int shape;
int color;
int number;
int bluetooth;
int battery;
int dotIndex;
int date;
int help_num;

// Bluetooth
int bluetooth_status;
static int BT_NEVER         = 0;
static int BT_ON_DISCONNECT = 1;
static int BT_ALWAYS        = 2;

// Battery
int battery_level;
int battery_option;
int battery_modality;
char battery_buffer[5];
static int BA_NEVER           = 0;
static int BA_UNDER_20_PERC   = 1;
static int BA_ALWAYS          = 2;
static int BA_PERCENT_WARNING = 30;
static int BA_PERCENT_ALARM   = 10;

// Date string
char date_buffer[30];
char bufferLocale[6];

// Struttura che contiene la paletta dei colori per ogni singola modalità
typedef struct{
  GColor background;
  GColor text;
  GColor strokeDot;
  GColor fillDot;
  GColor time;
} Color;

char *logTime();