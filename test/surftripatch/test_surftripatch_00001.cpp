// ========================================================================== //
//           ** BitPit mesh ** Test 001 for class surftri_patch **            //
//                                                                            //
// Test construction, modifiers and communicators for surftri_patch.          //
// ========================================================================== //
/*---------------------------------------------------------------------------*\
 *
 *  bitpit
 *
 *  Copyright (C) 2015-2016 OPTIMAD engineering Srl
 *
 *  -------------------------------------------------------------------------
 *  License
 *  This file is part of bitbit.
 *
 *  bitpit is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License v3 (LGPL)
 *  as published by the Free Software Foundation.
 *
 *  bitpit is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with bitpit. If not, see <http://www.gnu.org/licenses/>.
 *
\*---------------------------------------------------------------------------*/

// ========================================================================== //
// INCLUDES                                                                   //
// ========================================================================== //

// Standard Template Library
# include <array>
# include <vector>
# include <iostream>

// BitPit
# include "bitpit_common.hpp"                                                 // Utilities and common definitions
# include "bitpit_operators.hpp"                                              // STL containers operators
# include "bitpit_patch.hpp"                                                  // BitPit base patch
# include "bitpit_surftripatch.hpp"                                           // BitPit surftri patch

// ========================================================================== //
// NAMESPACES                                                                 //
// ========================================================================== //
using namespace std;
using namespace bitpit;

// ========================================================================== //
// GENERATE A TEST NON-MANIFOLD SURFACE TRIANGULATION FOR TESTS.              //
// ========================================================================== //
void generateTestTriangulation(
    SurfTriPatch                &mesh
) {

// ========================================================================== //
// void generateTestTriangulation(                                            //
//     SurfTriPatch                &mesh)                                     //
//                                                                            //
// Generate a non-manifold surface triangulation for tests.                   //
// ========================================================================== //
// INPUT                                                                      //
// ========================================================================== //
// - mesh    : SurfTriPatch, surface mesh patch                               //
// ========================================================================== //
// OUTPUT                                                                     //
// ========================================================================== //
// - none                                                                     //
// ========================================================================== //

// ========================================================================== //
// VARIABLES DECLARATION                                                      //
// ========================================================================== //

// Local variables
long                    nV = 27;
long                    nS = 33;

// Counters
int                     i, j;

// ========================================================================== //
// INITIALIZE TRIANGULATION                                                   //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //

    // Reserve memory for vertex & cell storage ----------------------------- //
    mesh.reserveVertices(nV);
    mesh.reserveCells(nS);
}

// ========================================================================== //
// GENERATE VERTEX LIST                                                       //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //
    double                      off = -0.5;
    array<double, 3>            vertex;
    
    // 0-row ---------------------------------------------------------------- //
    vertex.fill(0.0);
    for (i = 0; i < 8; ++i) {
        vertex[0] = double(i);
        mesh.addVertex(vertex);
    } //next i

    // 1-row ---------------------------------------------------------------- //
    vertex.fill(0.0);
    vertex[1] = 1.0;
    for (i = 0; i < 9; ++i) {
        vertex[0] = double(i) + off;
        mesh.addVertex(vertex);
    } //next i

    // 2-row ---------------------------------------------------------------- //
    vertex.fill(0.0);
    vertex[1] = 2.0;
    for (i = 0; i < 8; ++i) {
        vertex[0] = double(i);
        mesh.addVertex(vertex);
    } //next i

    // Orthogonal element(s) ------------------------------------------------ //
    vertex[0] = 0.5*(mesh.getVertex(3)[0] + mesh.getVertex(12)[0]);
    vertex[1] = 0.5*(mesh.getVertex(3)[1] + mesh.getVertex(12)[1]);
    vertex[2] = 0.5 * sqrt(3.0);
    mesh.addVertex(vertex);
    vertex[0] = 0.5*(mesh.getVertex(12)[0] + mesh.getVertex(21)[0]);
    vertex[1] = 0.5*(mesh.getVertex(12)[1] + mesh.getVertex(21)[1]);
    vertex[2] = 0.5 * sqrt(3.0);
    mesh.addVertex(vertex);
}

// ========================================================================== //
// GENERATE CONNECTIVITY                                                      //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //
    long int                    off;
    vector<long>                connectivity(3);

    // 0-row ---------------------------------------------------------------- //
    off = 8;
    for (i = 0; i < 7; ++i) {
        connectivity[0] = i;
        connectivity[1] = i + 1 + off;
        connectivity[2] = i + off;
        mesh.addCell(ElementInfo::TRIANGLE, true, connectivity);
        connectivity[0] = i;
        connectivity[1] = i + 1;
        connectivity[2] = i + 1 + off;
        mesh.addCell(ElementInfo::TRIANGLE, true, connectivity);
    } //next i
    connectivity[0] = i;
    connectivity[1] = i + 1 + off;
    connectivity[2] = i + off;
    mesh.addCell(ElementInfo::TRIANGLE, true, connectivity);

    // 1-row ---------------------------------------------------------------- //
    off = 9;
    for (i = 8; i < 15; ++i) {
        connectivity[0] = i;
        connectivity[1] = i + 1;
        connectivity[2] = i + off;
        mesh.addCell(ElementInfo::TRIANGLE, true, connectivity);
        connectivity[0] = i + 1;
        connectivity[1] = i + 1 + off;
        connectivity[2] = i + off;
        mesh.addCell(ElementInfo::TRIANGLE, true, connectivity);
    } //next i
    connectivity[0] = i;
    connectivity[1] = i + 1;
    connectivity[2] = i + off;
    mesh.addCell(ElementInfo::TRIANGLE, true, connectivity);

    // Orthogonal element --------------------------------------------------- //
    connectivity[0] = 3;
    connectivity[1] = 12;
    connectivity[2] = 25;
    mesh.addCell(ElementInfo::TRIANGLE, true, connectivity);
    connectivity[0] = 12;
    connectivity[1] = 26;
    connectivity[2] = 25;
    mesh.addCell(ElementInfo::TRIANGLE, true, connectivity);
    connectivity[0] = 12;
    connectivity[1] = 21;
    connectivity[2] = 26;
    mesh.addCell(ElementInfo::TRIANGLE, true, connectivity);
    
}

return;
}

// ========================================================================== //
// SUBTEST #002 Test adjacencies construction and update                      //
// ========================================================================== //
int subtest_002(
    void
) {

// ========================================================================== //
// int subtest_002(                                                           //
//     void)                                                                  //
//                                                                            //
// Test cell removal/insertion and adjacencies construction/updates on        //
// non-manifold surface triangulation.                                        //
// ========================================================================== //
// INPUT                                                                      //
// ========================================================================== //
// - none                                                                     //
// ========================================================================== //
// OUTPUT                                                                     //
// ========================================================================== //
// - err      : int, error flag:                                              //
//              err = 0  --> no error(s)                                      //
//              err = 1  --> error at step #1                                 //
//              err = 2  --> error at step #2                                 //
// ========================================================================== //

// ========================================================================== //
// VARIABLES DECLARATION                                                      //
// ========================================================================== //

// Local variables
SurfTriPatch                            mesh(0);
Cell                                    cell_17, cell_5, cell_7;
vector<long>                            cell_list;

// Counters
// none

// ========================================================================== //
// INITIALIZE MESH PARAMETERS                                                 //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //
    // none

    // Enable changes ------------------------------------------------------- //
    mesh.setExpert(true);
    //mesh.setParallel(1 ,0);
}

// ========================================================================== //
// OUTPUT MESSAGE                                                             //
// ========================================================================== //
{
    // Scope variables
    // none

    // Output message
    cout << "** ================================================================= **" << endl;
    cout << "** Test #00001 - sub-test #002 - Testing adjacencies construction    **" << endl;
    cout << "**                               and update.                         **" << endl;
    cout << "** ================================================================= **" << endl;
    cout << endl;
}

// ========================================================================== //
// GENERATE A DUMMY TRIANGULATION (STEP #1)                                   //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //
    SurfTriPatch                        envelope(0);
    vector<long>                        ring1, ring1_expected{6,7,8,21,22,23,30,31,32};

    // Set envelope attributes ---------------------------------------------- //
    envelope.setExpert(true);

    // Generate a dummy triangulation --------------------------------------- //
    cout << "** Generating non-manifold surface triangulation" << endl;
    generateTestTriangulation(mesh);
    cout << "   building adjacencies" << endl;
    mesh.buildAdjacencies();

    // Mesh stats ----------------------------------------------------------- //
    cout << "   mesh stats (step 1):" << endl;
    mesh.displayTopologyStats(cout, 3);

    // Check mesh topology -------------------------------------------------- //

    // Vertices stats
    if (mesh.getVertexCount() != 27)            return 1;
    if (mesh.countFreeVertices() != 20)         return 1;

    // Faces stats
    if (mesh.countFaces() != 59)                return 1;
    if (mesh.countFreeFaces() != 21)            return 1;

    // Cells stats
    if (mesh.getCellCount() != 33)              return 1;
    if (mesh.countFreeCells() != 21)            return 1;

    // Compute 1-ring of vertex 12 ------------------------------------------ //
    ring1 = mesh.findCellVertexOneRing(7, 2);
    sort(ring1.begin(), ring1.end());
    cout << "  1-ring of vertex (7, 2): " << ring1 << endl;

    // Check 1-ring of vertex (7,2) ----------------------------------------- //
    if (ring1 != ring1_expected)            return 1;

    // External envelope ---------------------------------------------------- //
    cout << "  extracting mesh envelope" << endl;
    mesh.extractEnvelope(envelope);
    envelope.buildAdjacencies();

    // Check external envelope topology ------------------------------------- //

    // Vertices stats
    if (envelope.getVertexCount() != 20)        return 1;
    if (envelope.countFreeVertices() != 0)      return 1;

    // Faces stats
    if (envelope.countFaces() != 20)            return 1;
    if (envelope.countFreeFaces() != 0)         return 1;

    // Cells stats
    if (envelope.getCellCount() != 21)          return 1;
    if (envelope.countFreeCells() != 0)         return 1;

    // Export triangulation ------------------------------------------------- //
    mesh.write("step1");
    cout << "  (mesh exported to \"step1.vtu\")" << endl;
    envelope.write("env_step1");
    cout << "  (external mesh envelope exported to \"env_step1.vtu\")" << endl;
    cout << endl;

}

// ========================================================================== //
// TEST CELL REMOVAL                                                          //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //
    SurfTriPatch                        envelope(0);
    vector<long>                        ring1, ring1_expected{6,8,21,22,23,30,31,32};

    // Set envelope attributes ---------------------------------------------- //
    envelope.setExpert(true);

    // Backup copy of cells ------------------------------------------------- //
    cout << "** Removing cell ID 5, 7, and 17" << endl;
    cell_17 = mesh.getCell(17);
    cell_17.resetAdjacencies();
    cell_5  = mesh.getCell(5);
    cell_5.resetAdjacencies();
    cell_7  = mesh.getCell(7);
    cell_7.resetAdjacencies();

    // Remove cells --------------------------------------------------------- //
    mesh.deleteCell(17);
    mesh.deleteCell(5);
    mesh.deleteCell(7);

    // Mesh stats ----------------------------------------------------------- //
    cout << "   mesh stats (step 2):" << endl;
    mesh.displayTopologyStats(cout, 3);

    // Check mesh topology -------------------------------------------------- //

    // Vertices stats
    if (mesh.getVertexCount() != 27)            return 2;
    if (mesh.countFreeVertices() != 24)         return 2;

    // Face stats
    if (mesh.countFaces() != 57)                return 2;
    if (mesh.countFreeFaces() != 25)            return 2;

    // Cells stats
    if (mesh.getCellCount() != 30)              return 2;
    if (mesh.countFreeCells() != 23)            return 2;


    // Compute 1-ring of vertex 12 ------------------------------------------ //
    cout << "1-ring" << endl;
    mesh.getCell(23).display(cout, 4);
    ring1 = mesh.findCellVertexOneRing(23, 0);
    sort(ring1.begin(), ring1.end());
    cout << "  1-ring of vertex (23, 0): " << ring1 << endl;

    // Check 1-ring of vertex (23,0) ---------------------------------------- //
    if (ring1 != ring1_expected)                return 2;

    // External envelope ---------------------------------------------------- //
    cout << "   extracting mesh envelope" << endl;
    mesh.extractEnvelope(envelope);
    envelope.buildAdjacencies();

    // Check external envelope topology ------------------------------------- //

    // Vertex stats
    if (envelope.getVertexCount() != 24)        return 2;
    if (envelope.countFreeVertices() != 1)      return 2;

    // Faces stats
    if (envelope.countFaces() != 24)            return 2;
    if (envelope.countFreeFaces() != 1)         return 2;

    // Cells stats
    if (envelope.getCellCount() != 25)          return 2;
    if (envelope.countFreeCells() != 1)         return 2;

    // Export triangulation ------------------------------------------------- //
    mesh.write("step2");
    cout << "   (mesh exported to \"step2.vtu\")" << endl;
    envelope.write("env_step2");
    cout << "   (external mesh envelope exported to \"env_step2.vtu\")" << endl;
    cout << endl;

}

// ========================================================================== //
// TEST CELL INSERTION                                                        //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //
    SurfTriPatch                        envelope(0);
    vector<long>                        ring1, ring1_expected{35};
    SurfTriPatch::CellIterator          it;

    // Set envelope attributes ---------------------------------------------- //
    envelope.setExpert(true);

    // Insert cells --------------------------------------------------------- //
    cout << "** inserting previously deleted cells" << endl;
    it = mesh.addCell(cell_17);
    cell_list.push_back(it->get_id());

    it = mesh.addCell(cell_5);
    cell_list.push_back(it->get_id());

    it = mesh.addCell(cell_7);
    cell_list.push_back(it->get_id());
    cout << "   cell list is: " << cell_list << endl;

    // Mesh stats topology -------------------------------------------------- //
    cout << "   mesh stats (step 3):" << endl;
    mesh.displayTopologyStats(cout, 3);

    // Check mesh ----------------------------------------------------------- //

    // Vertices stats
    if (mesh.getVertexCount() != 27)            return 3;
    if (mesh.countFreeVertices() != 24)         return 3;

    // Faces stats
    if (mesh.countFaces() != 66)                return 3;
    if (mesh.countFreeFaces() != 34)            return 3;

    // Cells stats
    if (mesh.getCellCount() != 33)              return 3;
    if (mesh.countFreeCells() != 26)            return 3;

    // Compute 1-ring of vertex 12 ------------------------------------------ //
    ring1 = mesh.findCellVertexOneRing(7, 2);
    sort(ring1.begin(), ring1.end());
    cout << "  1-ring of vertex (7, 2): " << ring1 << endl;

    // Check 1-ring of (35, 2) ---------------------------------------------- //
    if (ring1 != ring1_expected)                return 3;

    // External envelope ---------------------------------------------------- //
    cout << "   extracting mesh external envelope" << endl;
    mesh.extractEnvelope(envelope);
    envelope.buildAdjacencies();

    // Check external envelope topology ------------------------------------- //

    // Vertices stats
    if (envelope.getVertexCount() != 24)        return 3;
    if (envelope.countFreeVertices() != 0)      return 3;

    // Check faces stats
    if (envelope.countFaces() != 24)            return 3;
    if (envelope.countFreeFaces() != 0)         return 3;

    // Check cells stats
    if (envelope.getCellCount() != 34)          return 3;
    if (envelope.countFreeCells() != 0)         return 3;

    // Export triangulation ------------------------------------------------- //
    mesh.write("step3");
    cout << "   (mesh exported to \"step3.vtu\")" << endl;
    envelope.write("env_step3");
    cout << "   (external mesh envelope exported to \"env_step3.vtu\")" << endl;
    cout << endl;
}

// ========================================================================== //
// TEST UPDATE ADJACENCIES                                                    //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //
    SurfTriPatch                        envelope(0);
    vector<long>                        ring1, ring1_expected{6,8,21,22,23,30,31,32,35};

    // Set envelope attributes ---------------------------------------------- //
    envelope.setExpert(true);

    // Insert cells --------------------------------------------------------- //
    cout << "** updating adjacencies" << endl;
    mesh.updateAdjacencies(cell_list);

    // Mesh stats ----------------------------------------------------------- //
    cout << "   mesh stats (step 4):" << endl;
    mesh.displayTopologyStats(cout, 3);

    // Check mesh topology -------------------------------------------------- //

    // Vertices stats
    if (mesh.getVertexCount() != 27)            return 4;
    if (mesh.countFreeVertices() != 20)         return 4;

    // Faces stats
    if (mesh.countFaces() != 59)                return 4;
    if (mesh.countFreeFaces() != 21)            return 4;

    // Cells stats
    if (mesh.getCellCount() != 33)              return 4;
    if (mesh.countFreeCells() != 21)            return 4;

    // Compute 1-ring of vertex 12 ------------------------------------------ //
    ring1 = mesh.findCellVertexOneRing(35, 2);
    sort(ring1.begin(), ring1.end());
    cout << "  1-ring of vertex (35, 2): " << ring1 << endl;

    // Check 1-ring of vertex (7,2) ----------------------------------------- //
    if (ring1 != ring1_expected)                return 4;

    // External envelope ---------------------------------------------------- //
    cout << "   extracting mesh external envelope" << endl;
    mesh.extractEnvelope(envelope);
    envelope.buildAdjacencies();

    // Check external envelope topology ------------------------------------- //

    // Vertices stats
    if (envelope.getVertexCount() != 20)        return 4;
    if (envelope.countFreeVertices() != 0)      return 4;

    // Faces stats
    if (envelope.countFaces() != 20)            return 4;
    if (envelope.countFreeFaces() != 0)         return 4;

    // Cells stats
    if (envelope.getCellCount() != 21)          return 4;
    if (envelope.countFreeCells() != 0)         return 4;

    // Export triangulation ------------------------------------------------- //
    mesh.write("step4");
    cout << "   (mesh exported to \"step4.vtu\")" << endl;
    envelope.write("env_step4");
    cout << "   (external mesh envelope exported to \"env_step4.vtu\")" << endl;
    cout << endl;
}

// ========================================================================== //
// OUTPUT MESSAGE                                                             //
// ========================================================================== //
{
    // Scope variables -------------------------------------------------- //
    // none

    // Output message --------------------------------------------------- //
    cout << "** ================================================================= **" << endl;
    cout << "** Test #00001 - sub-test #002 - completed!                          **" << endl;
    cout << "** ================================================================= **" << endl;
    cout << endl;
}

return 0;
}

// ========================================================================== //
// SUBTEST #001 Test cell insertion and deletion                              //
// ========================================================================== //
int subtest_001(
    void
) {

// ========================================================================== //
// int subtest_001(                                                           //
//     void)                                                                  //
//                                                                            //
// Test insertion order.                                                      //
// ========================================================================== //
// INPUT                                                                      //
// ========================================================================== //
// - none                                                                     //
// ========================================================================== //
// OUTPUT                                                                     //
// ========================================================================== //
// - err       : int, error flag:                                             //
//               err = 0  --> no error(s)                                     //
//               err = 1  --> error at step #1                                //
//               err = 2  --> error at step #2                                //
// ========================================================================== //
    
// ========================================================================== //
// VARIABLES DECLARATION                                                      //
// ========================================================================== //

// Local variables
SurfTriPatch                    mesh(0);
vector<long>                    c_connect{0, 1, 2};
vector<long>                    g_connect{3, 4, 5};
Cell                            cell(0, ElementInfo::TRIANGLE, true);
Cell                            ghost(0, ElementInfo::TRIANGLE, false);
vector<long>                    expected;
vector<bool>                    internal;

// Counters
int                             i;

// ========================================================================== //
// INITIALIZE MESH PARAMETERS                                                 //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //
    // none

    // Enable changes ------------------------------------------------------- //
    mesh.setExpert(true);
}

// ========================================================================== //
// OUTPUT MESSAGE                                                             //
// ========================================================================== //
{
    // Scope variables
    // none

    // Output message
    cout << "** ================================================================= **" << endl;
    cout << "** Test #00001 - sub-test #001 - Testing cell insertion and deletion **" << endl;
    cout << "** ================================================================= **" << endl;
    cout << endl;
}

// ========================================================================== //
// INITIALIZE CELL                                                            //
// ========================================================================== //
{
    // Scope variables
    int                         j;
    int                         n;

    // Initialize internal cell
    cout << "** Initializing cell" << endl;
    n = cell.getVertexCount();
    for (j = 0; j < n; ++j) {
        cell.setVertex(j, c_connect[j]);
    } //next j
    n = ghost.getVertexCount();
    for (j = 0; j < n; ++j) {
        ghost.setVertex(j, g_connect[j]);
    } //next j
    cout << endl;
}

// ========================================================================== //
// INSERT CELLS (STEP #1)                                                     //
// ========================================================================== //
{
    // Scope variables ------------------------------------------------------ //
    const int                                   N = 5;
    SurfTriPatch::CellIterator                  it, et;

    // Insert internal cells (IDX 0-4) -------------------------------------- //
    // cells:  {0,1,2,3,4}
    // ghosts: {}
    cout << "** Inserting internal cells" << endl;
    for (i = 0; i < N/2; ++i) {
        mesh.addCell(cell);
        expected.push_back(long(i));
        internal.push_back(true);
    } //next i
    for (i = N/2; i < N; ++i) {
        mesh.addCell(ElementInfo::TRIANGLE, true, c_connect);
        expected.push_back(long(i));
        internal.push_back(true);
    } //next i
    cout << endl;

    // Check cell ordering -------------------------------------------------- //
    i = 0;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        if (it->get_id() != expected[i]) return 1;
        if (it->isInterior() != internal[i]) return 1;
        ++i;
    } //next it

    // Display mesh content ------------------------------------------------- //
    cout << "** After inserting internal cells" << endl;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        cout << "   cell: " << endl;
        it->display(cout, 4);
    } //next it
    cout << endl;

    // Insert ghost cells (IDX 5-9) ----------------------------------------- //
    // cells:  {0,1,2,3,4}
    // ghosts: {5,6,7,8,9}
    cout << "** Inserting ghost cells" << endl;
    for (i = 0; i < N/2; ++i) {
        mesh.addCell(ghost);
        expected.push_back(long(N + i));
        internal.push_back(false);
    } //next i
    for (i = N/2; i < N; ++i) {
        mesh.addCell(ElementInfo::TRIANGLE, false, g_connect);
        expected.push_back(long(N + i));
        internal.push_back(false);
    } //next i
    cout << endl;

    // Check cells ordering ------------------------------------------------- //
    i = 0;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        if (it->get_id() != expected[i]) return 1;
        if (it->isInterior() != internal[i]) return 1;
        ++i;
    } //next it

    // Display mesh content ------------------------------------------------- //
    cout << "** After inserting ghost cells" << endl;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        cout << "   cell: " << endl;
        it->display(cout, 4);
    } //next it
    cout << endl;

}

// ========================================================================== //
// REMOVE INSERT/CELLS (STEP #2)                                              //
// ========================================================================== //
{
    // Scope variables
    SurfTriPatch::CellIterator                  it, et;

    // Remove internal cells
    //bucket: {4,2,5,6}
    //cells:  {0,1,-1,3,-1}
    //ghosts: {-1,-1,7,8,9}
    mesh.deleteCell(4);
    mesh.deleteCell(2);
    mesh.deleteCell(5);
    mesh.deleteCell(6);
    expected.erase(expected.begin() + 6);
    expected.erase(expected.begin() + 5);
    expected.erase(expected.begin() + 4);
    expected.erase(expected.begin() + 2);
    internal.erase(internal.begin() + 6);
    internal.erase(internal.begin() + 5);
    internal.erase(internal.begin() + 4);
    internal.erase(internal.begin() + 2);

    // Check element order
    i = 0;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        if (it->get_id() != expected[i])        return 2;
        if (it->isInterior() != internal[i])    return 2;
        ++i;
    } //next it

    // Display mesh
    cout << "** After removing internal/ghost cells" << endl;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        cout << "   cell: " << endl;
        it->display(cout, 4);
    } //next it
    cout << endl;

    // Remove ghost cells
    //bucket = {}
    //cells:  {0,1,5,3,6}
    //ghosts: {2,4,7,8,9}
    mesh.addCell(ElementInfo::TRIANGLE, false, g_connect);
    mesh.addCell(ghost);
    mesh.addCell(cell);
    mesh.addCell(ElementInfo::TRIANGLE, true, c_connect);
    expected.insert(expected.begin() + 3, 4);
    expected.insert(expected.begin() + 3, 2);
    expected.insert(expected.begin() + 2, 5);
    expected.insert(expected.begin() + 4, 6);
    internal.insert(internal.begin() + 3, false);
    internal.insert(internal.begin() + 3, false);
    internal.insert(internal.begin() + 2, true);
    internal.insert(internal.begin() + 4, true);

    // Check element order
    i = 0;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        if (it->get_id() != expected[i])        return 2;
        if (it->isInterior() != internal[i])    return 2;
        ++i;
    } //next it

    cout << "** After inserting internal/ghost cells" << endl;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        cout << "   cell: " << endl;
        it->display(cout, 4);
    } //next it
    cout << endl;

    // Remove all internal cells and add 2 ghost cells
    //bucket: {5,0,3}
    //cells:  {}
    //ghosts: {6,1,2,4,7,8,9}
    mesh.deleteCell(6);
    mesh.deleteCell(1);
    mesh.deleteCell(5);
    mesh.deleteCell(0);
    mesh.deleteCell(3);
    mesh.addCell(ghost);
    mesh.addCell(ElementInfo::TRIANGLE, false, g_connect);
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.insert(expected.begin(), 1);
    expected.insert(expected.begin(), 6);
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.insert(internal.begin(), false);
    internal.insert(internal.begin(), false);

    // Check element order
    i = 0;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        if (it->get_id() != expected[i]) return 2;
        if (it->isInterior() != internal[i]) return 2;
        ++i;
    } //next it

    cout << "** After erasing all internal cells and inserting 2 new ghosts" << endl;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        cout << "   cell: " << endl;
        it->display(cout, 4);
    } //next it
    cout << endl;

    // Remove all ghosts add 2 internal cells
    //bucket: {3,4,2,1,6,9,7,8}
    //cells:  {5,0}
    //ghosts: {}
    mesh.deleteCell(4);
    mesh.deleteCell(2);
    mesh.deleteCell(1);
    mesh.deleteCell(6);
    mesh.deleteCell(9);
    mesh.deleteCell(7);
    mesh.deleteCell(8);
    mesh.addCell(cell);
    mesh.addCell(ElementInfo::TRIANGLE, true, c_connect);
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.erase(expected.begin());
    expected.insert(expected.begin(),0);
    expected.insert(expected.begin(),5);
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.erase(internal.begin());
    internal.insert(internal.begin(),true);
    internal.insert(internal.begin(),true);

    // Check element order
    i = 0;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        if (it->get_id() != expected[i]) return 2;
        if (it->isInterior() != internal[i]) return 2;
        ++i;
    } //next it

    cout << "** After erasing all ghost cells and inserting 2 new internal cells" << endl;
    et = mesh.cellEnd();
    for (it = mesh.cellBegin(); it != et; ++it) {
        cout << "   cell: " << endl;
        it->display(cout, 4);
    } //next it
    cout << endl;

}

// ========================================================================== //
// OUTPUT MESSAGE                                                             //
// ========================================================================== //
{
    // Scope variables
    // none

    // Output message
    cout << "** ================================================================= **" << endl;
    cout << "** Test #00001 - sub-test #001 - completed!                          **" << endl;
    cout << "** ================================================================= **" << endl;
    cout << endl;
}

return 0; }

// ========================================================================== //
// MAIN FOR TEST #00001                                                       //
// ========================================================================== //
int main(
    void
) {

// ========================================================================== //
// VARIABLES DECLARATION                                                      //
// ========================================================================== //

// Local variabels
int                             err = 0;

// ========================================================================== //
// RUN SUB-TEST #001                                                          //
// ========================================================================== //
err = subtest_001();
if (err > 0) return(10 + err);

// ========================================================================== //
// RUN SUB-TEST #002                                                          //
// ========================================================================== //
err = subtest_002();
if (err > 0) return(20 + err);

return err;

}
