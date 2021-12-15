import numpy as np
import argparse

def main():
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-r", "--radius", help="Radius [default = 1]", type = float, default = 1.)
    parser.add_argument("-d", "--domain", help="Domain length [Default = 2]", type=float, default = 2.)
    parser.add_argument("-n", "--number_particles", help="Number of particles per row [Default = 512]", type = int, default = 512)
    parser.add_argument('-f', '--filename', required=False, help=('Name of file (csv extension required: e.g. `-f filename.csv`)'), type=str, default="input.csv")
        
    args = parser.parse_args()
    Radius = args.radius
    Domain = args.domain
    NumberParticles = args.number_particles
    filename = args.filename


    # Particles' grid
    X = np.linspace(-Domain/2, Domain/2, NumberParticles)
    Y = np.linspace(-Domain/2, Domain/2, NumberParticles)
    xx,yy = np.meshgrid(X,Y)
    z = np.zeros(pow(NumberParticles, 2))

    # Initializing heat vector
    h_v = np.zeros((NumberParticles, NumberParticles))

    for i in range (NumberParticles):
        for j in range(NumberParticles):
            if (pow(xx[i,j],2) + pow(yy[i,j],2) < Radius):
                h_v[i,j] = 1
            else:
                h_v[i,j] = 0

    x = xx.flatten()
    y = yy.flatten()
    h = h_v.flatten()

    # Arguments in order as appearing in csv file:
    # X Y Z Vel_x Vel_y Vel_z Force_x Force_y Force_z Mass Temp Heat
    # only X, Y, and Heat are !=0
    file_data = np.column_stack((x, y, z, z, z, z, z, z, z, z, z, h))
    np.savetxt(filename, file_data, delimiter=" ")

# TODO: How t handle boundary as asked in 4.5

if __name__ == "__main__":
    main()