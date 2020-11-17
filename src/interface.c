#include "interface.h"

static Widgets *widgets;

void init_interface(int *argc, char ***argv)
{
  gtk_init(argc, argv);
  GtkBuilder *builder = NULL;
  FileHandlerData *data = malloc(sizeof(FileHandlerData));
  data->file_name = NULL;
  printf("%p\n", data);
  builder = gtk_builder_new_from_file("./ui/main_window.glade");
  connect_widgets(builder, &data);

  gtk_widget_show_all(GTK_WIDGET(widgets->window));
  gtk_builder_connect_signals(builder, NULL);
  gtk_main();
  g_slice_free(Widgets, widgets);
  g_slice_free(MenuItems, widgets->items);
  if (data->file_name != NULL)
  {
    g_free(data->file_name);
  }
  free(data);
}

void connect_widgets(GtkBuilder *builder, FileHandlerData **data)
{
  widgets = g_slice_new(Widgets);
  widgets->items = g_slice_new(MenuItems);
  widgets->window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
  widgets->items->quit_button = GTK_MENU_ITEM(gtk_builder_get_object(builder, "exit_app"));
  widgets->items->open_file = GTK_MENU_ITEM(gtk_builder_get_object(builder, "open_file"));
  g_signal_connect(widgets->window, "destroy", (GCallback)gtk_main_quit, NULL);
  printf("g sig %p\n", *data);
  g_signal_connect(widgets->items->open_file, "activate", (GCallback)menu_button_open_file, (gpointer)(*data));
}

void raise_error(char *buffer)
{
  fprintf(stderr, "%s", buffer);
}

//Listeners

void menu_button_open_file(GtkWidget *widget, gpointer data)
{
  FileHandlerData *file_data = (FileHandlerData *)data;
  printf("%p\n", data);
  printf("aa\n");
  if (file_data->file_name != NULL)
  {
    printf("%s", file_data->file_name);
  }
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File", widgets->window, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
  gint res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    file_data->file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    printf("aa%s\n", file_data->file_name);
  }
  gtk_widget_destroy(dialog);
}

void menu_button_quit()
{
  printf("Exiting program\n");
  exit(0);
}
