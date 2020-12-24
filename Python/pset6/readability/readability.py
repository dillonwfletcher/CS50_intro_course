from cs50 import get_string

def main():

    # get text to access for grade level
    text = get_string("Text: ")

    # initialize counts for number of letters and sentences in text
    letters, sentences = 0, 0

    # iterate through text and count number of letters and sentence
    for i in text:
        if i.isalpha():
            letters += 1
        elif i in ['?', '!', '.']:
            sentences += 1

    # strip any white space at beginning and end of text
    # split text by any whitespace effectively seperating text by words
    # words get put into list and len of list is total number of words in text
    words = len(text.strip().split())

    colemanLiau(letters, words, sentences)


def colemanLiau(l, w, s):

    # calculate average number of letters per 100 words
    L = l * 100/w

    # calculate average number of words per 100 words
    S = s * 100/w

    # calculate score base on Coleman-Liau formula
    score = round(0.0588 * L - 0.296 * S - 15.8)

    if score >= 16:
        print("Grade 16+")

    elif score < 1:
        print("Before Grade 1")

    else:
        print(f"Grade {round(0.0588 * L - 0.296 * S - 15.8)}")


main()
