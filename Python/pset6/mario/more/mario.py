from cs50 import get_int

def main():
    # initialize height variable
    height = 0

    # ask user for height between 1 and 8 using get_int until valid response is given
    while not 1 <= height <= 8:
        height = get_int("Height: ")

    # call function make to make pyramid by passing height twice
    # second height will remain constant
    make(height, height)

def make(row, height):
    # using second height as constant for height to keep track of how many spaces
    # are needed for each row on left side of pyramind

    # if row number is not 1 then call make on next row
    if row != 1:
        make(row-1, height)

    # draw left side of pyramid
    print(" " * (height-row), end="")
    print("#" * row, end="")

    # draw middle gap
    print(" " * 2, end="")

    # draw right side of pyramid
    print("#" * row)

main()
