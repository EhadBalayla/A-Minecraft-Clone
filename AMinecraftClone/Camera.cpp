#include "Camera.h"
#include "Game.h"

void Camera::TestCollision() {
    int X = (int)Position.x;
    int Y = (int)Position.y;
    int Z = (int)Position.z;
    glm::vec3 pushback(0.0);
    if (AABBHelper::AABBIntersect(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X, Y + 1, Z)))
        pushback += AABBHelper::ResolveCollision(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X, Y + 1, Z));
    if (AABBHelper::AABBIntersect(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X, Y - 1, Z)))
        pushback += AABBHelper::ResolveCollision(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X, Y - 1, Z));

    if (AABBHelper::AABBIntersect(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X + 1, Y, Z)))
        pushback += AABBHelper::ResolveCollision(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X + 1, Y, Z));
    if (AABBHelper::AABBIntersect(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X - 1, Y, Z)))
        pushback += AABBHelper::ResolveCollision(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X - 1, Y, Z));

    if (AABBHelper::AABBIntersect(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X, Y, Z + 1)))
        pushback += AABBHelper::ResolveCollision(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X, Y, Z + 1));
    if (AABBHelper::AABBIntersect(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X, Y, Z - 1)))
        pushback += AABBHelper::ResolveCollision(playerHitbox, Game::overworld->GetWorld().getBlockHitbox(X, Y, Z - 1));
    Position += pushback;
}