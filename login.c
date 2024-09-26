#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 30

GtkWidget *window;
GtkWidget *box;
GtkWidget *label1;
GtkWidget *spacer;
GtkWidget *textbox1;
GtkWidget *textbox2;
GtkWidget *addbox;
GtkWidget *messageWindow;
FILE *userLogged;
GtkWidget *labelbox;
GtkWidget *deleteTask, *checkButton;
GtkWidget *task;
GtkWidget *label;

int authStatus = 0, latestLine = 0;
char filename[30];
char chkv;

static void showMessage(const gchar *message);
// static void hideMessage();
static void signUpCheck(GtkWidget *button, gpointer data);
static void signUpPage(GtkWidget *button, gpointer data);
static void logInPage(GtkWidget *button, gpointer data);
void removeTask(GtkWidget *button, gpointer *data);
void doneTask(GtkWidget *checkbox, gpointer *data);

// yeta chai sab function haru for GUI so that i don't have to create
// everytime from scratch each time
static void clearWindow(GtkWidget *window) {
  gtk_window_set_child(window, NULL);
}
static void showWindow(GtkWidget *window, GtkWidget *box) {
  //   gtk_widget_show(window);
  gtk_window_set_child(GTK_WINDOW(window), box);
  gtk_window_present(GTK_WINDOW(window));
}
static void createButton(GtkWidget *parent, const char *label, const char *name,
                         GCallback callback, gpointer data) {
  GtkWidget *button = gtk_button_new_with_label(label);
  g_signal_connect(button, "clicked", callback, data);
  gtk_widget_set_name(button, name);
  gtk_widget_set_size_request(button, 100, -1);
  gtk_box_append(GTK_BOX(parent), button);
}
static void createDeleteButton(GtkWidget *parent, const char *label,
                               GCallback callback, gpointer data) {
  GtkWidget *button = gtk_button_new_with_label(label);
  gtk_widget_set_name(button, "delbutton");

  g_signal_connect(button, "clicked", callback, (gpointer)g_strdup(data));
  gtk_box_append(GTK_BOX(parent), button);
}

static void showMessage(const gchar *message) {
  messageWindow = gtk_window_new();

  gtk_window_set_title(GTK_WINDOW(messageWindow), "Message");
  gtk_window_set_default_size(GTK_WINDOW(messageWindow), 200, 100);

  GtkWidget *label = gtk_label_new(message);
  gtk_window_set_child(messageWindow, label);

  gtk_window_present(messageWindow);
  //   gtk_widget_show_all(messageWindow);
}

static void createLabel(GtkWidget *parent, const char *text, const char *name) {
  label = gtk_label_new(text);
  gtk_widget_set_name(label, name);
  gtk_box_append(GTK_BOX(parent), label);
}

void addSpacetofront(char *str) {
  int length = strlen(str);
  for (int i = length; i >= 0; i--) {
    str[i + 1] = str[i];
  }
  str[0] = ' ';
}
static void createLabelDoneTask(GtkWidget *parent, const char *text) {
  GtkWidget *donetasklabel = gtk_label_new(text);
  GtkWidget *donetaskbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_append(donetaskbox, donetasklabel);
  gtk_box_append(box, donetaskbox);

  // gtk_widget_set_name(label1, "text-label");
}

static void createLabelTask(GtkWidget *parent, const char *text, char status) {
  GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_append(GTK_BOX(parent), box2);
  gtk_widget_set_halign(box2, GTK_ALIGN_START);

  GtkWidget *label1 = gtk_label_new(text);
  gtk_box_append(GTK_BOX(box2), label1);

  GtkWidget *minibox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_box_append(GTK_BOX(parent), minibox);
  gtk_widget_set_halign(minibox, GTK_ALIGN_END);
  gtk_widget_set_name(minibox, "minibox");

  GtkWidget *btn = gtk_button_new();
  gtk_box_append(GTK_BOX(minibox), btn);
  gtk_widget_set_name(btn, "check-box");

  g_signal_connect(btn, "clicked", G_CALLBACK(doneTask),
                   (gpointer)g_strdup(text));
  if (status == '1') {
    gtk_button_set_label(btn, "Done");
    gtk_widget_set_name(parent, "task-container-done");
    gtk_widget_set_name(btn, "check-box-done");
    // gtk_widget_set_name(label1, "text-done");
    // gtk_check_button_set_active(checkbox,1);
  } else {
    gtk_button_set_label(btn, "UnDone");
    gtk_widget_set_name(parent, "task-container-undone");
    gtk_widget_set_name(btn, "check-box-undone");
    // gtk_widget_set_name(label1, "text-undone");
  }

  printf("%c", status);

  createDeleteButton(minibox, "X", G_CALLBACK(removeTask), text);
}

static void createSpacer(GtkWidget *parent, int height) {
  GtkWidget *spacer = gtk_label_new(NULL);
  gtk_widget_set_size_request(spacer, -1, height);
  gtk_box_append(GTK_BOX(parent), spacer);
}

static void createTextBox(GtkWidget *parent, GtkWidget **textbox,
                          const char *name) {
  GtkWidget *entry = gtk_entry_new();
  gtk_box_append(GTK_BOX(parent), entry);
  gtk_widget_set_name(entry, name);
  if (strcmp(name, "pw")) {
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entry), '*');
  }

  *textbox = entry;
}

static void switchPage(GtkWidget *parent, const char *label,
                       GtkWidget *widget) {
  createSpacer(parent, 5);
  createLabel(parent, label, "NULL");
}

static void loadCSS() {
  GtkCssProvider *css_provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(css_provider, "style.css");
  gtk_style_context_add_provider_for_display(
      gdk_display_get_default(), GTK_STYLE_PROVIDER(css_provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
static void addTask() {
  gchar *task = gtk_entry_get_buffer(addbox);
  gchar *tasktext = gtk_entry_buffer_get_text(task);

  // --------------------------I thought i would make a system where user inputs
  // a line and i will del that
  // --------------------------But that's for weak people
  // char strLatestLine[20];
  // printf("%d", latestLine);
  // sprintf(strLatestLine, "%d", latestLine + 1);
  // addChartofront(text, strLatestLine);
  // if (tasktext[0] != ' ') {
  //   addSpacetofront(tasktext);
  // }

  // strcat(strLatestLine, tasktext);

  userLogged = fopen(filename, "a+");

  fputs(tasktext, userLogged);
  fputs(",0\n", userLogged);

  fclose(userLogged);
  showTask();
}

void doneTask(GtkWidget *label1, gpointer *data) {
  char *donetask = (const char *)data;
  GtkWidget *donebox;
  GtkWidget *donelabel;
  gtk_widget_set_name(label1, "done-task");
  // remove task sanga similar function banau

  const char *text = (const char *)data;
  FILE *temp = fopen("temp1.txt", "w");
  char empty[20];
  char username[30];
  char taskdonetemp[20];
  char bufferTask[20];
  strcpy(username, filename);

  int lineNumber = 1;

  userLogged = fopen(filename, "r+");
  if (userLogged == NULL) {
    printf("Failed to open the file.\n");
    return;
  }

  int count;
  printf("Task from INput = %s\n", donetask);
  fseek(userLogged, 0, SEEK_SET);
  while (fgets(taskdonetemp, 1024, userLogged)) {
    strcpy(bufferTask, taskdonetemp);
    taskdonetemp[strcspn(taskdonetemp, "\n")] = '\0';
    chkv = taskdonetemp[strcspn(taskdonetemp, ",") + 1];
    printf("%c", chkv);
    taskdonetemp[strcspn(taskdonetemp, ",")] = '\0';
    if (chkv == '0') {
      if (strcmp(donetask, taskdonetemp) != 0) {
        fputs(bufferTask, temp);
      }

      else {
        sprintf(bufferTask, "%s,1\n", taskdonetemp);
        fputs(bufferTask, temp);
      }
    } else {
      if (strcmp(donetask, taskdonetemp) != 0) {
        fputs(bufferTask, temp);
      } else {
        sprintf(bufferTask, "%s,0\n", taskdonetemp);
        fputs(bufferTask, temp);
      }
    }
  }
  fclose(userLogged);
  fclose(temp);
  userLogged = fopen(filename, "w");
  temp = fopen("temp1.txt", "r+");
  if (userLogged == NULL) {
    printf("no");
  }
  while (fgets(bufferTask, 1024, temp)) {
    fputs(bufferTask, userLogged);
  }
  fclose(temp);
  fclose(userLogged);
  remove("temp1.txt");

  // printf("\nfilename :%s\n", filename);
  username[strcspn(username, ".")] = '\0';
  // printf("k:%s", username);
  latestLine = 0;
  todopage(username);

  createLabelDoneTask(box, donetask);
}

void removeTask(GtkWidget *button, gpointer *data) {
  const char *text = (const char *)data;
  FILE *temp = fopen("temp.txt", "w");
  printf("%s", text);
  char empty[20];
  char tasktodel[1024];
  char username[30];
  char bufferTask[30];
  strcpy(username, filename);
  // printf("%s", filename);

  int lineNumber = 1;

  userLogged = fopen(filename, "r+");
  if (userLogged == NULL) {
    printf("Failed to open the file.\n");
    return;
  }

  int count;
  fseek(userLogged, 0, SEEK_SET);
  while (fgets(tasktodel, 1024, userLogged)) {
    strcpy(bufferTask, tasktodel);
    tasktodel[strcspn(tasktodel, "\n")] = '\0';
    tasktodel[strcspn(tasktodel, ",")] = '\0';
    printf("%s", tasktodel);
    if (strcmp(tasktodel, text) != 0) {
      fputs(bufferTask, temp);
    }
  }
  fclose(userLogged);
  fclose(temp);

  userLogged = fopen(filename, "w");
  temp = fopen("temp.txt", "r+");
  if (userLogged == NULL) {
    printf("no");
  }
  char tasktoadd[1024];
  while (fgets(tasktoadd, 1024, temp)) {
    // tasktodel[strcspn(tasktoadd, "\n")] = '\0';
    // printf("%s", tasktoadd);

    // count = strcmp(tasktoadd, text);
    // printf("%s", tasktoadd);
    // printf("%s", text);
    // printf("%d", count);

    fputs(tasktoadd, userLogged);
  }
  fclose(temp);
  fclose(userLogged);
  remove("temp.txt");

  // printf("\nfilename :%s\n", filename);
  username[strcspn(username, ".")] = '\0';
  // printf("k:%s", username);
  latestLine = 0;
  todopage(username);
}

// printf("chalyo");

void showTask() {
  char buffer[100];
  int currentline = 0;
  userLogged = fopen(filename, "r");
  // userLogged = fopen(filename, "a+");
  GtkWidget *body = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_append(GTK_BOX(box), body);

  createLabel(body, "Tasks To Do: ", "todo-title");
  gtk_label_set_xalign(GTK_LABEL(label), 0);

  while (fgets(buffer, sizeof(buffer), userLogged)) {
    chkv = buffer[strcspn(buffer, ",") + 1];

    // Remove trailing newline character

    buffer[strcspn(buffer, ",")] = '\0';
    printf("\n%c", chkv);
    if (currentline >= latestLine) {
      task = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
      gtk_box_append(GTK_BOX(body), task);
      // gtk_widget_set_name(task, "task-container");
      // gtk_widget_set_name(task,task-container);
      gtk_box_set_homogeneous(GTK_BOX(task), TRUE);

      createLabelTask(task, buffer, chkv);
    }
    currentline += 1;
  }
  latestLine = currentline;
  // printf("file closed");
  fclose(userLogged);
  // gtk_window_set_child(window, labelbox);
}
// yo chai mero main function haru jasma main logic haru use hunxa
static void signUpCheck(GtkWidget *button, gpointer data) {
  char userfilename[20];
  const gchar *buffer1 = gtk_entry_get_buffer(GTK_ENTRY(textbox1));
  const gchar *buffer2 = gtk_entry_get_buffer(GTK_ENTRY(textbox2));
  const char *textbuffer1 = gtk_entry_buffer_get_text(buffer1);
  const char *textbuffer2 = gtk_entry_buffer_get_text(buffer2);

  FILE *logInInfo = fopen("Logininfo.txt", "a+");
  if (logInInfo != NULL) {
    fprintf(logInInfo, "%s %s\n", textbuffer1, textbuffer2);
    fclose(logInInfo);
  }
  strcpy(userfilename, textbuffer1);
  strcat(userfilename, ".txt");
  FILE *userData = fopen(userfilename, "a+");
  if (userData == NULL) {
    printf("\nerror\n");
  }
  fclose(userData);
}

static void signUpPage(GtkWidget *button, gpointer data) {
  //   gtk_widget_set_visible(window, 0);
  //   gtk_widget_set_visible(window, 0);

  clearWindow(window);
  gtk_window_set_title(GTK_WINDOW(window), "SIGNUPPAGE");

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
  gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
  gtk_widget_set_name(box, "login-container");

  GtkWidget *header = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
  gtk_widget_set_name(header, "header");
  gtk_box_append(GTK_BOX(box), header);
  createLabel(header, "SIGN UP", "title");

  GtkWidget *textboxContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
  gtk_box_append(GTK_BOX(box), textboxContainer);
  gtk_widget_set_name(textboxContainer, "textboxContainer");

  GtkWidget *label = gtk_label_new("Username: ");
  gtk_widget_set_name(label, "textboxTitle");
  gtk_box_append(GTK_BOX(textboxContainer), label);
  gtk_label_set_xalign(GTK_LABEL(label), 0);
  createTextBox(textboxContainer, &textbox1, "pw");

  GtkWidget *label1 = gtk_label_new("Password: ");
  gtk_widget_set_name(label1, "textboxTitle");
  gtk_box_append(GTK_BOX(textboxContainer), label1);
  gtk_label_set_xalign(GTK_LABEL(label1), 0);
  createTextBox(textboxContainer, &textbox2, "NULL");

  GtkWidget *btncontainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_box_append(GTK_BOX(box), btncontainer);
  gtk_widget_set_name(btncontainer, "btncontainer");

  createButton(btncontainer, "SIGNUP", "btn", G_CALLBACK(signUpCheck), NULL);
  createLabel(btncontainer, "Don't have an account?", "NULL");
  createButton(btncontainer, "Login", "btn", G_CALLBACK(logInPage), data);
  createSpacer(btncontainer, 8);

  g_signal_connect(window, "destroy", G_CALLBACK(g_abort), NULL);

  showWindow(window, box);
}

static void loginCheck(GtkWidget *button, gpointer data) {
  const gchar *buffer1 = gtk_entry_get_buffer(GTK_ENTRY(textbox1));
  const gchar *buffer2 = gtk_entry_get_buffer(GTK_ENTRY(textbox2));
  const char *textbuffer1 = gtk_entry_buffer_get_text(buffer1);
  const char *textbuffer2 = gtk_entry_buffer_get_text(buffer2);

  char authenticate[BUFFER_SIZE];

  FILE *logInInfo = fopen("Logininfo.txt", "r");
  if (logInInfo != NULL) {
    while (fgets(authenticate, sizeof(authenticate), logInInfo)) {
      char username[BUFFER_SIZE];
      char password[BUFFER_SIZE];
      int j = 0;

      while (authenticate[j] != ' ' && authenticate[j] != '\0') {
        username[j] = authenticate[j];
        j++;
      }
      username[j] = '\0';

      if (strcmp(username, textbuffer1) == 0) {
        int i = j + 1;
        int px = 0;

        while (authenticate[i] != '\n' && authenticate[i] != '\0') {
          password[px] = authenticate[i];
          px++;
          i++;
        }
        password[px] = '\0';

        if (strcmp(password, textbuffer2) == 0) {
          printf("Welcome!\n");
          //   createLabel(GTK_BOX(box), "Login sucessful");

          todopage(username);

          // yesle chai sab clear window garxa
          authStatus = 1;
          // yeta function call garnu parla
          break;
        } else {
          //   printf("Incorrect password\n");
          showMessage("INCORRECT PASSWORD");
        }
      }
    }

    fclose(logInInfo);
  }

  if (authStatus == 1) {
    // printf("auth status active");
  }
}

static void logInPage(GtkWidget *button, gpointer data) {
  clearWindow(window);
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
  gtk_window_set_title(GTK_WINDOW(window), "LOGINPAGE");

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_halign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
  gtk_widget_set_valign(GTK_WIDGET(box), GTK_ALIGN_CENTER);
  gtk_widget_set_name(box, "login-container");

  GtkWidget *header = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
  gtk_widget_set_name(header, "header");
  gtk_box_append(GTK_BOX(box), header);
  createLabel(header, "LOGIN", "title");

  GtkWidget *textboxContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
  gtk_box_append(GTK_BOX(box), textboxContainer);
  gtk_widget_set_name(textboxContainer, "textboxContainer");

  GtkWidget *label = gtk_label_new("Username: ");
  gtk_widget_set_name(label, "textboxTitle");
  gtk_box_append(GTK_BOX(textboxContainer), label);
  gtk_label_set_xalign(GTK_LABEL(label), 0);

  createTextBox(textboxContainer, &textbox1, "pw");
  GtkWidget *label1 = gtk_label_new("Password: ");
  gtk_widget_set_name(label1, "textboxTitle");
  gtk_box_append(GTK_BOX(textboxContainer), label1);
  gtk_label_set_xalign(GTK_LABEL(label1), 0);

  createTextBox(textboxContainer, &textbox2, "NULL");
  GtkWidget *btncontainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_box_append(GTK_BOX(box), btncontainer);
  gtk_widget_set_name(btncontainer, "btncontainer");

  createButton(btncontainer, "LOGIN", "btn", G_CALLBACK(loginCheck), NULL);
  createLabel(btncontainer, "Don't have an account?", "NULL");
  // switchPage(btncontainer, "Don't have an account", window);
  createButton(btncontainer, "SignUP", "btn", G_CALLBACK(signUpPage), data);
  createSpacer(btncontainer, 8);

  g_signal_connect(window, "destroy", G_CALLBACK(g_abort), NULL);

  showWindow(window, box);
}

void todopage(char username[30]) {
  clearWindow(window);
  gtk_window_set_title(GTK_WINDOW(window), "TO-DO-LIST");
  strcpy(filename, username);
  // aba balla add garne banaudai xu
  strcat(filename, ".txt");
  userLogged = fopen(filename, "a+");

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_name(box, "todo-box");

  GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
  gtk_widget_set_name(header, "header");
  gtk_box_append(GTK_BOX(box), header);

  GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_append(GTK_BOX(header), box2);
  gtk_widget_set_halign(box2, GTK_ALIGN_START);
  createButton(box2, "LOG OUT", "NULL", G_CALLBACK(logInPage), NULL);

  createLabel(header, "             Optimizelyf", "title");
  gtk_widget_set_halign(label, GTK_ALIGN_CENTER);

  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_append(GTK_BOX(box), container);
  gtk_widget_set_name(container, "container");
  gtk_widget_set_halign(GTK_WIDGET(container), GTK_ALIGN_CENTER);
  gtk_widget_set_valign(GTK_WIDGET(container), GTK_ALIGN_CENTER);

  // gtk_box_set_homogeneous(GTK_BOX(container), TRUE);

  GtkWidget *contiainTextBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_append(GTK_BOX(container), contiainTextBox);
  gtk_widget_set_name(contiainTextBox, "text-box-container");
  createTextBox(contiainTextBox, &addbox, "add-todo-list");

  createButton(contiainTextBox, "add", "add-btn", G_CALLBACK(addTask), NULL);

  // GtkWidget *checkbtn= gtk_check_button_new();
  //   gtk_box_append(GTK_BOX(container), checkbtn);
  //   gtk_check_button_set_active(checkbtn,1);
  // yeta dekhi modify gara
  if (userLogged == NULL) {
    perror("Failed to open file");
    return;
  } else {
    showTask();
  }

  // Read each line/task and add it to the listbox

  gtk_window_set_child(GTK_WINDOW(window), box);
  fclose(userLogged);
}

static void welcome(GtkApplication *app, gpointer data) {
  window = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
  gtk_window_set_title(GTK_WINDOW(window), "WELCOME");
  loadCSS();
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

  GtkWidget *header = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
  gtk_widget_set_name(header, "header");
  gtk_box_append(GTK_BOX(box), header);
  createLabel(header, "Welcome to Optimizelyf", "title");

  GtkWidget *body = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_box_append(GTK_BOX(box), body);
  createLabel(body, "With Optimizelyf, You can enjoy", "body-text");
  createLabel(body, "your day", "body-text");
  createLabel(body, "without worrying about", "body-text");
  createLabel(body, "missing out.", "body-text");
  gtk_widget_set_name(body, "body");

  GtkWidget *btncontainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_box_append(GTK_BOX(box), btncontainer);
  gtk_widget_set_name(btncontainer, "btncontainer");

  createButton(btncontainer, "LOGIN", "btn", G_CALLBACK(logInPage), app);
  createButton(btncontainer, "Create an account", "btn", G_CALLBACK(signUpPage),
               app);
  // widget haru lai name diyeko so that i can design
  gtk_widget_set_name(box, "welcome-page-box");
  gtk_widget_set_name(window, "background-window");
  createSpacer(box, 5);

  g_signal_connect(window, "destroy", G_CALLBACK(g_abort), NULL);

  showWindow(window, box);
}
// With Optimizelyf you can enjoy your day without worring about missing out.
int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("app1.test1.org", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(welcome), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
