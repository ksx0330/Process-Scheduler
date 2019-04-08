#ifndef COLORGENERATOR_H
#define COLORGENERATOR_H

#include <QColor>
#include <cmath>

class ColorGenerator
{
private:
    float currentHue = 0.0;

public:
    ColorGenerator();
    QColor getColor(int i);

    void clear();
};

#endif // COLORGENERATOR_H
