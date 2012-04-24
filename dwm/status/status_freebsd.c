#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/proc.h>

#define STATUS_SIZ 128
#define GETSYSCTL(name, var) getsysctl(name, &(var), sizeof(var))
#define UPDATE_INTERVAL 5

static Display *dpy;

struct sysinfo {
  struct loadavg ldavg;
  long cpu_cycles[CPUSTATES];
};

void setstatus(char *str) {
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

char *getdatetime()
{
	static char buf[64];
	time_t result;
	struct tm *resulttm;

	result = time(NULL);
	resulttm = localtime(&result);

	if(!resulttm)
    perror("Time");

	if(!strftime(buf, sizeof buf, "%a %b %d %H:%M", resulttm)) {
		fprintf(stderr, "strftime is 0.\n");
		exit(1);
	}

	return buf;
}

// Taken from top(8)
void getsysctl(const char *name, void *ptr, size_t len)
{
  size_t nlen = len;

  if (sysctlbyname(name, ptr, &nlen, NULL, 0) == -1) {
    perror("sysctl");
    abort();
  }

  if (nlen != len) {
    fprintf(stderr, "top: sysctl(%s...) expected %lu, got %lu\n",
            name, (unsigned long)len, (unsigned long)nlen);
    abort();
  }
}

const char *get_load_average()
{
  struct loadavg sysload;
  int i;
  static char buf[16];
  char *p;

  p = buf;

  // Load average
  GETSYSCTL("vm.loadavg", sysload);

  for (i = 0; i < 3; i++) {
    p+=snprintf(p, sizeof p, "%.2f", (float)sysload.ldavg[i] / sysload.fscale);

    if (i != 2)
      p += sprintf(p, " ");
  }

  return buf;
}

const char *get_cpu_stats(struct sysinfo *sys)
{
  /* Calculate total cpu utilization in % user, %nice, %system, %interrupt, %idle */
  int state;
  long diff[CPUSTATES], cpu_cycles_now[CPUSTATES];
  double cpu_pct[CPUSTATES];
  long total_change = 0, half_total;
  static char buf[256];
  char *p;
  float pct; // 1-idle

  GETSYSCTL("kern.cp_time", cpu_cycles_now);

  p = buf;

  // state is the counter for cp_times, j for cp_time
  for(state = 0; state < CPUSTATES; state++) {
    diff[state] = cpu_cycles_now[state] - sys->cpu_cycles[state];
    sys->cpu_cycles[state] = cpu_cycles_now[state]; // copy new values to old ones
    total_change += diff[state];
  }

  // don't divide by zero
  if (total_change == 0)
    total_change = 1;

  half_total = total_change / 2l;

  pct = .0;
  for(state=0; state < CPUSTATES-1; state++) { // the last value is idle
    cpu_pct[state] = (double)(diff[state] * 1000 + half_total)/total_change/10.0L;
    pct += cpu_pct[state];
  }

  p += snprintf(p, sizeof buf, "%.1f%%", pct);

  return buf;
}


int main(void)
{
	char status[STATUS_SIZ];
  struct sysinfo sys;
  int i;

  for(i = 0; i < CPUSTATES; i++)
    sys.cpu_cycles[i] = .0;

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "Cannot open display.\n");
		return 1;
	}

	for (;;sleep(UPDATE_INTERVAL)) {
		snprintf(status, sizeof status, "%s :: %s :: %s" , get_cpu_stats(&sys), get_load_average(), getdatetime());
		setstatus(status);
	}

	XCloseDisplay(dpy);

	return 0;
}
