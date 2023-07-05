import numpy as np
import copy
import scipy.io
import time


mat = scipy.io.mmread("can_229.mtx")
mat2 = mat.todense()
dense_matrix = np.squeeze(np.asarray(mat2))

np.set_printoptions(suppress=True)
A = np.array([[2, -1, 0, 0, 0], [-1, 2, -1, 0, 0], [0, -1, 2, -1, 0], [0, 0, -1, 2, -1], [0, 0, 0, -1, 2]],float)

B = np.array([[0.2, 0.3, -0.5], [0.6, -0.8, 0.2], [-1.0, 0.1, 0.9]], float)

I = np.array([[1,0,0,0,0],[0,1,0,0,0],[0,0,1,0,0],[0,0,0,1,0],[0,0,0,0,1]],float)

vector = np.array([1, 1, 1,1,1],float)
A2 = copy.deepcopy(A)

def normalize(x):
    lambdaa = abs(x).max()
    x_n = x / abs(x).max()  # normalize
    return lambdaa, x_n


def power_method(A,vector):
    eigenvalue = 0
    for i in range(1000):
        vector = np.dot(A, vector)
        eigenvalue, vector = normalize(vector)

    return eigenvalue, vector


eigenvalue, eigenvector = power_method(A,vector)

print("Eigenvalue power method: ", eigenvalue)
print("Eigenvector power method: ", eigenvector)

vector = np.array([1, 1, 1, 1, 1],float)


def inverse_power(A,alfa,vector):
    eigenvalue = 0
    shifted_inverseA = np.linalg.inv((A - alfa*I))
    for i in range(1000):
        vector = np.dot(shifted_inverseA, vector)
        eigenvalue, vector = normalize(vector)
    eigenvalue = 1/-eigenvalue + alfa # real eigenvalue
    return eigenvalue, vector

eigenvalue, eigenvector = inverse_power(A,2.2,vector)
print("Eigenvalue inverse method: ", eigenvalue)
print("Eigenvector inverse method: ", eigenvector)

vector = np.array([1, 1, 1, 1, 1],float)

eigenvalue, eigenvector = inverse_power(A,0,vector)
eigenvalue = -eigenvalue

print("Eigenvalue smallest: ", eigenvalue)
print("Eigenvector smallest: ", eigenvector)

vector = np.array([1, 1, 1],float)
eigenvalue, eigenvector = power_method(B,vector)

print("Eigenvalue for 1-d: ", eigenvalue)
print("Eigenvector for 1-d: ", eigenvector)


vector = np.array([1, 1, 1,1,1],float)

def power_k(A,k,vector):
    eigenvalue = 0
    sigma = 0
    eigenvalues = []
    eigenvectors = []
    for i in range(k):
        eigenvalue, vector = power_method(A,vector)
        eigenvalues.append(eigenvalue)
        eigenvectors.append(vector)
        A = A - (eigenvalue * np.outer(vector,vector) / (np.dot(vector, vector)))
    return eigenvalues,eigenvectors

eigenvalues, eigenvectors = power_k(A,2,vector)
print(f"Eigenvalues in power_k with k = {2}: ", eigenvalues)
print(f"Eigenvectors in power_k with k = {2}: ", eigenvectors)
A = copy.deepcopy(A2)
def subspace_iteration(A,k):
    X = np.array([[1] * k for i in range(len(A))])
    eigenvectors = []
    for i in range(k):
        for j in range(1000):
            Z = np.matmul(A, X)  # matmul
            X, R = np.linalg.qr(Z)  # Q R factorization
            X = np.transpose(X)
            eigenvalue,X[i] = normalize(X[i])
            X = np.transpose(X)
        #eigenvalues.append(eigenvalue)
        X = np.transpose(X)
        eigenvectors.append(X[i])
        X = np.transpose(X)
            #print(normalize(X))

    return eigenvalues,eigenvectors



eigenvalues, eigenvectors = subspace_iteration(A,2)
print(f"Eigenvalues in subspace with k = {2}: ",eigenvalues)
print(f"Eigenvectors in subspace with k = {2}: ",eigenvectors)

print("\n\n\n")
vector = np.array([1]*229,float)
start = time.time()
eigenvalues, eigenvectors = power_k(dense_matrix,20,vector)
end = time.time()
print("Time elapsed in power_k with can_229: ",end-start)
#print(f"Eigenvalues in power_k with k = {2}: ", eigenvalues)
#print(f"Eigenvectors in power_k with k = {2}: ", eigenvectors)

vector = np.array([2]*229,float)

start = time.time()
eigenvalues, eigenvectors = subspace_iteration(dense_matrix,20)
end = time.time()
print("Time elapsed in subspace iteration with can_229: ",end-start)
#print(f"Eigenvalues in can_229 with k = {2}: ",eigenvalues)
#print(f"Eigenvectors in can_229 with k = {2}: ",eigenvectors)