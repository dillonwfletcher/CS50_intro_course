from cs50 import get_int
from sys import exit

def main():

    # ask user for credit card number
    # keep asking until just credit card number with no words or hypen is given
    number = get_int("Number: ")

    # convert credit card number into a array holding all digits of card
    cc = [int(x) for x in str(number)]

    if cc[0] in [3,4,5] and len(cc) in [13,15,16]:
    # if cc length match lengths used for cards and first digit matches digits used, identify card
        identify(cc, len(cc))

    # if cc number cannot be validated print INVALID and exit program
    print("INVALID\n")
    exit(0)


def valid(num_array, length):
    # validates authenticy of cc using Luhn's algo
    # pass in arrary holding cc digits

    # initialize two vars to hold sums of numbers we will multiply by 2 (mlt2)
    # and sums of numbers we will not multiply by two (n_mlt)
    mlt2, n_mlt = 0, 0

    # iterate through cc number starting from the last digit in card number
    for i in range(length-1, -1, -1):

        # get digit from current index in cc number
        val = num_array[i]

        # if cc number length is even, index of last digit will be odd so...
        if length % 2 == 0:

            # add every digit at odd index to sum of n_mlt
            if i % 2 == 1:
                n_mlt += val

            # multiply digit at every even index by 2
            else:

                # if product is only one digit add to mlt2 sum
                if 2 * val < 10:
                    mlt2 += 2 * val

                # if product is more than one digit add each digit to mlt2 sum
                else:
                    for i in [int(x) for x in str(2 * val)]:
                        mlt2 += i

        # if cc num length is odd, index of last digit will be even so...
        else:

            # add every digit at even index to sum of n_mlt
            if i % 2 == 0:
                n_mlt += val

            # mult digit by 2 and add to mlt2 sum at every odd index
            else:

                # if product is only one digit add to mlt2 sum
                if 2 * val < 10:
                    mlt2 += 2 * val

                # if product is more than one digit add each digit to mlt2 sum
                else:
                    for i in [int(x) for x in str(2 * val)]:
                        mlt2 += i

    # check to see if total sum is divisible by 10 to confirm validation of cc
    if (mlt2 + n_mlt) % 10 == 0:
        return True

    # if cc is not able to be validated return false
    return False


def identify(num_array, length):
    # identifies what type of cc
    # accepts array of cc digits and cc length

    # if first digit and card is valid, card is Visa
    if num_array[0] == 4 and valid(num_array, length):
        print("VISA\n")
        exit(1)

    # if first two digits are 34 or 37 and card is valid, card is AMEX
    elif num_array[0] == 3 and num_array[1] in [4,7] and valid(num_array, length):
        print("AMEX\n")
        exit(1)

    # if first two digits are 51,52,53,54,or 55 and card is valid, card is MC
    elif num_array[0] == 5 and num_array[1] in range(1,6) and valid(num_array, length):
        print("MASTERCARD\n")
        exit(1)


main()
