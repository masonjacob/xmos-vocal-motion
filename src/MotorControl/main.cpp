#include <iostream>
#include <vector>
#include <cmath>  // Include the cmath library to use sqrt
#include <ostream>
#include <cstdlib> // Required for system()
#include "KinematicPoint.h"

//global variables
const int minJ2angle = -90;
const int maxJ2angle = 90;
const int L2length = 1;
const int minJ3angle = -90;
const int maxJ3angle = 90;
const int L3length = 1;
const int minJ4angle = -90;
const int maxJ4angle = 90;
const int L4length = 1;
const int J2step = 1;
const int J3step = 1;
const int J4step = 1;

//Weights for generating the nexxt set of angles. We generally want J2 to move as little as possible, so we make its weight bigger
//than the other weights. This way it contributes more towards our dist function later which will make movement of J2 more costly than
//movement of J3 or J4
double J2stepweight = 1;
double J3stepweight = 0.5;
double J4stepweight = 0.1;

//Generate starting Kpoint for our initial position, which will be 0 for all the angles and (0,0) for the x, y positon
KinematicPoint currentkpoint = KinematicPoint(0, 0, 0, 3, 0);
double xsensitivity = 0.01;
double ysensitivity = 0.01;

double maxX = L2length + L3length + L4length;
double maxY = maxX;
double minX = 0;
double minY = 0;

double newx = 0;
double newy = 0;

// Calculate the number of steps for each angle
int J2numsteps = (maxJ2angle - minJ2angle)/ J2step + 1;
int J3numsteps = (maxJ3angle - minJ3angle)/ J3step + 1;
int J4numsteps = (maxJ3angle - minJ3angle)/ J4step + 1;

std::vector<std::vector<std::vector<KinematicPoint>>> kpointsXYplane;

KinematicPoint GenerateNewKPoint(const KinematicPoint& currentKPoint, double newx, double newy);


// Define a constant for pi.
const double PI = 3.14159265358979323846;

// Function to convert degrees to radians
double degreesToRadians(double degrees) {
    return degrees * (PI / 180.0);
}

//This method will take in ur current angle positions and your new (x,y) position and give us back the kpoint with angles that will get
//us to our new (x,y) with the least amount of angle movement as possible
KinematicPoint GenerateNewKPoint(const KinematicPoint& currentKPoint, double newx, double newy) 
{
    KinematicPoint mindistKPoint = KinematicPoint();
    bool validnewpoint = false;

    //Loop through our kpoints using our current kpoint and find the minimum distance angles to create our new (x,y). That will the new angles we set the motors to
           int rowpos = std::round((newx + maxX)/xsensitivity);
           int colpos = std::round((newy + maxY)/ysensitivity);
            std::vector<KinematicPoint> pointstocheck = kpointsXYplane[rowpos][colpos];
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
                    mindistKPoint.setX(newx);
                    mindistKPoint.setY(newy);
                    mindist = J2stepweight*(fabs(kptocheck.getAngle1() - currentkpoint.getAngle1())) + J3stepweight*(fabs(kptocheck.getAngle2() - currentkpoint.getAngle2())) + J4stepweight*(fabs(kptocheck.getAngle3() - currentkpoint.getAngle3()));
                    validnewpoint = true;
                }
                else
                {
                    //Peform distance calculations and check them against our current running mindistkpoint
                    double tocheckdist = J2stepweight*(fabs(kptocheck.getAngle1() - currentkpoint.getAngle1())) + J3stepweight*(fabs(kptocheck.getAngle2() - currentkpoint.getAngle2())) + J4stepweight*(fabs(kptocheck.getAngle3() - currentkpoint.getAngle3()));
                    if(tocheckdist < mindist)
                    {
                        mindist = tocheckdist; //Update our running mindist
                        mindistKPoint.setAngle1(kptocheck.getAngle1()); //Update the mindistkpoint
                        mindistKPoint.setAngle2(kptocheck.getAngle2());
                        mindistKPoint.setAngle3(kptocheck.getAngle3());
                    }
                    
                
                }
            
                
                
            }
            return mindistKPoint;
    
}

/*
//This method will take in ur current angle positions and your new (x,y) position and give us back the kpoint with angles that will get
//us to our new (x,y) with the least amount of angle movement as possible
KinematicPoint GenerateNewKPoint(const KinematicPoint& currentKPoint, double newx, double newy) 
{

    KinematicPoint mindistKPoint = KinematicPoint();
    bool validnewpoint = false;

    //Loop through our kpoints using our current kpoint and find the minimum distance angles to create our new (x,y). That will the new angles we set the motors to
    for (int i = 0; i < J2numsteps; ++i) {
        for (int j = 0; j < J3numsteps; ++j) {
            for (int k = 0; k < J4numsteps; ++k) {
                
                KinematicPoint kptocheck = kpointGrid[i][j][k];
                double mindist;
                
                //Our generated x and y might be ever so slightly off so it makes sense to have some sensitivity
                if(fabs(kptocheck.getX() -newx) < xsensitivity && fabs(kptocheck.getY() - newy) < ysensitivity)
                {
                    //For the very first min found. Also doubles for telling us if a point is achievable
                    if(validnewpoint == false)
                    {
                        mindistKPoint.setAngle1(kptocheck.getAngle1());
                        mindistKPoint.setAngle2(kptocheck.getAngle2());
                        mindistKPoint.setAngle3(kptocheck.getAngle3());
                        mindistKPoint.setX(newx);
                        mindistKPoint.setY(newy);
                        mindist = J2stepweight*(fabs(kptocheck.getAngle1() - currentkpoint.getAngle1())) + J3stepweight*(fabs(kptocheck.getAngle2() - currentkpoint.getAngle2())) + J4stepweight*(fabs(kptocheck.getAngle3() - currentkpoint.getAngle3()));
                    }
                    else
                    {
                        //Peform distance calculations and check them against our current running mindistkpoint
                        double tocheckdist = J2stepweight*(fabs(kptocheck.getAngle1() - currentkpoint.getAngle1())) + J3stepweight*(fabs(kptocheck.getAngle2() - currentkpoint.getAngle2())) + J4stepweight*(fabs(kptocheck.getAngle3() - currentkpoint.getAngle3()));

                        if(tocheckdist < mindist)
                        {
                            mindist = tocheckdist; //Update our running mindist
                            mindistKPoint.setAngle1(kptocheck.getAngle1()); //Update the mindistkpoint
                            mindistKPoint.setAngle2(kptocheck.getAngle2());
                            mindistKPoint.setAngle3(kptocheck.getAngle3());
                        }
                        
                    
                    }
            
                }
                
            }
        }
    }
    
        return mindistKPoint;
    
}
*/


int main() 
{


    // Resize the outer two dimensions and initially each vector in the third dimension is empty
    int rows = 2 * std::round((maxX - minX)/xsensitivity) + 1; //We multiply by 2 to account for negative nums and + 1 to account for 0
    int columns = 2 * std::round((maxY - minY)/ysensitivity) + 1;

    //Rows will be as many possible X values within the sensitivity. Columns will be as many possible Y values.
    //This way we will be able to index a (x,y) position and then only loop through the kpoints there that have the correct (x,y) that were looking for
    //Later to index each position, we take the x and divide by the xsensitivity to get the index in our 3D array of the (x,y) position

    kpointsXYplane.resize(rows, std::vector<std::vector<KinematicPoint>>(columns)); 
    // Initialize KinematicPoints with all combinations of angles
    for (int J2ang = minJ2angle; J2ang < maxJ2angle; J2ang += J2step) 
    {
        for (int J3ang = minJ3angle; J3ang < maxJ3angle; J3ang += J3step) 
        {
            for (int J4ang = minJ4angle; J4ang < maxJ4angle; J4ang+= J4step) 
            {
                double J2angle = J2ang * J2step;
                double J3angle = J3ang * J3step;
                double J4angle = J4ang * J4step;

                // We will calculate every the x, y position with every angle possiblity
                double x = L2length*cos(degreesToRadians(J2angle)) + L3length*cos(degreesToRadians(J2angle + J3angle)) + L4length*cos(degreesToRadians(J2angle + J3angle + J4angle)); 
                double y = L2length*sin(degreesToRadians(J2angle)) + L3length*sin(degreesToRadians(J2angle + J3angle)) + L4length*sin(degreesToRadians(J2angle + J3angle + J4angle)); 

                //std::cout << "J2: " << J2angle << " J3: " << J3angle << " J4: " << J4angle << "\nx: " << x << " y: " << y << std::endl;

                //std::cout << "i: " << i << "\nj: " << j << "\nk: " << k << std::endl;

                int rowpos = std::round((x + maxX)/xsensitivity);
                int colpos = std::round((y + maxY)/ysensitivity);
              //  std::cout<< rowpos << " " << colpos << " " << KinematicPoint(J2angle, J3angle, J4angle, x, y) << std::endl;
                kpointsXYplane[rowpos][colpos].push_back(KinematicPoint(J2angle, J3angle, J4angle, x, y));


                //kpointGrid[i][j][k] = KinematicPoint(J2angle, J3angle, J4angle, x, y);
                //std::cout << kpointGrid[i][j][k] <<std::endl;
                //system("pause");
            }
        }
    }

    newx = -1;
    newy = 1;

          std::vector<KinematicPoint> points = kpointsXYplane[(newx+maxX)/xsensitivity][(newy + maxY)/ysensitivity];

        for(int a = 0; a < points.size(); a++)
        {
            KinematicPoint point = points[a];
            std::cout << point << " " << J2stepweight*(fabs(point.getAngle1() - currentkpoint.getAngle1())) + J3stepweight*(fabs(point.getAngle2() - currentkpoint.getAngle2())) + J4stepweight*(fabs(point.getAngle3() - currentkpoint.getAngle3())) << std::endl;
        }   

        currentkpoint = GenerateNewKPoint(currentkpoint, newx, newy);

        if(currentkpoint.getX() == newx && currentkpoint.getY() == newy) //We found a proper angle orientation to get to (x, y)
            std::cout << currentkpoint << std::endl;
        else
            std::cout << "Cannot produce x,y position";

    return 0;
}
