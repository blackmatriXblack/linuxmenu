/*
 * linux-real-tools-menu.c
 *
 * A GTK 3 application that shows a huge right-click menu filled exclusively
 * with REAL Linux commands and settings. Every menu item opens a terminal,
 * launches a GUI tool, or starts a genuine system settings panel.
 *
 * Compile:
 *   gcc -o linux-real-tools-menu linux-real-tools-menu.c $(pkg-config --cflags --libs gtk+-3.0)
 *
 * Run:
 *   ./linux-real-tools-menu
 *
 * Author : Assistant
 * License: Public Domain
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

/* ----- Helper: launch a command asynchronously ----- */
static void
run_cmd(const gchar *cmd)
{
    if (cmd && *cmd)
        g_spawn_command_line_async(cmd, NULL);
}

/* ----- Create a menu item that runs a command ----- */
static GtkWidget *
make_cmd_item(const gchar *label, const gchar *command)
{
    GtkWidget *item = gtk_menu_item_new_with_label(label);
    g_signal_connect(item, "activate", G_CALLBACK(run_cmd), (gpointer)command);
    return item;
}

/* ----- Open a URI (file or web) with the default handler ----- */
static void
open_uri_cb(GtkMenuItem *item, const gchar *uri)
{
    g_app_info_launch_default_for_uri(uri, NULL, NULL);
}

static GtkWidget *
make_uri_item(const gchar *label, const gchar *uri)
{
    GtkWidget *item = gtk_menu_item_new_with_label(label);
    g_signal_connect(item, "activate", G_CALLBACK(open_uri_cb), (gpointer)uri);
    return item;
}

/* ----- Create a submenu attached to a menu item ----- */
static GtkWidget *
make_submenu_item(const gchar *label, GtkWidget *submenu)
{
    GtkWidget *item = gtk_menu_item_new_with_label(label);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
    return item;
}

/* ----- Build the gigantic menu ----- */
static GtkWidget *
build_real_tools_menu(void)
{
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *sub, *sub2, *sub3;

    /* 1. SYSTEM SETTINGS (generic + DE-specific) */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("GNOME Control Center", "gnome-control-center"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("KDE System Settings", "systemsettings5"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Xfce Settings Manager", "xfce4-settings-manager"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("LXQt Configuration Center", "lxqt-config"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Cinnamon Settings", "cinnamon-settings"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("MATE Control Center", "mate-control-center"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Budgie Desktop Settings", "budgie-desktop-settings"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("⚙️ System Settings", sub));

    /* 2. DISPLAY & GRAPHICS */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Display Settings (GNOME)", "gnome-control-center display"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Display Settings (KDE)", "systemsettings5 kcm_kscreen"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Screen Layout (ARandR)", "arandr"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("NVIDIA Settings", "nvidia-settings"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("AMD Catalyst Control", "amdcccle"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Color Profile", "gnome-control-center color"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Night Light", "gnome-control-center night-light"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Wallpaper Changer (Variety)", "variety"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Compositor Settings (picom)", "picom --config"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("🖥️ Display", sub));

    /* 3. SOUND & AUDIO */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Sound Settings (GNOME)", "gnome-control-center sound"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Sound Settings (KDE)", "systemsettings5 kcm_pulseaudio"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("PulseAudio Volume Control", "pavucontrol"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("ALSA Mixer", "alsamixer"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("PipeWire Control", "qpwgraph"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Audio Recorder (Audacity)", "audacity"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Music Player (Rhythmbox)", "rhythmbox"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("🔊 Sound", sub));

    /* 4. NETWORKS */
    sub = gtk_menu_new();
    /* Basic settings */
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Network Settings (GNOME)", "gnome-control-center network"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Wi‑Fi Settings", "gnome-control-center wifi"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("VPN Connections", "nm-connection-editor"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Proxy", "gnome-control-center network proxy"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Bluetooth", "gnome-control-center bluetooth"));

    /* Network tools (in terminal) */
    sub2 = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("Ping google.com", "x-terminal-emulator -e ping -c 4 google.com"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("Tracepath", "x-terminal-emulator -e tracepath google.com"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("NetHogs (bandwidth)", "x-terminal-emulator -e sudo nethogs"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("iftop (traffic)", "x-terminal-emulator -e sudo iftop"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("nmap scan", "x-terminal-emulator -e nmap -sS localhost"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("Wireshark", "wireshark"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("tcpdump", "x-terminal-emulator -e sudo tcpdump -i any"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("whois info", "x-terminal-emulator -e whois example.com"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("dig DNS lookup", "x-terminal-emulator -e dig google.com"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("netstat -tulanp", "x-terminal-emulator -e sudo netstat -tulanp"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub2),
        make_cmd_item("ss (socket statistics)", "x-terminal-emulator -e ss -tulanp"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_submenu_item("Network Tools (terminal)", sub2));

    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Firewall (gufw)", "gufw"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("IPTables Status", "x-terminal-emulator -e sudo iptables -L"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("🌐 Networks", sub));

    /* 5. DISKS & STORAGE */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Disk Manager (gnome-disks)", "gnome-disks"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("GParted", "gparted"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Disk Usage Analyzer (baobab)", "baobab"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Mount Manager", "x-terminal-emulator -e sudo mount -a"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("List Block Devices (lsblk)", "x-terminal-emulator -e lsblk -f"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("File System Table (edit fstab)", "x-terminal-emulator -e sudo nano /etc/fstab"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("SMART Data", "gnome-disks"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("HDD Temperature (hddtemp)", "x-terminal-emulator -e sudo hddtemp /dev/sda"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("💾 Disks", sub));

    /* 6. PROCESSES & MONITORING */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("System Monitor (GNOME)", "gnome-system-monitor"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("KSysGuard (KDE)", "ksysguard"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("htop", "x-terminal-emulator -e htop"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("atop", "x-terminal-emulator -e atop"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("nmon", "x-terminal-emulator -e nmon"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Conky", "conky"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("top", "x-terminal-emulator -e top"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("ps aux", "x-terminal-emulator -e ps aux --sort=-%mem | head"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("lsof (open files)", "x-terminal-emulator -e sudo lsof -i -P -n"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("📊 Monitoring", sub));

    /* 7. ADMINISTRATION & SERVICES */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Services (systemctl)", "x-terminal-emulator -e systemctl status"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Logs (gnome-logs)", "gnome-logs"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("dmesg", "x-terminal-emulator -e dmesg -T | tail -50"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("journalctl (last 50 lines)", "x-terminal-emulator -e journalctl -xe -n 50"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Systemd Analyze blame", "x-terminal-emulator -e systemd-analyze blame"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Kernel Modules (lsmod)", "x-terminal-emulator -e lsmod | less"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Hardware Info (lshw)", "x-terminal-emulator -e sudo lshw -short"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("CPU Info (lscpu)", "x-terminal-emulator -e lscpu"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("PCI Devices (lspci)", "x-terminal-emulator -e lspci -k"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("USB Devices (lsusb)", "x-terminal-emulator -e lsusb -t"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Sensors (temperatures)", "x-terminal-emulator -e sensors"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Hostname & Domain (hostnamectl)", "x-terminal-emulator -e hostnamectl"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("🛠️ Administration", sub));

    /* 8. SOFTWARE & PACKAGE MANAGEMENT */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Software Manager (GNOME)", "gnome-software"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Discover (KDE)", "plasma-discover"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Update Manager", "update-manager"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Software Sources", "software-properties-gtk"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Synaptic Package Manager", "synaptic"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Snap Store", "snap-store"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Flatpak Settings", "flatpak-settings"));
    /* Terminal wrappers */
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("apt list installed", "x-terminal-emulator -e apt list --installed"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("dpkg -l", "x-terminal-emulator -e dpkg -l | less"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("flatpak list", "x-terminal-emulator -e flatpak list"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("snap list", "x-terminal-emulator -e snap list"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("📦 Software", sub));

    /* 9. USER ACCOUNTS & PRIVACY */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("User Accounts", "gnome-control-center user-accounts"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Online Accounts", "gnome-control-center online-accounts"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Passwords & Keys (seahorse)", "seahorse"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Privacy Settings", "gnome-control-center privacy"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Login Screen (GDM)", "gnome-control-center login-screen"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Date & Time", "gnome-control-center datetime"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Language & Region", "gnome-control-center region"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("👤 Accounts", sub));

    /* 10. DESKTOP APPEARANCE & TWEAKS */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Background", "gnome-control-center background"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Themes & Icons", "gnome-control-center appearance"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("GNOME Tweaks", "gnome-tweaks"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Extensions Manager", "gnome-extensions-app"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Startup Applications", "gnome-session-properties"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Default Applications", "gnome-control-center default-apps"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Screensaver & Lock", "gnome-control-center screen"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("🎨 Appearance", sub));

    /* 11. FILE MANAGERS & TERMINALS */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Files (Nautilus)", "nautilus"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Nemo (Cinnamon)", "nemo"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Dolphin (KDE)", "dolphin"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Thunar", "thunar"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("PCManFM", "pcmanfm"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Midnight Commander (MC)", "x-terminal-emulator -e mc"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("ranger", "x-terminal-emulator -e ranger"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("GNOME Terminal", "gnome-terminal"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Tilix", "tilix"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Terminator", "terminator"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Alacritty", "alacritty"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("📁 Files & Terminals", sub));

    /* 12. TEXT EDITORS */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("gedit", "gedit"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Kate", "kate"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Sublime Text", "subl"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("VS Code", "code"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Vim (terminal)", "x-terminal-emulator -e vim"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("nano (terminal)", "x-terminal-emulator -e nano"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("emacs (terminal)", "x-terminal-emulator -e emacs -nw"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("📝 Editors", sub));

    /* 13. OFFICE & PRODUCTIVITY */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("LibreOffice Writer", "libreoffice --writer"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("LibreOffice Calc", "libreoffice --calc"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("LibreOffice Impress", "libreoffice --impress"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Document Viewer (Evince)", "evince"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Okular (KDE)", "okular"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Calibre E‑book Manager", "calibre"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Gnumeric Spreadsheet", "gnumeric"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("📄 Office", sub));

    /* 14. MULTIMEDIA */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("VLC Media Player", "vlc"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("mpv Media Player", "mpv"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Rhythmbox", "rhythmbox"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Audacity", "audacity"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("GIMP", "gimp"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Inkscape", "inkscape"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Image Viewer (eog)", "eog"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Sound Recorder", "gnome-sound-recorder"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("OBS Studio", "obs"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("HandBrake", "handbrake"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("🎬 Multimedia", sub));

    /* 15. INTERNET & COMMUNICATION */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Firefox", "firefox"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Google Chrome", "google-chrome-stable"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Brave Browser", "brave-browser"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Evolution Mail", "evolution"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Thunderbird", "thunderbird"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("HexChat IRC", "hexchat"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Slack", "slack"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Telegram Desktop", "telegram-desktop"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("🌍 Internet", sub));

    /* 16. UTILITIES (tiny helpers) */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Calculator", "gnome-calculator"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Archive Manager (file-roller)", "file-roller"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Screenshot", "gnome-screenshot --interactive"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Character Map", "gnome-character-map"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Dictionary", "gnome-dictionary"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("File Search (Catfish)", "catfish"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("Clipboard Manager (Diodon)", "diodon"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("🧰 Utilities", sub));

    /* 17. SYSTEM TOOLS (CLI diagnostics) */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("top", "x-terminal-emulator -e top"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("vmstat", "x-terminal-emulator -e vmstat 1"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("iostat", "x-terminal-emulator -e iostat -x 1"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("free -h", "x-terminal-emulator -e free -h"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("uptime", "x-terminal-emulator -e uptime"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("uname -a", "x-terminal-emulator -e uname -a"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("df -h", "x-terminal-emulator -e df -h"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("du -sh $HOME", "x-terminal-emulator -e du -sh $HOME"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_cmd_item("ls -la", "x-terminal-emulator -e ls -la $HOME"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("💻 Diagnostics", sub));

    /* 18. PLACES (quick folders) */
    sub = gtk_menu_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_uri_item("Home", "file:///home"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_uri_item("Desktop", "file:///home/$USER/Desktop"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_uri_item("Documents", "file:///home/$USER/Documents"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_uri_item("Downloads", "file:///home/$USER/Downloads"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_uri_item("Music", "file:///home/$USER/Music"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_uri_item("Pictures", "file:///home/$USER/Pictures"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_uri_item("Videos", "file:///home/$USER/Videos"));
    gtk_menu_shell_append(GTK_MENU_SHELL(sub),
        make_uri_item("Trash", "trash:///"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),
        make_submenu_item("📂 Places", sub));

    gtk_widget_show_all(menu);
    return menu;
}

/* ----- Right‑click handler ----- */
static gboolean
on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        GtkWidget *menu = build_real_tools_menu();
        gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);
        return TRUE;
    }
    return FALSE;
}

int main(int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),
                         "Linux REAL Tools Menu – Right‑click Anywhere");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 380);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(window, "button-press-event",
                     G_CALLBACK(on_button_press), NULL);

    GtkWidget *label = gtk_label_new(
        "Right‑click anywhere in this window to open\n"
        "a massive menu of REAL Linux commands.\n\n"
        "Every single entry launches an actual tool or\n"
        "system settings panel. Over 150 genuine items.\n\n"
        "Note: Some tools require installation\n"
        "(they are standard on most distributions).");
    gtk_container_add(GTK_CONTAINER(window), label);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
