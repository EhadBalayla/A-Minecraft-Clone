#pragma once
#include "AABB.h"
#include "glm/glm.hpp"
#include "Block.h"
#include <vector>

class Entity
{
public:
	virtual void Update(float DeltaTime) = 0;

	void SetPosition(float x, float y, float z);
	void SetRotation(float y, float p);

	glm::dvec3 GetPosition();
	glm::vec2 GetRotation();

	glm::vec3 getForwardVector();
	glm::vec3 getRightVector();
	glm::vec3 getLookVector();

	AABB& GetCollision();
protected:
	AABB aabb;
	int entityHeight = 1;

	glm::dvec3 position = glm::dvec3(0.0f);
	float yaw = 0.0f, pitch = 0.0f;

	glm::dvec3 velocity = glm::vec3(0.0f);

	void MoveAndCollide(float DeltaTime);

	bool IsOnGround = false;

	float acceleration = 0.5f;
	double maxMovementSpeed = 5.2f;
	double friction = 0.8f;
	float gravity = 15.0f;
};

