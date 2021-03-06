////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// STL A* Search implementation
// (C)2001 Justin Heyes-Jones
//
// Finding a path on a simple grid maze
// This shows how to do shortest path finding using A*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stlastar.h" // See header for copyright and usage information
#include <iostream>
#include <stdio.h>

#include <cmath>
#include <ctime>

#define DEBUG_LISTS 0
#define DEBUG_LIST_LENGTHS_ONLY 0

using namespace std;

// Global data

// The world map

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;

int map[MAP_WIDTH * MAP_HEIGHT] = {

//    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 00
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 01
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 02
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 03
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 04
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 05
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 06
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 07
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 08
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 09
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 10
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 11
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 12
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 13
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 14
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 15
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 16
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 17
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 18
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 19

/*
//    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 00
      1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,   // 01
      1, 9, 9, 1, 1, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 1,   // 02
      1, 9, 9, 1, 1, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 1,   // 03
      1, 9, 1, 1, 1, 1, 9, 9, 1, 9, 1, 9, 1, 1, 1, 1, 9, 9, 1, 1,   // 04
      1, 9, 1, 1, 9, 1, 1, 1, 1, 9, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1,   // 05
      1, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 1, 1, 1, 1, 1,   // 06
      1, 9, 9, 9, 9, 9, 9, 9, 9, 1, 1, 1, 9, 9, 9, 9, 9, 9, 9, 1,   // 07
      1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1,   // 08
      1, 9, 1, 9, 9, 9, 9, 9, 9, 9, 1, 1, 9, 9, 9, 9, 9, 9, 9, 1,   // 09
      1, 9, 1, 1, 1, 1, 9, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 10
      1, 9, 9, 9, 9, 9, 1, 9, 1, 9, 1, 9, 9, 9, 9, 9, 1, 1, 1, 1,   // 11
      1, 9, 1, 9, 1, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 1,   // 12
      1, 9, 1, 9, 1, 9, 9, 9, 1, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 1,   // 13
      1, 9, 1, 1, 1, 1, 9, 9, 1, 9, 1, 9, 1, 1, 1, 1, 9, 9, 1, 1,   // 14
      1, 9, 1, 1, 9, 1, 1, 1, 1, 9, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1,   // 15
      1, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 1, 1, 1, 1, 1,   // 16
      1, 1, 9, 9, 9, 9, 9, 9, 9, 1, 1, 1, 9, 9, 9, 1, 9, 9, 9, 9,   // 17
      1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1,   // 18
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 19
*/
      };

// map helper functions

/*
 * Description: This function returns a coordinate value on the map intialized above
 * Parameter: int x is the x coordinate
 * Parameter: int y is the y coordinate
 * Return: the value represented at the x,y coordinate
 */
int GetMap(int x, int y) {

   if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
      return 9;
   }

   return map[(y * MAP_WIDTH) + x];
}

// Definitions

/*
 * Description: MapSearchNode describes a potential user state
 */
class MapSearchNode: public AStarState<MapSearchNode> {
public:
   unsigned int x;	 // the (x,y) positions of the node
   unsigned int y;

   MapSearchNode() {
      x = y = 0;
   }
   MapSearchNode(unsigned int px, unsigned int py) {
      x = px;
      y = py;
   }

   virtual float GoalDistanceEstimate(MapSearchNode &nodeGoal);
   virtual bool IsGoal(MapSearchNode &nodeGoal);
   virtual bool GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
         MapSearchNode *parent_node);
   virtual float GetCost(MapSearchNode &successor);
   virtual bool IsSameState(MapSearchNode &rhs);

   void PrintNodeInfo();
};

bool MapSearchNode::IsSameState(MapSearchNode &rhs) {

   // same state in a maze search is simply when (x,y) are the same
   if ((x == rhs.x) && (y == rhs.y)) {
      return true;
   }
   else {
      return false;
   }

}

void MapSearchNode::PrintNodeInfo() {
   char str[100];
   sprintf(str, "Node position : (%d,%d)\n", x, y);

   cout << str;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal. 

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal) {
   float xd = float(((float) x - (float) nodeGoal.x));
   float yd = float(((float) y - (float) nodeGoal.y));

   //TODO commented out this (looks like this uses manhattan distance without a tie breaker)
   //return xd + yd;

   //TODO either add tie breaker or use Euclidean distance algorithm
   //Euclidean distance algorithm:
   float h = sqrt(xd*xd + yd*yd);
   cout << "h: " << h << endl;
   return h;

}

bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal) {

   if ((x == nodeGoal.x) && (y == nodeGoal.y)) {
      return true;
   }

   return false;
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
      MapSearchNode *parent_node) {

   int parent_x = -1;
   int parent_y = -1;

   if (parent_node) {
      parent_x = parent_node->x;
      parent_y = parent_node->y;
   }

   MapSearchNode NewNode;

   // push each possible move except allowing the search to go backwards

   if ((GetMap(x - 1, y) < 9) && !((parent_x == x - 1) && (parent_y == y))) {
      NewNode = MapSearchNode(x - 1, y);
      astarsearch->AddSuccessor(NewNode);
   }

   if ((GetMap(x, y - 1) < 9) && !((parent_x == x) && (parent_y == y - 1))) {
      NewNode = MapSearchNode(x, y - 1);
      astarsearch->AddSuccessor(NewNode);
   }

   if ((GetMap(x + 1, y) < 9) && !((parent_x == x + 1) && (parent_y == y))) {
      NewNode = MapSearchNode(x + 1, y);
      astarsearch->AddSuccessor(NewNode);
   }

   if ((GetMap(x, y + 1) < 9) && !((parent_x == x) && (parent_y == y + 1))) {
      NewNode = MapSearchNode(x, y + 1);
      astarsearch->AddSuccessor(NewNode);
   }

   return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is 
// conceptually where we're moving

float MapSearchNode::GetCost(MapSearchNode &successor) {
   return (float) GetMap(x, y);

}

// Main

int main(int argc, char *argv[]) {

   clock_t t = clock();

   cout << "STL A* Search implementation\n(C)2001 Justin Heyes-Jones\n";

   // Our sample problem defines the world as a 2d array representing a terrain
   // Each element contains an integer from 0 to 5 which indicates the cost
   // of travel across the terrain. Zero means the least possible difficulty
   // in travelling (think ice rink if you can skate) whilst 5 represents the
   // most difficult. 9 indicates that we cannot pass.

   // Create an instance of the search class...

   AStarSearch<MapSearchNode> astarsearch;

   unsigned int SearchCount = 0;

   const unsigned int NumSearches = 1;

   while (SearchCount < NumSearches) {
      srand(time(NULL));

      //TODO commented out randomized start and goal nodes for user-specified positions
      // Create a start state
      MapSearchNode nodeStart(0, 4);
      //nodeStart.x = rand() % MAP_WIDTH;
      //nodeStart.y = rand() % MAP_HEIGHT;

      // Define the goal state
      MapSearchNode nodeEnd(4, 0);
      //nodeEnd.x = rand() % MAP_WIDTH;
      //nodeEnd.y = rand() % MAP_HEIGHT;

      // Set Start and goal states

      astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

      unsigned int SearchState;
      unsigned int SearchSteps = 0;

      do {
         SearchState = astarsearch.SearchStep();

         SearchSteps++;

#if DEBUG_LISTS

         cout << "Steps:" << SearchSteps << "\n";

         int len = 0;

         cout << "Open:\n";
         MapSearchNode *p = astarsearch.GetOpenListStart();
         while( p )
         {
            len++;
#if !DEBUG_LIST_LENGTHS_ONLY
            ((MapSearchNode *)p)->PrintNodeInfo();
#endif
            p = astarsearch.GetOpenListNext();

         }

         cout << "Open list has " << len << " nodes\n";

         len = 0;

         cout << "Closed:\n";
         p = astarsearch.GetClosedListStart();
         while( p )
         {
            len++;
#if !DEBUG_LIST_LENGTHS_ONLY
            p->PrintNodeInfo();
#endif
            p = astarsearch.GetClosedListNext();
         }

         cout << "Closed list has " << len << " nodes\n";
#endif

      } while (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);

      if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED) {
         cout << "Search found goal state\n";

         MapSearchNode *node = astarsearch.GetSolutionStart();

#if DISPLAY_SOLUTION
         cout << "Displaying solution\n";
#endif
         int steps = 0;

         node->PrintNodeInfo();
         for (;;) {
            node = astarsearch.GetSolutionNext();

            if (!node) {
               break;
            }

            node->PrintNodeInfo();
            steps++;

         };

         cout << "Solution steps " << steps << endl;

         // Once you're done with the solution you can free the nodes up
         astarsearch.FreeSolutionNodes();

      }
      else if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED) {
         cout << "Search terminated. Did not find goal state\n";

      }

      // Display the number of loops the search went through
      cout << "SearchSteps : " << SearchSteps << "\n";

      SearchCount++;

      astarsearch.EnsureMemoryFreed();
   }


   int x = 0;
   for (double i = 0; i < 9000000; i += 0.5) {
      x = sqrt(x) * i;
   }

   cout << "duration: " << (clock() - t) / (double)CLOCKS_PER_SEC << " seconds" << endl;

   return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
