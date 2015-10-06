import csv

data1 = []
data2 = []
data3 = []
data4 = []

with open('instancia_1.out', 'rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=",")
    for row in reader:
        data1.append(row[0])
        
with open('instancia_2.out', 'rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=",")
    for row in reader:
        data2.append(row[0])
        
with open('instancia_3.out', 'rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=",")
    for row in reader:
        data3.append(row[0])
        
with open('instancia_4.out', 'rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=",")
    for row in reader:
        data4.append(row[0])
        
with open('ranking.csv', 'wb') as csvfile:
    writer = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
    writer.writerow(["Nro. Web"] + ["PageRank"]);
    for i in xrange(len(data1)):
        writer.writerow([i+1] + [data1[i]])
        
with open('ranking_modificado.csv', 'wb') as csvfile:
    writer = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
    writer.writerow(["Nro. Web"] + ["PageRank(c = 0.6)"] + ["PageRank(c = 0.85)"] + ["PageRank(c = 0.95)"]);
    for i in xrange(len(data1)):
        writer.writerow([i+1] + [data2[i]] + [data3[i]] + [data4[i]])        