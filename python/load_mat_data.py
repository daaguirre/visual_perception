import os

import numpy as np
from scipy.io import loadmat


def main():
    mat_file_path = "/home/diego/Documents/_91bfcc07c57f342de5453f0d60740e93_RoboticsPerceptionWeek4AssignmentCode/data.mat"
    data = loadmat(mat_file_path, squeeze_me=True)
    data_struct = data["data"][()]
    x1 = data_struct[0]
    x2 = data_struct[1]
    x3 = data_struct[2]
    R = data_struct[3]
    C = data_struct[4]
    K = data_struct[5]
    img1 = data_struct[6]
    img2 = data_struct[7]
    img3 = data_struct[8]
    with open("data_x1.npy", 'wb') as f:
        np.save(f, x1)    
    with open("data_x2.npy", 'wb') as f:
        np.save(f, x2)    
    with open("data_x3.npy", 'wb') as f:
        np.save(f, x3)    


if __name__ == "__main__":
    main()
