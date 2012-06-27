/*
 * ShapeProbability.cpp
 *
 *  Created on: Jun 28, 2011
 *      Author: hendrik
 */

#include "ShapeProbability.h"

ShapeProbability::ShapeProbability() {
	// TODO Auto-generated constructor stub

}

ShapeProbability::~ShapeProbability() {
	// TODO Auto-generated destructor stub
}

float ShapeProbability::getProbability() const
{
    return probability;
}

btCollisionShape *ShapeProbability::getShape() const
{
    return shape;
}

void ShapeProbability::setProbability(float probability)
{
    this->probability = probability;
}

void ShapeProbability::setShape(btCollisionShape *shape)
{
    this->shape = shape;
}

float ShapeProbability::getProbBad() const
{
    return probBad;
}

float ShapeProbability::getProbGood() const
{
    return probGood;
}

float ShapeProbability::getProbReflect() const
{
    return probReflect;
}

void ShapeProbability::setProbBad(float probBad)
{
    this->probBad = probBad;
}

void ShapeProbability::setProbGood(float probGood)
{
    this->probGood = probGood;
}

void ShapeProbability::setProbReflect(float probReflect)
{
    this->probReflect = probReflect;
}




