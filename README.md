# Web-based-Datalogger
A sample project in Interactive Pervasive Computing
There are 3 types of files in this project:

Arduino Source Code
```````````````````
The .ino files are Arduino source code files. Use your Arduino IDE to open it.
Compile and upload to your Arduino board.

PHP Scripts
```````````
The .php files are to be placed in the htdocs directory of your MAMP webserver.
They are designed to interact with the database below. These scripts are called by
the Arduino source code above. Each call is a GET request containing the script name
and some sensor parameters.

Database
````````
The arduio.sql file is the database. You may import it into your MAMP's SQL Server.
It contains 3 tables used to store data from Arduino.
