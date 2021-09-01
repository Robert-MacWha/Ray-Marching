#include "RawModel.h"

RawModel::RawModel(int vaoID, int vertexCount, vector<int> attribIDs) {

	RawModel::vaoID = vaoID;
	RawModel::vertexCount = vertexCount;
	RawModel::attribIDs = attribIDs;

}

//* getters
int RawModel::getVaoID() {

	return vaoID;

}

int RawModel::getVertexCount() {

	return vertexCount;

}

vector<int> RawModel::getAttribIDs() {
	
	return attribIDs;

}