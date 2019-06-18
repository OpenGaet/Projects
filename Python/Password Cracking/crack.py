import sys
import crypt


def main():

    # ASCII TABLE important values solely including upper and lower alphabet
    ALPHASTART = 65
    UPPEND = 90
    LOWSTART = 96
    ALPHAEND = 123

    # check num og args
    if not len(sys.argv) == 2:
        print("Error: Only enter one argument")
        return 1

    # Set hash equal to hash given as arg
    hashList = ""
    salt = ""

    hashList = sys.argv[1]

    # Salt is the first two chars of the hash in DES crypt funct
    salt = hashList[0] + hashList[1]

    print(salt)

    # Initialize guess password and its resulting hash
    passwd = ""
    guesHash = ""

    # for single char password, run through alphabet, generate and compare guess hashes to actual
    i = ALPHASTART
    while i < ALPHAEND:
        passwd = chr(i)
        passwd.strip()
        guesHash = crypt.crypt(passwd, salt)

        if guesHash == hashList:
            print(passwd)
            return 0

        if i == UPPEND:  # at end of uppercase alphabet skip to lowercase
            i = LOWSTART
        i += 1

    # Two Char Password
    i = ALPHASTART
    while i < ALPHAEND:
        passwd = chr(i)

        j = ALPHASTART
        while j < ALPHAEND:
            passwd = chr(i) + chr(j)
            passwd.strip()
            guesHash = crypt.crypt(passwd, salt)

            if guesHash == hashList:
                print(passwd)
                return 0

            if j == UPPEND:  # at end of uppercase alphabet skip to lowercase
                j = LOWSTART
            j += 1

        if i == UPPEND:  # at end of uppercase alphabet skip to lowercase
            i = LOWSTART
        i += 1

    # Three Char Passwd
    i = ALPHASTART
    while i < ALPHAEND:
        passwd = chr(i)

        j = ALPHASTART
        while j < ALPHAEND:
            passwd = chr(i) + chr(j)

            k = ALPHASTART
            while k < ALPHAEND:
                passwd = chr(i) + chr(j) + chr(k)
                passwd.strip()
                guesHash = crypt.crypt(passwd, salt)

                if guesHash == hashList:
                    print(passwd)
                    return 0

                if k == UPPEND:  # at end of uppercase alphabet skip to lowercase
                    k = LOWSTART
                k += 1

            if j == UPPEND:  # at end of uppercase alphabet skip to lowercase
                j = LOWSTART
            j += 1

        if i == UPPEND:  # at end of uppercase alphabet skip to lowercase
            i = LOWSTART
        i += 1

    # Four Char Passwd
    i = ALPHASTART
    while i < ALPHAEND:
        passwd = chr(i)

        j = ALPHASTART
        while j < ALPHAEND:
            passwd = chr(i) + chr(j)

            k = ALPHASTART
            while k < ALPHAEND:
                passwd = chr(i) + chr(j) + chr(k)

                l = ALPHASTART
                while l < ALPHAEND:
                    passwd = chr(i) + chr(j) + chr(k) + chr(l)
                    passwd.strip()
                    guesHash = crypt.crypt(passwd, salt)

                    if guesHash == hashList:
                        print(passwd)
                        return 0

                    if (l == UPPEND):  # at end of uppercase alphabet skip to lowercase
                        l = LOWSTART
                    l += 1

                if k == UPPEND:  # at end of uppercase alphabet skip to lowercase
                    k = LOWSTART
                k += 1

            if j == UPPEND:  # at end of uppercase alphabet skip to lowercase
                j = LOWSTART
            j += 1

        if i == UPPEND:  # at end of uppercase alphabet skip to lowercase
            i = LOWSTART
        i += 1


if __name__ == "__main__":
    main()