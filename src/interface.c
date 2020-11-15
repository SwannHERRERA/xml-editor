#include "interface.h"

void init_interface(int *argc, char ***argv)
{
  gtk_init(argc, argv);
  GtkWidget *window = NULL;
  GtkBuilder *builder = NULL;

  builder = gtk_builder_new_from_file("./ui/main_window.glade");
  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  g_signal_connect(window, "destroy", (GCallback)gtk_main_quit, NULL);

  gtk_widget_show_all(window);
  gtk_main();
}