import sys
import numpy as np
from matplotlib import pyplot as plt

### HW1 : CENG 371


def Q1(n):
    epsilon = sys.float_info.epsilon
    f = n*(((n+1)/n) -1) -1
    g = f / epsilon
    return g
plt.xlabel('x - axis')
plt.ylabel('y - axis')
x = list(range(1,1001)) # [1,1000]
y = []
for n in x:
   y.append(Q1(n))
   #if (Q1(n) == 0): check for g(n) == 0 ,Q1) b:
       #print(n)

plt.plot(x, y)
plt.title('g(n)')
#plt.show()
####################################
nums = [np.double(0.0)]*(pow(10,6) + 1)
def naive_single():
    for n in range(1,pow(10,6) + 1):
        temp = np.double(np.double(1) + (np.double(pow(10,6)) + np.double(1) - np.double(n)) * np.double(pow(10,-8)))
        nums[n] = np.double(temp)
    return None
naive_single()
nums.pop(0)
sum = np.double(0.0)
for elem in nums:
    sum += np.double(elem)
print(sum)
# summation algorithm
def kahanSum(fa):
    sum = np.double(0.0)

    # Variable to store the error
    c = np.double(0.0)

    # Loop to iterate over the array
    for f in np.double(fa):
        y = np.double(np.double(f) - np.double(c))
        t = np.double(np.double(sum) + np.double(y))

        # Algebraically, c is always 0
        # when t is replaced by its
        # value from the above expression.
        # But, when there is a loss,
        # the higher-order y is cancelled
        # out by subtracting y from c and
        # all that remains is the
        # lower-order error in c
        c = np.double((np.double(t) - np.double(sum)) - np.double(y))
        sum = np.double(t)

    return np.double(sum)

print("Naive sum: ",np.double(sum) )
print("compensated num: ", kahanSum(np.double(nums)))
print("Pairwise: ",np.sum(nums, dtype=np.double))
#print(sys.float_info.epsilon)
#print (np.finfo(np.single).eps)
#print(np.finfo(np.double).eps)
