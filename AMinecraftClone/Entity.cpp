#include "Entity.h"
#include "Utilities.h"
#include "Game.h"

Entity::Entity() {

}

void Entity::SetPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

void Entity::SetRotation(float y, float p) {
	yaw = y;
	pitch = p;
}

glm::vec3 Entity::getForwardVector() {
	float yawRadians = glm::radians(yaw);
	glm::vec3 forward;
	forward.x = cos(yawRadians);
	forward.y = 0.0f;
	forward.z = sin(yawRadians);
	return forward;
}

glm::vec3 Entity::getRightVector() {
	return glm::normalize(glm::cross(getForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Entity::getLookVector() {
	float yawRad = glm::radians(yaw);
	float pitchRad = glm::radians(pitch);

	glm::vec3 forward;
	forward.x = cos(pitchRad) * cos(yawRad);
	forward.y = sin(pitchRad);
	forward.z = cos(pitchRad) * sin(yawRad);

	return glm::normalize(forward);
}


glm::dvec3 Entity::GetPosition() {
	return position;
}

glm::vec2 Entity::GetRotation() {
	return glm::vec2(yaw, pitch);
}

void Entity::Gravity(float DeltaTime) {
	Ray groundDetectionRay = Utils::shootRay(position, glm::vec3(0.0, -1.0, 0.0), 1.0);
	if (Utils::RayHitBlock(groundDetectionRay).HitBlock == nullptr)
	{
		if (IsOnGround) {
			IsOnGround = false;
			std::cout << "just left ground right now" << std::endl;
		}
	}
	else
	{
		if (!IsOnGround) {
			IsOnGround = true;
			std::cout << "just hit ground right now" << std::endl;
			velocity.y = 0.0;
		}
	}
}

void Entity::MoveAndCollide(float DeltaTime) {
	
	//collision detection
	Ray ray;
	for (int i = 0; i < entityHeight; i++) {
		//detection on the positive X
		ray = Utils::shootRay(position + glm::dvec3(0.0, i, 0.0), glm::vec3(1.0, 0.0, 0.0), 0.25);
		if (Utils::RayHitBlock(ray).HitBlock)
			velocity.x = std::min(velocity.x, 0.0f);

		//detection on the negative X
		ray = Utils::shootRay(position + glm::dvec3(0.0, i, 0.0), glm::vec3(-1.0, 0.0, 0.0), 0.25);
		if (Utils::RayHitBlock(ray).HitBlock)
			velocity.x = std::max(velocity.x, 0.0f);

		//detection on the positive Z
		ray = Utils::shootRay(position + glm::dvec3(0.0, i, 0.0), glm::vec3(0.0, 0.0, 1.0), 0.25);
		if (Utils::RayHitBlock(ray).HitBlock)
			velocity.z = std::min(velocity.z, 0.0f);

		//detection on the positive Z
		ray = Utils::shootRay(position + glm::dvec3(0.0, i, 0.0), glm::vec3(0.0, 0.0, -1.0), 0.25);
		if (Utils::RayHitBlock(ray).HitBlock)
			velocity.z = std::max(velocity.z, 0.0f);
	}

	//detection on the positive Y
	ray = Utils::shootRay(position + glm::dvec3(0.0, entityHeight - 1, 0.0), glm::vec3(0.0, 1.0, 0.0), 0.8); //we want to give an entity a little bit of crawling space
	if (Utils::RayHitBlock(ray).HitBlock)
		velocity.y = std::max(velocity.y, 0.0f);

	//detection on the negative Y
	Ray downRay = Utils::shootRay(position, glm::vec3(0.0, -1.0, 0.0), 1.0);
	if (Utils::RayHitBlock(downRay).HitBlock)
	{
		if (!IsOnGround) {
			IsOnGround = true;
			velocity.y = 0.0;
		}
	}
	else
	{
		velocity += glm::vec3(0.0, -0.00025 * DeltaTime, 0.0);
		if (IsOnGround) {
			IsOnGround = false;
		}
	}



	//clamp horizontal movement
	glm::vec3 temporalClampedVelocity = glm::normalize(velocity) * maxMovementSpeed; //normalized velocity only on the x and z axes not on the y
	if(velocity.x > 0)
		velocity.x = std::min(velocity.x, temporalClampedVelocity.x);
	else if(velocity.x < 0)
		velocity.x = std::max(velocity.x, temporalClampedVelocity.x);
	
	if (velocity.z > 0)
		velocity.z = std::min(velocity.z, temporalClampedVelocity.z);
	else if (velocity.z < 0)
		velocity.z = std::max(velocity.z, temporalClampedVelocity.z);


	//apply friction
	glm::vec3 temporalFrictionForce = glm::normalize(velocity) * friction;
	if (velocity.x != 0)
		if (std::abs(velocity.x) - std::abs(temporalFrictionForce.x) >= 0)
			velocity.x -= temporalFrictionForce.x;
		else if (std::abs(velocity.x) - std::abs(temporalFrictionForce.x) < 0)
			velocity.x -= temporalFrictionForce.x + (std::abs(velocity.x) - std::abs(temporalFrictionForce.x)) * -1;

	if (velocity.z != 0)
		if (std::abs(velocity.z) - std::abs(temporalFrictionForce.z) >= 0)
			velocity.z -= temporalFrictionForce.z;
		else if (std::abs(velocity.z) - std::abs(temporalFrictionForce.z) < 0)
			velocity.z -= temporalFrictionForce.z + (std::abs(velocity.z) - std::abs(temporalFrictionForce.z)) * -1;



	position += velocity;
}