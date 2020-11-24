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
  data->widgets->items->save_file_as = GTK_MENU_ITEM(gtk_builder_get_object(builder, "save_file_as"));
  data->widgets->items->validate_xml = GTK_MENU_ITEM(gtk_builder_get_object(builder, "validate_xml"));
  g_signal_connect(data->widgets->window, "destroy", (GCallback)gtk_main_quit, NULL);
  g_signal_connect(data->widgets->main_text_view, "populate-popup", (GCallback)text_view_populate_popup_menu, (gpointer)data);
  g_signal_connect(data->widgets->items->quit_button, "activate", (GCallback)gtk_main_quit, NULL);
  g_signal_connect(data->widgets->items->open_file, "activate", (GCallback)menu_button_open_file, (gpointer)data);
  g_signal_connect(data->widgets->items->save_file, "activate", (GCallback)menu_button_save_file, (gpointer)data);
  g_signal_connect(data->widgets->items->save_file_as, "activate", (GCallback)menu_button_save_file_as, (gpointer)data);
  g_signal_connect(data->widgets->items->validate_xml, "activate", (GCallback)start_xml_validation, (gpointer)data);
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
    if (gui_data->file_name != NULL)
    {
      free(gui_data->file_name);
    }
    gui_data->file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    printf("File selected : %s \n", gui_data->file_name);
    FILE *file = fopen(gui_data->file_name, "r");
    if (file == NULL)
    {
      fprintf(stderr, "Error opening file %s\n", gui_data->file_name);
    }
    GtkTextBuffer *buffer = NULL;
    char *str = file_get_content(file);
    buffer = gtk_text_view_get_buffer(gui_data->widgets->main_text_view);
    gtk_text_buffer_set_text(buffer, str, -1);
    if (str != NULL)
    {
      free(str);
    }
    fclose(file);
  }
  gtk_widget_destroy(dialog);
}

void menu_button_save_file(GtkWidget *widget, gpointer data)
{
  printf("Saving file\n");
  GuiData *gui_data = (GuiData *)data;
  if (gui_data->file_name == NULL)
  {
    menu_button_save_file_as(widget, data);
  }
  if (gui_data->file_name != NULL)
  {
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(gui_data->widgets->main_text_view);
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *str = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    save_to_file(gui_data->file_name, str);
    if (str != NULL)
    {
      free(str);
    }
  }
}

void menu_button_save_file_as(GtkWidget *widget, gpointer data)
{
  printf("Saving file as\n");
  GuiData *gui_data = (GuiData *)data;
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Save file as", gui_data->widgets->window, GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel", GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
  gint res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    if (gui_data->file_name != NULL)
    {
      free(gui_data->file_name);
    }
    gui_data->file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(gui_data->widgets->main_text_view);
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *str = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    save_to_file(gui_data->file_name, str);
    if (str != NULL)
    {
      free(str);
    }
  }
  gtk_widget_destroy(dialog);
}

void text_view_populate_popup_menu(GtkWidget *widget, GtkWidget *popup, gpointer data)
{
  GtkWidget *item = gtk_menu_item_new_with_label("Validate");
  gtk_menu_attach(GTK_MENU(popup), item, 0, 1, 7, 8);
  g_signal_connect(item, "activate", (GCallback)start_xml_validation, (gpointer)data);
  gtk_widget_show_all(popup);
}

void start_xml_validation(GtkWidget *widget, gpointer data)
{
  printf("Validating XML\n");
  GuiData *gui_data = (GuiData *)data;
  if (gui_data->file_name == NULL)
  {
    fprintf(stderr, "Error opening file try saving it first\n");
    return;
  }
  GtkTextIter start, end;
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(gui_data->widgets->main_text_view);
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  char *str = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
  char *root_name;
  char *dtd_string = find_doctype(str, &root_name);
  if (dtd_string != NULL)
  {
    DTD_Element *dtd = parse_dtd(dtd_string, root_name);

    printf("\n######## Starting PARSE XML ########\n");

    xml_element *root = parse_xml(str);
    // print_element(root);
    printf("\n######## Finished PARSE XML ########\n");

    if (check_dtd_correspond_to_xml(dtd, root))
    {
      printf("XML is corresponding to DTD\n");
    }
    else
    {
      printf("XML is NOT corresponding to DTD\n");
    }

    if (dtd != NULL)
    {
      free_DTD(dtd);
    }
    free_element(root);
    free(root_name);
  }

  free(str);
  free(dtd_string);
}