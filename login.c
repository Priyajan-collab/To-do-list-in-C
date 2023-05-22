#include <stdio.h>
#include <string.h>
void signup();
struct UserData {
  char username[30];
  char password[20];
};

void main() {
  char response[2], answer[2] = "y";
  printf("Do you want to sign up, y/n \n");
  scanf("%c", response);
  int value = strcmp(response, answer);
  printf("%d", value);
  if (value == 0) {
    signUp();
  } else {
    printf("Bye\n");
  }
}

void signUp() {
  char name[30];
  char password[30];
  struct UserData User1;
  printf("Welcome, Thank you for choosing to join our community\n");
  printf("Please Enter appropriate User Name\n");
  scanf("%s", name);
  printf("Please Enter appropriate Password\n");
  scanf("%s", password);
  strcpy(User1.username, name);
  strcpy(User1.password, password);
  printf("%s\n", User1.username);
  printf("%s\n", User1.password);
}