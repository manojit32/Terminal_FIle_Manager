Manojit Chakraborty
2018201032

This is an application for Terminal Based File Manager.
There are two modes in this application. 1) Normal Mode 2) Command Mode
Users can compile the code by writing "make all" in the terminal
After compiling, users have to write "./filemanager" to run the application.
The application will start at normal mode.
Upon pressing ":", users can switch to command mode.
Upon pressing ESC key, users can switch back to normal mode.

In Normal Mode:

Users can scroll through the files and directory lists using UP and DOWN arrows.
By pressing ENTER, users can open a directory or a file.
Upon pressing BACKSPACE, users can go up one level.
Upon pressing left arrow, the application will go to the previously visited folder.
Upon pressing right arrow, the application will open next visited folder.
By pressing "h", users will go to the root directory i.e the directory from which the application has started.

In Command Mode :

Users can use "create_file /filename /destination" to create files.
Users can use "create_dir /foldername /destination" to create directories.
Users can use "rename /oldname /newname" to rename files or folders.
Users can use "delete /name" to delete files or folders.
Users can give "goto foldername" to go to directories, "goto /" to move to root directory of the application.
Users can give "copy /name(s) /destination" to copy files or folders (multiple also).
Users can give "move /name(s) /destination" to move files or folders (multiple also).
Users can give "snapshot /foldername /filename" to create a snapshot of a directory like "ls-R". 
Users can give "search name" to search any filename or foldername. All search results will be shown in normal mode. Upon clicking ENTER, users can go into the folders or open files from the search result directly.
Users can give "exit" command to come out of the application.
