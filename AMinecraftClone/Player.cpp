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

void Player::AddStarterItems() {
    Item i;
    i.setType(ItemType::GrassBlock);
    AddItem(i, 32);

    i.setType(ItemType::CobblestoneBlock);
    AddItem(i, 48);

    i.setType(ItemType::DiamondBlock);
    AddItem(i, 32);

    i.setType(ItemType::BrickBlock);
    AddItem(i, 32);

    i.setType(ItemType::WoodBlock);
    AddItem(i, 32);

    i.setType(ItemType::WoodPlankBlock);
    AddItem(i, 32);

    i.setType(ItemType::StoneBlock);
    AddItem(i, 32);

    i.setType(ItemType::ObsidianBlock);
    AddItem(i, 32);

    i.setType(ItemType::BedrockBlock);
    AddItem(i, 32);
}

void Player::UpdateChunksAroundPlayer() {
    if (ShouldUpdateChunks) {
        int newChunkX = static_cast<int>(std::floor(position.x / 16.0));
        int newChunkZ = static_cast<int>(std::floor(position.z / 16.0));

        if (newChunkX != currentChunkX || newChunkZ != currentChunkZ) {
            Game::overworld->GetWorld().UpdateChunks(newChunkX, newChunkZ);

            currentChunkX = newChunkX;
            currentChunkZ = newChunkZ;
        }

        for (int i = 1; i <= 4; i++) {
            int LODSize = GetLODSize(i);

            int newLodChunkX = newChunkX / LODSize;
            int newLodChunkZ = newChunkZ / LODSize;

            if (newLodChunkX != currentLODChunkX[i - 1] || newLodChunkZ != currentLODChunkZ[i - 1]) {
                Game::overworld->GetWorld().UpdateLODs(newLodChunkX, newLodChunkZ, i);

                currentLODChunkX[i - 1] = newLodChunkX;
                currentLODChunkZ[i - 1] = newLodChunkZ;
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
        else if (e.key.keysym.sym == SDLK_c) { //for jumping
            CreativeMode = !CreativeMode;
        }
        else if (e.key.keysym.sym == SDLK_b) { //for jumping
            ShouldUpdateChunks = !ShouldUpdateChunks;
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
                    Game::m_AudioManager.StartSound("Sounds\\grass4.ogg");
                }
            }
        }
        else if (e.button.button == SDL_BUTTON_RIGHT) { //for right click
            if(CanPerformAbility()) { //check if no menu is open
                if(m_PlayerItems[selectedSlot - 1].m_Item.getType() != ItemType::NoItem) {
                    switch (m_PlayerItems[selectedSlot - 1].m_Item.getData().type) {
                    case ItemUsageType::PlaceableBlock:
                        PlayerPlaceBlocks();
                        Game::m_AudioManager.StartSound("Sounds\\wood3.ogg");
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
    Game::m_UIManager.GetScreen(0)->GetWidget(2)->SetPosition(glm::vec2(420.2195 + 55 * slotMultiplier, 689.7805));
}

void Player::AddItem(Item item, int amount) { //add item to inventory
    int currentAmount = amount;
    for (int i = 0; i < 36; i++) { //iterates through the slots in the inventory
        if (m_PlayerItems[i].m_Item.getType() == ItemType::NoItem) { //if the item in the currently iterated inventory slot is NoItem (item equivalent of block air... AKA hardcoded to act as nothing)
            
            m_PlayerItems[i].m_Amount = 0; //resets the amount of the item to be 0 just to be safe... it should already be in theory
            m_PlayerItems[i].m_Item.setOnlyType(item.getType()); //sets the type to the item's type
            m_PlayerItems[i].m_Item.setData(item.getData()); //copies the item data directly from the given item

            while (currentAmount > 0 && m_PlayerItems[i].m_Amount < m_PlayerItems[i].m_Item.getData().StackAmount) {
                m_PlayerItems[i].m_Amount++;
                currentAmount--;
            }
        }
        else if (m_PlayerItems[i].m_Item == item && m_PlayerItems[i].m_Amount < m_PlayerItems[i].m_Item.getData().StackAmount) //if the item that you want to add is already in the current hotbar slot and the amount isn't the stack amount
        {
            //basically if the item already exists in currently iterated slot and its not in the stack amount... add more one at a time so that we won't go above the stack count
            while (currentAmount > 0 && m_PlayerItems[i].m_Amount < m_PlayerItems[i].m_Item.getData().StackAmount) {
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


//place block item
void Player::PlayerPlaceBlocks() {
    Ray ray = Utils::shootRay(getCameraPosition(), getLookVector(), 5.0f);
    RayHitReturnParams rayInfo = Utils::RayHitBlock(ray, 0.025);
    if (rayInfo.HitBlock) {
        glm::ivec3 blockPos = rayInfo.HitBlock->getWorldPosition();
        BlockType typeToPlace = m_PlayerItems[selectedSlot - 1].m_Item.getData().blockID;
        switch (rayInfo.HitFace) {
        case Face::Top:
            Game::overworld->GetWorld().PlaceBlock(blockPos.x, blockPos.y + 1, blockPos.z, typeToPlace);
            break;
        case Face::Bottom:
            Game::overworld->GetWorld().PlaceBlock(blockPos.x, blockPos.y - 1, blockPos.z, typeToPlace);
            break;
        case Face::Right:
            Game::overworld->GetWorld().PlaceBlock(blockPos.x + 1, blockPos.y, blockPos.z, typeToPlace);
            break;
        case Face::Left:
            Game::overworld->GetWorld().PlaceBlock(blockPos.x - 1, blockPos.y, blockPos.z, typeToPlace);
            break;
        case Face::Front:
            Game::overworld->GetWorld().PlaceBlock(blockPos.x, blockPos.y, blockPos.z + 1, typeToPlace);
            break;
        case Face::Back:
            Game::overworld->GetWorld().PlaceBlock(blockPos.x, blockPos.y, blockPos.z - 1, typeToPlace);
            break;
        }
    }
}




glm::ivec2 Player::GetCurrentChunkCoords() {
    return glm::ivec2(currentChunkX, currentChunkZ);
}