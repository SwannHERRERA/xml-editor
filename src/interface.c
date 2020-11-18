#include "interface.h"

void init_interface(int *argc, char ***argv)
{
  gtk_init(argc, argv);
  GtkBuilder *builder = NULL;
  GuiData *data = malloc(sizeof(GuiData));
  data->file_name = NULL;
  builder = gtk_builder_new_from_file("./ui/main_window.glade");
  connect_widgets(builder, data);

  gtk_widget_show_all(GTK_WIDGET(data->widgets->window));
  gtk_builder_connect_signals(builder, NULL);
  gtk_main();
  g_slice_free(GuiWidgets, data->widgets);
  g_slice_free(MenuItems, data->widgets->items);
  if (data->file_name != NULL)
  {
    g_free(data->file_name);
  }
  free(data);
}

void connect_widgets(GtkBuilder *builder, GuiData *data)
{
  data->widgets = g_slice_new(GuiWidgets);
  data->widgets->items = g_slice_new(MenuItems);
  data->widgets->window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
  data->widgets->main_text_view = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "main_text_view"));
  data->widgets->items->quit_button = GTK_MENU_ITEM(gtk_builder_get_object(builder, "exit_app"));
  data->widgets->items->save_file = GTK_MENU_ITEM(gtk_builder_get_object(builder, "save_file"));
  data->widgets->items->open_file = GTK_MENU_ITEM(gtk_builder_get_object(builder, "open_file"));
  g_signal_connect(data->widgets->window, "destroy", (GCallback)gtk_main_quit, NULL);
  g_signal_connect(data->widgets->items->open_file, "activate", (GCallback)menu_button_open_file, (gpointer)data);
  g_signal_connect(data->widgets->items->save_file, "activate", (GCallback)menu_button_save_file, (gpointer)data);
}

void raise_error(char *buffer)
{
  fprintf(stderr, "%s", buffer);
}

//Listeners

void menu_button_open_file(GtkWidget *widget, gpointer data)
{
  printf("Opening file choose dialog\n");
  GuiData *gui_data = (GuiData *)data;
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File", gui_data->widgets->window, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
  gint res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    gui_data->file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    printf("File selected : %s \n", gui_data->file_name);
    FILE *file = fopen(gui_data->file_name, "r");
    if (file == NULL)
    {
      fclose(file);
      fprintf(stderr, "Error opening file %s\n", gui_data->file_name);
      exit(EXIT_FAILURE);
    }
    GtkTextBuffer *buffer = NULL;
    char *str = NULL;
    buffer = gtk_text_view_get_buffer(gui_data->widgets->main_text_view);
    str = file_get_content(file);
    gtk_text_buffer_set_text(buffer, str, -1);
  }
  gtk_widget_destroy(dialog);
}

void menu_button_save_file(GtkWidget *widget, gpointer data)
{
  printf("Saving file\n");
  GuiData *gui_data = (GuiData *)data;
  if (gui_data->file_name != NULL)
  {
    save_to_file(gui_data->file_name, "");
  }
}

void menu_button_quit()
{
  printf("Exiting program\n");
  exit(0);
}
