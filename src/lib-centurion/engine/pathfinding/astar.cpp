#include "pathfinding.h"

#include <queue>

#include <logger.h>
#include <settings.h>

#include <game/pass.h>

using namespace std;
using namespace glm;

namespace astar {

	namespace 
	{
		int closedNodes[GRID_ARRAY_SIZE] = { 0 };
		int openNodes[GRID_ARRAY_SIZE] = { 0 };
		int dirMap[GRID_ARRAY_SIZE] = { 0 };
	};

	vector<vector<unsigned int>> readPassMatrix(string &path, string &classname) {
		vector<vector<unsigned int>> mat;
		ifstream fin(path);
		if (!fin.good()) {
			stringstream ss;

			ss << "A problem occurred while reading " + classname + " PASS file.";
			Logger::Warn(ss.str());
		}
		else {
			string line, value;
			while (getline(fin, line)) {
				if (line.length() > 0) {
					vector<unsigned int> line_values;
					stringstream s(line);
					while (getline(s, value, ',')) line_values.push_back(stoi(value));
					mat.push_back(line_values);
				}
			}
		}
		return mat;
	}

	

	vector<ivec2> pathFind(const Location &locStart, const Location &locFinish) {

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
				dirMap[i * GRID_SIZE_X + j] = 0;
				closedNodes[i * GRID_SIZE_X + j] = 0;
				openNodes[i * GRID_SIZE_X + j] = 0;
			}
		}

		// create the start node and push into list of open nodes
		pNode1 = new Node(locStart, 0, 0);
		pNode1->calculateFValue(locFinish);
		q[qi].push(*pNode1);

		vector<ivec2> finalPath;

		finalPath.push_back(ivec2(locFinish.col * GRID_CELL_SIZE, locFinish.row * GRID_CELL_SIZE));
		finalPath.push_back(ivec2(locFinish.col * GRID_CELL_SIZE, locFinish.row * GRID_CELL_SIZE));

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
			openNodes[row * GRID_SIZE_X + col] = 0;

			// mark it on the closed nodes list
			closedNodes[row * GRID_SIZE_X + col] = 1;

			// stop searching when the goal state is reached
			if (row == locFinish.row && col == locFinish.col) {

				// generate the path from finish to start from dirMap

				int pCount = 2;
				bool addPoint;
				int i;
				while (!(row == locStart.row && col == locStart.col)) {
					i = 0;
					addPoint = false;
					j = dirMap[row * GRID_SIZE_X + col];
					row += iDir[j];
					col += jDir[j];

					while (!addPoint && i != NDIR) {
						unsigned int idx = (row + iDir[i]) * GRID_SIZE_X + col + jDir[i];
						if (Pass::GetGrid2DValueByIndex(idx) == 1){
							addPoint = true;
						}
						i++;
					}
					if (addPoint) {
						if (pCount == 2) {
							finalPath.push_back(ivec2(col * GRID_CELL_SIZE, row * GRID_CELL_SIZE));
							pCount = 0;
						}
						pCount++;
					}
				}

				// push start location
				finalPath.push_back(ivec2(locStart.col* GRID_CELL_SIZE, locStart.row * GRID_CELL_SIZE));

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
				unsigned int idx = iNext * GRID_SIZE_X + jNext;
				unsigned int grid2dValue = Pass::GetGrid2DValueByIndex(idx);

				if (!(iNext < 0 || iNext > IDIM - 1 || jNext < 0 || jNext > JDIM - 1 ||
					grid2dValue == 1 || closedNodes[iNext * GRID_SIZE_X + jNext] == 1)) {

					// generate a child node
					pNode2 = new Node(Location(iNext, jNext), pNode1->getGValue(), pNode1->getFValue());
					pNode2->updateGValue(i);
					pNode2->calculateFValue(locFinish);

					// if it is not in the open list then add into that
					if (openNodes[iNext * GRID_SIZE_X + jNext] == 0) {
						openNodes[iNext * GRID_SIZE_X + jNext] = pNode2->getFValue();
						q[qi].push(*pNode2);
						// mark its parent node direction
						dirMap[iNext * GRID_SIZE_X + jNext] = (i + NDIR / 2) % NDIR;
					}

					// already in the open list
					else if (openNodes[iNext * GRID_SIZE_X + jNext] > pNode2->getFValue()) {
						// update the FValue info
						openNodes[iNext * GRID_SIZE_X + jNext] = pNode2->getFValue();

						// update the parent direction info,  mark its parent node direction
						dirMap[iNext * GRID_SIZE_X + jNext] = (i + NDIR / 2) % NDIR;

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