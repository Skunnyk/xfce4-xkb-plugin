# $Id$

plugindir = $(libdir)/xfce4/panel/plugins
plugin_LTLIBRARIES = libxkb.la

libxkb_la_SOURCES =								\
	xfce4-xkb-plugin.h								\
	xfce4-xkb-plugin-private.h							\
	xfce4-xkb-plugin.c								\
	xkb-settings-dialog.h								\
	xkb-settings-dialog.c								\
	xkb-config.h									\
	xkb-config.c									\
	xkb-util.h									\
	xkb-util.c									\
	xkb-cairo.h									\
	xkb-cairo.c									\
	xkb-callbacks.h									\
	xkb-callbacks.c


libxkb_la_CFLAGS =								\
	@LIBXFCE4PANEL_CFLAGS@								\
	@GTK_CFLAGS@									\
	@LIBXFCE4UI_CFLAGS@								\
	@LIBXKLAVIER_CFLAGS@								\
	@LIBRSVG_CFLAGS@								\
	@LIBWNCK_CFLAGS@								\
	@GARCON_CFLAGS@								\
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\"						\
	-DFLAGSDIR=\"$(datadir)/xfce4/xkb/flags\"					\
	-DWNCK_I_KNOW_THIS_IS_UNSTABLE

libxkb_la_LIBADD = 								\
	@LIBXFCE4PANEL_LIBS@								\
	@GTK_LIBS@									\
	@LIBXFCE4UI_LIBS@								\
	@LIBXKLAVIER_LIBS@								\
	@LIBWNCK_LIBS@									\
	@GARCON_LIBS@									\
	@LIBRSVG_LIBS@

libxkb_la_LDFLAGS =                                                 \
        -avoid-version                                                  \
        -module                                                         \
        -no-undefined                                                   \
        -export-symbols-regex '^xfce_panel_module_(preinit|init|construct)' \
        $(PLATFORM_LDFLAGS)                                             \
        $(NULL)


#
# Desktop file
#
desktopdir = $(datadir)/xfce4/panel/plugins
desktop_in_in_files = xkb-plugin.desktop.in.in
desktop_in_files = $(desktop_in_in_files:.desktop.in.in=.desktop.in)
%.desktop.in: %.desktop.in.in
	sed -e "s,\@libexecdir\@,$(libexecdir),g" < $< > $@
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST =								\
	$(desktop_in_in_files)

CLEANFILES =								\
	$(desktop_in_files)						\
	$(desktop_DATA)

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:

