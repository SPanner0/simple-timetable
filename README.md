# Timetable Console Application
<p>
A simple time table application for school, work, or whatever you want!
</p>


## Introduction
<p>
This is console application that provides a nice way of keeping track of students' schedule with a neat benefit of checking for scheduling conflicts.<br>
The application was written as a college assignment but it may be used by anyone for anything.<br>
</p>


## How to use

Requirements:
<p>
All you need is the GNU gcc compiler to compile from the source code to use on your machine; there are no third-party libraries used by the application.<br>
A Makefile is provided to conveniently compile the code, simply run this command:<br>
</p>

`make release`  

<p>This will make a binary file named "timetable" and you can run it by running:<br></p>

`./timetable`

<p>For the debuggable version, you can run:</p>

`make debug`
`./debug-timetable`


## The Menus
<p>Upon starting the application, you will be greeted with a welcome message and the main menu, showcased below:</p>

```
Please select an option by entering a number corresponding to the option you want below:

1. List Classes
2. Add Class
3. Delete Class
4. Edit Class
5. Save Changes to File
6. Print Timetable
7. Exit Program
Enter here:
```

<p>Simply enter the number for the option you want into the terminal.</p>


## The Options

### 1. List Classes
<p>
Simply list the classes currently present in the program. An unsaved added state will be indicated with a '+' and an unsaved deleted state with a '-' at the beginning of a list item.<br>
Conflicts will be indicated at the end of a list item with "(CONFLICT!)".
</p>
Sample output:

```
Here are the list of classes you have saved, a + means that class is added but unsaved and a '-' means that class is deleted but unsaved:
 1) Math, Monday, 9:30
+2) Chemistry, Friday, 13:00 (CONFLICT!)
 3) Physics, Friday, 13:00 (CONFLICT!)
-4) English, Thursday, 15:30
```

### 2. Add Class
<p>
Add a class to the list but does not save it to the file.<br>
Follow the instructions to add a class to the list.<br>
<strong> Do note that the program only supports a 24 hour time format and has limits as to what time is allowed; by default, it is between 8 to 17<br>
This is to prevent the timetable from messing up on smaller terminals.</strong>
</p>

### 3. Delete Class
<p>
Delete a class from the list, changes won't be reflected in save file until explicitly saved using the <em>Save to File</em> option.
</p>

### 4. Edit Class
<p> Basically the same thing as deleting a class and adding a new one.</p>

### 5. Save Changes to File
<p> Writes all current classes on the list to the file to be saved permanently. Also clears unsaved added and deleted flags on each class.</p>

### 6. Print Timetable
<p>
Print the actual timetable to the terminal. Conflicts will be shown in place of the class of that slot if there were to be a conflict.<br>
<em>Unsaved deleted class will not be shown in the timetable.</em>
</p>
Sample output:

```
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|         |8:00     |8:30     |9:00     |9:30     |10:00    |10:30    |11:00    |11:30    |12:00    |12:30    |13:00    |13:30    |14:00    |14:30    |15:00    |15:30    |16:00    |16:30    |
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|Monday   |         |         |         |CONFLICT |         |         |         |         |Physics  |         |         |         |         |         |         |         |         |         |
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|Tuesday  |         |Biology  |         |         |         |         |         |         |         |         |         |         |         |         |         |         |         |         |
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|Wednesday|         |         |         |         |         |         |         |         |         |         |         |         |         |         |         |Chemistry|         |         |
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|Thursday |         |         |         |         |English  |         |         |         |         |         |         |         |         |         |         |         |         |         |
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|Friday   |         |         |         |         |         |         |         |         |         |         |         |         |         |         |         |Math     |         |         |
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
```

### 7. Exit Program
<p> Asks user to save any unsaved changes if unsaved changes are detected. Terminates the program after.</p>