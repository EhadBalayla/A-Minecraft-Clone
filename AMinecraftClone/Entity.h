#pragma once
#include "AABB.h"
#include "glm/glm.hpp"
#include "Block.h"
#include <vector>

class Entity
{
public:

	AABB aabb;

	Entity();

	virtual void Update(float DeltaTime) = 0;

	void SetPosition(float x, float y, float z);
	void SetRotation(float y, float p);

	glm::dvec3 GetPosition();
	glm::vec2 GetRotation();

	glm::vec3 getForwardVector(); //returns the forward vector of the yaw rotation
	glm::vec3 getRightVector(); //returns the right vector of the yaw rotation
	glm::vec3 getLookVector(); //returns the look vector

protected:

	//transform
	glm::dvec3 position = glm::dvec3(0.0f); //position
	float yaw = 0.0f, pitch = 0.0f; //rotation

	//is what used for the velocity calculations
	glm::vec3 velocity = glm::vec3(0.0f);

	void Gravity(float DeltaTime);
	void MoveAndCollide(float DeltaTime);

	bool IsOnGround = false;

	int entityHeight = 1; //used to determine how tall an entity is
	float acceleration = 0.001f; //the acceleration of the entity's movement
	float maxMovementSpeed = 0.1; //the max movement speed of the entity
	float friction = 0.01;
};

