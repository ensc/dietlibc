#include <time.h>
#include <linux/sched.h>

int __sched_setparam(pid_t pid, const struct sched_param *p);
int sched_setparam(pid_t pid, const struct sched_param *p);

int __sched_getparam(pid_t pid, struct sched_param *p);
int sched_getparam(pid_t pid, struct sched_param *p);

int __sched_getscheduler(pid_t pid);
int sched_getscheduler(pid_t pid);

int __sched_setscheduler(pid_t pid, int policy, const struct sched_param *p);
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *p);

int __sched_yield(void);
int sched_yield(void);

int __sched_get_priority_max(int policy);
int sched_get_priority_max(int policy);

int __sched_get_priority_min(int policy);
int sched_get_priority_min(int policy);

int __sched_rr_get_interval(pid_t pid, struct timespec *tp);
int sched_rr_get_interval(pid_t pid, struct timespec *tp);

