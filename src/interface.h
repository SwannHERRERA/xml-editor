#ifndef _interface_h_
#define _interface_h_
#include <gtk/gtk.h>

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

void connect_widgets(GtkBuilder *builder);
void init_interface(int *argc, char ***argv);

// Listeners
void menu_button_quit();
#endif