#include <stdio.h>
#include <string.h>
// auto ko lagi ta yo chal cha
// aru pachi halam la
struct user {
  char task[100];
  char time[100];
};

void main() {
  FILE *f1;
  int id, cond;

  printf("Enter your ID: ");
  scanf("%d", &id);

  struct user ilich;

  switch (id) {
    case 1:
      printf("\t--Welcome Ilich--\n");
      printf("To make a new Task list: \nEnter 1\n");
      printf("To add to an existing Task list: \nEnter 2\n");
      printf("to see the list of task: \n Enter 3 \n");
      printf("To remove data from Task list: \nEnter 4\n");
      scanf("%d", &cond);

      switch (cond) {
        case 1:
          printf("New list created\n");
          f1 = fopen("ilich.txt", "w");
          printf("Enter the task: \n");
          scanf(" %[^\n]", ilich.task);
          printf("\n Enter the time:");
          scanf(" %[^\n]", ilich.time);
          fprintf(f1, "%s", ilich.task);
          fprintf(f1, "  %s", ilich.time);
          fclose(f1);
          printf("\nData added\n");
          break;
        case 2:

          f1 = fopen("ilich.txt", "a");
          printf("Enter the task: \n");
          scanf(" %[^\n]", ilich.task);
          printf("\n Enter the time:");
          scanf(" %[^\n]", ilich.time);
          fprintf(f1, "\n %s", ilich.task);
          fprintf(f1, " [%s]", ilich.time);
          fclose(f1);
          printf("\nData added\n");
          break;
        case 3:
          f1 = fopen("ilich.txt", "r");
          printf("\t--The task you have left are given below:--\n\n");
          while ((fgets(ilich.task, sizeof(ilich), f1)) != NULL) {
            printf("%s", ilich);
          }

          fclose(f1);

          break;

        default:
          printf("Invalid response\n");
          break;
      }
      break;
    default:
      printf("Invalid ID\n");
      break;
  }
}