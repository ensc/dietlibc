#include <mntent.h>

main() {
  FILE* f=fopen("test/fstab","r");
  struct mntent* e;
  while ((e=getmntent(f))) {
#if 0
                      char    *mnt_fsname;    /* name of mounted file system */
                      char    *mnt_dir;       /* file system path prefix */
                      char    *mnt_type;      /* mount type (see mntent.h) */
                      char    *mnt_opts;      /* mount options (see mntent.h) */
                      int     mnt_freq;       /* dump frequency in days */
                      int     mnt_passno;     /* pass number on parallel fsck */
#endif
    printf("fsname %s\n  dir %s\n  type %s\n  opts %s\n  freq %d\n  passno %d\n\n",
	   e->mnt_fsname,e->mnt_dir,e->mnt_type,e->mnt_opts,e->mnt_freq,e->mnt_passno);
  }
}
