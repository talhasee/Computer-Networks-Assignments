import matplotlib.pyplot as plt
import numpy as np
x_ = []
y_ = []
ptr = open("tcp-example.tr", "r")
ctr = 0
data = ptr.readlines()
q = dict()

for row in data:
    arr = row.strip().split()
    arr2 = row.strip()
    Seq_ID = int(arr[36][4:]) #Sequence Number is located at 36th index in tcp-example.tr file
    T = float(arr2[2:arr2.find(' ',2)])
    if arr[0] == "+":
        q[Seq_ID] = T
    elif arr[0] == "-" and Seq_ID in q.keys():
        if len(x_) > 0 and x_[len(x_) - 1] == T:
            y_[-1] = (y_[-1] * ctr + T - q[Seq_ID]) / (ctr + 1)
            ctr += 1
        else:
            y_.append(T - q[Seq_ID])
            ctr = 1
            x_.append(T)       
        del q[Seq_ID]
xpoints = np.array(x_)
ypoints = np.array(y_)
plt.plot(xpoints, ypoints, "g+")
plt.xlabel("Time(s)")
plt.ylabel("Queuing Delay(s)")
plt.show()
