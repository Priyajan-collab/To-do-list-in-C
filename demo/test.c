#include <stdio.h>
#include <string.h>

void main() {
  char test[5];
  char ans[5] = "yes";
  printf("Enter test data\n");
  scanf("%s", test);

  printf("%s", test);

  int value = strcmp(ans, test);

  printf("%d", value);

  if (value == 0) {
    printf("fine");
  } else {
    printf("fail");
  }
}