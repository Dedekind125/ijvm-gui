#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "ijvm.h"
#include "gui.h"
#include "helpers.h"

GUI app;

int main(int argc, char *argv[])
{
    /* Init GUI */
    gtk_init(&argc, &argv);

    /* Builder (glade) */
    app.builder = gtk_builder_new();
    gtk_builder_add_from_file (app.builder, "src/main.glade", NULL);

    /* get widgets from builder */
    app.mainWindow = GTK_WIDGET(gtk_builder_get_object(app.builder, "mainWindow"));
    app.aboutDialog = GTK_WIDGET(gtk_builder_get_object(app.builder, "aboutDialog"));
    app.inputBuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(app.builder, "inputBuffer"));
    app.outputBuffer = GTK_TEXT_BUFFER(gtk_builder_get_object(app.builder, "outputBuffer"));
    app.userInput = GTK_ENTRY(gtk_builder_get_object(app.builder, "userInput"));

    /* set temp files path */
    app.temp_ijvmfile = get_appData_filepath("temp.ijvm");
    app.temp_infile = get_appData_filepath("temp_in.txt");
    app.temp_outfile = get_appData_filepath("temp_out.txt");
    
    app.hasIjvm = false;

    gtk_builder_connect_signals(app.builder, NULL);
    g_object_unref(app.builder);
    gtk_widget_show(app.mainWindow);
    gtk_main();

    return 0;
}

gpointer run_ijvm (gpointer data)
{
    /* set ijvm input & output */
    FILE *fileIn, *fileOut;
    fileIn  = fopen(app.temp_infile,"rb");
    fileOut = fopen(app.temp_outfile, "wb");
    set_output(fileOut);
    set_input(fileIn);

    run();

    fclose(fileIn);
    fclose(fileOut);

    outputFile_to_outputBuffer();
    remove_temp_files();
    
    return false; // to avoid looping
}

/*******************\
* SIGNAL CALLBACKS  *
\*******************/

void on_startButton_clicked(GtkButton *button, GtkLabel *text_label)
{
    gtk_text_buffer_set_text(app.outputBuffer,"",0);
    
    if(!gtk_text_buffer_get_char_count(app.inputBuffer)){ // nothing to run
        return;
    }

    if (app.hasIjvm){ // user clicked run without resetting
        destroy_ijvm();
        app.hasIjvm = false;
    }

    if ((app.ijvmFile == NULL) || (app.ijvmFile == app.temp_ijvmfile)){ // user input binary
        inputBuffer_to_temp_ijvmFile();
    }

    if (init_ijvm(app.ijvmFile) < 0){
        gtk_text_buffer_set_text(app.outputBuffer,"*Couldn't load binary*\0!",-1);
        remove_temp_files();
        return;
    }
    app.hasIjvm = true;
    entry_text_to_inFile();

    /* Create the thread to run the ijvm */
    GThread *thread = g_thread_new("runThread",run_ijvm,NULL);
    g_thread_unref(thread);
}

void on_stopButton_clicked(GtkButton *button, GtkLabel *text_label)
{
    if (get_ijvm_status() == true){ // ijvm is running
        set_ijvm_status(false);
        destroy_ijvm();
        app.hasIjvm = false;

    } else if (app.hasIjvm){ //reset
        destroy_ijvm();
        app.hasIjvm = false;
    }
    gtk_text_buffer_set_text(app.outputBuffer,"",0);
}

void on_saveButton_clicked(GtkButton *button, GtkLabel *text_label)
{
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;

    dialog = gtk_file_chooser_dialog_new ("Save File", (GtkWindow*) app.mainWindow, action,
                                          "_Cancel", GTK_RESPONSE_CANCEL,
                                          "_Open", GTK_RESPONSE_ACCEPT, NULL);

    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT){
        char *filename = gtk_file_chooser_get_filename(chooser);
        save_outputBuffer(filename);
    }
    gtk_widget_destroy (dialog);
}

void on_loadButton_clicked(GtkButton *button, GtkLabel *text_label)
{
    set_ijvm_status(false);
	
    GtkWidget *dialog;
	char *filename = NULL;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;

	dialog = gtk_file_chooser_dialog_new ("Open File", (GtkWindow*) app.mainWindow, action,
		                                  "_Cancel", GTK_RESPONSE_CANCEL,
                                          "_Open", GTK_RESPONSE_ACCEPT, NULL);

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "ijvm files");
    gtk_file_filter_add_pattern(filter, "*.ijvm");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	} else {
        gtk_widget_destroy(dialog);
        return;
    }
    ijvmFile_to_inputBuffer(filename);
    app.ijvmFile = filename;

    gtk_widget_destroy(dialog);
}

void on_infoButton_clicked(GtkButton *button, GtkLabel *text_label)
{
    gtk_widget_show(app.aboutDialog);
}

void on_aboutDialog_delete_event(GtkDialog *dialog, gint response_id, gpointer user_data)
{
    gtk_widget_hide(app.aboutDialog);
}

void on_inputBuffer_changed(GtkTextBuffer *textbuffer, gpointer user_data)
{
    if (app.ijvmFile != NULL){
        app.ijvmFile = NULL;
    }
}

/**********************\
* GTK_BUFFERS HANDLING *
\**********************/

void inputBuffer_to_temp_ijvmFile()
{
    GtkTextIter start, end;
    gchar *hexText;
    FILE *fp;

    gtk_text_buffer_get_bounds (app.inputBuffer, &start, &end);
    hexText = gtk_text_buffer_get_text (app.inputBuffer, &start, &end, FALSE);
    remove_spaces(hexText);

    int hexTextSize = (strlen(hexText));
    unsigned char byteArray[hexTextSize/2];

    fp = fopen(app.temp_ijvmfile, "wb");
    if ((hexTextSize % 2) != 0)
    {
        fclose(fp);
        app.ijvmFile = app.temp_ijvmfile;
        return;
    }

    for (int i = 0; i < hexTextSize/2; i++){
        sscanf(hexText, "%2hhx", &byteArray[i]);
        hexText += 2;
    }
    fwrite(byteArray, 1, hexTextSize/2, fp);
    fclose(fp);

    app.ijvmFile = app.temp_ijvmfile;
}


void ijvmFile_to_inputBuffer(char *ijvmFile)
{
    FILE *fp;
    fp = fopen(ijvmFile, "rb");
    long bufferSize = get_file_size(fp);

    uint8_t* buffer = (uint8_t *)malloc((unsigned long) bufferSize * sizeof(uint8_t));
    fread(buffer, (unsigned long) bufferSize, 1, fp);
    fclose(fp);

    char hexText[(bufferSize * 2) + 1];
    char *ptr = &hexText[0];

    for (int i = 0; i < bufferSize; i++){
        ptr += sprintf(ptr, "%02x", buffer[i]);
    }

    gtk_text_buffer_set_text(app.inputBuffer,(const gchar*)hexText,-1);
    free(buffer);
}

void outputFile_to_outputBuffer()
{
    FILE *fp;
    fp = fopen(app.temp_outfile, "rb");

    long bufferSize = get_file_size(fp);
    char *text = malloc(bufferSize);
    fread(text,bufferSize,1,fp);

    if (bufferSize == 0){
        gtk_text_buffer_set_text(app.outputBuffer,"*Execution completed without output*\0",-1);
    } else {
        gtk_text_buffer_set_text(app.outputBuffer,(const gchar*)text,bufferSize);
    }
    fclose(fp);
    free(text);
}

void save_outputBuffer(char *filename)
{
    GtkTextIter start, end;
    gchar *text;

    gtk_text_buffer_get_bounds (app.outputBuffer, &start, &end);
    text = gtk_text_buffer_get_text (app.outputBuffer, &start, &end, FALSE);

    FILE *fp;
    fp  = fopen(filename,"wb");
    fprintf(fp, "%s", text);
    fclose(fp);
}

void entry_text_to_inFile()
{
    FILE *fileIn;
    fileIn  = fopen(app.temp_infile,"wb");
    
    const char *input = gtk_entry_get_text(app.userInput);
    
    fprintf(fileIn, "%s", input);
    fclose(fileIn);
}

void remove_temp_files()
{
    remove(app.temp_ijvmfile);
    remove(app.temp_infile);
    remove(app.temp_outfile);
}
