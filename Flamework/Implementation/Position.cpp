//
//  Position.cpp
//  Flamework
//
//  Created by Bernard Schroffenegger on 20.05.15.
//
//

#include "Position.h"
#include "addendum.hpp"


Position::Position()
: _position(vmml::vec3f(0,0,0)) {
}

Position::Position(vmml::vec3f position) {
    _position = position;
}

Position::~Position() {
}

void Position::setPosition(vmml::vec3f position) {
    _position = position;
}

vmml::vec3f Position::getPosition() {
    return _position;
}

vmml::mat4f Position::getModelMatrix(float originalObjectSize) {
    return vmml::create_translation(_position)
    * vmml::create_scaling(1/100.f * _position.z() * originalObjectSize);
}

float Position::x() {
    return _position.x();
}

float Position::y() {
    return _position.y();
}

float Position::z() {
    return _position.z();
}


bool Position::isValid() {
    if (_position.z() > 110) return false;
    else return true;
}
