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
const int minS1angle = -179;
const int maxS1angle = 180;

const int minJ1angle = -45;
const int maxJ1angle = 45;
const int L1length = 1;

const int minJ2angle = -45;
const int maxJ2angle = 45;
const int L2length = 1;

const int minJ3angle = -45;
const int maxJ3angle = 45;
const int L3length = 1;

const int minS3angle = 0;
const int maxS3angle = 1;

const int S1step = 1;
const int J1step = 1;
const int J2step = 1;
const int J3step = 1;
const int S3step = 1;

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

//This method will take in ur current angle positions and your new (x,y) position and give us back the kpoint with angles that will get
//us to our new (x,y) with the least amount of angle movement as possible
KinematicPoint GenerateNewKPoint(const KinematicPoint& currentKPoint, double newx, double newy, double newz) 
{
    KinematicPoint mindistKPoint = KinematicPoint();
    bool validnewpoint = false;

    //Loop through our kpoints using our current kpoint and find the minimum distance angles to create our new (x,y). That will the new angles we set the motors to
           int xpos = std::round((newx + maxX));
           int ypos = std::round((newy + maxY));
           int zpos = std::round((newz + maxZ));
           std::cout << xpos << " " << ypos << " " << zpos << std::endl;
            std::vector<KinematicPoint> pointstocheck = kpointsXYZplane[xpos][ypos][zpos];
            for (int k = 0; k < pointstocheck.size(); ++k) 
            {
                KinematicPoint kptocheck = pointstocheck[k];
                std::cout << kptocheck << std::endl;
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


int main() 
{

    auto start = std::chrono::high_resolution_clock::now();

   








    // Resize the outer two dimensions and initially each vector in the third dimension is empty
    /*
    int xpositions = 2 * std::round((maxX - minX)/xsensitivity) + 1; //We multiply by 2 to account for negative nums and + 1 to account for 0
    int ypositions = 2 * std::round((maxY - minY)/ysensitivity) + 1;
    int zpositions = 2 * std::round((maxZ -minZ)/zsensitivity) + 1;
    */

    int xpositions = 7;
    int ypositions = 7;
    int zpositions = 7;

    //Rows will be as many possible X values within the sensitivity. Columns will be as many possible Y values.
    //This way we will be able to index a (x,y) position and then only loop through the kpoints there that have the correct (x,y) that were looking for
    //Later to index each position, we take the x and divide by the xsensitivity to get the index in our 3D array of the (x,y) position

     // Resize x dimension
    kpointsXYZplane.resize(xpositions);

    // Resize y dimension for each x
    for (int x = 0; x < xpositions; ++x) {
        kpointsXYZplane[x].resize(ypositions);

        // Resize z dimension for each y at current x
        for (int y = 0; y < ypositions; ++y) {
            kpointsXYZplane[x][y].resize(zpositions); // Initialize KinematicPoints for each z at current x,y
        }
    }

     //auto start = std::chrono::high_resolution_clock::now();

    // Initialize KinematicPoints with all combinations of angles
    for (int S1angle = minS1angle; S1angle < maxS1angle; S1angle += S1step) 
    {
    for(int J1angle = minJ1angle; J1angle < maxJ1angle; J1angle += J1step)
    {
    for (int J2angle = minJ2angle; J2angle < maxJ2angle; J2angle += J2step) 
    {
    for (int J3angle = minJ3angle; J3angle < maxJ3angle; J3angle+= J3step) 
    {
    for (int S3angle = minS3angle; S3angle < maxS3angle; S3angle += S3step)
    {
        


        // We will calculate every the x, y position with every angle possiblity
        double x = cos(degreesToRadians(S1angle)) *
           (
             L1length * sin(degreesToRadians(J1angle)) + 
             L2length * sin(degreesToRadians(J1angle + J2angle))
           ) + 
           cos(degreesToRadians(S1angle + S3angle)) *
           L3length * sin(degreesToRadians(J1angle + J2angle + J3angle));
            
        double y = 
        sin(degreesToRadians(S1angle))*
        (L1length*sin(degreesToRadians(J1angle)) + 
        L2length*sin(degreesToRadians(J1angle + J2angle)) 
        )
        + sin(degreesToRadians(S1angle + S3angle))*L3length*sin(degreesToRadians(J1angle + J2angle + J3angle)); 
        double z = L1length*sin(degreesToRadians(J1angle)) + L2length*sin(degreesToRadians(J1angle + J2angle)) + L3length*sin(degreesToRadians(J1angle + J2angle + J3angle));

    
    //    int xpos = std::round((x + maxX)/xsensitivity);
     //   int ypos = std::round((y + maxY)/ysensitivity);
     //   int zpos = std::round((z + maxZ)/zsensitivity);

        double xpos = (x + maxX);
        double ypos = (y + maxY);
        double zpos = (z + maxZ);

        double epsilon = 0.1; // Define your tolerance

        if (std::fabs((xpos) - static_cast<int>(xpos)) < epsilon && std::fabs((ypos) - static_cast<int>(ypos)) < epsilon && std::fabs((zpos) - static_cast<int>(zpos)) < epsilon) {
            // It's approximately a whole number
           // std::cout << "here" << std::round(xpos) << " " << std::round(ypos) << " " << std::round(zpos);
            if(kpointsXYZplane[std::round(xpos)][std::round(ypos)][std::round(zpos)].size() <= 5)
            {
                kpointsXYZplane[std::round(xpos)][std::round(ypos)][std::round(zpos)].push_back(KinematicPoint(S1angle, J1angle, J2angle, J3angle, S3angle, x, y, z));
            }
        }

        /*
        if(fabs(x-1) < 0.1 && fabs(y-0) < 0.1 && fabs(z-1) < 0.1)
        {
        std::cout << KinematicPoint(S1angle, J1angle, J2angle, J3angle, S3angle, x, y, z) << std::endl;
        std::cout << std::round(x/xsensitivity) << " " << std::round(y/ysensitivity) << " " << std::round(z/zsensitivity) << std::endl;
        system("pause");
        }
        */

        //std::cout << kpointGrid[i][j][k] <<std::endl;
        //system("pause");
    }
    }
    }
    }
    std::cout << S1angle << std::endl;
    }

    // Get the ending timepoint
 //   auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the duration
   // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    // Output the duration
    //std::cout << "Time taken by function: " << duration.count() << " milliseconds" << std::endl;




      size_t xyz_storage = calculateStorage(kpointsXYZplane);
    std::cout << "Approximate storage for kpointsXYZplane: " << xyz_storage << " bytes\n";
    
/*
    int x_size = 7; // Size along the x dimension
    int y_size = 7; // Size along the y dimension
    int z_size = 7; // Size along the z dimension


    // Initialize the 3D vector
    std::vector<std::vector<std::vector<bool>>> vec3D(
        x_size, std::vector<std::vector<bool>>(
            y_size, std::vector<bool>(z_size, false)
        )
    );
    */
 /*  
    for(int i = -30; i <= 30; i += 10)
    {
    for(int j = -30; j <= 30; j+=10)
    {
    for(int k = -30; k <= 30; k+=10)
    {
        if(kpointsXYZplane[i+30][j+30][k+30].size() > 0)
            std::cout << "X: " << (i )/10 << " Y: " << (j)/10 << " Z: " << (k)/10 << std::endl;
           // vec3D[i/10][j/10][k/10] = true;
    }    
    }
    }
*/
    int count = 0;

    for(int i = -3; i <= 3; i += 1)
    {
    for(int j = -3; j <= 3; j+=1)
    {
    for(int k = -3; k <= 3; k+=1)
    {

        if(kpointsXYZplane[i+3][j+3][k+3].size() > 0)
        {
            std::cout << kpointsXYZplane[i+3][j+3][k+3][0] << std::endl;
            std::cout << "X: " << i << " Y: " << j << " Z: " << k << std::endl;
            count += 1;
        }

    }    
    }
    }

    std::cout << "count " << count << std::endl;

/*
   int layer_number = 0;
    for (const auto& layer : kpointsXYZplane) {
        std::cout << "Layer " << layer_number << ":\n";
        for (const auto& row : layer) {
            for (bool val : row) {
                std::cout << (val ? "1" : "0") << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        ++layer_number;
    }
 */   
   
    newx = 2;
    newy = 2;
    newz = 2;

/*
    std::vector<KinematicPoint> points = kpointsXYZplane[(newx+maxX)/xsensitivity][(newy + maxY)/ysensitivity][(newz+maxZ)/zsensitivity];

        for(int a = 0; a < points.size(); a++)
        {
            KinematicPoint point = points[a];
            double tocheckdist = S1stepweight*(fabs(point.getAngle1() - currentkpoint.getAngle1())) + J1stepweight*(fabs(point.getAngle2() - currentkpoint.getAngle2())) + J2stepweight*(fabs(point.getAngle3() - currentkpoint.getAngle3())) + J3stepweight*(fabs(point.getAngle4() - currentkpoint.getAngle4())) +S3stepweight*(fabs(point.getAngle5() - currentkpoint.getAngle5()));

            std::cout << point << " " << tocheckdist << std::endl;
        }   
 */


        currentkpoint = GenerateNewKPoint(currentkpoint, newx, newy, newz);


        if(currentkpoint.getX() == newx && currentkpoint.getY() == newy && currentkpoint.getZ() == newz) //We found a proper angle orientation to get to (x, y)
            std::cout << "Success!" << std::endl << currentkpoint << std::endl;
        else
            std::cout << "Cannot produce x,y position";

    

    return 0;
}
