#include <stdlib.h>
#include <string.h>
#include <errno.h>

int putenv(const char *string) {
  size_t len;
  size_t i, envc;
  int remove=0;
  char *tmp;
  const char **ep;
  char **newenv;
  static char **origenv;
  if (!origenv) origenv=environ;
  if (!(tmp=strchr(string,'='))) {
    len=strlen(string);
    remove=1;
  } else
    len=tmp-string;
  ep=(const char**)environ;
  if (!ep) return 0;
  for (i=envc=0, ep[i]; ep[i]; ++i) {
    if (*string == ep[i][0] &&
	!memcmp(string,*ep,len) &&
	(*ep)[len]=='=') {
      if (remove) continue;
      ep[envc++]=string;
      remove=1;		// remove dupes
    }
  }
  if (!remove) {
    // we were trying to replace something and didn't find it
    // so realloc and add here
    newenv = (char**) realloc(environ==origenv?0:environ,
			      (envc+2)*sizeof(char*));
    if (!newenv) return -1;
    if (envc && (environ==origenv)) {
      memcpy(newenv,origenv,envc*sizeof(char*));
    }
    newenv[envc++]=(char*)string;
    environ=newenv;
  }
  environ[envc]=0;		// terminate env
  return 0;
}

#ifdef UNITTEST
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int
main ()
{
	char* a="FOO=bar";
	char* b="FOO=baz";
	char* fraud[]={a, b, 0};
	environ=fraud;

	// test fraudulent environments
	assert(putenv("FOO") != -1);	// invalid env: FOO appears twice
	assert(getenv("FOO") == 0);	// we want putenv to remove both

	// revert
	free(environ); environ=fraud; fraud[0]=a; fraud[1]=b; fraud[2]=0;
	// make sure FOO=1 removes both previous FOOs
	assert(putenv("FOO=1") != -1);	// invalid env: FOO appears twice
	assert(!strcmp(environ[0],"FOO=1") && environ[1]==0);

	// now do some more basic tests
	static char foo[] = "Hello=World";
	assert (putenv ("foo=bar") != -1);
	assert (!strcmp (getenv ("foo"), "bar"));
	assert (putenv ("foo=baz") != -1);
	assert (!strcmp (getenv ("foo"), "baz"));
	putenv (foo);
	assert (!strcmp (getenv ("Hello"), "World"));
	foo[6] = 'M';
	assert (!strcmp (getenv ("Hello"), "Morld"));

	return 0;
}
#endif
