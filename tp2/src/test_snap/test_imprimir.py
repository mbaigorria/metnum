import csv

with open('norma.txt','rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    for row in reader:
        print float(row[0]) + 1