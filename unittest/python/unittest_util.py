##
# @file   unittest_util.py
# @author Yibo Lin
# @date   Apr 2020
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


class EnumsTest(unittest.TestCase):
    def testMessageType(self):
        self.assertEqual(str(edi.MessageType.kNone), "MessageType.kNone")
        self.assertEqual(str(edi.MessageType.kInfo), "MessageType.kInfo")
        self.assertEqual(str(edi.MessageType.kWarn), "MessageType.kWarn")
        self.assertEqual(str(edi.MessageType.kError), "MessageType.kError")
        self.assertEqual(str(edi.MessageType.kDebug), "MessageType.kDebug")
        self.assertEqual(str(edi.MessageType.kAssert), "MessageType.kAssert")

    def testSignalDirection(self):
        self.assertEqual(str(edi.SignalDirection.kInput),
                         "SignalDirection.kInput")
        self.assertEqual(str(edi.SignalDirection.kOutput),
                         "SignalDirection.kOutput")
        self.assertEqual(str(edi.SignalDirection.kInout),
                         "SignalDirection.kInout")
        self.assertEqual(str(edi.SignalDirection.kUnknown),
                         "SignalDirection.kUnknown")

    def testPlaceStatus(self):
        self.assertEqual(str(edi.PlaceStatus.kUnplaced),
                         "PlaceStatus.kUnplaced")
        self.assertEqual(str(edi.PlaceStatus.kPlaced), "PlaceStatus.kPlaced")
        self.assertEqual(str(edi.PlaceStatus.kSuggested),
                         "PlaceStatus.kSuggested")
        self.assertEqual(str(edi.PlaceStatus.kFixed), "PlaceStatus.kFixed")
        self.assertEqual(str(edi.PlaceStatus.kLocked), "PlaceStatus.kLocked")
        self.assertEqual(str(edi.PlaceStatus.kUnknown), "PlaceStatus.kUnknown")

    def testOrient(self):
        self.assertEqual(str(edi.Orient.kN), "Orient.kN")
        self.assertEqual(str(edi.Orient.kS), "Orient.kS")
        self.assertEqual(str(edi.Orient.kW), "Orient.kW")
        self.assertEqual(str(edi.Orient.kE), "Orient.kE")
        self.assertEqual(str(edi.Orient.kFN), "Orient.kFN")
        self.assertEqual(str(edi.Orient.kFS), "Orient.kFS")
        self.assertEqual(str(edi.Orient.kFW), "Orient.kFW")
        self.assertEqual(str(edi.Orient.kFE), "Orient.kFE")
        self.assertEqual(str(edi.Orient.kUnknown), "Orient.kUnknown")

    def testSignalType(self):
        self.assertEqual(str(edi.SignalType.kSignal), "SignalType.kSignal")
        self.assertEqual(str(edi.SignalType.kPower), "SignalType.kPower")
        self.assertEqual(str(edi.SignalType.kGround), "SignalType.kGround")
        self.assertEqual(str(edi.SignalType.kClock), "SignalType.kClock")
        self.assertEqual(str(edi.SignalType.kAnalog), "SignalType.kAnalog")
        self.assertEqual(str(edi.SignalType.kReset), "SignalType.kReset")
        self.assertEqual(str(edi.SignalType.kScan), "SignalType.kScan")
        self.assertEqual(str(edi.SignalType.kTieoff), "SignalType.kTieoff")
        self.assertEqual(str(edi.SignalType.kUnknown), "SignalType.kUnknown")


class PrintTest(unittest.TestCase):
    def testPrint(self):
        edi.ediPrint(edi.MessageType.kNone,
                     "test %s\n" % (edi.MessageType.kNone))
        edi.ediPrint(edi.MessageType.kInfo,
                     "test %s\n" % (edi.MessageType.kInfo))
        edi.ediPrint(edi.MessageType.kWarn,
                     "test %s\n" % (edi.MessageType.kWarn))
        edi.ediPrint(edi.MessageType.kError,
                     "test %s\n" % (edi.MessageType.kError))
        edi.ediPrint(edi.MessageType.kDebug,
                     "test %s\n" % (edi.MessageType.kDebug))
        edi.ediPrint(edi.MessageType.kAssert,
                     "test %s\n" % (edi.MessageType.kAssert))


if __name__ == '__main__':
    sys.argv.pop(0)
    unittest.main()
