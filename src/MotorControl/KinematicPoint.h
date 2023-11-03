// KinematicPoint.h
#ifndef KINEMATICPOINT_H
#define KINEMATICPOINT_H
#include <ostream>

class KinematicPoint {
private:
    double angle1;
    double angle2;
    double angle3;
    double x;
    double y;

public:
    // Constructor
 // Default constructor
    KinematicPoint() : angle1(0), angle2(0), angle3(0), x(0), y(0) {}

    KinematicPoint(double a1, double a2, double a3, double xCoord, double yCoord);

    // Accessors (getters)
    double getAngle1() const;
    double getAngle2() const;
    double getAngle3() const;
    double getX() const;
    double getY() const;

    // Mutators (setters)
    void setAngle1(double a1);
    void setAngle2(double a2);
    void setAngle3(double a3);
    void setX(double xCoord);
    void setY(double yCoord);

    // Overload the stream insertion operator declaration
    friend std::ostream& operator<<(std::ostream& os, const KinematicPoint& kp);
};

#endif // KINEMATICPOINT_H