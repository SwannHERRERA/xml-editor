#ifndef _interface_h_
#define _interface_h_
#include <gtk/gtk.h>

typedef struct FileHandlerData FileHandlerData;
struct FileHandlerData
{
  char *file_name;
};

typedef struct
{
  GtkMenuItem *new_file;
  GtkMenuItem *open_file;
  GtkMenuItem *save_file;
  GtkMenuItem *save_file_as;
  GtkMenuItem *quit_button;
} MenuItems;

typedef struct
{
  GtkWindow *window;
  MenuItems *items;
} Widgets;

void connect_widgets(GtkBuilder *builder, FileHandlerData **data);
void init_interface(int *argc, char ***argv);
void raise_error(char *buffer);

// Listeners
void menu_button_quit();
void menu_button_about();
void menu_button_save_file_as();
void menu_button_save_file();
void menu_button_open_file(GtkWidget *widget, gpointer data);
void menu_button_new_file();
#endif