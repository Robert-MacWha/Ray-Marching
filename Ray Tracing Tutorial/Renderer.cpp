#include "Renderer.h"

void Renderer::Render(RawModel model) {

	glBindVertexArray(model.getVaoID());
	glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, 0);

}

void Renderer::Render(TextureModel textureModel) {

	RawModel model = textureModel.getModel();

	// activate the model's attributes and texture
	enableAttribs(model.getAttribIDs());
	glActiveTexture(textureModel.getTextureID());

	// render the model
	glBindVertexArray(model.getVaoID());
	glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, 0);

	// disable the attributes
	disableAttribs(model.getAttribIDs());

}

void  Renderer::enableAttribs(vector<int> attribIDs) {

	for (auto i = attribIDs.cbegin(); i != attribIDs.cend(); ++i) {

		int id = *i;
		glEnableVertexAttribArray(id);

	}

}

void  Renderer::disableAttribs(vector<int> attribIDs) {

	for (auto i = attribIDs.cbegin(); i != attribIDs.cend(); ++i) {

		int id = *i;
		glDisableVertexAttribArray(id);

	}

}