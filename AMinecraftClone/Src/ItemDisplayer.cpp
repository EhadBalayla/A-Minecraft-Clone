#include "ItemDisplayer.h"
#include "Game.h"

void ItemDisplayer::Render() {
	ItemData id = Game::e_ItemRegistery[item->m_Item.getType()];

	if (id.type == ItemUsageType::PlaceableBlock && item->m_Item.getType() != ItemType::NoItem) {
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
		glDisable(GL_BLEND);

		Game::terrainAtlas.bind();

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));
		transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 1.0f));
		transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		Game::e_InventoryBlockShader.use();
		Game::e_InventoryBlockShader.setMat4("proj", Game::ScreenProjection);
		Game::e_InventoryBlockShader.setMat4("model", transform);

		BlockData bd = Game::e_BlockRegistery[id.blockID];

		Game::e_InventoryBlockShader.setInt("TopIDX", bd.uv.Top);
		Game::e_InventoryBlockShader.setInt("RightIDX", bd.uv.Right);
		Game::e_InventoryBlockShader.setInt("LeftIDX", bd.uv.Left);

		glBindVertexArray(Game::tempVAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);
	}

	if (item->m_Amount > 1) {
		countText.position = position + glm::vec2(5.0f, 12.5f);
		countText.setText(std::to_string(item->m_Amount));
		countText.Render();
	}
}