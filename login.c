#include <stdio.h>
#include <string.h>
#define buffer1 30
#define buffer2 20
FILE *logInInfo;
void saveInfo(char auth1[buffer1], char auth2[buffer2]);
void signUp();
struct UserData {
  char username[buffer1];
  char password[buffer2];
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
  char name[buffer1];
  char password[buffer1];
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
  saveInfo(User1.username, User1.password);
}

void saveInfo(char auth1[buffer1], char auth2[buffer2]) {
  // Yo chai file matra create gareko if file xaina vaye
  logInInfo = fopen("Logininfo.txt", "r");
  if (logInInfo == NULL) {
    logInInfo = fopen("logininfo.txt", "w");
    fclose(logInInfo);
  }
  fclose(logInInfo);

  // yeta chai aba save garne ho user ko info
  logInInfo = fopen("Logininfo.txt", "w");
  fputs("\n", logInInfo);
  fputs(auth1, logInInfo);
  fputs("\n", logInInfo);
  fputs(auth2, logInInfo);
  fclose(logInInfo);
}