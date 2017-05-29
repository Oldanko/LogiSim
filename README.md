# LogiSim
An implementation of a A* algorithm with a visual representation.

Initial graph is randomly generated when the program is started.
Initial graph is a tree, therefore acyclic, with a root being a node closest to average position of all nodes.

Program works in two modes: road creator mode and path finder mode.
To switch between modes press the spacebar. Selected mode will be output in a console window.

Road creator mode allows for creation of new lines between nodes of the graph.
To create a road first select a starting node with a click of the left mouse button. 
A white line will appear connecting the node and a cursor. 
To deselect the node press the right mouse button or switch the mode with the spacebar.
Then select a destination node. A new line will be added to the graph.

Path finder mode allows to find the shortest(presumably) path between two points on the graph.
To find the path, first select starting point with a click of the left mouse button.
Selected node will be highlighted in green.
To deselect the node press the right mouse button or switch the mode with the spacebar.
Select a destination node with a click of a left mouse button.
The path between two nodes will be highlighted in yellow.
