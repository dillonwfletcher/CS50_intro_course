from sys import argv, exit
import csv
import re

def main(argc, argv):

    # check to make sure correct number of command-line arguments were given
    if argc != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    # open csv containing STR counts
    with open(argv[1], "r") as file:

        # read in contents of csv into memory
        reader = csv.reader(file)

        # store each row of csv into a list
        rows = list(reader)

    # open txt file with dna sequence
    with open(argv[2], "r") as file:

        # read in contents of txt file into memory
        txt = file.read()

    # grab the first row of csv to able to access the STRs ignoring "name" column
    header = rows[0]

    # create a dictionary to hold the STR values and the resulting consec count from sequence
    results_dict = {}

    # iterate through header list
    for j in range(1, len(header)):

        # set exp we will search for equal to one of DNA STR from header
        exp = header[j]

        # initialize variable we will use to keep track of max consec occurances
        exp_max = 0

        # iterate through DNA sequence
        for i in range(len(txt)):

            # call find function and return count of consec occurances to result
            result = find(i, len(exp), exp, txt, 0)

            # if result is greater than current max consec occurances set result to max
            if result > exp_max:
                exp_max = result

        # add entry to dictionary where key is exp searched for and value is max # of occurances
        results_dict[exp] = exp_max

    # find individual that matches sequence results in database
    match(rows, header, results_dict)



def find(i, x, exp, text, count):
    # finds max consec matches for each STR
    # accepts current index i, x as lenght of exp, exp as STR expression
    # text as DNA sequence text, and count as initial count

    #initialize result to count which is zero to keep track of results
    result = count

    # calculate j to allow us to compare exp against substring in sequence of equal length
    j = i + x

    # if STR exp match found keep searching for a consec match
    if exp == text[i:j]:
        count += 1
        result = find(j, x, exp, text, count)

    # return max consec occurences of STR expression
    return result


def match(base, head, key):
    # accepts base: list of each indiv and their STR score
    # accepts head: list of STRs searching for
    # accepts key: dictionary that contains values from searching sequence

    # set match to defalut as No Match which will be printed if no match is found
    match = "No Match"

    # set found to false; set to true once match is found
    found = False

    # iterate through each row of database ignoring names of individuals
    for r in base[1:]:

        # iterate through each index of head list
        for h in range(1, len(head)):

            # if the STR value in key doesnt match STR value in database break and go to next row in database
            if key[head[h]] != int(r[h]):
                found = False
                break

            # set found to true since match has been found
            found = True

        # if all STR values in key match STR values in database, set match equal to the individual
        # with the matching values and break loop since individual has been found
        if found:
            match = r[0]
            break

    # print matching individual's name
    print(match)


main(len(argv), argv)

