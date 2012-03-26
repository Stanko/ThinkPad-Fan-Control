/*
ThinkPad fan controll
Copyright 2008, Stanko Tadić <stanko@mfhinc.net>

ThinkPad fan controll is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License 
version 2 as published by the Free Software Foundation.  
Note that I am not granting permission to redistribute or 
modify glock under the terms of any later version of the 
General Public License.

Glock is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program (in the file "COPYING"); if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111, USA.

Compile using:
gcc -Wall -g -o tp_fan_control tp_fan_control.c -export-dynamic `pkg-config gtk+-2.0 libglade-2.0 --cflags --libs`
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>

#define AUTO 10
#define FULL 100

/* location of UI XML file relative to path in which program is running */
#define BUILDER_XML_FILE "fans.xml"
#include <gtk/gtk.h>

int visible = 1, running = 0;
int fansLevel = AUTO;
int sleepTime, oldSleepTime=0;
gint timeout;

GtkSpinButton *sleepField, *criticalField, *safeField;
GtkStatusbar *statusbar;
GtkLabel *level, *txtLog;
GtkWidget *window;
GtkButton *run, *exitButton, *hide;


void on_window_destroy (GtkButton *button);
int fans();
void run_clicked();
void exit_clicked();

void tray_icon_on_click(GtkStatusIcon *status_icon, gpointer user_data);
void tray_icon_on_menu(GtkStatusIcon *status_icon, guint button, guint activate_time, gpointer user_data);
static GtkStatusIcon *create_tray_icon();

int main (int argc, char *argv[]){

// pokazivaci na widgete
    GtkBuilder *builder;
    GtkStatusIcon *tray_icon;


//inicijalizacija
    gtk_init (&argc, &argv);
    
    system("echo level auto > /proc/acpi/ibm/fan");

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, BUILDER_XML_FILE, NULL);
    
// tray ikona
    tray_icon = create_tray_icon();

    window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
    run = GTK_BUTTON (gtk_builder_get_object (builder, "run"));
    exitButton = GTK_BUTTON (gtk_builder_get_object (builder, "exit"));
    hide = GTK_BUTTON (gtk_builder_get_object (builder, "hide"));
    
    sleepField = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "sleep"));  
    criticalField = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "critical"));  
    safeField = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "safe"));  
    
    txtLog = GTK_LABEL (gtk_builder_get_object (builder, "log"));
    statusbar = GTK_STATUSBAR (gtk_builder_get_object (builder, "statusbar"));  
    level = GTK_LABEL (gtk_builder_get_object (builder, "level"));  
    
    g_signal_connect(G_OBJECT(run), "clicked", G_CALLBACK(run_clicked), NULL);
    g_signal_connect(G_OBJECT(exitButton), "clicked", G_CALLBACK(on_window_destroy), NULL);
    g_signal_connect(G_OBJECT(hide), "clicked", G_CALLBACK(tray_icon_on_click), NULL);

    gtk_builder_connect_signals (builder, NULL);
    g_object_unref (G_OBJECT (builder));
    
    gtk_widget_show (window);

    gtk_main ();
    
    return 0;
}

int fans(){
    int temp;
    FILE* tempInput, *sysIn;
    char message[80];
    char tempMessage[120];
    char tmp[30];
    int criticalTemp = gtk_spin_button_get_value_as_int (criticalField);
    int safeTemp = gtk_spin_button_get_value_as_int (safeField);

    tempInput = fopen("/proc/acpi/ibm/thermal","r");
    if(tempInput ==NULL)
        gtk_label_set_text(txtLog,"YOU ARE NOT RUNNING KERNEL WITH THINKPAD PATCH!");
    fscanf(tempInput,"temperatures:	%d",&temp);
    fclose(tempInput);


    sysIn = popen("date '+%H:%M:%S'","r");
    fgets(tmp,9,sysIn);
    pclose(sysIn);
    sprintf(message,"Temperature: %dC, Checked at %s",temp,tmp);
    gtk_statusbar_push(statusbar,0,message);
    
    if(temp>=criticalTemp && fansLevel==AUTO){
        system("echo level full-speed > /proc/acpi/ibm/fan");
        fansLevel = FULL;
        sprintf(tempMessage,"======================\n%s\n======================\nTemperature is %d, critical is %d\nTurning fans to full-speed!",tmp,temp,criticalTemp);
        gtk_label_set_text(txtLog,tempMessage);
        gtk_label_set_text(level,"Fans level - full speed");
    }
    else if(temp<=safeTemp && fansLevel==FULL){
        system("echo level auto > /proc/acpi/ibm/fan");
        fansLevel = AUTO;

        sprintf(tempMessage,"======================\n%s\n======================\nTemperature is %d, safe is %d\nTurning fans to auto!",tmp,temp,safeTemp);
        gtk_label_set_text(txtLog,tempMessage);
        gtk_label_set_text(level,"Fans level - auto");
    }
    return TRUE;
}

void run_clicked(){
// get sleep time
    sleepTime = gtk_spin_button_get_value_as_int (sleepField);
    
    if(running == 1){
        gtk_timeout_remove(timeout);
        timeout = g_timeout_add_seconds(sleepTime,fans,NULL);
    }
    else{
        timeout = g_timeout_add_seconds(sleepTime,fans,NULL);
        running = 1;
    }
    gtk_button_set_label(run,"Change sleep time");
}


void on_window_destroy (GtkButton *button){
    system("date '+.:: %H:%M:%S ::.'");
    printf("EXITING - turning fans to auto!\n");
    printf("----------------------------------------\n");
    system("echo level auto > /proc/acpi/ibm/fan");
    gtk_main_quit();
}


void tray_icon_on_click(GtkStatusIcon *status_icon, gpointer user_data){
    if(visible){
        gtk_widget_hide(window);
        visible=0;
    }
    else{
        gtk_widget_show(window);
        visible=1;
    }
}

void tray_icon_on_menu(GtkStatusIcon *status_icon, guint button, guint activate_time, gpointer user_data){
    printf("Popup menu\n");
}

static GtkStatusIcon *create_tray_icon() {
    GtkStatusIcon *tray_icon;

    tray_icon = gtk_status_icon_new();
    g_signal_connect(G_OBJECT(tray_icon), "activate", G_CALLBACK(tray_icon_on_click), NULL);
    g_signal_connect(G_OBJECT(tray_icon), "popup-menu",G_CALLBACK(tray_icon_on_menu), NULL);
    gtk_status_icon_set_from_icon_name(tray_icon, GTK_STOCK_REFRESH);
    gtk_status_icon_set_tooltip(tray_icon, "Fan control");
    gtk_status_icon_set_visible(tray_icon, TRUE);

    return tray_icon;
}

