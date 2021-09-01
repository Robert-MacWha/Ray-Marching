#include "TextureModel.h"

TextureModel::TextureModel(RawModel model, GLuint textureID) {

	TextureModel::model = model;
	TextureModel::textureID = textureID;

}

RawModel TextureModel::getModel() {

	return model;

}

GLuint TextureModel::getTextureID() {

	return textureID;

}