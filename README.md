# linuxmenu
a tool of linux for everyone in c
# Linux Real Tools Menu - Technical Documentation

## 1. Project Overview

**Linux Real Tools Menu** is a lightweight desktop application developed using GTK 3. Its core function is to provide a right-click context menu containing a vast array of real Linux system commands, graphical tools, and system settings entries.

This program aims to provide advanced users, system administrators, or Linux enthusiasts with a centralized interface for quick access to common system tools, eliminating the need to memorize complex command-line arguments or navigate through multi-layered system settings.

### Key Features
- **Authenticity**: Every menu item corresponds to a real Linux command or GUI application.
- **Broad Compatibility**: Covers tools from major desktop environments including GNOME, KDE, Xfce, LXQt, Cinnamon, MATE, and more.
- **Clear Categorization**: Tools are organized into 18 major categories such as System Settings, Display, Sound, Network, Disks, Monitoring, and Administration.
- **Asynchronous Execution**: Commands are launched asynchronously, ensuring the main interface remains responsive.
- **Terminal Integration**: CLI tools are automatically executed within `x-terminal-emulator`.

---

## 2. Technology Stack & Dependencies

### Programming Language
- C (C99/C11 standard)

### Core Libraries
- **GTK+ 3.0**: Used for building the Graphical User Interface (GUI), menu systems, and event handling.
- **GLib**: Used for process spawning (`g_spawn_command_line_async`) and URI launching (`g_app_info_launch_default_for_uri`).

### Build Dependencies
GTK 3 development libraries are required. On Debian/Ubuntu-based distributions:
```bash
sudo apt-get install libgtk-3-dev
```

### Runtime Dependencies (Tools must be installed separately)
The application itself is merely a launcher. The following tools must be pre-installed on the system to function correctly:
- **General**: `gnome-control-center`, `gnome-terminal` (or equivalent), `nautilus`
- **Network**: `nethogs`, `iftop`, `nmap`, `wireshark`, `tcpdump`, `dig`, `netstat`/`ss`
- **Disks**: `gparted`, `baobab`, `lsblk`, `hddtemp`
- **Monitoring**: `htop`, `atop`, `nmon`, `conky`, `lsof`
- **Others**: `vim`, `nano`, `mc`, `ranger`, `vlc`, `gimp`, `firefox`, etc.

> **Note**: If a specific tool is not installed, clicking its corresponding menu item will likely result in no action or an error, depending on the system's error handling.

---

## 3. Architecture Design

### 3.1 Core Modules

#### 3.1.1 Command Execution Engine (`run_cmd`)
```c
static void run_cmd(const gchar *cmd) {
    if (cmd && *cmd)
        g_spawn_command_line_async(cmd, NULL);
}
```
- **Function**: Asynchronously executes shell commands using GLib's `g_spawn_command_line_async`.
- **Advantage**: Non-blocking; the main window responds immediately after a menu item is clicked, while the tool starts in the background.

#### 3.1.2 Menu Item Factory Functions
- **`make_cmd_item`**: Creates a menu item that executes a command.
  - **Label**: Text displayed to the user.
  - **Command**: The actual shell string to execute.
- **`make_uri_item`**: Creates a menu item that opens a URI (file or web link).
  - Uses `g_app_info_launch_default_for_uri`, allowing the system's default application to handle the request (e.g., file manager opening a folder).
- **`make_submenu_item`**: Creates a parent menu item attached to a submenu.

#### 3.1.3 Menu Builder (`build_real_tools_menu`)
- **Structure**: Uses a nested structure where the main menu contains multiple submenus.
- **Content**: Hardcodes approximately 150+ menu items divided into 18 categories.
- **Dynamic Nature**: The menu is rebuilt every time a right-click occurs (called within `on_button_press`). This ensures the state is fresh, though it introduces negligible performance overhead for this scale.

### 3.2 Event Handling

#### Right-Click Detection (`on_button_press`)
```c
static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        GtkWidget *menu = build_real_tools_menu();
        gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);
        return TRUE;
    }
    return FALSE;
}
```
- Listens for the `button-press-event` signal.
- Checks if the event is a right-click (`button == 3`).
- Dynamically generates the menu and pops it up at the mouse pointer's location.

---

## 4. Functional Modules Detailed

| Category | Description | Typical Tool Examples |
| :--- | :--- | :--- |
| **⚙️ System Settings** | Entry points for various DE settings | `gnome-control-center`, `systemsettings5`, `xfce4-settings-manager` |
| **🖥️ Display** | Display, resolution, GPU settings | `arandr`, `nvidia-settings`, `gnome-control-center display` |
| **🔊 Sound** | Audio control, volume, recording | `pavucontrol`, `alsamixer`, `qpwgraph`, `audacity` |
| **🌐 Networks** | Network config, diagnostics | `nm-connection-editor`, `ping`, `nmap`, `wireshark`, `gufw` |
| **💾 Disks** | Disk management, partitioning, mounting | `gparted`, `gnome-disks`, `lsblk`, `mount` |
| **📊 Monitoring** | Process and resource monitoring | `htop`, `gnome-system-monitor`, `top`, `lsof` |
| **🛠️ Administration** | Logs, services, hardware info | `journalctl`, `systemctl`, `dmesg`, `lshw`, `lspci` |
| **📦 Software** | Package managers, software centers | `synaptic`, `gnome-software`, `apt list`, `flatpak list` |
| **👤 Accounts** | Users, privacy, region settings | `gnome-control-center user-accounts`, `seahorse` |
| **🎨 Appearance** | Themes, wallpapers, extensions | `gnome-tweaks`, `gnome-extensions-app`, `variety` |
| **📁 Files & Terminals** | File managers, terminal emulators | `nautilus`, `dolphin`, `gnome-terminal`, `tilix`, `mc` |
| **📝 Editors** | Text editors | `gedit`, `code`, `vim`, `nano`, `kate` |
| **📄 Office** | Office suites, document viewers | `libreoffice`, `evince`, `okular`, `calibre` |
| **🎬 Multimedia** | Media players, image editing | `vlc`, `mpv`, `gimp`, `inkscape`, `obs` |
| **🌍 Internet** | Browsers, email, communication | `firefox`, `chrome`, `thunderbird`, `telegram-desktop` |
| **🧰 Utilities** | Small helper tools | `gnome-calculator`, `gnome-screenshot`, `catfish` |
| **💻 Diagnostics** | System diagnostic commands | `uname -a`, `df -h`, `free -h`, `uptime` |
| **📂 Places** | Quick access to directories | Home, Desktop, Documents, Downloads, Trash |

---

## 5. Compilation & Execution

### 5.1 Compilation
Execute the following command in the project root directory:

```bash
gcc -o linux-real-tools-menu linux-real-tools-menu.c $(pkg-config --cflags --libs gtk+-3.0)
```

### 5.2 Execution
```bash
./linux-real-tools-menu
```

### 5.3 Usage Instructions
1. Launch the application. A main window will appear.
2. **Right-click** anywhere within the window.
3. A large context menu will pop up.
4. Click any menu item to launch the corresponding application or command.

---

## 6. Code Structure & Key Functions

### 6.1 Header Includes
```c
#include <gtk/gtk.h>   // GTK 3 GUI Library
#include <stdlib.h>    // Standard Library
#include <string.h>    // String Handling
```

### 6.2 Helper Functions
- `run_cmd()`: Wraps `g_spawn_command_line_async` to simplify command execution.
- `make_cmd_item()`: Creates a menu item bound to a command.
- `make_uri_item()`: Creates a menu item bound to a URI.
- `make_submenu_item()`: Creates a submenu container.

### 6.3 Main Logic
- `build_real_tools_menu()`: **Core function**. Constructs the entire menu tree. Contains definitions for all menu items.
- `on_button_press()`: Event callback that triggers the menu display.
- `main()`: Initializes GTK, creates the main window, connects signals, and enters the main loop.

---

## 7. Known Limitations & Considerations

1. **Desktop Environment Dependency**:
   - Some menu items are specific to certain desktop environments (e.g., `gnome-control-center` works best in GNOME, `systemsettings5` in KDE). Clicking these in non-corresponding environments may result in no action or failure.
   - **Recommendation**: Modify the source code to remove irrelevant entries based on your specific desktop environment.

2. **Permission Issues**:
   - Some commands require `sudo` privileges (e.g., `nethogs`, `iftop`, `lshw`, `mount`).
   - The current implementation calls `sudo` directly, which will prompt for a password in the terminal. If passwordless sudo is not configured, the user must manually enter their password.
   - **Security Warning**: Use graphical launchers containing `sudo` with caution in production environments. Ensure users understand the privilege escalation behavior.

3. **Terminal Emulator Compatibility**:
   - The code uses `x-terminal-emulator` as a generic terminal launcher. This works on most modern Linux distributions, but minimal systems may require replacing it with a specific terminal (e.g., `gnome-terminal`, `konsole`, `xterm`).

4. **Performance**:
   - Rebuilding the entire menu tree on every right-click has a small overhead. While negligible for ~150 items, it could cause slight delays on very low-end hardware. An optimization would be to cache the menu object, though memory management would need careful handling.

5. **Hardcoded Paths**:
   - The "Places" section uses `/home/$USER/...`. This may fail in configurations where the home directory is not located under `/home`. A more robust approach would use GLib functions like `g_get_home_dir()` to dynamically retrieve paths.

---

## 8. Extension & Maintenance Suggestions

### 8.1 Customizing the Menu
Users can edit the `build_real_tools_menu()` function to add or remove items. For example:
- Add a custom script: `make_cmd_item("My Backup Script", "/home/user/scripts/backup.sh")`
- Add a web link: `make_uri_item("Arch Wiki", "https://wiki.archlinux.org")`

### 8.2 Conditional Compilation
Menu items can be dynamically shown/hidden based on desktop environment variables. For example:
```c
const gchar *de = g_getenv("XDG_CURRENT_DESKTOP");
if (g_strcmp0(de, "GNOME") == 0) {
    // Add GNOME-specific items
}
```

### 8.3 Configuration File Support
Currently, all menu items are hardcoded in the C file. Future versions could support loading menu structures from JSON or XML configuration files, improving flexibility and maintainability.

### 8.4 Icon Support
Current menu items use text labels only. Icons can be added using `gtk_image_menu_item_new_with_label` to enhance visual experience.

---

## 9. License

This project code is licensed under the **Public Domain**. You are free to use, modify, and distribute it without attribution.

---

## 10. Appendix: Common Command Reference

| Command | Purpose |
| :--- | :--- |
| `gnome-control-center` | GNOME System Settings |
| `pavucontrol` | PulseAudio Volume Control |
| `htop` | Interactive Process Viewer |
| `nmap` | Network Scanning Tool |
| `gparted` | Partition Editor |
| `journalctl -xe` | View System Logs |
| `lsblk -f` | List Block Devices & Filesystems |
| `x-terminal-emulator -e <cmd>` | Execute command in terminal |

---

**Author**: zerowjh 
**Last Updated**: 2026-04-25  
**Version**: 1.0
