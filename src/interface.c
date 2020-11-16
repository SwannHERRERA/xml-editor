#include "interface.h"

static Widgets *widgets;

void init_interface(int *argc, char ***argv)
{
  gtk_init(argc, argv);
  GtkBuilder *builder = NULL;

  builder = gtk_builder_new_from_file("./ui/main_window.glade");
  connect_widgets(builder);

  g_signal_connect(widgets->window, "destroy", (GCallback)gtk_main_quit, NULL);

  gtk_widget_show_all(GTK_WIDGET(widgets->window));
  gtk_builder_connect_signals(builder, NULL);
  gtk_main();
  g_slice_free(Widgets, widgets);
}

void connect_widgets(GtkBuilder *builder)
{
  widgets = g_slice_new(Widgets);
  widgets->window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
  widgets->items = g_slice_new(MenuItems);
  widgets->items->quit_button = GTK_MENU_ITEM(gtk_builder_get_object(builder, "exit_app"));
}

//Listeners

void menu_button_quit()
{
  printf("Exiting program\n");
  exit(0);
}
