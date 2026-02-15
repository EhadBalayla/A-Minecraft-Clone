#include "ItemDisplayer.h"
#include "Game.h"
#include "iostream"

void ItemDisplayer::Render() {
	ItemData id = Game::e_ItemRegistery[item->m_Item];

	if (id.type == ItemUsageType::PlaceableBlock && item->m_Item != ItemType::NoItem && item->m_Amount > 0) {
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
		glDisable(GL_BLEND);

		Game::terrainAtlas.bind();

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(position.x * Game::ScrSizeRel, position.y * Game::ScrSizeRel, 0.0f));
		transform = glm::scale(transform, glm::vec3(scale.x * Game::ScrSizeRel, scale.y * Game::ScrSizeRel, 1.0f));
		transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		Game::e_InventoryBlockShader.use();
		Game::e_InventoryBlockShader.setMat4("proj", *projMtx);
		Game::e_InventoryBlockShader.setMat4("model", transform);

		BlockData bd = Game::e_BlockRegistery[id.blockID];

		Game::e_InventoryBlockShader.setInt("TopIDX", bd.uv.Top);
		Game::e_InventoryBlockShader.setInt("RightIDX", bd.uv.Right);
		Game::e_InventoryBlockShader.setInt("LeftIDX", bd.uv.Left);

		glBindVertexArray(Game::tempVAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);
	
		if (item->m_Amount > 1) {
			countText.position = position + glm::vec2(5.0f, 12.5f);
			countText.setText(std::to_string(item->m_Amount));
			countText.Render();
		}
	}
}
void ItemDisplayer::Update() {
	int MouseX = 0;
	int MouseY = 0;
	auto mouseState = SDL_GetMouseState(&MouseX, &MouseY);

	float* m = reinterpret_cast<float*>(projMtx);

	float halfWidth = 1.0f / m[0];
	float halfHeight = 1.0f / m[5];
	float centerX = -m[12] / m[0];
	float centerY = -m[13] / m[5];

	float Right = centerX + halfWidth;
	float Bottom = centerY - halfHeight;
	float Left = centerX - halfWidth;
	float Top = centerY + halfHeight;

	float realXPos = (position.x * Game::ScrSizeRel - Left) / (Right - Left) * (Game::ScrSizeX);
	float realYPos = Game::ScrSizeY - (Bottom - position.y * Game::ScrSizeRel) / (Bottom - Top) * (Game::ScrSizeY);

	if ((MouseX > realXPos - ((scale.x * Game::ScrSizeRel) / 2.0f) && MouseX < realXPos + ((scale.x * Game::ScrSizeRel) / 2.0f)) &&
		(MouseY > realYPos - ((scale.y * Game::ScrSizeRel) / 2.0f) && MouseY < realYPos + ((scale.y * Game::ScrSizeRel) / 2.0f))) {
		if (!IsHovered) { //if wasn't hovered already then make hovered
			IsHovered = true;
		}
		
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if (!Game::FirstClick) {
				Game::FirstClick = true;
				if (item->m_Amount > 0 && CACHEDITEM.m_Amount > 0 && SlotsItemEqual(item, &CACHEDITEM)) {
					ItemData id = Game::e_ItemRegistery[item->m_Item];
					int remainder = id.StackAmount - item->m_Amount;
					int amountToRemove = CACHEDITEM.m_Amount - remainder;
					if (amountToRemove >= 0) {
						CACHEDITEM.removeAmount(remainder);
						item->addAmount(remainder);
					}
					else {
						item->addAmount(CACHEDITEM.m_Amount);
						CACHEDITEM.removeAmount(CACHEDITEM.m_Amount);
					}
				} else item->SwapSlot(&CACHEDITEM);
			}
		}
		else if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			if (!Game::FirstClick) {
				Game::FirstClick = true;
				if (CACHEDITEM.m_Amount > 0) {
					if (item->m_Amount > 0) {
						if (item->m_Item == CACHEDITEM.m_Item) {
							ItemData id = Game::e_ItemRegistery[item->m_Item];
							if (item->m_Amount + 1 <= id.StackAmount) {
								item->m_Amount++;
								CACHEDITEM.m_Amount--;
							}
						}
					}
					else {
						item->m_Item = CACHEDITEM.m_Item;
						item->m_Amount++;
						CACHEDITEM.decrement();
					}
				}
				else {
					if (item->m_Amount > 0) {
						int remainder = item->m_Amount / 2.0f;
						item->m_Amount -= remainder;
						CACHEDITEM.m_Amount += remainder;
						CACHEDITEM.m_Item = item->m_Item;
					}
				}
			}
		}
		else {
			if (Game::FirstClick)
				Game::FirstClick = false;
		}
	}
	else {
		if (IsHovered) {
			IsHovered = false;
		}
	}
}

void ItemDisplayer::SetTextProj(glm::mat4& MTX) {
	countText.projMtx = &MTX;
}