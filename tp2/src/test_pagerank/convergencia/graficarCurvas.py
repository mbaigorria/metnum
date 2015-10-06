import csv
import matplotlib.pyplot as plt
import math

#plt.xkcd()

dataN = []
data2265 = []

with open('2265.txt','rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    i = 1
    for row in reader:
        dataN.append(i)
        data2265.append(row[0])
        i += 1

fig = plt.figure()
sub = fig.add_subplot(1,1,1)
sub.scatter(dataN, data2265, color='blue', edgecolor='black')
#sub.scatter(dataDim, dataTimeLU, color='red', edgecolor='black', label='LU')
#plt.legend(loc='upper right', scatterpoints=1)
plt.axis([0.0, 16.0, 0.0, 0.5])
plt.xlabel('Nro. Iteraciones')
plt.ylabel('delta')
plt.title('Convergencia de L1')
#plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
fig.savefig('2265.pdf')
plt.close(fig)

dataN = []
data5000 = []

with open('5000.txt','rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    i = 1
    for row in reader:
        dataN.append(i)
        data5000.append(row[0])
        i += 1

fig = plt.figure()
sub = fig.add_subplot(1,1,1)
sub.scatter(dataN, data5000, color='blue', edgecolor='black')
#sub.scatter(dataDim, dataTimeLU, color='red', edgecolor='black', label='LU')
#plt.legend(loc='upper right', scatterpoints=1)
plt.axis([0.0, 16.0, 0.0, 0.5])
plt.xlabel('Nro. Iteraciones')
plt.ylabel('delta')
plt.title('Convergencia de L1')
#plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
fig.savefig('5000.pdf')
plt.close(fig)