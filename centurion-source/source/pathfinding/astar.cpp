#include <pathfinding>

using namespace std;
using namespace glm;

namespace astar {

	int *GridMatrix() { return gridMatrix; }
	int *GridMatrix2D() { return gridMatrix2D; }
	int *ClosedNodes() { return closedNodes; }
	int *OpenNodes() { return openNodes; }
	int *DirMap() { return dirMap; }

	vector<vector<int>> readPassMatrix(string &path, string &classname) {
		vector<vector<int>> mat;
		ifstream fin(path);
		if (!fin.good()) {
			stringstream ss;

			ss << "A problem occurred while reading " + classname + " PASS file.";
			Logger::Warn(ss.str());
		}
		else {
			string line, value;
			while (getline(fin, line)) {
				if (line.length() > 0){
					vector<int> line_values;
					stringstream s(line);
					while (getline(s, value, ',')) line_values.push_back(stoi(value));
					mat.push_back(line_values);
				}
			}
		}
		return mat;
	}
	bool checkAvailability(vector<vector<int>> &building_grid, vec3 &position) {
		bool b = true;
		vec2 pos = vec2((int)position.x / astar::cellGridSize - building_grid[0].size() / 2, (int)position.y / astar::cellGridSize - building_grid.size() / 2);
		for (int i = 0; i < building_grid.size(); i++) {
			for (int j = 0; j < building_grid[0].size(); j++) {
				int k = ((int)building_grid.size() - i + (int)pos.y) * gridWidth + j + (int)pos.x;
				if (k >= 0 && k < gridWidth * gridHeight){
					if (GridMatrix()[k] == 1 && building_grid[i][j] == 1){
						b = false;
						break;
					}
				}
				else {
					b = false;
					break;
				}
			}
		}
		return b;
	}
	void updatePassMatrix(vector<vector<int>> &building_grid, vec3 &position) {
		vec2 pos = vec2((int)position.x / astar::cellGridSize - building_grid[0].size() / 2, (int)position.y / astar::cellGridSize - building_grid.size() / 2);
		for (int i = 0; i < building_grid.size(); i++) {
			for (int j = 0; j < building_grid[0].size(); j++) {
				int k = ((int)building_grid.size() - i + (int)pos.y) * gridWidth + j + (int)pos.x;
				if (GridMatrix()[k] == 0) {
					GridMatrix()[k] = building_grid[i][j];
				}
			}
		}
	}

	void clearPassMatrix(vector<vector<int>> &building_grid, vec3 &position) {
		vec2 pos = vec2((int)position.x / astar::cellGridSize - building_grid[0].size() / 2, (int)position.y / astar::cellGridSize - building_grid.size() / 2);
		for (int i = 0; i < building_grid.size(); i++) {
			for (int j = 0; j < building_grid[0].size(); j++) {
				int k = ((int)building_grid.size() - i + (int)pos.y) * gridWidth + j + (int)pos.x;
				GridMatrix()[k] = 0;
			}
		}
	}

	int getGridInfoFromPoint(float x, float y) {
		return GridMatrix()[(int)y / cellGridSize * gridWidth + (int)x / cellGridSize];
	}

	vector<ivec2> pathFind(const Location &locStart, const Location &locFinish){

		// list of open (not-yet-checked-out) nodes
		static priority_queue<Node> q[2];

		// q index
		static int qi;

		static Node* pNode1;
		static Node* pNode2;
		static int i, j, row, col, iNext, jNext;
		static char c;
		qi = 0;

		// reset the Node lists (0 = ".")
		for (i = 0; i < IDIM; i++) {
			for (j = 0; j < JDIM; j++) {
				DirMap()[i * gridWidth + j] = 0;
				ClosedNodes()[i * gridWidth + j] = 0;
				OpenNodes()[i * gridWidth + j] = 0;
			}
		}

		// create the start node and push into list of open nodes
		pNode1 = new Node(locStart, 0, 0);
		pNode1->calculateFValue(locFinish);
		q[qi].push(*pNode1);

		vector<ivec2> finalPath;

		finalPath.push_back(ivec2(locFinish.col * cellGridSize, locFinish.row * cellGridSize));
		finalPath.push_back(ivec2(locFinish.col * cellGridSize, locFinish.row * cellGridSize));

		// A* search
		while (!q[qi].empty()) {
			// get the current node w/ the lowest FValue
			// from the list of open nodes
			pNode1 = new Node(q[qi].top().getLocation(),
				q[qi].top().getGValue(), q[qi].top().getFValue());

			row = (pNode1->getLocation()).row;
			col = pNode1->getLocation().col;

			// remove the node from the open list
			q[qi].pop();
			OpenNodes()[row * gridWidth + col] = 0;

			// mark it on the closed nodes list
			ClosedNodes()[row * gridWidth + col] = 1;

			// stop searching when the goal state is reached
			if (row == locFinish.row && col == locFinish.col) {

				// generate the path from finish to start from dirMap
			
				int pCount = 2; 
				bool addPoint;
				int i;
				while (!(row == locStart.row && col == locStart.col)) {
					i = 0; 
					addPoint = false;
					j = DirMap()[row * gridWidth + col];
					row += iDir[j];
					col += jDir[j];

					while (!addPoint && i != NDIR) {
						if (GridMatrix2D()[(row + iDir[i]) * gridWidth + col + jDir[i]] == 1) {
							addPoint = true;
						}
						i++;
					}	
					if (addPoint){
						if (pCount == 2) {
							finalPath.push_back(ivec2(col * cellGridSize, row * cellGridSize));
							pCount = 0;
						}
						pCount++;
					}	
				}

				// push start location
				finalPath.push_back(ivec2(locStart.col* cellGridSize, locStart.row * cellGridSize));

				// reverse vector
				reverse(finalPath.begin(), finalPath.end());

				// garbage collection
				delete pNode1;

				// empty the leftover nodes
				while (!q[qi].empty()) q[qi].pop();

				return finalPath;
			}

			// generate moves in all possible directions
			for (i = 0; i < NDIR; i++) {
				iNext = row + iDir[i];
				jNext = col + jDir[i];

				// if not wall (obstacle) nor in the closed list
				if (!(iNext < 0 || iNext > IDIM - 1 || jNext < 0 || jNext > JDIM - 1 ||
					GridMatrix2D()[iNext * gridWidth + jNext] == 1 || ClosedNodes()[iNext * gridWidth + jNext] == 1)) {

					// generate a child node
					pNode2 = new Node(Location(iNext, jNext), pNode1->getGValue(), pNode1->getFValue());
					pNode2->updateGValue(i);
					pNode2->calculateFValue(locFinish);

					// if it is not in the open list then add into that
					if (OpenNodes()[iNext * gridWidth + jNext] == 0) {
						OpenNodes()[iNext * gridWidth + jNext] = pNode2->getFValue();
						q[qi].push(*pNode2);
						// mark its parent node direction
						DirMap()[iNext * gridWidth + jNext] = (i + NDIR / 2) % NDIR;
					}

					// already in the open list
					else if (OpenNodes()[iNext * gridWidth + jNext] > pNode2->getFValue()) {
						// update the FValue info
						OpenNodes()[iNext * gridWidth + jNext] = pNode2->getFValue();

						// update the parent direction info,  mark its parent node direction
						DirMap()[iNext * gridWidth + jNext] = (i + NDIR / 2) % NDIR;

						// replace the node by emptying one q to the other one
						// except the node to be replaced will be ignored
						// and the new node will be pushed in instead
						while (!(q[qi].top().getLocation().row == iNext &&
							q[qi].top().getLocation().col == jNext)) {
							q[1 - qi].push(q[qi].top());
							q[qi].pop();
						}

						// remove the wanted node
						q[qi].pop();

						// empty the larger size q to the smaller one
						if (q[qi].size() > q[1 - qi].size()) qi = 1 - qi;
						while (!q[qi].empty()) {
							q[1 - qi].push(q[qi].top());
							q[qi].pop();
						}
						qi = 1 - qi;

						// add the better node instead
						q[qi].push(*pNode2);
					}
					else delete pNode2;
				}
			}
			delete pNode1;
		}
	
		// no path found
		vector<ivec2> emptyPath;
		return emptyPath;
	}

}