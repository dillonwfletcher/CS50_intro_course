from sys import argv, exit
from cs50 import SQL

# ensure correct number of command line arguments
if len(argv) != 2:
    print("Usage: python roster.py House")
    exit(0)

# set up database connection to allow for execution of SQL queries within python script
db = SQL("sqlite:///students.db")

# create a list filled with dictionary entries for each student of house requested from command line
house = db.execute(f"SELECT * FROM students WHERE house = '{argv[1]}' ORDER BY last, first")

# iterate through list and return each students information
for student in house:

    # print out student information
    print(student['first'] + " ",
          student['middle'] + " " if student['middle'] != None else "",
          student['last'],
          ", born ", student['birth'],
          sep='')
