#include "Player.h"
#include "Game.h"
#include "Utilities.h"
#include "PlayerInventory.h"

Player::Player() {
    aabb = { glm::vec3(-0.3, 0.0, -0.3), glm::vec3(0.3, 1.8, 0.3) };
    entityHeight = 2; //player is two blocks tall

    acceleration = 0.8f;
    maxMovementSpeed = 5.2f;
    friction = 0.5f;
    gravity = 15.0f;
}
Player::~Player() {
}

glm::dvec3 Player::getCameraPosition() {
    return position + glm::dvec3(0.0, cameraHeight, 0.0);
}

glm::mat4 Player::getViewMatrix() {
    glm::vec3 front = {
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * sin(glm::radians(yaw))
    };

                        
    return glm::lookAt(glm::vec3(0.0, cameraHeight, 0.0), glm::vec3(0.0, cameraHeight, 0.0) + glm::normalize(front), {0, 1, 0});
}
glm::mat4 Player::getViewOnlyMatrix() {
    glm::vec3 front = {
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * sin(glm::radians(yaw))
    };

    //this is where the position        //also here is position
    return glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0) + glm::normalize(front), { 0, 1, 0 });
}

void Player::AddStarterItems() {
    AddItem(ItemType::GrassBlock, 32);
    AddItem(ItemType::CobblestoneBlock, 32);
    AddItem(ItemType::DiamondBlock, 32);
    AddItem(ItemType::BrickBlock, 32);
    AddItem(ItemType::WoodBlock, 32);
    AddItem(ItemType::WoodPlankBlock, 32);
    AddItem(ItemType::StoneBlock, 32);
    AddItem(ItemType::ObsidianBlock, 32);
    AddItem(ItemType::BedrockBlock, 32);
}

void Player::UpdateChunksAroundPlayer() {
    if (ShouldUpdateChunks) {
        int newChunkX = static_cast<int>(std::floor(position.x / 16.0));
        int newChunkZ = static_cast<int>(std::floor(position.z / 16.0));

        if (newChunkX != currentChunkX || newChunkZ != currentChunkZ) {
            Game::level->GetWorld().UpdateChunks(newChunkX, newChunkZ, 0);

            currentChunkX = newChunkX;
            currentChunkZ = newChunkZ;
        }

        if (Game::LODCount > 1) {
            for (int i = 1; i <= Game::LODCount - 1; i++) {
                int LODSize = GetLODSize(i);

                int newLodChunkX = newChunkX / LODSize;
                int newLodChunkZ = newChunkZ / LODSize;

                if (newLodChunkX != currentLODChunkX[i - 1] || newLodChunkZ != currentLODChunkZ[i - 1]) {
                    Game::level->GetWorld().UpdateChunks(newLodChunkX, newLodChunkZ, i);

                    currentLODChunkX[i - 1] = newLodChunkX;
                    currentLODChunkZ[i - 1] = newLodChunkZ;
                }
            }
        }
    }
}

void Player::Update(float DeltaTime) {
    UpdateChunksAroundPlayer();

    if (!CreativeMode) {
        if (CanPerformAbility()) { //basically if inventory is open don't do movement
            if (IsMovingForward)
                velocity += getForwardVector() * DeltaTime * acceleration;
            if (IsMovingBackward)
                velocity -= getForwardVector() * DeltaTime * acceleration;
            if (IsMovingLeft)
                velocity -= getRightVector() * DeltaTime * acceleration;
            if (IsMovingRight)
                velocity += getRightVector() * DeltaTime * acceleration;
            if (IsJumping && IsOnGround) {
                velocity.y += 6.5f;
            }
        }
        MoveAndCollide(DeltaTime);
    }
    else {
        if (IsMovingForward)
            position += getForwardVector() * DeltaTime * CreativeSpeed;
        if (IsMovingBackward)
            position -= getForwardVector() * DeltaTime * CreativeSpeed;
        if (IsMovingLeft)
            position -= getRightVector() * DeltaTime * CreativeSpeed;
        if (IsMovingRight)
            position += getRightVector() * DeltaTime * CreativeSpeed;
        if (IsJumping)
            position += glm::vec3(0.0f, 1.0f, 0.0f) * DeltaTime * CreativeSpeed;
        if(IsShifting)
            position += glm::vec3(0.0f, -1.0f, 0.0f) * DeltaTime * CreativeSpeed;
    }

    RayBlockInfo rayInfo = Game::level->GetWorld().RaycastBlock(getCameraPosition(), getLookVector(), 5.0f);
    rayBlock = rayInfo.block;
    rayBlockPos = rayInfo.blockPos;
    rayHitPos = rayInfo.hitPos;
}

bool Player::CanPerformAbility() {
    return !IsInventory && !IsPaused;
}

void Player::ProcessInput(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) { //non real time keybinds for example when pressing T for chat
        if (e.key.keysym.sym == SDLK_c) { //for jumping
            CreativeMode = !CreativeMode;
        }
        else if (e.key.keysym.sym == SDLK_ESCAPE) {
            if (!IsPaused) {
                IsPaused = true;
                SDL_ShowCursor(1);
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            else {
                IsPaused = false;
                SDL_ShowCursor(0);
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }
        }
        else if (e.key.keysym.sym == SDLK_F3) {
            Game::ShowDebugMenu = !Game::ShowDebugMenu;
        }
        else if (e.key.keysym.sym == SDLK_e) { //for opening inventory
            if (!IsInventory) {
                IsInventory = true;
                SDL_ShowCursor(1);
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            else {
                IsInventory = false;
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




        else if (e.key.keysym.sym == SDLK_f) {
            Game::ShowChunkBorder = !Game::ShowChunkBorder;
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
                if (rayBlock != BlockType::Air) {
                    Game::level->GetWorld().BreakBlock(rayBlockPos.x, rayBlockPos.y, rayBlockPos.z);
                    Game::m_AudioManager.StartSound("Sounds\\grass4.wav");
                }
            }
        }
        else if (e.button.button == SDL_BUTTON_RIGHT) { //for right click
            if(CanPerformAbility()) { //check if no menu is open
                if(m_PlayerItems[selectedSlot - 1].m_Item != ItemType::NoItem) {
                    ItemData id = Game::e_ItemRegistery[m_PlayerItems[selectedSlot - 1].m_Item];
                    switch (id.type) {
                    case ItemUsageType::PlaceableBlock:
                        PlayerPlaceBlocks();
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
    IsShifting = keystates[SDL_SCANCODE_LSHIFT]; //basically if pressing left shift
    IsJumping = keystates[SDL_SCANCODE_SPACE]; //basically if holding space
}


void Player::setSelectedSlot(uint8_t slot) {
    selectedSlot = slot;
    uint8_t slotMultiplier = slot - 1;
    Game::hudScreen.Selection.position = glm::vec2(-241.75855 + 60.5 * slotMultiplier, -30.2195);
}

void Player::AddItem(ItemType item, int amount) { //add item to inventory
    int currentAmount = amount;
    ItemData id = Game::e_ItemRegistery[item];
    for (int i = 0; i < 36; i++) { //iterates through the slots in the inventory
        if (m_PlayerItems[i].m_Item == ItemType::NoItem) { //if the item in the currently iterated inventory slot is NoItem (item equivalent of block air... AKA hardcoded to act as nothing)
            
            m_PlayerItems[i].m_Amount = 0; //resets the amount of the item to be 0 just to be safe... it should already be in theory
            m_PlayerItems[i].m_Item = item; //sets the type to the item's type

            while (currentAmount > 0 && m_PlayerItems[i].m_Amount < id.StackAmount) {
                m_PlayerItems[i].m_Amount++;
                currentAmount--;
            }
        }
        else if (m_PlayerItems[i].m_Item == item && m_PlayerItems[i].m_Amount < id.StackAmount) //if the item that you want to add is already in the current hotbar slot and the amount isn't the stack amount
        {
            //basically if the item already exists in currently iterated slot and its not in the stack amount... add more one at a time so that we won't go above the stack count
            while (currentAmount > 0 && m_PlayerItems[i].m_Amount < id.StackAmount) {
                m_PlayerItems[i].m_Amount++;
                currentAmount--;
            }
        }

        //after we finished adding items to a slot, check if we added all of the amounts or we only finished filling a stack
        if (currentAmount == 0) {
            return;
        }
    }
}
void Player::RemoveItem(InventoryItem* item, int amountToRemove) {
    item->m_Amount -= amountToRemove;
    if (item->m_Amount <= 0) {
        item->m_Amount = 0;
        item->m_Item = ItemType::NoItem;
    }
}


//place block item
void Player::PlayerPlaceBlocks() {
    if (rayBlock != BlockType::Air) {
        Game::m_AudioManager.StartSound("Sounds\\stone4.wav");
        RemoveItem(&m_PlayerItems[selectedSlot - 1], 1);
        ItemData id = Game::e_ItemRegistery[m_PlayerItems[selectedSlot - 1].m_Item];
        BlockType typeToPlace = id.blockID;

        BlockFace hitFace = BlockFace::Top;
        double distX = rayHitPos.x - (rayBlockPos.x + 0.5);
        double distY = rayHitPos.y - (rayBlockPos.y + 0.5);
        double distZ = rayHitPos.z - (rayBlockPos.z + 0.5);

        if (std::fabs(distX) > std::fabs(distY) && std::fabs(distX) > std::fabs(distZ))
            hitFace = (distX > 0 ? BlockFace::Right : BlockFace::Left);
        else if (std::fabs(distY) > std::fabs(distX) && std::fabs(distY) > std::fabs(distZ))
            hitFace = (distY > 0 ? BlockFace::Top : BlockFace::Bottom);
        else
            hitFace = (distZ > 0 ? BlockFace::Back : BlockFace::Front);

        switch (hitFace) {
        case BlockFace::Top:
            Game::level->GetWorld().PlaceBlock(rayBlockPos.x, rayBlockPos.y + 1, rayBlockPos.z, typeToPlace);
            break;
        case BlockFace::Bottom:
            Game::level->GetWorld().PlaceBlock(rayBlockPos.x, rayBlockPos.y - 1, rayBlockPos.z, typeToPlace);
            break;
        case BlockFace::Right:
            Game::level->GetWorld().PlaceBlock(rayBlockPos.x + 1, rayBlockPos.y, rayBlockPos.z, typeToPlace);
            break;
        case BlockFace::Left:
            Game::level->GetWorld().PlaceBlock(rayBlockPos.x - 1, rayBlockPos.y, rayBlockPos.z, typeToPlace);
            break;
        case BlockFace::Front:
            Game::level->GetWorld().PlaceBlock(rayBlockPos.x, rayBlockPos.y, rayBlockPos.z - 1, typeToPlace);
            break;
        case BlockFace::Back:
            Game::level->GetWorld().PlaceBlock(rayBlockPos.x, rayBlockPos.y, rayBlockPos.z + 1, typeToPlace);
            break;
        }
    }
}




glm::ivec2 Player::GetCurrentChunkCoords() {
    return glm::ivec2(currentChunkX, currentChunkZ);
}