from cs50 import SQL
import csv
from sys import argv, exit

# check to ensure correct number of command-line arguments
if len(argv) != 2:
    print("Usage: python import.py data.csv")
    exit(0)

# set up database connection to allow for execution of SQL queries within python script
db = SQL("sqlite:///students.db")

# reset table for testing purposes
db.execute("DELETE FROM students;")

# open CSV file
with open(argv[1], "r") as file:

    # create a DictReader
    reader = csv.DictReader(file)

    # iterate through csv file
    for row in reader:

        # split full name by " " b/w words and assing to array to be able to access first, middle, and last names
        full_name = row["name"].split()

        # len of array is home many words in name
        # if len is 2 indicates no middle name so leave middle value null in db
        if len(full_name) == 2:

            # insert values into db using SQL
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?,?,?,?,?)",
                       full_name[0], None, full_name[1], row['house'], int(row['birth']))

        # if len is 3 indicates a middle name so assign names according to order in array
        elif len(full_name) == 3:

            # insert values into db using SQL
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?,?,?,?,?)",
                       full_name[0], full_name[1], full_name[2], row['house'], int(row['birth']))
