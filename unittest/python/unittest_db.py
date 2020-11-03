##
# @file   unittest_db.py
# @author Tuo Dai
# @date   June 2020
#

import pdb
import os
import sys
import copy
import unittest

if len(sys.argv) < 2:
    print("usage: python script.py module_dir")

module_dir = sys.argv[1]

sys.path.append(module_dir)
import openedi as edi
sys.path.pop()

class DatabaseTest(unittest.TestCase):
    def testIOstream(self):
        db = edi.db.Database()
        m0 = db.addModel("model0")
        m0.setModelType(edi.ModelType.kCell)
        mt0 = m0.addTerm("term0")
        mt0.setSignalDirect(edi.SignalDirection.kInput)
        mt1= m0.addTerm("term1")
        mt1.setSignalDirect(edi.SignalDirection.kOutput)
        self.assertEqual(mt0.getSignalDirect(), edi.SignalDirection.kInput)
        self.assertEqual(mt1.getSignalDirect(), edi.SignalDirection.kOutput)
        self.assertEqual(m0.getModelType(), edi.ModelType.kCell)
        design = db.getDesign()
        inst0 = design.addInst()
        inst0.getAttr().setName("inst0")
        p0 = edi.geo.Point2DInt(0, 1)
        inst0.getAttr().setLoc(p0)
        inst0.addModel(m0)
        attr1 = edi.db.InstAttr()
        attr1.setName("inst1")
        p1 = edi.geo.Point2DInt(2, 3)
        attr1.setLoc(p1)
        inst1 = design.addInst(attr1)
        inst1.addModel(m0)
        net0 = design.addNet()
        net0.getAttr().setName("net0")
        inst_term0 = design.addInstTerm()
        inst_term0.getAttr().setModelTerm(mt0)
        inst_term0.setInst(inst0)
        inst_term0.setNet(net0)
        inst0.addInstTerm(inst_term0)
        net0.addInstTerm(inst_term0)
        self.assertEqual(inst1.getAttr().getName(), "inst1")
        inst_term1 = design.addInstTerm()
        inst_term1.getAttr().setModelTerm(mt1)
        inst_term1.setInst(inst1)
        inst_term1.setNet(net0)
        inst1.addInstTerm(inst_term1)
        net0.addInstTerm(inst_term1)
        db.uniquify()
        db.flatten()
        edi.db.write(db, "unittest_db.txt", 0)

        db2 = edi.db.Database()
        edi.db.read(db2, "unittest_db.txt", 0)
        print("We have %d models in db2." % (db2.numModels()))
        print("We have %d insts in db2.design_." % (db2.getDesign().numInsts()))
        print("We have %d nets in db2.design_." % (db2.getDesign().numNets()))
        print("We have %d inst_terms in db2.design_." % (db2.getDesign().numInstTerms()))
        self.assertEqual(db.numModels(), db2.numModels())
        self.assertEqual(db.getDesign().numInsts(), db2.getDesign().numInsts())
        self.assertEqual(db.getDesign().numNets(), db2.getDesign().numNets())
        self.assertEqual(db.getDesign().numInstTerms(), db2.getDesign().numInstTerms())
        self.assertEqual(db.getModel(0).getModelType(), db2.getModel(0).getModelType())

        edi.db.write(db, "unittest_db.dat", 1)

        # db2 = edi.db.Database()
        edi.db.read(db2, "unittest_db.dat", 1)
        self.assertEqual(db.numModels(), db2.numModels())
        self.assertEqual(db.getDesign().numInsts(), db2.getDesign().numInsts())
        self.assertEqual(db.getDesign().numNets(), db2.getDesign().numNets())
        self.assertEqual(db.getDesign().numInstTerms(), db2.getDesign().numInstTerms())
        
    def testModule(self):
        db = edi.db.Database()
        model0 = db.addModel("model0")
        mt0_0 = model0.addTerm("m0-term0")
        mt0_0.setSignalDirect(edi.SignalDirection.kInput)
        mt0_1 = model0.addTerm("m0-term1")
        mt0_1.setSignalDirect(edi.SignalDirection.kOutput)

        model1 = db.addModel("model1")
        mt1_0 = model1.addTerm("m1-term0")
        mt1_0.setSignalDirect(edi.SignalDirection.kInput)
        mt1_1 = model1.addTerm("m1-term1")
        mt1_1.setSignalDirect(edi.SignalDirection.kInput)
        mt1_2 = model1.addTerm("m1-term1")
        mt1_2.setSignalDirect(edi.SignalDirection.kOutput)

        model2_mterm0 = db.addModel("model2_mterm0")
        mt2_t0 = model2_mterm0.addTerm("model2-term0")
        mt2_t0.setSignalDirect(edi.SignalDirection.kOutput)
        model2_mterm0.setModelType(edi.ModelType.kModelTerm)

        model2_mterm1 = db.addModel("model2_mterm1")
        mt2_t1 = model2_mterm1.addTerm("model2-term1")
        mt2_t1.setSignalDirect(edi.SignalDirection.kOutput)
        model2_mterm1.setModelType(edi.ModelType.kModelTerm)

        model2_mterm2 = db.addModel("model2_mterm2")
        mt2_t2 = model2_mterm2.addTerm("model2-term2")
        mt2_t2.setSignalDirect(edi.SignalDirection.kInput)
        model2_mterm2.setModelType(edi.ModelType.kModelTerm)   

        model2 = db.addModel("model2")
        mt2_0 = model2.addTerm("model2-term0")
        mt2_0.setSignalDirect(edi.SignalDirection.kInput)
        mt2_1 = model2.addTerm("model2-term1")
        mt2_1.setSignalDirect(edi.SignalDirection.kInput)
        mt2_2 = model2.addTerm("model2-term2")
        mt2_2.setSignalDirect(edi.SignalDirection.kOutput)
        model2.setModelType(edi.ModelType.kModule)

        m2_design = model2.addDesign("m2-d")

        i_attr = edi.db.InstAttr()

        i_attr.setName("inst0")
        inst0 = m2_design.addInst(i_attr)
        inst0.addModel(model0)

        i_attr.setName("inst1")
        inst1 = m2_design.addInst(i_attr)
        inst1.addModel(model0)

        i_attr.setName("inst2")
        inst2 = m2_design.addInst(i_attr)
        inst2.addModel(model1)

        m2_design.setInitModelTerms(3)

        i_attr.setName("inst_mterm0")
        inst_mterm0 = m2_design.addInst(i_attr)
        inst_mterm0.addModel(model2_mterm0)
        m2_design.setInstModelTerm(0, m2_design.numInsts() - 1)

        i_attr.setName("inst_mterm1")
        inst_mterm1 = m2_design.addInst(i_attr)
        inst_mterm1.addModel(model2_mterm1)
        m2_design.setInstModelTerm(1, m2_design.numInsts() - 1)

        i_attr.setName("inst_mterm3")
        inst_mterm2 = m2_design.addInst(i_attr)
        inst_mterm2.addModel(model2_mterm2)
        m2_design.setInstModelTerm(2, m2_design.numInsts() - 1)

        net0 = m2_design.addNet()
        net0.getAttr().setName("net0")
        inst_term0 = m2_design.addInstTerm()
        inst_term0.getAttr().setModelTerm(mt2_t0)
        inst_term0.setNet(net0)
        inst_term0.setInst(inst_mterm0)
        net0.addInstTerm(inst_term0)
        inst_mterm0.addInstTerm(inst_term0)
        inst_term1 = m2_design.addInstTerm()
        inst_term1.getAttr().setModelTerm(mt0_0)
        inst_term1.setInst(inst0)
        inst_term1.setNet(net0)
        net0.addInstTerm(inst_term1)
        inst0.addInstTerm(inst_term1)

        net1 = m2_design.addNet()
        net1.getAttr().setName("net1")
        inst_term0 = m2_design.addInstTerm()
        inst_term0.getAttr().setModelTerm(mt2_t1)
        inst_term0.setNet(net1)
        inst_term0.setInst(inst_mterm1)
        net1.addInstTerm(inst_term0)
        inst_mterm1.addInstTerm(inst_term0)
        inst_term1 = m2_design.addInstTerm()
        inst_term1.getAttr().setModelTerm(mt0_0)
        inst_term1.setInst(inst1)
        inst_term1.setNet(net1)
        net1.addInstTerm(inst_term1)
        inst1.addInstTerm(inst_term1)

        net2 = m2_design.addNet()
        net2.getAttr().setName("net2")
        inst_term0 = m2_design.addInstTerm()
        inst_term0.getAttr().setModelTerm(mt0_1)
        inst_term0.setNet(net2)
        inst_term0.setInst(inst0)
        net2.addInstTerm(inst_term0)
        inst0.addInstTerm(inst_term0)
        inst_term1 = m2_design.addInstTerm()
        inst_term1.getAttr().setModelTerm(mt1_0)
        inst_term1.setNet(net2)
        inst_term1.setInst(inst2)
        net2.addInstTerm(inst_term1)
        inst2.addInstTerm(inst_term1)

        net3 = m2_design.addNet()
        net3.getAttr().setName("net3")
        inst_term0 = m2_design.addInstTerm()
        inst_term0.getAttr().setModelTerm(mt0_1)
        inst_term0.setNet(net3)
        inst_term0.setInst(inst1)
        net3.addInstTerm(inst_term0)
        inst1.addInstTerm(inst_term0)
        inst_term1 = m2_design.addInstTerm()
        inst_term1.getAttr().setModelTerm(mt1_1)
        inst_term1.setNet(net3)
        inst_term1.setInst(inst2)
        net3.addInstTerm(inst_term1)
        inst2.addInstTerm(inst_term1)

        net4 = m2_design.addNet()
        net4.getAttr().setName("net4")
        inst_term0 = m2_design.addInstTerm()
        inst_term0.getAttr().setModelTerm(mt1_2)
        inst_term0.setNet(net4)
        inst_term0.setInst(inst2)
        net4.addInstTerm(inst_term0)
        inst2.addInstTerm(inst_term0)
        inst_term1 = m2_design.addInstTerm()
        inst_term1.getAttr().setModelTerm(mt2_t2)
        inst_term1.setNet(net4)
        inst_term1.setInst(inst_mterm2)
        net4.addInstTerm(inst_term1)
        inst_mterm2.addInstTerm(inst_term1)

        edi.db.write(db, "unittest_module.txt", 0)

        print("The database has %d models." % (db.numModels())) # =6
        print("The model2 design has %d insts." % (model2.getDesign().numInsts())) # =6
        print("The model2 design has %d nets." % (model2.getDesign().numNets())) # =5


    def testFlatten(self):
        db = edi.db.Database()
        try:
            edi.db.read(db, "../../../unittest/python/unittest_bef.txt", 0)
        except:
            edi.db.read(db, "unittest_bef.txt", 0)

        print("Hierachical design has %d insts." % (db.getDesign().numInsts())) # =5
        print("Hierachical design has %d net." % (db.getDesign().numNets())) # =4
        print("Hierachical design has %d inst_terms." % (db.getDesign().numInstTerms())) # =9
        # self.assertEqual(db.numModels(), db2.numModels())
        self.assertEqual(db.getDesign().numInsts(), 5)
        self.assertEqual(db.getDesign().numNets(), 4)
        self.assertEqual(db.getDesign().numInstTerms(), 9)
        # print("read done.")
        db.uniquify()
        # print("uniquified.")
        db.flatten()
        # print("flattened.")

        print("FLattened design has %d insts." % (db.getDesign().numInsts())) # =10
        print("Flattened design has %d net." % (db.getDesign().numNets())) # =9
        print("Flattened design has %d inst_terms." % (db.getDesign().numInstTerms())) # =19

        self.assertEqual(db.numModels(), 7)
        self.assertEqual(db.getDesign().numInsts(), 10)
        self.assertEqual(db.getDesign().numNets(), 9)
        self.assertEqual(db.getDesign().numInstTerms(), 19)

        edi.db.write(db, "unittest_aft.txt", 0)
        # print("write done.")

        db2 = edi.db.Database()
        try:
            edi.db.read(db2, "../../../unittest/python/unittest_flatten.txt", 0)
        except:
            edi.db.read(db2, "unittest_flatten.txt", 0)

        self.assertEqual(db.numModels(), db2.numModels())
        self.assertEqual(db.getDesign().numInsts(), db2.getDesign().numInsts())
        self.assertEqual(db.getDesign().numNets(), db2.getDesign().numNets())
        self.assertEqual(db.getDesign().numInstTerms(), db2.getDesign().numInstTerms())

if __name__ == '__main__':
    sys.argv.pop(0)
    unittest.main()
