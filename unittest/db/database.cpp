/**
 * File              : database.cpp
 * Author            : Yibo Lin <yibolin@pku.edu.cn>
 * Date              : 04.26.2020
 * Last Modified Date: 04.26.2020
 * Last Modified By  : Yibo Lin <yibolin@pku.edu.cn>
 */

#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "db/core/db.h"

EDI_BEGIN_NAMESPACE

namespace unitest {

void testStream(char const* filename, std::ios_base::openmode mode) {
#if 0
    // remove-database
  // write
  {
    ediPrint(kDebug, "writing to %s\n", filename);
    Database db;

    // create a cell
    db.initCell(2);
    auto cell0 = db.addCell("cell0");
    cell0->initTerms(3);
    auto mt0 = cell0->addTerm("term0");
    mt0->setSignalDirect(SignalDirection::kInput);
    auto mt1 = cell0->addTerm("term1");
    mt1->setSignalDirect(SignalDirection::kOutput);

    /* remove-design
    // create a design
    auto design = db.getDesign();
    design->initInsts(2);
    design->initPins(5);
    design->initNets(2);
    design->initInstsOfTerms(2);
    auto inst0 = design->addInst();
    inst0->initPins(1);
    inst0->getAttr()->setName("inst0");
    // inst0->getAttr()->setCell(cell0);
    inst0->getAttr()->setLoc({0, 1});
    inst0->addCell(cell0);
    InstAttr attr1;
    attr1.setName("inst1");
    // attr1.setCell(cell0);
    attr1.setLoc({2, 3});
    auto inst1 = design->addInst(attr1);
    inst1->addCell(cell0);
    inst1->initPins(1);

    auto net0 = design->addNet();
    net0->getAttr()->setName("net0");
    net0->initPins(2);
    
    auto inst_term0 = design->addPin();
    inst_term0->getAttr()->setTerm(mt0);
    inst_term0->setInst(inst0);
    inst_term0->setNet(net0);
    inst0->addPin(inst_term0);
    net0->addPin(inst_term0);

    auto inst_term1 = design->addPin();

    inst_term1->getAttr()->setTerm(mt1);
    inst_term1->setInst(inst1);
    inst_term1->setNet(net0);
    inst1->addPin(inst_term1);
    net0->addPin(inst_term1);
    */

    OStream<std::ofstream> os(filename, std::ios::out | mode);
    ASSERT_EQ(os.isOpen(), true);

    os << db;
    os.close();

    db.writeVerilog(std::string(filename) + ".v");
  }

  // read
  {
    ediPrint(kDebug, "reading from %s\n", filename);
    Database db;
    IStream<std::ifstream> is(db, filename, std::ios::in | mode);
    ASSERT_EQ(is.isOpen(), true);

    is >> db;
    is.close();

    ASSERT_EQ(db.numCells(), 1);
    auto cell0 = db.getCell(0);
    ASSERT_EQ(cell0->getName(), "cell0");
    ASSERT_EQ(cell0->numTerms(), 2);
    auto mt0 = cell0->getTerm(0);
    auto mt1 = cell0->getTerm(1);
    ASSERT_EQ(mt0->getName(), "term0");
    ASSERT_EQ(mt0->getSignalDirect(), SignalDirection::kInput);
    ASSERT_EQ(mt1->getName(), "term1");
    ASSERT_EQ(mt1->getSignalDirect(), SignalDirection::kOutput);

    /*remove-design
    auto design = db.getDesign();
    ASSERT_EQ(design->numInsts(), 2);
    ASSERT_EQ(design->numNets(), 1);
    ASSERT_EQ(design->numPins(), 2);

    auto inst0 = design->getInst(0);
    auto inst1 = design->getInst(1);
    ASSERT_EQ(inst0->getAttr()->getName(), "inst0");
    ASSERT_EQ(inst0->numPins(), 1);
    ASSERT_EQ(inst1->getAttr()->getName(), "inst1");
    ASSERT_EQ(inst1->numPins(), 1);

    auto net0 = design->getNet(0);
    net0->initPins(4);
    ASSERT_EQ(net0->getAttr()->getName(), "net0");
    ASSERT_EQ(net0->numPins(), 2);

    auto inst_term0 = design->getPin(0);
    auto inst_term1 = design->getPin(1);

    ASSERT_EQ(net0->getPin(0), inst_term0);
    ASSERT_EQ(net0->getPin(1), inst_term1);

    ASSERT_EQ(inst0->getPin(0), inst_term0);
    ASSERT_EQ(inst1->getPin(0), inst_term1);
    */
  }
#endif
}

void testUniquify() {
#if 0 
   // remove-database
  Database db;
  db.initCell(1);
  // std::cout<<"test start!"<<std::endl;
  ediPrint(kDebug, "starting uniquify test\n");
  {
    // create a cell
    auto cell0 = db.addCell("cell0");
    cell0->initTerms(2);
    auto mt0 = cell0->addTerm("term0");
    mt0->setSignalDirect(SignalDirection::kInput);
    auto mt1 = cell0->addTerm("term1");
    mt1->setSignalDirect(SignalDirection::kOutput);
    ediPrint(kDebug, "created cell \n");
    // cell0->addDesign("adder");
    // cell0->getDesign()->setType(1);
    cell0->setCellType(CellType::kCell);
    // ediPrint(kDebug, "setted cell \n");

    /* remove-design
    // create a design
    auto design = db.getDesign();
    design->initInsts(2);
    design->initNets(1);
    design->initPins(2);
    design->initInstsOfTerms(0);
    auto inst0 = design->addInst();
    inst0->initPins(1);
    inst0->getAttr()->setName("inst0");
    // inst0->getAttr()->setCell(cell0);
    inst0->getAttr()->setLoc({0, 1});
    inst0->addCell(cell0);
    // ediPrint(kDebug, "created design \n");

    // auto inst1 = design->addInst();
    // inst1->getAttr()->setName("inst1");
    // inst1->getAttr()->setCell(cell0);
    // inst1->getAttr()->setLoc({2, 3});
    InstAttr attr1;
    attr1.setName("inst1");
    // attr1.setCell(cell0);
    attr1.setLoc({2, 3});
    auto inst1 = design->addInst(attr1);
    inst1->addCell(cell0);
    inst1->initPins(1);
    // ASSERT_EQ(inst0->getDesign()->getAttr(), 0);
    // ASSERT_EQ(inst1->getDesign()->getAttr(), 0);
    ediPrint(kDebug, "created inst1 \n");

    auto net0 = design->addNet();
    net0->getAttr()->setName("net0");

    auto inst_term0 = design->addPin();
    inst_term0->getAttr()->setTerm(mt0);
    inst_term0->setInst(inst0);
    inst_term0->setNet(net0);
    inst0->addPin(inst_term0);
    net0->addPin(inst_term0);

    auto inst_term1 = design->addPin();
    inst_term1->getAttr()->setTerm(mt1);
    inst_term1->setInst(inst1);
    inst_term1->setNet(net0);
    inst1->addPin(inst_term1);
    net0->addPin(inst_term1);
    */

  }
  ediPrint(kDebug, "before uniquify \n");
  db.writeVerilog("before_uniquify.v");
  //db.drawGraphviz("before_uniquify.dot");
  db.uniquify();
  ediPrint(kDebug, "finish uniquify \n");
  db.writeVerilog("finish_uniquify.v");
  //db.drawGraphviz("finish_uniquify.dot");
  {
      /* remove-design
    ASSERT_EQ(db.numCells(), 1);
    auto cell0 = db.getCell(0);
    ASSERT_EQ(cell0->getName(), "cell0");
    ASSERT_EQ(cell0->numTerms(), 2);
    // ASSERT_EQ(cell0->getDesign()->getTyp(), 1);
    auto mt0 = cell0->getTerm(0);
    auto mt1 = cell0->getTerm(1);
    ASSERT_EQ(mt0->getName(), "term0");
    ASSERT_EQ(mt0->getSignalDirect(), SignalDirection::kInput);
    ASSERT_EQ(mt1->getName(), "term1");
    ASSERT_EQ(mt1->getSignalDirect(), SignalDirection::kOutput);

    auto design = db.getDesign();
    ASSERT_EQ(design->numInsts(), 2);
    ASSERT_EQ(design->numNets(), 1);
    ASSERT_EQ(design->numPins(), 2);

    auto inst0 = design->getInst(0);
    auto inst1 = design->getInst(1);
    ASSERT_EQ(inst0->getAttr()->getName(), "inst0");
    ASSERT_EQ(inst0->numPins(), 1);
    ASSERT_EQ(inst1->getAttr()->getName(), "inst1");
    ASSERT_EQ(inst1->numPins(), 1);
    ASSERT_EQ(inst0->getDesign()->getUniquified(), 1);
    ASSERT_EQ(inst0->getDesign()->getUniquified(), 1);

    auto net0 = design->getNet(0);
    ASSERT_EQ(net0->getAttr()->getName(), "net0");
    ASSERT_EQ(net0->numPins(), 2);

    auto inst_term0 = design->getPin(0);
    auto inst_term1 = design->getPin(1);

    ASSERT_EQ(net0->getPin(0), inst_term0);
    ASSERT_EQ(net0->getPin(1), inst_term1);

    ASSERT_EQ(inst0->getPin(0), inst_term0);
    ASSERT_EQ(inst1->getPin(0), inst_term1);
    */
  }
#endif
}

void testFlatten(char const* filename) {
#if 0
    // remove-database
  ediPrint(kDebug, "starting flatten test\n");
  Database db;

  db.initCell(6);
  auto cell0 = db.addCell("cell0");
  cell0->initTerms(2);
  auto mt0_0 = cell0->addTerm("cell0-term0");
  mt0_0->setSignalDirect(SignalDirection::kInput);
  auto mt0_1 = cell0->addTerm("cell0-term1");
  mt0_1->setSignalDirect(SignalDirection::kOutput);
  cell0->setCellType(CellType::kCell);
  // auto m0_design = cell0->addDesign("m0-d");
  // m0_design->setType(1);

  auto cell1 = db.addCell("cell1");
  cell1->initTerms(3);
  auto mt1_0 = cell1->addTerm("cell1-term0");
  mt1_0->setSignalDirect(SignalDirection::kInput);
  auto mt1_1 = cell1->addTerm("cell1-term1");
  mt1_1->setSignalDirect(SignalDirection::kInput);
  auto mt1_2 = cell1->addTerm("cell1-term2");
  mt1_2->setSignalDirect(SignalDirection::kOutput);
  cell1->setCellType(CellType::kCell);
  // auto m1_design = cell1->addDesign("m1-d");
  // m1_design->setType(1);

  auto cell2_mterm0 = db.addCell("cell2_mterm0");
  cell2_mterm0->initTerms(1);
  auto mt2_t0 = cell2_mterm0->addTerm("cell2-term0");
  mt2_t0->setSignalDirect(SignalDirection::kOutput);
  cell2_mterm0->setCellType(CellType::kTerm);
  // auto m2_t0_design = cell2_mterm0->addDesign("m2-t0-d");
  // m2_t0_design->setType(1);

  auto cell2_mterm1 = db.addCell("cell2_mterm1");
  cell2_mterm1->initTerms(1);
  auto mt2_t1 = cell2_mterm1->addTerm("cell2-term1");
  mt2_t1->setSignalDirect(SignalDirection::kOutput);
  cell2_mterm1->setCellType(CellType::kTerm);
  // auto m2_t1_design = cell2_mterm1->addDesign("m2-t1-d");
  // m2_t1_design->setType(1);

  auto cell2_mterm2 = db.addCell("cell2_mterm2");
  cell2_mterm2->initTerms(1);
  auto mt2_t2 = cell2_mterm2->addTerm("cell2-term2");
  mt2_t2->setSignalDirect(SignalDirection::kInput);
  cell2_mterm2->setCellType(CellType::kTerm);
  // auto m2_t2_design = cell2_mterm2->addDesign("m2-t2-d");
  // m2_t2_design->setType(1);

  auto cell2 = db.addCell("cell2");
  cell2->initTerms(3);
  auto mt2_0 = cell2->addTerm("cell2-term0");
  mt2_0->setSignalDirect(SignalDirection::kInput);
  auto mt2_1 = cell2->addTerm("cell2-term1");
  mt2_1->setSignalDirect(SignalDirection::kInput);
  auto mt2_2 = cell2->addTerm("cell2-term2");
  mt2_2->setSignalDirect(SignalDirection::kOutput);
  cell2->setCellType(CellType::kModule);

  //cell2 design
  {
      /* remove-design
    auto m2_design = cell2->addDesign("m2-d");
    m2_design->initInsts(6);
    m2_design->initInstsOfTerms(3);
    m2_design->initNets(5);
    m2_design->initPins(10);
    InstAttr i_attr;
    i_attr.setName("inst0");
    auto inst0 = m2_design->addInst(i_attr);
    inst0->addCell(cell0);
    inst0->initPins(2);

    i_attr.setName("inst1");
    auto inst1 = m2_design->addInst(i_attr);
    inst1->addCell(cell0);
    inst1->initPins(2);

    i_attr.setName("inst2");
    auto inst2 = m2_design->addInst(i_attr);
    inst2->addCell(cell1);
    inst2->initPins(3);

    // define inst for cell terms
    m2_design->initInstsOfTerms(3);

    i_attr.setName("inst_mterm0");
    auto inst_mterm0 = m2_design->addInst(i_attr);
    inst_mterm0->addCell(cell2_mterm0);
    inst_mterm0->initPins(1);
    m2_design->setInstOfTerm(0, m2_design->numInsts() - 1);

    i_attr.setName("inst_mterm1");
    auto inst_mterm1 = m2_design->addInst(i_attr);
    inst_mterm1->addCell(cell2_mterm1);
    inst_mterm1->initPins(1);
    m2_design->setInstOfTerm(1, m2_design->numInsts() - 1);

    i_attr.setName("inst_mterm2");
    auto inst_mterm2 = m2_design->addInst(i_attr);
    inst_mterm2->addCell(cell2_mterm2);
    inst_mterm2->initPins(1);
    m2_design->setInstOfTerm(2, m2_design->numInsts() - 1);

    auto net0 = m2_design->addNet();
    net0->getAttr()->setName("net0");
    net0->initPins(2);
    auto inst_term0 = m2_design->addPin();
    inst_term0->getAttr()->setTerm(mt2_t0);
    inst_term0->setNet(net0);
    inst_term0->setInst(inst_mterm0);
    net0->addPin(inst_term0);
    inst_mterm0->addPin(inst_term0);
    auto inst_term1 = m2_design->addPin();
    inst_term1->getAttr()->setTerm(mt0_0);
    inst_term1->setInst(inst0);
    inst_term1->setNet(net0);
    net0->addPin(inst_term1);
    inst0->addPin(inst_term1);

    auto net1 = m2_design->addNet();
    net1->getAttr()->setName("net1");
    net1->initPins(2);
    inst_term0 = m2_design->addPin();
    inst_term0->getAttr()->setTerm(mt2_t1);
    inst_term0->setNet(net1);
    inst_term0->setInst(inst_mterm1);
    net1->addPin(inst_term0);
    inst_mterm1->addPin(inst_term0);
    inst_term1 = m2_design->addPin();
    inst_term1->getAttr()->setTerm(mt0_0);
    inst_term1->setInst(inst1);
    inst_term1->setNet(net1);
    net1->addPin(inst_term1);
    inst1->addPin(inst_term1);

    auto net2 = m2_design->addNet();
    net2->getAttr()->setName("net2");
    net2->initPins(2);
    inst_term0 = m2_design->addPin();
    inst_term0->getAttr()->setTerm(mt0_1);
    inst_term0->setNet(net2);
    inst_term0->setInst(inst0);
    net2->addPin(inst_term0);
    inst0->addPin(inst_term0);
    inst_term1 = m2_design->addPin();
    inst_term1->getAttr()->setTerm(mt1_0);
    inst_term1->setNet(net2);
    inst_term1->setInst(inst2);
    net2->addPin(inst_term1);
    inst2->addPin(inst_term1);

    auto net3 = m2_design->addNet();
    net3->getAttr()->setName("net3");
    net3->initPins(2);
    inst_term0 = m2_design->addPin();
    inst_term0->getAttr()->setTerm(mt0_1);
    inst_term0->setNet(net3);
    inst_term0->setInst(inst1);
    net3->addPin(inst_term0);
    inst1->addPin(inst_term0);
    inst_term1 = m2_design->addPin();
    inst_term1->getAttr()->setTerm(mt1_1);
    inst_term1->setNet(net3);
    inst_term1->setInst(inst2);
    net3->addPin(inst_term1);
    inst2->addPin(inst_term1);

    auto net4 = m2_design->addNet();
    net4->getAttr()->setName("net4");
    net4->initPins(2);
    inst_term0 = m2_design->addPin();
    inst_term0->getAttr()->setTerm(mt1_2);
    inst_term0->setNet(net4);
    inst_term0->setInst(inst2);
    net4->addPin(inst_term0);
    inst2->addPin(inst_term0);
    inst_term1 = m2_design->addPin();
    inst_term1->getAttr()->setTerm(mt2_t2);
    inst_term1->setNet(net4);
    inst_term1->setInst(inst_mterm2);
    net4->addPin(inst_term1);
    inst_mterm2->addPin(inst_term1);
    // m2_design->setType(1);
    */
  }

  /* remove-design
  auto design = db.getDesign();
  design->initInsts(5);
  design->initInstsOfTerms(0);
  design->initNets(4);
  design->initPins(9);
  InstAttr i_attr;
  i_attr.setName("inst0");
  auto inst0 = design->addInst(i_attr);
  inst0->addCell(cell0);
  inst0->initPins(2);
  i_attr.setName("inst1");
  auto inst1 = design->addInst(i_attr);
  inst1->addCell(cell0);
  inst1->initPins(2);
  i_attr.setName("inst2");
  auto inst2 = design->addInst(i_attr);
  inst2->addCell(cell1);
  inst2->initPins(3);
  i_attr.setName("inst3");
  auto inst3 = design->addInst(i_attr);
  inst3->addCell(cell2);
  inst3->initPins(3);
  i_attr.setName("inst4");
  auto inst4 = design->addInst(i_attr);
  inst4->addCell(cell0);
  inst4->initPins(2);

  auto net0 = design->addNet();
  net0->getAttr()->setName("net0");
  net0->initPins(2);
  auto inst_term0 = design->addPin();
  inst_term0->getAttr()->setTerm(mt0_1);
  inst_term0->setInst(inst0);
  inst_term0->setNet(net0);
  net0->addPin(inst_term0);
  inst0->addPin(inst_term0);
  auto inst_term1 = design->addPin();
  inst_term1->getAttr()->setTerm(mt1_0);
  inst_term1->setNet(net0);
  inst_term1->setInst(inst2);
  net0->addPin(inst_term1);
  inst2->addPin(inst_term1);

  auto net1 = design->addNet();
  net1->getAttr()->setName("net1");
  net1->initPins(3);
  inst_term0 = design->addPin();
  inst_term0->getAttr()->setTerm(mt0_1);
  inst_term0->setInst(inst1);
  inst_term0->setNet(net1);
  net1->addPin(inst_term0);
  inst1->addPin(inst_term0);
  inst_term1 = design->addPin();
  inst_term1->getAttr()->setTerm(mt1_1);
  inst_term1->setNet(net1);
  inst_term1->setInst(inst2);
  net1->addPin(inst_term1);
  inst2->addPin(inst_term1);
  auto inst_term2 = design->addPin();
  inst_term2->getAttr()->setTerm(mt2_1);
  inst_term2->setInst(inst3);
  inst_term2->setNet(net1);
  inst3->addPin(inst_term2);
  net1->addPin(inst_term2);

  auto net2 = design->addNet();
  net2->getAttr()->setName("net2");
  net2->initPins(2);
  inst_term0 = design->addPin();
  inst_term0->getAttr()->setTerm(mt1_2);
  inst_term0->setInst(inst2);
  inst_term0->setNet(net2);
  net2->addPin(inst_term0);
  inst2->addPin(inst_term0);
  inst_term1 = design->addPin();
  inst_term1->getAttr()->setTerm(mt2_0);
  inst_term1->setNet(net2);
  inst_term1->setInst(inst3);
  net2->addPin(inst_term1);
  inst3->addPin(inst_term1);

  auto net3 = design->addNet();
  net3->getAttr()->setName("net3");
  net3->initPins(2);
  inst_term0 = design->addPin();
  inst_term0->getAttr()->setTerm(mt2_2);
  inst_term0->setInst(inst3);
  inst_term0->setNet(net3);
  inst3->addPin(inst_term0);
  net3->addPin(inst_term0);
  inst_term1 = design->addPin();
  inst_term1->getAttr()->setTerm(mt0_0);
  inst_term1->setInst(inst4);
  inst_term1->setNet(net3);
  inst4->addPin(inst_term1);
  net3->addPin(inst_term1);
  
  ASSERT_EQ(db.getDesign()->getUniquified(), 0); 

  OStream<std::ofstream> os0("unittest_bef.txt", std::ios_base::out | std::ios_base::openmode());
  ASSERT_EQ(os0.isOpen(), true);
  ediPrint(kDebug, "writing to %s\n", "unittest_bef.txt");
  os0 << db;
  os0.close();
  
  ediPrint(kDebug, "reading from %s\n", "unittest_bef.txt");
  IStream<std::ifstream> is(db, "unittest_bef.txt", std::ios::in | std::ios_base::openmode());
  ASSERT_EQ(is.isOpen(), true);

  is >> db;
  is.close();
  
  ASSERT_EQ(db.getDesign()->getUniquified(), 0); 
  ediPrint(kDebug, "starting uniquify\n");
  db.uniquify();
  ediPrint(kDebug, "ending uniquify\n");

  OStream<std::ofstream> os1("unittest_uniquify.txt", std::ios_base::out | std::ios_base::openmode());
  ASSERT_EQ(os1.isOpen(), true);
  ediPrint(kDebug, "writing to %s\n", "unittest_uniquify.txt");
  os1 << db;
  os1.close();
  
  IStream<std::ifstream> is1(db, "unittest_uniquify.txt", std::ios::in | std::ios_base::openmode());
  ASSERT_EQ(is1.isOpen(), true);

  is1 >> db;
  is1.close();

  ASSERT_EQ(db.getDesign()->getUniquified(), 1); 
  ASSERT_EQ(db.getDesign()->numInsts(), 5);
  ASSERT_EQ(db.getDesign()->numNets(), 4);
  ASSERT_EQ(db.getDesign()->numPins(), 9);
  ASSERT_EQ(db.getDesign()->getFlattened(), 0); 

  ASSERT_EQ(db.getDesign()->getFlattened(), 0); 
  // ASSERT_EQ(db.getDesign()->getInst(3)->getDesign()->getFlattened(), 0);
  
  ediPrint(kDebug, "starting flatten\n");

  db.writeVerilog("before_flatten.v");
  db.drawGraphviz("before_flatten.dot");
  db.flatten();
  ediPrint(kDebug, "ending flatten\n");
  db.writeVerilog("finish_flatten.v");
  db.drawGraphviz("finish_flatten.dot");

  ASSERT_EQ(db.getDesign()->getFlattened(), 1); 
  ASSERT_EQ(db.getDesign()->numInsts(), 10);
  ASSERT_EQ(db.getDesign()->numPins(), 19);
  ASSERT_EQ(db.getDesign()->numNets(), 9);

  */
  OStream<std::ofstream> os(filename, std::ios_base::out | std::ios_base::openmode());
  ASSERT_EQ(os.isOpen(), true);
  ediPrint(kDebug, "writing to %s\n", filename);
  os << db;
  os.close();

  IStream<std::ifstream> is2(db, filename, std::ios::in | std::ios_base::openmode());
  ASSERT_EQ(is2.isOpen(), true);

  is2 >> db;
  is2.close();
#endif
}

/// GTest class for fft module testing
class DatabaseTest : public ::testing::Test {
 public:
  void testBinaryStream() {
    char const* filename = "unittest_database.dat";
    ediPrint(kDebug, "%s\n", "start test binary stream.");
    testStream(filename, std::ios::binary);
  }

  void testAsciiStream() {
    char const* filename = "unittest_database.txt";
    testStream(filename, std::ios_base::openmode());
  }

  void testUniquifySimple() {
    testUniquify();
  }

  void testFlattenS() {
    char const* filename = "unittest_flatten.txt";
    testFlatten(filename);
  }

};

TEST_F(DatabaseTest, BinaryStream) { testBinaryStream(); }

TEST_F(DatabaseTest, AsciiStream) { testAsciiStream(); }

TEST_F(DatabaseTest, Uniquify) { testUniquifySimple(); }

TEST_F(DatabaseTest, Flatten) { testFlattenS(); }

}  // namespace unitest

EDI_END_NAMESPACE
