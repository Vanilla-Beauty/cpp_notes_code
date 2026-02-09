#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  uid_t euid = geteuid();
  printf("Effective User ID: %d\n", euid);

  if (euid == 0) {
    printf("Running as root, special permissions are available.\n");
  } else {
    printf("Not running as root, limited permissions.\n");
  }

  return 0;
}