/* vim: set backspace=2 ts=4 softtabstop=4 sw=4 cinoptions=>4 expandtab autoindent smartindent: */
/* xkb-settings-dialog.c
 * Copyright (C) 2008 Alexander Iliev <sasoiliev@mamul.org>
 *
 * Parts of this program comes from the XfKC tool:
 * Copyright (C) 2006 Gauvain Pocentek <gauvainpocentek@gmail.com>
 *
 * A part of this file comes from the gnome keyboard capplet (control-center):
 * Copyright (C) 2003 Sergey V. Oudaltsov <svu@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>

#include <libxfce4panel/xfce-panel-plugin.h>
#include <libxfce4ui/libxfce4ui.h>

#include "xfce4-xkb-plugin.h"
#include "xfce4-xkb-plugin-private.h"
#include "xkb-settings-dialog.h"
#include "xkb-util.h"

GtkTreeIter current_iter;
GtkWidget *settings_dialog;
GtkWidget *default_layout_menu;

enum combo_enum
{
    DESC = 0,
    NOM,
    COMBO_NUM
};

enum tree_enum
{
    DEFAULT_LAYOUT = 0,
    LAYOUTS,
    VARIANTS,
    TREE_NUM
};

enum enumeration
{
    AVAIL_LAYOUT_TREE_COL_DESCRIPTION = 0,
    AVAIL_LAYOUT_TREE_COL_ID,
    NUM
};


/**************************************************************/

static void
on_settings_close (GtkDialog *dialog, gint response, t_xkb *xkb)
{
    xfce_panel_plugin_unblock_menu (xkb->plugin);

    xfce_xkb_save_config (xkb->plugin, xkb);

    gtk_widget_destroy (GTK_WIDGET (dialog));
}

static void
on_display_type_changed (GtkComboBox *cb, t_xkb *xkb)
{
    xkb->display_type = gtk_combo_box_get_active (cb);
    xkb_refresh_gui (xkb);
}

static void
on_display_textsize_changed (GtkHScale *scale, t_xkb *xkb)
{
    xkb->display_text_scale = gtk_range_get_value (GTK_RANGE (scale));
    xkb_refresh_gui (xkb);
}

static void
on_display_imgsize_changed (GtkHScale *scale, t_xkb *xkb)
{
    xkb->display_img_scale = gtk_range_get_value (GTK_RANGE (scale));
    xkb_refresh_gui (xkb);
}

static void
on_group_policy_changed (GtkComboBox *cb, t_xkb *xkb)
{
    xkb->group_policy = gtk_combo_box_get_active (cb);
    xkb_config_set_group_policy (xkb->group_policy);
}

void
xfce_xkb_configure (XfcePanelPlugin *plugin,
                    t_xkb *xkb)
{
    GtkWidget *display_type_optmenu, *group_policy_combo;
    GtkWidget *vbox, *display_type_frame, *group_policy_frame, *bin;
    GtkWidget *display_textsize_frame, *display_textsize_scale;
    GtkWidget *display_imgsize_frame, *display_imgsize_scale;

    xfce_panel_plugin_block_menu (plugin);

    settings_dialog = xfce_titled_dialog_new_with_buttons (_("Keyboard Layouts"),
            NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            "gtk-close", GTK_RESPONSE_OK, NULL);

    gtk_window_set_icon_name (GTK_WINDOW (settings_dialog), "xfce4-settings");

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (gtk_dialog_get_content_area(GTK_DIALOG(settings_dialog)))), vbox);

    /*****/
    display_type_frame = xfce_gtk_frame_box_new (_("Show layout as:"), &bin);
    gtk_widget_show (display_type_frame);
    gtk_box_pack_start (GTK_BOX (vbox), display_type_frame, TRUE, TRUE, 2);

    display_type_optmenu = gtk_combo_box_text_new ();
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (display_type_optmenu), _("image"));
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (display_type_optmenu), _("text"));
    gtk_widget_set_size_request (display_type_optmenu, 230, -1);
    gtk_container_add (GTK_CONTAINER (bin), display_type_optmenu);

    /* text size option */
    display_textsize_frame = xfce_gtk_frame_box_new (_("Text size:"), &bin);
    gtk_widget_show (display_textsize_frame);
    gtk_box_pack_start (GTK_BOX (vbox), display_textsize_frame, TRUE, TRUE, 2);

    display_textsize_scale = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    //gtk_range_set_update_policy (GTK_RANGE (display_textsize_scale), GTK_UPDATE_CONTINUOUS);
    gtk_scale_set_value_pos (GTK_SCALE (display_textsize_scale), GTK_POS_RIGHT);
    gtk_widget_set_size_request (display_textsize_scale, 230, -1);
    gtk_container_add (GTK_CONTAINER (bin), display_textsize_scale);

    /* image size option */
    display_imgsize_frame = xfce_gtk_frame_box_new (_("Image size:"), &bin);
    gtk_widget_show (display_imgsize_frame);
    gtk_box_pack_start (GTK_BOX (vbox), display_imgsize_frame, TRUE, TRUE, 2);

    display_imgsize_scale = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    //gtk_range_set_update_policy (GTK_RANGE (display_imgsize_scale), GTK_UPDATE_CONTINUOUS);
    gtk_scale_set_value_pos (GTK_SCALE (display_imgsize_scale), GTK_POS_RIGHT);
    gtk_widget_set_size_request (display_imgsize_scale, 230, -1);
    gtk_container_add (GTK_CONTAINER (bin), display_imgsize_scale);

    group_policy_frame = xfce_gtk_frame_box_new (_("Manage layout:"), &bin);
    gtk_widget_show (group_policy_frame);
    gtk_box_pack_start (GTK_BOX (vbox), group_policy_frame, TRUE, TRUE, 2);

    group_policy_combo = gtk_combo_box_text_new ();
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (group_policy_combo), _("globally"));
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (group_policy_combo), _("per window"));
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (group_policy_combo), _("per application"));
    gtk_widget_set_size_request (group_policy_combo, 230, -1);
    gtk_container_add (GTK_CONTAINER (bin), group_policy_combo);
    gtk_widget_show (group_policy_combo);

    gtk_widget_show_all (vbox);

    g_signal_connect ((gpointer) settings_dialog, "response",
            G_CALLBACK (on_settings_close), xkb);

    gtk_combo_box_set_active (GTK_COMBO_BOX (display_type_optmenu), xkb->display_type);
    gtk_range_set_value (GTK_RANGE (display_textsize_scale), xkb->display_text_scale);
    gtk_range_set_value (GTK_RANGE (display_imgsize_scale), xkb->display_img_scale);
    gtk_combo_box_set_active (GTK_COMBO_BOX (group_policy_combo), xkb->group_policy);

    g_signal_connect (display_type_optmenu, "changed", G_CALLBACK (on_display_type_changed), xkb);
    g_signal_connect (group_policy_combo, "changed", G_CALLBACK (on_group_policy_changed), xkb);
    g_signal_connect (display_textsize_scale, "value_changed", G_CALLBACK (on_display_textsize_changed), xkb);
    g_signal_connect (display_imgsize_scale, "value_changed", G_CALLBACK (on_display_imgsize_changed), xkb);

    gtk_widget_show (settings_dialog);

}

void
xfce_xkb_about (XfcePanelPlugin *plugin)
{
    GtkWidget *about;
    const gchar* authors[] = {
        "Alexander Iliev <sasoiliev@mamul.org>",
        "Gauvain Pocentek <gauvainpocentek@gmail.com>",
        "Igor Slepchin <igor.slepchin@gmail.com>",
        NULL
    };

    about = gtk_about_dialog_new ();
    gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (about),
            _("Keyboard Layouts Plugin"));
    gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (about),
            PACKAGE_VERSION);
    gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG (about),
            NULL);
    gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (about),
            xfce_get_license_text (XFCE_LICENSE_TEXT_GPL));
    gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (about),
            (const gchar**) authors);
    gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (about),
            _("Allows you to configure and use multiple keyboard layouts."));
    gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (about),
            "http://goodies.xfce.org/");
    gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (about),
            _("Other plugins available here"));
    gtk_dialog_run (GTK_DIALOG (about));
    gtk_widget_destroy (about);
}
