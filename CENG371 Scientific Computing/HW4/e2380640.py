import numpy as np
import cv2
import scipy
import matplotlib.pyplot as pyp
import time
from PIL import Image


p = 5 # parameter p
error_list1 = []
error_list2 = []
def approximate_svd(A, k):
    omega = np.random.normal(0, 1, (A.shape[1], k + p))  # initialize gaussian random omega matrix
    Y = np.matmul(A,omega) # Y = A*omega
    Q,R = np.linalg.qr(Y) # QR factorization of Y
    B = np.matmul(np.transpose(Q), A) # B = Q^T * A
    Uhat, Sigma, V = np.linalg.svd(B,full_matrices=False)
    Sigma = np.diag(Sigma)
    U = np.matmul(Q,Uhat) # U = Q * Uhat
    return U, Sigma, V

"""Cameraman approximate"""
"""im = Image.open('cameraman.jpg')
im_mat = np.array(im)
k = 25
U, S, VT = approximate_svd(im_mat,k)

approx_mat = np.dot(U[:, :k], np.dot(np.diag(S[:k]), VT[:k, :]))
approx_im = Image.fromarray(np.uint8(approx_mat))
approx_im.save("cameraman_approx.jpg")"""

"""Cameraman svd"""
"""im = Image.open('cameraman.jpg')
im_mat = np.array(im)
U, S, VT = np.linalg.svd(im_mat)
k = 25
approx_mat = np.dot(U[:, :k], np.dot(np.diag(S[:k]), VT[:k, :]))
approx_im = Image.fromarray(np.uint8(approx_mat))
approx_im.save("cameraman_svd.jpg")"""

"""Cameraman svds"""
"""im = Image.open('cameraman.jpg')
im_mat = np.array(im)
k = 25
U, S, VT = scipy.sparse.linalg.svds(np.float64(im_mat), k)
approx_mat = np.dot(U[:, :k], np.dot(np.diag(S[:k]), VT[:k, :]))
approx_im = Image.fromarray(np.uint8(approx_mat))
approx_im.save("cameraman_svds.jpg")"""

"""Fingerprint approximate"""
"""im = Image.open('fingerprint.jpg')
im_mat = np.array(im)
k = 25
U, S, VT = approximate_svd(im_mat,k)

approx_mat = np.dot(U[:, :k], np.dot(np.diag(S[:k]), VT[:k, :]))
approx_im = Image.fromarray(np.uint8(approx_mat))
approx_im.save("fingerprint_approx.jpg")"""

"""Fingerprint svd"""
"""im = Image.open('fingerprint.jpg')
im_mat = np.array(im)
U, S, VT = np.linalg.svd(im_mat)
k = 25
approx_mat = np.dot(U[:, :k], np.dot(np.diag(S[:k]), VT[:k, :]))
approx_im = Image.fromarray(np.uint8(approx_mat))
approx_im.save("fingerprint_svd.jpg")"""

"""Fingerprint svds"""
"""im = Image.open('fingerprint.jpg')
im_mat = np.array(im)
k = 25
U, S, VT = scipy.sparse.linalg.svds(np.float64(im_mat), k)
approx_mat = np.dot(U[:, :k], np.dot(np.diag(S[:k]), VT[:k, :]))
approx_im = Image.fromarray(np.uint8(approx_mat))
approx_im.save("fingerprint_svds.jpg")"""


"""Cameraman"""

path = r'C:\Users\lebro\Desktop\THE4\cameraman.jpg'
cameraman = cv2.imread(path)
cameraman = cv2.cvtColor(cameraman, cv2.COLOR_BGR2GRAY)
cameraman = cameraman.astype(np.float64)


rank = np.linalg.matrix_rank(cameraman)
U, Sigma, V = np.linalg.svd(cameraman)
Sigma = np.diag(Sigma)
divider = np.matmul(np.matmul(U,Sigma),V)
runtime1 = []
runtime2 = []
for k in range(1,rank):
    start = time.time()
    U_k, Sigma_k, V_k = approximate_svd(cameraman, k)
    stop = time.time()
    runtime1.append(stop-start)
    start = time.time()
    U_hat_k, Sigma_hat_k, V_hat_k = scipy.sparse.linalg.svds(cameraman, k)
    stop = time.time()
    runtime2.append(stop-start)
    Sigma_hat_k = np.diag(Sigma_hat_k)
    error_list1.append(np.linalg.norm(np.matmul(np.matmul(U_k,Sigma_k),V_k) - divider,2) / np.linalg.norm(divider,2))
    error_list2.append(np.linalg.norm(np.dot(np.dot(U_hat_k,Sigma_hat_k),V_hat_k) - divider,2) / np.linalg.norm(divider,2))
    print("k: ",k)

pyp.plot(range(1,rank), error_list1)
pyp.title("Cameraman 1st error")
pyp.xlabel("k")
pyp.ylabel("Relative error")
pyp.show()

pyp.plot(range(1,rank), error_list2)
pyp.title("Cameraman 2nd error")
pyp.xlabel("k")
pyp.ylabel("Relative error")
pyp.show()

pyp.plot(range(1,rank), runtime1)
pyp.title("Cameraman runtime of approximate_svd")
pyp.xlabel("k")
pyp.ylabel("Runtime")
pyp.show()

pyp.plot(range(1,rank), runtime2)
pyp.title("Cameraman runtime of svds")
pyp.xlabel("k")
pyp.ylabel("Runtime")
pyp.show()

"""Fingerprint """

error_list1 = []
error_list2 = []

path = r'C:\Users\lebro\Desktop\THE4\fingerprint.jpg'

fingerprint = cv2.imread(path)
fingerprint = cv2.cvtColor(fingerprint, cv2.COLOR_BGR2GRAY)
fingerprint = fingerprint.astype(np.float64)


rank = np.linalg.matrix_rank(fingerprint)
U, Sigma, V = np.linalg.svd(fingerprint,full_matrices=False)
Sigma = np.diag(Sigma)
divider = np.matmul(np.matmul(U,Sigma),V)
k = 1
runtime1 = []
runtime2 = []
while k < rank:
    start = time.time()
    U_k, Sigma_k, V_k = approximate_svd(fingerprint, k)
    stop = time.time()
    runtime1.append(stop - start)
    start = time.time()
    U_hat_k, Sigma_hat_k, V_hat_k = scipy.sparse.linalg.svds(fingerprint, k)
    stop = time.time()
    runtime2.append(stop - start)
    Sigma_hat_k = np.diag(Sigma_hat_k)
    error_list1.append(np.linalg.norm(np.matmul(np.matmul(U_k,Sigma_k),V_k) - divider,2) / np.linalg.norm(divider,2))
    error_list2.append(np.linalg.norm(np.dot(np.dot(U_hat_k,Sigma_hat_k),V_hat_k) - divider,2) / np.linalg.norm(divider,2))
    print("k: ",k)
    k += 25

pyp.plot(np.arange(1,rank,25), error_list1)
pyp.title("Fingerprint 1st error")
pyp.xlabel("k")
pyp.ylabel("Relative error")
pyp.show()

pyp.plot(np.arange(1,rank,25), error_list2)
pyp.title("Fingerprint 2nd error")
pyp.xlabel("k")
pyp.ylabel("Relative error")
pyp.show()

pyp.plot(np.arange(1,rank,25), runtime1)
pyp.title("Fingerprint runtime of approximate_svd")
pyp.xlabel("k")
pyp.ylabel("Runtime")
pyp.show()

pyp.plot(np.arange(1,rank,25), runtime2)
pyp.title("Fingerprint runtime of svds")
pyp.xlabel("k")
pyp.ylabel("Runtime")
pyp.show()


