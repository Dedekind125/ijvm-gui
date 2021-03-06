# IJVM GUI

A cross-platform GUI application of an emulator capable of executing [IJVM](https://en.wikipedia.org/wiki/IJVM) byte-code in C.

The application was build with GTK+ 3 and [Glade builder](https://glade.gnome.org/).
For compiling and running the code under different operation systems brief instructions are given.

### Linux
To compile and run the gui the libgtk-3-dev package is needed: `sudo apt-get install libgtk-3-dev`.  
Ιf the icons inside the app are not being displayed properly, you have to install also the [adwaita-icon-theme](https://gitlab.gnome.org/GNOME/adwaita-icon-theme):  
`sudo apt-get install adwaita-icon-theme-full`

Then, compile the code with `make ijvm-gui` and run it with `./ijvm-gui`

More information about compiling GTK+ 3 on linux: [GTK+ 3 official documentation](https://developer.gnome.org/gtk3/stable/gtk-building.html)

### Mac OSX
To compile and run the GUI on macOS you must install GTK+ 3 via [Homebrew](http://macappstore.org/gtk3/) `brew install gtk+3`.  
Ιf the icons inside the app are not being displayed properly, you have to install also the [adwaita-icon-theme](https://gitlab.gnome.org/GNOME/adwaita-icon-theme):  
`brew install adwaita-icon-theme`

Then, compile the code with `make ijvm-gui` and run it with `./ijvm-gui`

### Windows

#### Installation Package
Instead of building the application on Windows you can directly download a proper installation package(`ijvm-setup.exe`) via the [Releases page](https://github.com/Dedekind125/IJVM/releases/tag/v1.0).   
After the installation, you will be able to run the application as native Windows program.

#### Compiling
Compiling GTK3+ application under Windows is quite painful.
I would recommend install and use the [MSYS2](https://www.msys2.org/) project as the [official documentation suggests](https://www.gtk.org/docs/installations/windows/#using-gtk-from-msys2-packages). 

*Note: Unfortunately MSYS2 does not support the [BSD sys/socket.h](https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/socket.h.html) so the networking does not work on the Windows.*

Finally, to compile the code run `make ijvm-gui` and then run it with `./ijvm-gui.exe`
