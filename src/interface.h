#ifndef _interface_h_
#define _interface_h_
#include <gtk/gtk.h>
#include "file_helper.h"
#include "parse_dtd.h"
#include "parse_xml.h"
#include "check_xml_corresponding.h"

typedef struct
{
  GtkMenuItem *new_file;
  GtkMenuItem *open_file;
  GtkMenuItem *save_file;
  GtkMenuItem *save_file_as;
  GtkMenuItem *quit_button;
  GtkMenuItem *validate_xml;
} MenuItems;

typedef struct
{
  GtkWindow *window;
  GtkTextView *main_text_view;
  GtkStatusbar *status_bar;
  MenuItems *items;
} GuiWidgets;

typedef struct
{
  char *file_name;
  GuiWidgets *widgets;
} GuiData;

void connect_widgets(GtkBuilder *builder, GuiData *data);
void init_interface(int *argc, char ***argv);
void raise_error(char *buffer);

// Listeners
void menu_button_quit();
void menu_button_about();
void menu_button_save_file_as(GtkWidget *widget, gpointer data);
void menu_button_save_file();
void menu_button_open_file(GtkWidget *widget, gpointer data);
void menu_button_new_file(GtkWidget *widget, gpointer data);
void text_view_populate_popup_menu(GtkWidget *widget, GtkWidget *popup, gpointer data);
void start_xml_validation(GtkWidget *widget, gpointer data);
#endif