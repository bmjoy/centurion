#include "a_star.h"

std::vector<glm::ivec2> aStar::pathFind(const Location &locStart, const Location &locFinish){

	// list of open (not-yet-checked-out) nodes
	static std::priority_queue<Node> q[2];

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
			PATH::DIR_MAP[i][j] = 0;
			PATH::CLOSED_NODES[i][j] = 0;
			PATH::OPEN_NODES[i][j] = 0;
		}
	}

	// create the start node and push into list of open nodes
	pNode1 = new Node(locStart, 0, 0);
	pNode1->calculateFValue(locFinish);
	q[qi].push(*pNode1);

	std::vector<glm::ivec2> finalPath = { };

	finalPath.push_back(glm::ivec2(locFinish.col * PATH::CELL_GRID_SIZE, locFinish.row * PATH::CELL_GRID_SIZE));
	finalPath.push_back(glm::ivec2(locFinish.col * PATH::CELL_GRID_SIZE, locFinish.row * PATH::CELL_GRID_SIZE));

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
		PATH::OPEN_NODES[row][col] = 0;

		// mark it on the closed nodes list
		PATH::CLOSED_NODES[row][col] = 1;

		// stop searching when the goal state is reached
		if (row == locFinish.row && col == locFinish.col) {

			// generate the path from finish to start from dirMap
			
			int pCount = 2; 
			bool addPoint;
			int i;
			while (!(row == locStart.row && col == locStart.col)) {
				i = 0; 
				addPoint = false;
				j = PATH::DIR_MAP[row][col];
				row += iDir[j];
				col += jDir[j];

				while (!addPoint && i != NDIR) {
					if (PATH::GRID_MATRIX_2D[row + iDir[i]][col + jDir[i]] == 1) {
						addPoint = true;
					}
					i++;
				}	
				if (addPoint){
					if (pCount == 2) {
						finalPath.push_back(glm::ivec2(col * PATH::CELL_GRID_SIZE, row * PATH::CELL_GRID_SIZE));
						pCount = 0;
					}
					pCount++;
				}	
			}

			// push start location
			finalPath.push_back(glm::ivec2(locStart.col* PATH::CELL_GRID_SIZE, locStart.row * PATH::CELL_GRID_SIZE));

			// reverse vector
			std::reverse(finalPath.begin(), finalPath.end());

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
				PATH::GRID_MATRIX_2D[iNext][jNext] == 1 || PATH::CLOSED_NODES[iNext][jNext] == 1)) {

				// generate a child node
				pNode2 = new Node(Location(iNext, jNext), pNode1->getGValue(), pNode1->getFValue());
				pNode2->updateGValue(i);
				pNode2->calculateFValue(locFinish);

				// if it is not in the open list then add into that
				if (PATH::OPEN_NODES[iNext][jNext] == 0) {
					PATH::OPEN_NODES[iNext][jNext] = pNode2->getFValue();
					q[qi].push(*pNode2);
					// mark its parent node direction
					PATH::DIR_MAP[iNext][jNext] = (i + NDIR / 2) % NDIR;
				}

				// already in the open list
				else if (PATH::OPEN_NODES[iNext][jNext] > pNode2->getFValue()) {
					// update the FValue info
					PATH::OPEN_NODES[iNext][jNext] = pNode2->getFValue();

					// update the parent direction info,  mark its parent node direction
					PATH::DIR_MAP[iNext][jNext] = (i + NDIR / 2) % NDIR;

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
	std::vector<glm::ivec2> emptyPath = { };
	return emptyPath;
}
