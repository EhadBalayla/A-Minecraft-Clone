#include "Entity.h"
#include "Utilities.h"
#include "Game.h"

#undef min
#undef max

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

AABB& Entity::GetCollision() {
	return aabb;
}

void Entity::MoveAndCollide(float DeltaTime) {
	float dt = DeltaTime * 0.001f;
	velocity.y -= gravity * dt;
	if (velocity.y < -78.4f)
		velocity.y = -78.4f;

	//clamp horizontal movement
	glm::vec3 temporalClampedVelocity = glm::normalize(velocity) * maxMovementSpeed; //normalized velocity only on the x and z axes not on the y
	if (velocity.x > 0) velocity.x = std::min(velocity.x, temporalClampedVelocity.x);
	else if (velocity.x < 0) velocity.x = std::max(velocity.x, temporalClampedVelocity.x);

	if (velocity.z > 0) velocity.z = std::min(velocity.z, temporalClampedVelocity.z);
	else if (velocity.z < 0) velocity.z = std::max(velocity.z, temporalClampedVelocity.z);


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

	
	float dx = velocity.x * dt;
	float dy = velocity.y * dt;
	float dz = velocity.z * dt;

	glm::dvec3 newPos = position;

	glm::ivec3 abovePos = glm::vec3(position.x, position.y + entityHeight, position.z);
	glm::ivec3 belowPos = glm::vec3(position.x, position.y - 1, position.z);
	BlockType above = Game::level->GetWorld().getBlockAt(position.x, position.y + entityHeight, position.z);
	BlockType below = Game::level->GetWorld().getBlockAt(position.x, position.y - 1, position.z);

	newPos.y += dy;
	if (dy > 0 && above != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(abovePos), aabb.MovedTo(newPos))) {
		newPos.y = Block::blockHitbox.MovedTo(abovePos).min.y - entityHeight;
		velocity.y = 0;
	}
	else if (dy < 0 && below != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(belowPos), aabb.MovedTo(newPos))) {
		newPos.y = Block::blockHitbox.MovedTo(belowPos).max.y;
		velocity.y = 0;
		IsOnGround = true;
	}
	else IsOnGround = false;

	glm::ivec3 forwardPos = glm::vec3(position.x, position.y, position.z + 1);
	glm::ivec3 backwardPos = glm::vec3(position.x, position.y, position.z - 1);
	BlockType forward = Game::level->GetWorld().getBlockAt(position.x, position.y, position.z + 1);
	BlockType backward = Game::level->GetWorld().getBlockAt(position.x, position.y, position.z - 1);

	newPos.z += dz;
	if (dz > 0 && forward != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(forwardPos), aabb.MovedTo(newPos))) {
		newPos.z = forwardPos.z + aabb.min.z;
		velocity.z = 0;
	}
	else if (dz < 0 && backward != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(backwardPos), aabb.MovedTo(newPos))) {
		newPos.z = backwardPos.z + 1 + aabb.max.z;
		velocity.z = 0;
	}

	glm::ivec3 rightPos = glm::vec3(position.x + 1, position.y, position.z);
	glm::ivec3 leftPos = glm::vec3(position.x - 1, position.y, position.z);
	BlockType right = Game::level->GetWorld().getBlockAt(position.x + 1, position.y, position.z);
	BlockType left = Game::level->GetWorld().getBlockAt(position.x - 1, position.y, position.z);

	newPos.x += dx;
	if (dx > 0 && right != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(rightPos), aabb.MovedTo(newPos))) {
		newPos.x = rightPos.x + aabb.min.x;
		velocity.x = 0;
	}
	else if (dx < 0 && left != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(leftPos), aabb.MovedTo(newPos))) {
		newPos.x = leftPos.x + 1 + aabb.max.x;
		velocity.x = 0;
	}
	
	position = newPos;
}