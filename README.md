# mysqlDatabaseManager
Allows control of a mysql database through command line

Outside users will not have access to database as it can only be accessed through uoguelph socs server

To Compile:
gcc db.c -o db -lmysqlclient -L/usr/lib/x86_64-linux-gnu/
For me, mysqlclient library stored under /usr/lib/x86_64-linux-gnu/. May be stored in different directory for other users.
