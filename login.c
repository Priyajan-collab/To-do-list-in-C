#include <stdio.h>
#include <string.h>
#define buffer1 30
#define buffer2 20
FILE *logInInfo;
char logdetails[100];
void saveInfo(char auth1[buffer1], char auth2[buffer2]);
void signUp();
void logInSys();
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
    logInSys();
    // printf("Bye\n");
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
  logInInfo = fopen("Logininfo.txt", "a");

  // fputs("\n", logInInfo);
  // fputs("***", logInInfo);
  fputs(auth1, logInInfo);
  fputs(" ", logInInfo);
  fputs(auth2, logInInfo);
  // fputs("***", logInInfo);
  fputs("\n", logInInfo);
  fclose(logInInfo);
}

void logInSys() {
  char getUser[buffer1], getPw[buffer2];
  int ind1, ind2;
  printf("Please enter your Username and password\n");
  printf("Username \n");
  scanf("%s", getUser);
  printf("Password \n");
  scanf("%s", getPw);
  // yesma ayeko string lai modify gareko, eutai array ma haleko
  for (ind1 = 0; getUser[ind1] != '\0'; ind1++)
    ;
  getUser[ind1] += ' ';
  ind1++;
  getUser[ind1] = '\0';

  for (ind2 = 0; getPw[ind2] != '\0'; ind2++) {
    getUser[ind1] += getPw[ind2];
    ind1++;
    getUser[ind1] = '\0';
  }
  getUser[ind1] = '\n';
  ind1++;
  getUser[ind1] = '\0';
  // printf("\n\t%s", getUser);
  // aba balla user ko authenticity check hane ho
  char authenticate[buffer1];
  logInInfo = fopen("logininfo.txt", "r");
  while (fgets(authenticate, 30, logInInfo)) {
    int checkValidity = strcmp(authenticate, getUser);
    // printf("\n%d", checkValidity);
    // printf("\n%s", authenticate);
    if (checkValidity == 0) {
      printf("Welcome %s", authenticate);
      break;
    } else if (feof(logInInfo)) {
      printf("\nnot found");
      break;
    } else {
      printf("\nchecking...");
    }
  }
  fclose(logInInfo);
}

// void fixString(char authval1[], char authval2[]) {
//   int ind1, ind2;
//   for (ind1 = 0; authval1[ind1] != '\0'; ind1++)
//     ;
//   authval1[ind1] += ' ';
//   ind1++;
//   authval1[ind1] += '\0';
//   for (ind2 = 0; authval2[ind2] != '\0'; ind2++) {
//     authval1[ind1] += authval2[ind2];
//     ind1++;
//     authval1[ind1] += '\0';
//   }

//   strcpy(logdetails, authval1);
// }