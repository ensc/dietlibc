
extern int __dns_decodename(unsigned char *packet,unsigned int offset,unsigned char *dest,unsigned int maxlen);

int dn_expand(unsigned char *msg, unsigned char *eomorig, unsigned char *comp_dn, unsigned char *exp_dn, int length) {
  return __dns_decodename(msg,comp_dn-msg,exp_dn,length)-(comp_dn-msg);
}

