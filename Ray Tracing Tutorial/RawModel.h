#pragma once

#include <vector>

using namespace std;

class RawModel
{

private:

	int vaoID;
	int vertexCount;

	vector<int> attribIDs;

public:
	RawModel() = default;
	RawModel(int vaoID, int vertexCount, vector<int> attribIDs);

	//* GETTERS
	int getVaoID();
	int getVertexCount();
	vector<int> getAttribIDs();

};
