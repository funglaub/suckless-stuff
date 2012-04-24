#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
/* #include <iwlib.h> */
#include <X11/Xlib.h>

#define STATUS_SIZ 128
#define UPDATE_INTERVAL 5

#define READ_SYS(f, file, buf, fmt, var)        \
  if((f = fopen(file, "r"))){                   \
    for(;;){                                    \
      if(!fgets(buf, sizeof buf - 1, f))        \
        break;                                  \
      if(!sscanf(buf, fmt, var)) {              \
        perror(file);                           \
        abort();                                \
      }                                         \
      break;                                    \
    }                                           \
    fclose(f);                                  \
  }


static Display *dpy;

void setstatus(char *str) {
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

const char *getdatetime()
{
	static char buf[64];
	time_t result;
	struct tm *resulttm;

	result = time(NULL);
	resulttm = localtime(&result);

	if(!resulttm)
    perror("Time");

	if(!strftime(buf, sizeof buf, "%H:%M", resulttm)) {
		fprintf(stderr, "strftime is 0.\n");
		exit(1);
	}

	return buf;
}

const char *getbatteryinfo() {
  static char buf[128];
  FILE *f;
  unsigned long capacity, remaining, rate;
  float percentage, timeleft;
  char state[32], time[32];
  char symbol;
  int hours, minutes;

  strcpy(state, "Unknown");
  strcpy(time,"N/A");

  // charge_* in Ah
  // current_now in A

  READ_SYS(f, "/sys/class/power_supply/BAT0/charge_full", buf, "%lu", &capacity);
  READ_SYS(f, "/sys/class/power_supply/BAT0/charge_now", buf, "%lu", &remaining);
  READ_SYS(f, "/sys/class/power_supply/BAT0/current_now", buf, "%lu", &rate);
  READ_SYS(f, "/sys/class/power_supply/BAT0/status", buf, "%s", state);

  timeleft = .0;

  if(!strcmp(state, "Full") || !strcmp(state, "Charged"))
    symbol = 'f';
  else if(!strcmp(state, "Discharging")) {
    symbol = '-';
    timeleft = (float)remaining / (float)rate; // in hours
  }
  else if(!strcmp(state, "Charging")) {
    symbol = '+';
    timeleft = ((float)capacity - (float)remaining) / (float)rate; // in hours
  }
  else
    symbol = '?';

  percentage = (float)remaining / (float)capacity * 100.;

  hours = (int)floorf(timeleft);
  minutes = (timeleft - (float)hours) * 60;

  snprintf(buf, sizeof buf, "(%c) %.2f%% [%02d:%02dh]", symbol, percentage, hours, minutes);
  return buf;
}



int main(void)
{
	char status[STATUS_SIZ];
  /* int skfd = iw_sockets_open(); */
  /* struct wireless_info w; */

  /* if(!(iw_get_basic_config(skfd, "wlan0", &w.b))) { */
  /*   printf("%s\n", w.b.essid); */
  /* } */

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "Cannot open display.\n");
		return 1;
	}

	for (;;sleep(UPDATE_INTERVAL)) {
		snprintf(status, sizeof status, "%s :: %s", getbatteryinfo(), getdatetime());
		setstatus(status);
	}

	XCloseDisplay(dpy);
	return 0;
}
