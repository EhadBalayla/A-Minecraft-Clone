#include "Player.h"
#include "Game.h"
#include "Utilities.h"
#include "PlayerInventory.h"

Player::Player() {
    aabb = { glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0) };
    entityHeight = 2; //player is two blocks tall
}
Player::~Player() {
}

glm::vec3 Player::getCameraPosition() {
    return position + glm::dvec3(0.0, cameraHeight, 0.0);
}

glm::mat4 Player::getViewMatrix() {
    glm::vec3 front = {
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * sin(glm::radians(yaw))
    };

                        //this is where the position        //also here is position
    return glm::lookAt(glm::vec3(0.0, cameraHeight, 0.0), glm::vec3(0.0, cameraHeight, 0.0) + glm::normalize(front), {0, 1, 0});
}

void Player::UpdateChunksAroundPlayer() {
    int newChunkX = static_cast<int>(std::floor(position.x / 16.0));
    int newChunkZ = static_cast<int>(std::floor(position.z / 16.0));

    if (newChunkX != currentChunkX || newChunkZ != currentChunkZ) {
        Game::overworld->UpdateChunks(newChunkX, newChunkZ);

        currentChunkX = newChunkX;
        currentChunkZ = newChunkZ;
    }
}

void Player::Update(float DeltaTime) {

    if(CanPerformAbility()) { //basically if inventory is open don't do movement
        if (IsMovingForward)
            velocity += getForwardVector() * DeltaTime * acceleration;
        if (IsMovingBackward)
            velocity -= getForwardVector() * DeltaTime * acceleration;
        if (IsMovingLeft)
            velocity -= getRightVector() * DeltaTime * acceleration;
        if (IsMovingRight)
            velocity += getRightVector() * DeltaTime * acceleration;
    }
    UpdateChunksAroundPlayer();

    MoveAndCollide(DeltaTime);

}

bool Player::CanPerformAbility() {
    return !inventory && !m_PauseMenu;
}

void Player::ProcessInput(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) { //non real time keybinds for example when pressing T for chat
        if (e.key.keysym.sym == SDLK_t) { //for opening chat
            std::cout << "Type X: ";
            int x;
            std::cin >> x;
            std::cout << std::endl;
            std::cout << "Type Y: ";
            int y;
            std::cin >> y;
            std::cout << std::endl;
            std::cout << "Type Z: ";
            int z;
            std::cin >> z;
            std::cout << std::endl;

            SetPosition(x, y, z);
        }
        else if (e.key.keysym.sym == SDLK_SPACE) { //for jumping
            if (IsOnGround) {
                velocity.y = 0.1f;
            }
        }
        else if (e.key.keysym.sym == SDLK_ESCAPE) { 
            if (!m_PauseMenu) {
                auto ptr = std::make_unique<PauseMenu>();
                m_PauseMenu = ptr.get();
                Game::m_UIManager.AddScreen(std::move(ptr));
                SDL_ShowCursor(1);
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            else {
                Game::m_UIManager.RemoveScreen(m_PauseMenu);
                m_PauseMenu = nullptr;
                SDL_ShowCursor(0);
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }
        }
        else if (e.key.keysym.sym == SDLK_e) { //for opening inventory
            if (!inventory) {
                auto ptr = std::make_unique<PlayerInventory>();
                inventory = ptr.get();
                Game::m_UIManager.AddScreen(std::move(ptr));
                SDL_ShowCursor(1);
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            else {
                Game::m_UIManager.RemoveScreen(inventory);
                inventory = nullptr;
                SDL_ShowCursor(0);
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }
        }
        else if (e.key.keysym.sym == SDLK_1) { 
            setSelectedSlot(1);
        }
        else if (e.key.keysym.sym == SDLK_2) { 
            setSelectedSlot(2);
        }
        else if (e.key.keysym.sym == SDLK_3) { 
            setSelectedSlot(3);
        }
        else if (e.key.keysym.sym == SDLK_4) { 
            setSelectedSlot(4);
        }
        else if (e.key.keysym.sym == SDLK_5) {
            setSelectedSlot(5);
        }
        else if (e.key.keysym.sym == SDLK_6) { 
            setSelectedSlot(6);
        }
        else if (e.key.keysym.sym == SDLK_7) { 
            setSelectedSlot(7);
        }
        else if (e.key.keysym.sym == SDLK_8) {
            setSelectedSlot(8);
        }
        else if (e.key.keysym.sym == SDLK_9) { 
            setSelectedSlot(9);
        }
    }
    else if (e.type == SDL_MOUSEMOTION) { //for mouse movements
        if(CanPerformAbility()) { //check if no menu is open
            float dx = e.motion.xrel;
            float dy = e.motion.yrel;

            yaw += dx * mouseSensitivity;
            pitch -= dy * mouseSensitivity;

            //clamps the pitch
            if (pitch > 89.9)
                pitch = 89.9;
            else if (pitch < -89.9)
                pitch = -89.9;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN) { //for mouse 
        if (e.button.button == SDL_BUTTON_LEFT) { //for left click
            if(CanPerformAbility()) { //check if no menu is open
                Ray ray = Utils::shootRay(getCameraPosition(), getLookVector(), 5.0f);
                RayHitReturnParams rayInfo = Utils::RayHitBlock(ray);
                if (rayInfo.HitBlock) {
                    rayInfo.HitBlock->BreakBlock();
                }
            }
        }
        else if (e.button.button == SDL_BUTTON_RIGHT) { //for right click
            if(CanPerformAbility()) { //check if no menu is open
                Ray ray = Utils::shootRay(getCameraPosition(), getLookVector(), 5.0f);
                RayHitReturnParams rayInfo = Utils::RayHitBlock(ray, 0.025);
                if (rayInfo.HitBlock) {
                    glm::ivec3 blockPos = rayInfo.HitBlock->getWorldPosition();
                    switch (rayInfo.HitFace) {
                    case Face::Top:
                        Game::overworld->PlaceBlock(blockPos.x, blockPos.y + 1, blockPos.z, BlockType::Cobblestone);
                        break;
                    case Face::Bottom:
                        Game::overworld->PlaceBlock(blockPos.x, blockPos.y - 1, blockPos.z, BlockType::Cobblestone);
                        break;
                    case Face::Right:
                        Game::overworld->PlaceBlock(blockPos.x + 1, blockPos.y, blockPos.z, BlockType::Cobblestone);
                        break;
                    case Face::Left:
                        Game::overworld->PlaceBlock(blockPos.x - 1, blockPos.y, blockPos.z, BlockType::Cobblestone);
                        break;
                    case Face::Front:
                        Game::overworld->PlaceBlock(blockPos.x, blockPos.y, blockPos.z + 1, BlockType::Cobblestone);
                        break;
                    case Face::Back:
                        Game::overworld->PlaceBlock(blockPos.x, blockPos.y, blockPos.z - 1, BlockType::Cobblestone);
                        break;
                    }
                }
            }
        }
    }

    //for real time held inputs
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    IsMovingForward = keystates[SDL_SCANCODE_W]; //basically scan if pressing W
    IsMovingBackward = keystates[SDL_SCANCODE_S]; //basically scan if pressing S
    IsMovingLeft = keystates[SDL_SCANCODE_A]; //basically scan if pressing A
    IsMovingRight = keystates[SDL_SCANCODE_D]; //basically scan if pressing D

}


void Player::setSelectedSlot(uint8_t slot) {
    selectedSlot = slot;
    uint8_t slotMultiplier = slot - 1;
    Game::m_UIManager.GetScreen(0)->GetWidget(2)->SetPosition(glm::vec2(420.2195 + 55 * slotMultiplier, 689.7805));
}