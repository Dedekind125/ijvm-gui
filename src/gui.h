#include <gtk/gtk.h>

typedef struct GUI
{
	GtkWidget 		*mainWindow;
	GtkWidget 		*aboutDialog;
	GtkBuilder 		*builder;

	GtkTextBuffer 	*inputBuffer;
	GtkTextBuffer 	*outputBuffer;
	GtkEntry 		*userInput;

	char 			*ijvmFile;
	char 			*temp_ijvmfile;
	char 			*temp_infile;
	char 			*temp_outfile;

	bool 			hasIjvm;
} GUI;

/**
 * Thread function: Sets the input & output files and runs the ijvm.
 **/
gpointer run_ijvm (gpointer);

/**
 * Copies the data from the GTKTextView box to a temp ijvm file. 
 **/ 
void inputBuffer_to_temp_ijvmFile(void);

/**
 * Copies the data of the ijvm output to the GTK output buffer
 * in order to be displayed in the GUI.
 **/ 
void outputFile_to_outputBuffer(void);

/**
 * Copies the data from the ijvm file to the inputBuffer in
 * order to be displayed in the GUI.
 * @param filename The name of the ijvm file
 **/ 
void ijvmFile_to_inputBuffer(char* filename);

/**
 * Saves the contents of the output buffer to a file.
 * @param filename The name of the file where the contents will be saved.
 **/
void save_outputBuffer(char* filename);

/**
 * Copies the data from the entry field to a file.
 * This file will be the input file of the ijvm machine.
 **/
void entry_text_to_inFile(void);

/**
 * Removes the files in the appication data directory
 **/
void remove_temp_files(void);
