#ifndef TMR_TACS_CREATOR_H
#define TMR_TACS_CREATOR_H

/*
  The following file contains objects that facilitate the creation of
  TACSAssembler ojbects. These objects automate setting connectivity,
  node locations and boundary conditions in parallel for both forests
  of quadtrees and forests of octrees.

  These are virtual base classes. It is necessary to override the
  createElement() and optionally the createAuxElement()
  functions. These functions create the appropriate TACS element
  objects needed for TACSAssembler.
*/

#include "TMRQuadForest.h"
#include "TMROctForest.h"
#include "TACSAssembler.h"

/*
  Specify a list of boundary conditions through a list of attributes
  and boundary condition information for each node
*/
class TMRBoundaryConditions : public TMREntity {
 public:
  TMRBoundaryConditions();
  ~TMRBoundaryConditions();

  // Add a boundary condition associated with the specified attribute  
  void addBoundaryCondition( const char *attr, 
                             int num_bcs, const int bc_nums[] );

  // Get the number of boundary conditions
  int getNumBoundaryConditions();
  void getBoundaryCondition( int bc, const char **_attr, int *_num_bcs,
                             const int **_bcs_nums );

 public:
  // The number of boundary conditions
  int num_bcs;

  // Linked list sub-clas for the boundary conditions -- there are
  // typically only a handful of these objects
  class BCNode {
  public:
    BCNode( const char *_attr, int _num_bcs, const int *_bc_nums );
    ~BCNode();
    BCNode *next;
    char *attr;
    int num_bcs;
    int *bc_nums;
  } *bc_root, *bc_current;
};

/*
  The creator object for quadrilateral meshes

  This sets the quads/elements into a new TACSAssembler object
*/
class TMRQuadTACSCreator : public TMREntity {
 public:
  TMRQuadTACSCreator( TMRBoundaryConditions *_bcs );
  virtual ~TMRQuadTACSCreator();

  // Create an element for the given quadrant
  virtual TACSElement *createElement( int order,
                                      TMRQuadForest *forest,
                                      TMRQuadrant quad ) = 0;

  // Create any auxiliary element for the given quadrant
  virtual TACSElement *createAuxElement( int order,
                                         TMRQuadForest *forest,
                                         TMRQuadrant quad ){
    return NULL;
  }

  // Create the TACSAssembler object with the given order for this forest
  TACSAssembler *createTACS( int order, TMRQuadForest *forest );

 private:
  // Set the boundary conditions
  void setBoundaryConditions( TMRQuadForest *forest,
                              TACSAssembler *tacs );

  // Set the node locations
  void setNodeLocations( TMRQuadForest *forest, 
                         TACSAssembler *tacs );
  
  TMRBoundaryConditions *bcs;
};

/*
  The creator object for octant meshes

  This sets octants/elements into a new TACSAssembler object
*/
class TMROctTACSCreator : public TMREntity {
 public:
  TMROctTACSCreator( TMRBoundaryConditions *_bcs );
  virtual ~TMROctTACSCreator();

  // Create an element for the given octant
  virtual TACSElement *createElement( int order,
                                      TMROctForest *forest,
                                      TMROctant oct ) = 0;

  // Create any auxiliary element for the given octant
  virtual TACSElement *createAuxElement( int order,
                                         TMROctForest *forest,
                                         TMROctant oct ){
    return NULL;
  }

  // Add a boundary condition associated with the specified attribute  
  void addBoundaryCondition( const char *attr, 
                             int num_bcs, const int bc_nums[] );

  // Create the TACSAssembler object with the given order for this forest
  TACSAssembler *createTACS( int order, TMROctForest *forest );

 private:
  // Set the boundary conditions
  void setBoundaryConditions( TMROctForest *forest,
                              TACSAssembler *tacs );

  // Set the node locations
  void setNodeLocations( TMROctForest *forest, 
                         TACSAssembler *tacs );

  TMRBoundaryConditions *bcs;
};

#endif // TMR_TACS_CREATOR
