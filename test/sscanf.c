#include <stdio.h>

int main() {
  char server_version_string[]="SSH-1.99-OpenSSH_2.9p2\n";
  int remote_major, remote_minor;
  char remote_version[1000];
  if (sscanf(server_version_string, "SSH-%d.%d-%[^\n]\n",
	    &remote_major, &remote_minor, remote_version) != 3) return 1;
  if (remote_major!=1 || remote_minor!=99 || strcmp(remote_version,"OpenSSH_2.9p2"))
    return 1;
//  printf("%d.%d.%.100s\n",remote_major,remote_minor,remote_version);
  return 0;
}
