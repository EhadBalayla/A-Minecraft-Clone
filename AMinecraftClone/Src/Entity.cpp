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
	if (velocity.y < -78.4)
		velocity.y = -78.4;

	//clamp horizontal movement
	glm::dvec3 temporalClampedVelocity = glm::normalize(velocity) * maxMovementSpeed; //normalized velocity only on the x and z axes not on the y
	if (velocity.x > 0.0) velocity.x = std::min(velocity.x, temporalClampedVelocity.x);
	else if (velocity.x < 0.0) velocity.x = std::max(velocity.x, temporalClampedVelocity.x);

	if (velocity.z > 0.0) velocity.z = std::min(velocity.z, temporalClampedVelocity.z);
	else if (velocity.z < 0.0) velocity.z = std::max(velocity.z, temporalClampedVelocity.z);


	//apply friction
	glm::dvec3 temporalFrictionForce = glm::normalize(velocity) * friction;
	if (velocity.x != 0.0)
		if (std::abs(velocity.x) - std::abs(temporalFrictionForce.x) >= 0.0)
			velocity.x -= temporalFrictionForce.x;
		else if (std::abs(velocity.x) - std::abs(temporalFrictionForce.x) < 0.0)
			velocity.x -= temporalFrictionForce.x + (std::abs(velocity.x) - std::abs(temporalFrictionForce.x)) * -1.0;

	if (velocity.z != 0.0)
		if (std::abs(velocity.z) - std::abs(temporalFrictionForce.z) >= 0.0)
			velocity.z -= temporalFrictionForce.z;
		else if (std::abs(velocity.z) - std::abs(temporalFrictionForce.z) < 0.0)
			velocity.z -= temporalFrictionForce.z + (std::abs(velocity.z) - std::abs(temporalFrictionForce.z)) * -1.0;

	



	double dx = velocity.x * dt;
	double dy = velocity.y * dt;
	double dz = velocity.z * dt;

	glm::dvec3 newPos = position;
	glm::i64vec3 fPos(std::floor(position.x), std::floor(position.y), std::floor(position.z)); //fPos stands for floored pos
	glm::i64vec3 pPos, nPos;
	BlockType pBlock, nBlock;



	pPos = fPos + glm::i64vec3(0, entityHeight, 0);
	nPos = fPos + glm::i64vec3(0, -1, 0);
	pBlock = Game::level->GetWorld().getBlockAt(pPos.x, pPos.y, pPos.z);
	nBlock = Game::level->GetWorld().getBlockAt(nPos.x, nPos.y, nPos.z);

	newPos.y += dy;
	if (dy > 0.0 && pBlock != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(pPos), aabb.MovedTo(newPos))) {
		newPos.y = Block::blockHitbox.MovedTo(pPos).min.y - entityHeight;
		velocity.y = 0.0;
	}
	else if (dy < 0.0 && nBlock != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(nPos), aabb.MovedTo(newPos))) {
		newPos.y = Block::blockHitbox.MovedTo(nPos).max.y;
		velocity.y = 0.0;
		IsOnGround = true;
	}
	else IsOnGround = false;



	pPos = fPos + glm::i64vec3(0, 0, 1);
	nPos = fPos + glm::i64vec3(0, 0, -1);
	pBlock = Game::level->GetWorld().getBlockAt(pPos.x, pPos.y, pPos.z);
	nBlock = Game::level->GetWorld().getBlockAt(nPos.x, nPos.y, nPos.z);

	newPos.z += dz;
	if (dz > 0.0 && pBlock != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(pPos), aabb.MovedTo(newPos))) {
		newPos.z = pPos.z + aabb.min.z;
		velocity.z = 0.0;
	}
	else if (dz < 0.0 && nBlock != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(nPos), aabb.MovedTo(newPos))) {
		newPos.z = nPos.z + 1.0 + aabb.max.z;
		velocity.z = 0.0;
	}



	pPos = fPos + glm::i64vec3(1, 0, 0);
	nPos = fPos + glm::i64vec3(-1, 0, 0);
	pBlock = Game::level->GetWorld().getBlockAt(pPos.x, pPos.y, pPos.z);
	nBlock = Game::level->GetWorld().getBlockAt(nPos.x, nPos.y, nPos.z);

	newPos.x += dx;
	if (dx > 0.0 && pBlock != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(pPos), aabb.MovedTo(newPos))) {
		newPos.x = pPos.x + aabb.min.x;
		velocity.x = 0.0;
	}
	else if (dx < 0.0 && nBlock != BlockType::Air && AABBHelper::Intersects(Block::blockHitbox.MovedTo(nPos), aabb.MovedTo(newPos))) {
		newPos.x = nPos.x + 1.0 + aabb.max.x;
		velocity.x = 0.0;
	}
	
	position = newPos;
}