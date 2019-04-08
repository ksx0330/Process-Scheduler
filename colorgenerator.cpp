#include "colorgenerator.h"

ColorGenerator::ColorGenerator()
{

}

QColor ColorGenerator::getColor(int i) {
    QColor color;
    color = QColor::fromHslF(currentHue, 1.0, 0.5);
    currentHue += 0.618033988749895f;
    currentHue = std::fmod(currentHue, 1.0f);

    return color;
}

void ColorGenerator::clear() {
    currentHue = 0.0;
}
