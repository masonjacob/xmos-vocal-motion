// KinematicPoint.cpp
#include "KinematicPoint.h"
#include <iostream>
#include <ostream>

// Constructor implementation
KinematicPoint::KinematicPoint(double a1, double a2, double a3, double xCoord, double yCoord)
    : angle1(a1), angle2(a2), angle3(a3), x(xCoord), y(yCoord) {}

// Getters implementation
double KinematicPoint::getAngle1() const { return angle1; }
double KinematicPoint::getAngle2() const { return angle2; }
double KinematicPoint::getAngle3() const { return angle3; }
double KinematicPoint::getX() const { return x; }
double KinematicPoint::getY() const { return y; }

// Setters implementation
void KinematicPoint::setAngle1(double a1) { angle1 = a1; }
void KinematicPoint::setAngle2(double a2) { angle2 = a2; }
void KinematicPoint::setAngle3(double a3) { angle3 = a3; }
void KinematicPoint::setX(double xCoord) { x = xCoord; }
void KinematicPoint::setY(double yCoord) { y = yCoord; }

std::ostream& operator<<(std::ostream& os, const KinematicPoint& kp) {
    return os << "Angle1: " << kp.getAngle1()
              << ", Angle2: " << kp.getAngle2()
              << ", Angle3: " << kp.getAngle3()
              << ", X: " << kp.getX()
              << ", Y: " << kp.getY();
}