strings = []
string0 = []
string1 = []
string2 = []

with open("keynotesreduced.csv") as f:

    lines = f.readlines()

    for l in lines:
        strings.append(l.split(",")[1].strip())
        string0.append(l.split(",")[2].strip())
        string1.append(l.split(",")[3].strip())
        string2.append(l.split(",")[4].strip())

def printList(l):
    s = ""
    for item in l[1:]:
        s += item + ", "
    s = s[:len(s)-2]
    print("waveform{" + s + "};")



printList(strings)
printList(string0)
printList(string1)
printList(string2)
