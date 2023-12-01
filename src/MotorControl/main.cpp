#include <iostream>
#include <vector>
#include <cmath>  // Include the cmath library to use sqrt
#include <ostream>
#include <cstdlib> // Required for system()
#include <type_traits>  
#include <chrono>
#include <Eigen/Dense>
#include "KinematicPoint.h"

//global variables
const double minS1angle = -180;
const double maxS1angle = 180;

const double minJ1angle = -135;
const double maxJ1angle = 135;
const double L1length = 1;

const double minJ2angle = -135;
const double maxJ2angle = 135;
const double L2length = 1;

const double minJ3angle = -135;
const double maxJ3angle = 135;
const double L3length = 1;

const double minS3angle = -180;
const double maxS3angle = 180;

const int S1step = 5;
const int J1step = 5;
const int J2step = 5;
const int J3step = 5;
const int S3step = 5;

//Weights for generating the nexxt set of angles. We generally want J1 to move as little as possible, so we make its weight bigger
//than the other weights. This way it contributes more towards our dist function later which will make movement of J1 more costly than
//movement of J2 or J3
double S1stepweight = 1;
double J1stepweight = 1;
double J2stepweight = 0.5;
double J3stepweight = 0.1;
double S3stepweight = 0.01;

//Generate starting Kpoint for our initial position, which will be 0 for all the angles and (0,0) for the x, y positon
KinematicPoint currentkpoint = KinematicPoint(0, 0, 0, 0, 0, 0, 0, 3);
                                    //        S1,J1,J2,J3,S3  X, Y, Z
double xsensitivity = 0.1;
double ysensitivity = 0.1;
double zsensitivity = 0.1;

double maxX = L1length + L2length + L3length;
double maxY = maxX;
double maxZ = maxX;
double minX = 0;
double minY = 0;
double minZ = 0;

double newx = 0;
double newy = 0;
double newz = 0;

// Calculate the number of steps for each angle
int S1numsteps = (maxS1angle - minS1angle)/ S1step + 1;
int J1numsteps = (maxJ1angle - minJ1angle)/ J1step + 1;
int J2numsteps = (maxJ2angle - minJ2angle)/ J2step + 1;
int J3numsteps = (maxJ3angle - minJ3angle)/ J3step + 1;
int S3numsteps = (maxS3angle - minS3angle)/ S3step + 1;

std::vector<std::vector<std::vector<std::vector<KinematicPoint>>>> kpointsXYZplane;

KinematicPoint GenerateNewKPoint(const KinematicPoint& currentKPoint, double newx, double newy, double newz);


// Define a constant for pi.
const double PI = 3.14159265358979323846;

// Function to convert degrees to radians
double degreesToRadians(double degrees) {
    return degrees * (PI / 180.0);
}

// Function for x position
double xPosition(double S1angle, double J1angle, double J2angle, double J3angle, double S3angle) {
    return std::cos(degreesToRadians(S1angle)) *
           (L1length * std::sin(degreesToRadians(J1angle)) + 
            L2length * std::sin(degreesToRadians(J1angle + J2angle))) + 
           std::cos(degreesToRadians(S1angle + S3angle)) *
           L3length * std::sin(degreesToRadians(J1angle + J2angle + J3angle));
}

// Function for y position
double yPosition(double S1angle, double J1angle, double J2angle, double J3angle, double S3angle) {
    return std::sin(degreesToRadians(S1angle)) *
           (L1length * std::sin(degreesToRadians(J1angle)) + 
            L2length * std::sin(degreesToRadians(J1angle + J2angle))) +
           std::sin(degreesToRadians(S1angle + S3angle)) * L3length * std::sin(degreesToRadians(J1angle + J2angle + J3angle));
}

// Function for z position
double zPosition(double S1angle, double J1angle, double J2angle, double J3angle, double S3angle) {
    return L1length * std::sin(degreesToRadians(J1angle)) + 
           L2length * std::sin(degreesToRadians(J1angle + J2angle)) + 
           L3length * std::sin(degreesToRadians(J1angle + J2angle + J3angle));
}

// Function to approximate partial derivative
double approximatePartialDerivative(double (*func)(double, double, double, double, double), 
                                    double S1angle, double J1angle, double J2angle, double J3angle, double S3angle, 
                                    int angleIdx, 
                                    double h = 1e-5) {
    // Array to store current angles
    double angles[5] = {S1angle, J1angle, J2angle, J3angle, S3angle};

    // Modify angle at index for forward difference
    angles[angleIdx] += h;
    double forward = func(angles[0], angles[1], angles[2], angles[3], angles[4]);

    // Modify angle at index for backward difference
    angles[angleIdx] -= 2 * h;
    double backward = func(angles[0], angles[1], angles[2], angles[3], angles[4]);

    // Compute derivative
    return (forward - backward) / (2 * h);
}

//This method will take in ur current angle positions and your new (x,y) position and give us back the kpoint with angles that will get
//us to our new (x,y) with the least amount of angle movement as possible
KinematicPoint GenerateNewKPoint(const KinematicPoint& currentKPoint, double newx, double newy, double newz) 
{
    KinematicPoint mindistKPoint = KinematicPoint();
    bool validnewpoint = false;

    //Loop through our kpoints using our current kpoint and find the minimum distance angles to create our new (x,y). That will the new angles we set the motors to
           int xpos = std::round((newx + maxX)/xsensitivity);
           int ypos = std::round((newy + maxY)/ysensitivity);
           int zpos = std::round((newz + maxZ)/zsensitivity);
           std::cout << xpos << " " << ypos << " " << zpos << std::endl;
            std::vector<KinematicPoint> pointstocheck = kpointsXYZplane[xpos][ypos][zpos];
            for (int k = 0; k < pointstocheck.size(); ++k) 
            {
                
                KinematicPoint kptocheck = pointstocheck[k];
                double mindist;
                
                
                //For the very first min found. Also doubles for telling us if a point is achievable
                if(validnewpoint == false)
                {
                    mindistKPoint.setAngle1(kptocheck.getAngle1());
                    mindistKPoint.setAngle2(kptocheck.getAngle2());
                    mindistKPoint.setAngle3(kptocheck.getAngle3());
                    mindistKPoint.setAngle4(kptocheck.getAngle4());
                    mindistKPoint.setAngle5(kptocheck.getAngle5());
                    mindistKPoint.setX(newx);
                    mindistKPoint.setY(newy);
                    mindistKPoint.setZ(newz);
                    mindist = S1stepweight*(fabs(kptocheck.getAngle1() - currentkpoint.getAngle1())) + J1stepweight*(fabs(kptocheck.getAngle2() - currentkpoint.getAngle2())) + J2stepweight*(fabs(kptocheck.getAngle3() - currentkpoint.getAngle3())) + J3stepweight*(fabs(kptocheck.getAngle4() - currentkpoint.getAngle4())) +S3stepweight*(fabs(kptocheck.getAngle5() - currentkpoint.getAngle5()));
                    validnewpoint = true;
                }
                else
                {
                    //Peform distance calculations and check them against our current running mindistkpoint
                    double tocheckdist = S1stepweight*(fabs(kptocheck.getAngle1() - currentkpoint.getAngle1())) + J1stepweight*(fabs(kptocheck.getAngle2() - currentkpoint.getAngle2())) + J2stepweight*(fabs(kptocheck.getAngle3() - currentkpoint.getAngle3())) + J3stepweight*(fabs(kptocheck.getAngle4() - currentkpoint.getAngle4())) +S3stepweight*(fabs(kptocheck.getAngle5() - currentkpoint.getAngle5()));
                    if(tocheckdist < mindist)
                    {
                        mindist = tocheckdist; //Update our running mindist
                        mindistKPoint.setAngle1(kptocheck.getAngle1()); //Update the mindistkpoint   
                        mindistKPoint.setAngle2(kptocheck.getAngle2());
                        mindistKPoint.setAngle3(kptocheck.getAngle3());
                        mindistKPoint.setAngle4(kptocheck.getAngle4());
                        mindistKPoint.setAngle5(kptocheck.getAngle5());
                    }
                    
                
                }
            
                
                
            }
            return mindistKPoint;
    
}

// Forward declaration of the function template to calculate storage for any type
template<typename T>
size_t calculateStorage(const T& value);

// Function template to calculate storage for vectors, specializes for vector types
template<typename T>
size_t calculateVectorStorage(const std::vector<T>& vec) {
    size_t storage = sizeof(vec); // Size of vector object itself
    for (const auto& item : vec) {
        storage += calculateStorage(item); // Calculate storage of each item in vector
    }
    storage += (vec.capacity() - vec.size()) * sizeof(T); // Account for vector capacity
    return storage;
}

// Function template specialization for non-vector types, assumes trivial storage calculation
template<typename T>
size_t calculateStorage(const T& value) {
    return sizeof(value); // By default, return the size of the value
}

// Template specialization for vectors, calls calculateVectorStorage
template<typename T>
size_t calculateStorage(const std::vector<T>& vec) {
    return calculateVectorStorage(vec);
}

KinematicPoint getKinematicPointAtoB(KinematicPoint currentkp, double desiredx, double desiredy, double desiredz)
{
    //KinematicPoint currentkp;


     // Example usage: Compute partial derivatives at a specific configuration
 //   double S1angle = 0, J1angle = 11, J2angle =44, J3angle = 27, S3angle = 0;
   // double desiredx = -2, desiredy = 0, desiredz = -2;
  //  double currentx = 2, currenty = 0, currentz = 2;

    double S1angle = currentkp.getAngle1();
    double J1angle = currentkp.getAngle2();
    double J2angle = currentkp.getAngle3();
    double J3angle = currentkp.getAngle4();
    double S3angle = currentkp.getAngle5();
    double currentx = currentkp.getX();
    double currenty = currentkp.getY();
    double currentz = currentkp.getZ();




    // Compute partial derivatives for x
    double dx_dS1 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 0);
    double dx_dJ1 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 1);
    double dx_dJ2 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 2);
    double dx_dJ3 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 3);
    double dx_dS3 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 4);

    double dy_dS1 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 0);
    double dy_dJ1 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 1);
    double dy_dJ2 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 2);
    double dy_dJ3 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 3);
    double dy_dS3 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 4);

   // double dz_dS1 = approximatePartialDerivative(zPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 0);
    double dz_dJ1 = approximatePartialDerivative(zPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 1);
    double dz_dJ2 = approximatePartialDerivative(zPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 2);
    double dz_dJ3 = approximatePartialDerivative(zPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 3);
    //double dz_dS3 = approximatePartialDerivative(zPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 4);
    // ... similarly compute other derivatives

 //// Create a 5x3 matrix
    Eigen::MatrixXd partials(3, 5);
    partials << dx_dS1, dx_dJ1, dx_dJ2, dx_dJ3, dx_dS3,
          dy_dS1, dy_dJ1, dy_dJ2, dy_dJ3, dy_dS3, 
          0, dz_dJ1, dz_dJ2, dz_dJ3, 0;

    Eigen::MatrixXd desiredxyz(3, 1);
    desiredxyz << desiredx,
                  desiredy,
                  desiredz;

    Eigen::MatrixXd currentxyz(3, 1);
    currentxyz << currentx,
                  currenty,
                  currentz;

    Eigen::MatrixXd b = desiredxyz - currentxyz;

    // Compute the SVD
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(partials, Eigen::ComputeFullU | Eigen::ComputeFullV);

    Eigen::MatrixXd delta_angles = svd.solve(b);

    // Define a tolerance for the convergence
    double tolerance = 0.1; // Example value, adjust as needed

// while ((desiredxyz - currentxyz).norm() > tolerance) {

    while (std::abs(desiredxyz(0,0) - currentxyz(0,0)) > tolerance || std::abs(desiredxyz(1,0) - currentxyz(1,0)) > tolerance || std::abs(desiredxyz(2,0) - currentxyz(2,0)) > tolerance) {
    
    /*
        S1angle += delta_angles(0, 0); // Access the first element
        J1angle += delta_angles(1, 0); // Access the second element
        J2angle += delta_angles(2, 0); // Access the third element
        J3angle += delta_angles(3, 0); // Access the fourth element
        S3angle += delta_angles(4, 0); // Access the fifth element
    */

    
        // Update and limit each joint angle
        S1angle = std::min(std::max(S1angle + delta_angles(0, 0), minS1angle), maxS1angle);
        J1angle = std::min(std::max(J1angle + delta_angles(1, 0), minJ1angle), maxJ1angle);
        J2angle = std::min(std::max(J2angle + delta_angles(2, 0), minJ2angle), maxJ2angle);
        J3angle = std::min(std::max(J3angle + delta_angles(3, 0), minJ3angle), maxJ3angle);
        S3angle = std::min(std::max(S3angle + delta_angles(4, 0), minS3angle), maxS3angle);



      //  std::cout << "Currentxyz " << currentxyz << std::endl;
/*
        std::cout << "S1: " << fmod(S1angle, 360.0) 
                << "\nJ1: " << fmod(J1angle, 360.0) 
                << "\nJ2: " << fmod(J2angle, 360.0) 
                << "\nJ3: " << fmod(J3angle, 360.0) 
                << "\nS3: " << fmod(S3angle, 360.0) 
                << std::endl;
*/
        currentxyz(0, 0) = xPosition(S1angle, J1angle, J2angle, J3angle, S3angle);
        currentxyz(1,0) = yPosition(S1angle, J1angle, J2angle, J3angle, S3angle);
        currentxyz(2,0) = zPosition(S1angle, J1angle, J2angle, J3angle, S3angle);


      /*  
        std::cout << "S1: " << S1angle 
                << "\nJ1: " << J1angle 
                << "\nJ2: " << J2angle 
                << "\nJ3: " << J3angle 
                << "\nS3: " << S3angle 
                << std::endl;

        std::cout << currentxyz << std::endl;
    */

       currentkp = KinematicPoint(S1angle, J1angle, J2angle, J3angle, S3angle, currentxyz(0,0), currentxyz(1,0), currentxyz(2,0));
      // std::cout << currentkp << std::endl;
     //  std::cout << std::abs(desiredxyz(0,0) - currentxyz(0,0)) << " " << std::abs(desiredxyz(1,0) - currentxyz(1,0)) << " " << std::abs(desiredxyz(0,0) - currentxyz(0,0)) << std::endl;

/*
        std::cout << "X: " << xPosition(fmod(S1angle, 360.0), fmod(J1angle, 360.0), fmod(J2angle, 360.0), fmod(J3angle, 360.0), fmod(S3angle, 360.0)) 
          << " Y: " << yPosition(fmod(S1angle, 360.0), fmod(J1angle, 360.0), fmod(J2angle, 360.0), fmod(J3angle, 360.0), fmod(S3angle, 360.0)) 
          << " Z: " << zPosition(fmod(S1angle, 360.0), fmod(J1angle, 360.0), fmod(J2angle, 360.0), fmod(J3angle, 360.0), fmod(S3angle, 360.0)) 
          << std::endl;
          */


        // Compute partial derivatives for x
        dx_dS1 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 0);
        dx_dJ1 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 1);
        dx_dJ2 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 2);
        dx_dJ3 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 3);
        dx_dS3 = approximatePartialDerivative(xPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 4);

        dy_dS1 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 0);
        dy_dJ1 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 1);
        dy_dJ2 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 2);
        dy_dJ3 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 3);
        dy_dS3 = approximatePartialDerivative(yPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 4);

    // double dz_dS1 = approximatePartialDerivative(zPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 0);
        dz_dJ1 = approximatePartialDerivative(zPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 1);
        dz_dJ2 = approximatePartialDerivative(zPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 2);
        dz_dJ3 = approximatePartialDerivative(zPosition, S1angle, J1angle, J2angle, J3angle, S3angle, 3);

        partials << dx_dS1, dx_dJ1, dx_dJ2, dx_dJ3, dx_dS3,
            dy_dS1, dy_dJ1, dy_dJ2, dy_dJ3, dy_dS3, 
            0, dz_dJ1, dz_dJ2, dz_dJ3, 0;

        double lambda = 0.0001; // Example damping factor, tune as needed. Higher number more stability, less precision. Lower number more precision, less stability

        // Update joint angles with the computed changes
        // This step depends on how your system's angles are represented
        // Example: S1angle += delta_angles(0, 0), etc.

        // Recompute the current position based on new joint angles using forward kinematics
        // currentxyz << new x, new y, new z positions based on updated angles

        // Recompute the Jacobian matrix with updated joint angles
        // Update partials matrix here

        // Compute the damped pseudo-inverse
        Eigen::MatrixXd I = Eigen::MatrixXd::Identity(partials.rows(), partials.rows());
        Eigen::MatrixXd dampedInverse = partials.transpose() * (partials * partials.transpose() + lambda * lambda * I).inverse();

        // Update b with the new current position
        b = desiredxyz - currentxyz;

        // Solve for the new delta_angles using the damped pseudo-inverse
        delta_angles = dampedInverse * b;

    }

    return currentkp;



}

int main() 
{

    auto start = std::chrono::high_resolution_clock::now();

    double currentx = 2, currenty = 0, currentz = 2;
    double finalx = 1, finaly = 1, finalz = 1;
    double S1angle = 0, J1angle = 11, J2angle =44, J3angle = 27, S3angle = 0;

    KinematicPoint currentkp = KinematicPoint(S1angle, J1angle, J2angle, J3angle, S3angle, currentx, currenty, currentz);

    int numofpointsAtoB = 10; // Custom length for the vector
    std::vector<KinematicPoint> pointsbetweenAtoB;
    std::cout << getKinematicPointAtoB(currentkp, 1.63636, 0, 1.63636) << std::endl;

      // Loop to generate and store the points
    for (int i = 1; i <= numofpointsAtoB + 1; i++) 
    {
        
        double t = i / (double)(numofpointsAtoB + 1); // Calculate the fraction

        


        double desiredx = currentkp.getX() + t * (finalx - currentkp.getX());
        double desiredy = currentkp.getY() + t * (finaly - currentkp.getY());
        double desiredz = currentkp.getZ() + t * (finalz - currentkp.getZ());

        //Round to the milimeter
        desiredx = std::round(desiredx * 10.0) / 10.0;
        desiredy = std::round(desiredy * 10.0) / 10.0;
        desiredz = std::round(desiredz * 10.0) / 10.0;

        std::cout << desiredx << std::endl;        
       std::cout << desiredy << std::endl;        
        std::cout << desiredz << std::endl;        

        KinematicPoint resultkp = getKinematicPointAtoB(currentkp, desiredx, desiredy, desiredz);        

      std::cout << resultkp << std::endl;

     //   std::cout << "i: " << i << std::endl;

        pointsbetweenAtoB.push_back(resultkp);
    }



     // Get the ending timepoint
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    // Output the duration
    std::cout << "Time taken by function: " << duration.count() << " milliseconds" << std::endl;








   
    return 0;
}
