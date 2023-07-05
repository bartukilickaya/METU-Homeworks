import numpy as np
from scipy.linalg import hilbert
import timeit
#hilbert(n)
np.set_printoptions(suppress=True)
A = np.array([[1, 2, 3], [3, 13, 4], [4, 8, 9]])
B = np.array([[1., 2., 3., 4.], [4., 5., 6., 7.], [7., 8., 9., 2.], [0., 0., 10., 2.]])
C = np.array([[1.,2.,3.],[4.,5.,6.],[7.,8.,-1.]])
D = np.array([[1.,2.,3.,4.,12.],[4.,5.,6.,1.,3.],[7.,8.,9.,6.,6.],[2.,3.,1.,1.,2.],[3.,3.,2.,3.,5.]])
t = "e-3"
time = 0.0
t1 = "e-12"
temp = []
templst = []
n = 3
Upper = []
Lower = []
# recursive sherman solution
def sherman(A, k, n):
    if (k < n):
        lower = A[0:k, 0:k]
        lower = np.tril(lower)
        np.fill_diagonal(lower, 1)  # set l1
        A[0:k, k] = np.matmul(np.linalg.inv(lower), A[0:k, k])
        upper = np.triu(A[0:k, 0:k])
        A[k, 0:k] = np.matmul(A[k, 0:k], np.linalg.inv(upper))
        A[k, k] = A[k, k] - np.matmul(A[k, 0:k], A[0:k, k])
        if (A[k][k] == 0):
            print("Error")
            return
        return sherman(A, k + 1, n)
    else:
        upper = np.triu(A)
        lower = np.tril(A)
        np.fill_diagonal(lower, 1)  # set l1
        #print("Sherman L value: ")  # L value
        #print(lower)
        #print("Sherman U value: ")  # U value
        #print(upper)
        return (upper,lower)

def picketts(A,k,n,Upper,Lower):
    if( k>=n ):
        #print("Pickett's L value: ")  # L value
        #print(Lower)
        #print("Pickett's U value: ")  # U value
        #print(Upper)
        return (Upper,Lower)
    elif( k == -1 ):
        Lower = np.tril(A)
        Upper = np.triu(A)
        return picketts(A,k+1,n,Upper,Lower)
    else:
        u1k = np.matmul(np.linalg.inv(Lower[0:k,0:k]),A[0:k,k]) # u1k =
        Upper[0:k,k] = u1k
        A[k:n,0:k] = np.matmul(Lower[k:n,0:k],Upper[0:k,0:k]) # A_k1 = L_k1 * U_11
        Lower[k,k] = 1
        ukk = (A[k:n,k] - np.matmul(Lower[k:n,0:k],u1k))[0] # n-k*1 - n-k*1
        Upper[k,k] = ukk
        if( ukk != 0 ):
            Lower[k:n, k] = ((A[k:n, k] - np.matmul(Lower[k:n, 0:k], u1k)) / ukk)
        else:
            print("Error")
            return
        temp = np.copy(Lower)
        np.fill_diagonal(temp,0)
        A = Upper + temp
        return picketts(A,k+1,n,Upper,Lower)


def crout(A,k,n,Upper,Lower):
    if( k == -1 ):
        Lower = np.tril(A)
        Upper = np.triu(A)
        return  crout(A,k+1,n,Upper,Lower)
    elif (k >= n):
        #print("crout L value:")
        #np.fill_diagonal(Lower,1)
        #print(Lower)
        #print("crout U value:")
        #print(Upper)
        return (Upper,Lower)
    else:
        A[k:n,k] = A[k:n,k] - np.matmul(A[k:n,0:k],A[0:k,k])
        #Lower[k,k] = 1
        pk = 0
        while(pk <= k):
            if abs(A[pk,k]) >= np.max(np.absolute(A[k:, k])):
                break
            else:
                pk+=1
        if( pk > k ):
            pk = np.argmax(np.absolute(A[0:, k]))
        temp = np.copy(A[k,0:n])
        A[k,0:n] = A[pk,0:n]
        A[pk,0:n] = temp
        if( A[k,k] != 0):
            A[k+1:n,k] = A[k+1:n,k] / A[k,k]
            return crout(A,n,n,Upper,Lower)
        A[k,k+1:n] = A[k,k+1:n] - np.matmul(A[k,0:k],A[0:k,k+1:n])
        return crout(A,k+1,n,Upper,Lower)



"""def crout(A,k,n):
    while (k < n-1):
        A[k+1:n,k] = A[k+1:,k] / A[k,k]
        A[k+1:n,k+1:n] = A[k+1:n,k+1:n] - np.matmul(A[k+1:n,k],A[k,k+1:n])
        k+=1
    print(A)"""

if __name__ == '__main__':
    i = 1

    while (i < 300):
        hilb = hilbert(i)
        start_time = timeit.default_timer()
        (Sherman_Upper,Sherman_Lower) = sherman(hilb,1,i)
        sherman_time = (timeit.default_timer() - start_time)
        start_time = timeit.default_timer()
        (picketts_Upper,picketts_Lower) = picketts(hilb,-1,i,0,0)
        pickett_time = (timeit.default_timer() - start_time)
        start_time = timeit.default_timer()
        (crout_Upper,crout_Lower) = crout(hilb,-1,i,0,0)
        crout_time = (timeit.default_timer() - start_time)
        sherman_error = np.linalg.norm(hilb-np.matmul(Sherman_Lower,Sherman_Upper), ord=2) / np.linalg.norm(hilb, ord=2)
        picketts_error = np.linalg.norm(hilb-np.matmul(picketts_Lower,picketts_Upper), ord=2) / np.linalg.norm(hilb, ord=2)
        crout_error = np.linalg.norm(hilb-np.matmul(crout_Lower,crout_Upper), ord=2) / np.linalg.norm(hilb, ord=2)
        print("i = ",i ," Sherman Error: ",sherman_error,t1,"Sherman Time : ",sherman_time)
        print("i = ",i ," Pickett's Error: ",picketts_error, "Pickett's Time : ", pickett_time)
        print("i = ",i ," Crout Error: ",crout_error,"Crout Time : ",crout_time,t)
        i += 25

