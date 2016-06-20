extern char **environ;

int main(int argc, char *argv[], char *envp[])
{
	return envp == environ ? 0 : 1;
}
