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
GtkWidget *deleteTask;

int authStatus = 0, latestLine = 0;
char filename[30];

static void showMessage(const gchar *message);
// static void hideMessage();
static void signUpCheck(GtkWidget *button, gpointer data);
static void signUpPage(GtkWidget *button, gpointer data);
static void logInPage(GtkWidget *button, gpointer data);
void removeTask(GtkWidget *button, gpointer *data);

// yeta chai sab function haru for GUI so that i don't have to create everytime
// from scratch each time
static void clearWindow(GtkWidget *window) {
  gtk_window_set_child(window, NULL);
}
static void showWindow(GtkWidget *window, GtkWidget *box) {
  //   gtk_widget_show(window);
  gtk_window_set_child(window, box);
  gtk_window_present(GTK_WINDOW(window));
}
static void createButton(GtkWidget *parent, const char *label,
                         GCallback callback, gpointer data) {
  GtkWidget *button = gtk_button_new_with_label(label);
  g_signal_connect(button, "clicked", callback, data);
  gtk_box_append(GTK_BOX(parent), button);
}
static void createDeleteButton(GtkWidget *parent, const char *label,
                               GCallback callback, gpointer data) {
  GtkWidget *button = gtk_button_new_with_label(label);

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

static void createLabel(GtkWidget *parent, const char *text) {
  GtkWidget *label = gtk_label_new(text);
  gtk_box_append(GTK_BOX(parent), label);
}

void addSpacetofront(char *str) {
  int length = strlen(str);
  for (int i = length; i >= 0; i--) {
    str[i + 1] = str[i];
  }
  str[0] = ' ';
}

static void createLabelTask(GtkWidget *parent, const char *text) {
  GtkWidget *label1 = gtk_label_new(text);
  gtk_box_append(GTK_BOX(parent), label1);
  createDeleteButton(box, "X", G_CALLBACK(removeTask), text);

  gtk_widget_set_name(label1, "text-label");
}

static void createSpacer(GtkWidget *parent, int height) {
  GtkWidget *spacer = gtk_label_new(NULL);
  gtk_widget_set_size_request(spacer, -1, height);
  gtk_box_append(GTK_BOX(parent), spacer);
}

static void createTextBox(GtkWidget *parent, GtkWidget **textbox) {
  GtkWidget *entry = gtk_entry_new();
  gtk_box_append(GTK_BOX(parent), entry);

  *textbox = entry;
}

static void switchPage(GtkWidget *parent, const char *label,
                       GtkWidget *widget) {
  createSpacer(parent, 30);
  createLabel(parent, label);
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
  fputs("\n", userLogged);

  fclose(userLogged);
  showTask();
}

void removeTask(GtkWidget *button, gpointer *data) {
  const char *text = (const char *)data;
  FILE *temp = fopen("temp.txt", "w");
  char empty[20];
  char tasktodel[1024];
  char filename_c[30];

  int lineNumber = 1;

  userLogged = fopen(filename, "r+");
  if (userLogged == NULL) {
    printf("Failed to open the file.\n");
    return;
  }

  int count;
  fseek(userLogged, 0, SEEK_SET);
  while (fgets(tasktodel, 1024, userLogged)) {
    tasktodel[strcspn(tasktodel, "\n")] = '\0';

    // count = strcmp(tasktodel, text);
    // printf("%s", tasktodel);
    // printf("%s", text);
    // printf("%d", count);
    if (strcmp(tasktodel, text) != 0) {
      fputs(tasktodel, temp);
      fputs("\n", temp);
    }
  }
  fclose(userLogged);
  fclose(temp);

  printf("%s", filename);

  userLogged = fopen(filename, "w");
  temp = fopen("temp.txt", "r+");
  if (userLogged == NULL) {
    printf("no");
  }
  char tasktoadd[1024];
  while (fgets(tasktoadd, 1024, temp)) {
    // tasktodel[strcspn(tasktoadd, "\n")] = '\0';
    printf("%s", tasktoadd);

    // count = strcmp(tasktoadd, text);
    // printf("%s", tasktoadd);
    // printf("%s", text);
    // printf("%d", count);

    fputs(tasktoadd, userLogged);
  }
  fclose(temp);
  fclose(userLogged);
  remove("temp.txt");
}

// printf("chalyo");

void showTask() {
  char buffer[100];
  int currentline = 0;
  userLogged = fopen(filename, "r");
  // userLogged = fopen(filename, "a+");

  while (fgets(buffer, sizeof(buffer), userLogged)) {
    // Remove trailing newline character

    buffer[strcspn(buffer, "\n")] = '\0';
    if (currentline >= latestLine) {
      createLabelTask(box, buffer);
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

  createTextBox(box, &textbox1);
  createTextBox(box, &textbox2);
  createButton(box, "SIGNUP", G_CALLBACK(signUpCheck), NULL);

  switchPage(box, "Already have an account", window);
  createButton(box, "Login", G_CALLBACK(logInPage), data);

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

  createTextBox(box, &textbox1);
  createTextBox(box, &textbox2);
  createButton(box, "LOGIN", G_CALLBACK(loginCheck), NULL);

  switchPage(box, "Don't have an account", window);
  createButton(box, "SignUP", G_CALLBACK(signUpPage), data);

  g_signal_connect(window, "destroy", G_CALLBACK(g_abort), NULL);

  showWindow(window, box);
}
void todopage(char username[30]) {
  clearWindow(window);

  strcpy(filename, username);
  // aba balla add garne banaudai xu
  strcat(filename, ".txt");
  userLogged = fopen(filename, "a+");
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_name(box, "todo-box");

  createTextBox(box, &addbox);
  createButton(box, "add", G_CALLBACK(addTask), NULL);
  gtk_widget_set_name(addbox, "add-box");
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
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  createLabel(box, "Welcome to Optimizelyf");
  createSpacer(box, 100);

  createButton(box, "logInPage", G_CALLBACK(logInPage), app);
  createButton(box, "Signuppage", G_CALLBACK(signUpPage), app);
  // widget haru lai name diyeko so that i can design
  gtk_widget_set_name(box, "welcome-page-box");
  gtk_widget_set_name(window, "background-window");

  g_signal_connect(window, "destroy", G_CALLBACK(g_abort), NULL);

  showWindow(window, box);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("app1.test1.org", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(welcome), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
