extern int __dns_servers;
extern void __dns_readstartfiles(void);

int res_init(void) {
  __dns_servers=0;
  __dns_readstartfiles();
}
